#include "VivaGraph.h"

VivaNode::VivaNode (PajeContainer *container, tp_layout *layout)
{
  this->node = node_new (container->name.c_str(), this);
  this->layout = layout;
  this->container = container;
  layout_add_node (layout, this->node);
}

VivaNode::~VivaNode ()
{
  layout_remove_node (layout, this->node);
  node_free (this->node);
  this->node = NULL;
}

wxPoint VivaNode::position ()
{
  tp_point pos = node->particle->position;
  return wxPoint (pos.x*100, pos.y*100);
}

// bool operator!= (const VivaNode& t1, const VivaNode& t2)
// {
//   return t1.container->name != t2.container->name;
// }

// bool operator== (const VivaNode& t1, const VivaNode& t2)
// {
//   return t1.container->name == t2.container->name;
// }


VivaRunner::VivaRunner (tp_layout *layout, GraphFrame *view)
  : wxThread (wxTHREAD_JOINABLE)
{
  this->layout = layout;
  this->view = view;
  this->keepRunning = true;
  if(wxTHREAD_NO_ERROR == Create()) {
    Run();
  }
}

void *VivaRunner::Entry (void)
{
  while(!TestDestroy() && keepRunning){
    double limit = layout_stabilization_limit (layout);
    double current = layout_stabilization (layout);
    if (current > limit){
      break;
    }else{
      layout_compute (layout);
    }
    wxCommandEvent event (VivaGraphLayoutUpdated);
    wxPostEvent (view, event);
  }
  return static_cast<ExitCode>(NULL);
}

VivaGraph::VivaGraph ()
{
  layout = layout_new ();
  view = NULL;
  runner = NULL;
}

VivaGraph::~VivaGraph (void)
{
  layout_free (layout);
  layout = NULL;
}

void VivaGraph::stop_runner (void)
{
  if (runner){
    runner->keepRunning = false;
    runner->Wait();
    runner = NULL;
  }
}

void VivaGraph::start_runner (void)
{
  stop_runner();
  if (!runner){
    if (!view){
      throw "Unable to launch VivaRunner because view is not defined";
    }
    if (!layout){
      throw "Unable to launch VivaRunner because layout is not defined";
    }
    layout_reset_energies(layout);
    runner = new VivaRunner (layout, view);
  }
}

void VivaGraph::setView (GraphFrame *view)
{
  this->view = view;
}

void VivaGraph::timeLimitsChanged (void)
{
}

void VivaGraph::timeSelectionChanged (void)
{
  //udpate layout according to new time/spatial integration
}

void VivaGraph::hierarchyChanged (void)
{
  //clean-up nodes
  nodes.clear();

  //add the root
  VivaNode *node = new VivaNode (rootInstance(), layout);
  nodes.push_back (node);
}

VivaNode *VivaGraph::getSelectedNodeByPosition (wxPoint p)
{
#define NODE_SIZE 30
  tp_rect mask;
  mask.size.width = (double)NODE_SIZE/100;
  mask.size.height = (double)NODE_SIZE/100;
#undef NODE_SIZE

  tp_point point;
  point.x = (double)p.x/100;
  point.y = (double)p.y/100;

  tp_node *selected = layout_find_node_by_position (layout, point, mask);
  if (!selected) return NULL;
  return ((VivaNode*)selected->data);
}

bool VivaGraph::hasChildren (PajeContainer *container)
{
  //this should be customized according to graph configuration
  std::vector<PajeType*> ret;
  std::vector<PajeType*>::iterator it;
  ret = containedTypesForContainerType (container->type);
  for (it = ret.begin(); it != ret.end(); it++){
    if (isContainerType(*it)){
      std::vector<PajeContainer*> conts;
      std::vector<PajeContainer*>::iterator it2;
      conts = enumeratorOfContainersTypedInContainer ((*it), container);
      if (conts.size()) return true;
    }
  }
  return false;
}

bool VivaGraph::hasParent (PajeContainer *container)
{
  //this should be customized according to graph configuration
  if (container->parent) return true;
  else return false;
}

void VivaGraph::expandNode (VivaNode *node)
{
  PajeContainer *container = node->container;

  //delete the node
  deleteNode (node);

  //this should be customized accoridng to graph configuration
  //add its children to the graph
  std::vector<PajeType*> ret;
  std::vector<PajeType*>::iterator it;
  ret = containedTypesForContainerType (container->type);
  for (it = ret.begin(); it != ret.end(); it++){
    if (isContainerType(*it)){
      std::vector<PajeContainer*> conts;
      std::vector<PajeContainer*>::iterator it2;
      conts = enumeratorOfContainersTypedInContainer ((*it), container);
      for (it2 = conts.begin(); it2 != conts.end(); it2++){
        addNode (*it2);
      }
    }
  }
}

void VivaGraph::collapseNode (PajeContainer *container)
{
  //this should be customized accoridng to graph configuration
  //delete all the children of PajeContainer
  std::vector<PajeType*> ret;
  std::vector<PajeType*>::iterator it;
  ret = containedTypesForContainerType (container->type);
  for (it = ret.begin(); it != ret.end(); it++){
    if (isContainerType(*it)){
      std::vector<PajeContainer*> conts;
      std::vector<PajeContainer*>::iterator it2;
      conts = enumeratorOfContainersTypedInContainer ((*it), container);
      for (it2 = conts.begin(); it2 != conts.end(); it2++){
        deleteNode (nodeMap[*it2]);
      }
    }
  }

  //add the parent container
  addNode (container);
}

void VivaGraph::addNode (PajeContainer *container)
{
  VivaNode *node = new VivaNode (container, layout);
  nodes.push_back (node);
  nodeMap[container] = node;
}

void VivaGraph::deleteNode (VivaNode *node)
{
  PajeContainer *container = node->container;
  std::vector<VivaNode*>::iterator itx;
  itx = find (nodes.begin(), nodes.end(), node);
  nodes.erase (itx);
  nodeMap.erase (container);
  delete node;
}

void VivaGraph::leftMouseClicked (wxPoint p)
{
  VivaNode *clickedNode = getSelectedNodeByPosition (p);
  if (!clickedNode) return;
  if (!hasChildren(clickedNode->container)) return;

  this->stop_runner ();
  {
    PajeContainer *clickedContainer = clickedNode->container;

    //expand the clicked node
    expandNode (clickedNode);

    //tell view that the graph changed
    wxCommandEvent event (VivaGraphChanged);
    wxPostEvent (view, event);
  }
  this->start_runner ();
}

void VivaGraph::rightMouseClicked (wxPoint p)
{
  VivaNode *clickedNode = getSelectedNodeByPosition (p);
  if (!clickedNode) return;
  if (!hasParent(clickedNode->container)) return;

  this->stop_runner();
  {
    PajeContainer *clickedContainer = clickedNode->container;
    PajeContainer *parent = clickedContainer->parent;

    //collapse the parent of the clicked node
    collapseNode (parent);

    //tell view that the graph changed
    wxCommandEvent event (VivaGraphChanged);
    wxPostEvent (view, event);
  }
  this->start_runner();
}

void VivaGraph::qualityChanged (int quality)
{
  layout_set_quality (layout, quality);
  layout_reset_energies (layout);
  this->start_runner();
}
