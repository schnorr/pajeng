#include "VivaGraph.h"


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

VivaGraph::VivaGraph (std::string conffile)
{
  int i;
  layout = layout_new ();
  view = NULL;
  runner = NULL;

  //extract types for nodes and edges
  libconfig::Config cfg;
  cfg.readFile (conffile.c_str());
  try {
    libconfig::Setting &node = cfg.lookup ("node");
    if (!node.isList()){
      //throw something
    }
    for (i = 0; i < node.getLength(); i++){
      nodeTypes.insert (node[i]);
    }

    libconfig::Setting &edge = cfg.lookup ("edge");
    if (!edge.isList()){
      //throw something
    }
    for (i = 0; i < edge.getLength(); i++){
      edgeTypes.insert (edge[i]);
    }
  }catch (...){
    throw "configuration file is not valid";
  }

  //get the composition configuration for nodes
  configuration = &cfg.getRoot();
  configuration->remove ("node");
  configuration->remove ("edge");
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
    PajeType *type = (*it);
    if (isContainerType(type) && nodeTypes.count (type->name)){
      std::vector<PajeContainer*> conts;
      std::vector<PajeContainer*>::iterator it2;
      conts = enumeratorOfContainersTypedInContainer (type, container);
      if (conts.size()) return true;
    }
  }
  return false;
}

bool VivaGraph::hasParent (PajeContainer *container)
{
  if (container->parent) return true;
  else return false;
}

void VivaGraph::expandNode (VivaNode *node)
{
  PajeContainer *container = node->container;

  //delete the node
  deleteNode (node);

  //add its children to the graph
  std::vector<PajeType*> ret;
  std::vector<PajeType*>::iterator it;
  ret = containedTypesForContainerType (container->type);
  for (it = ret.begin(); it != ret.end(); it++){
    PajeType *type = (*it);
    if (isContainerType(type) && nodeTypes.count (type->name)){
      std::vector<PajeContainer*> conts;
      std::vector<PajeContainer*>::iterator it2;
      conts = enumeratorOfContainersTypedInContainer (type, container);
      for (it2 = conts.begin(); it2 != conts.end(); it2++){
        addNode (*it2);
      }
    }
  }
}

void VivaGraph::collapseNode (PajeContainer *container)
{
  //delete all the children of PajeContainer
  std::vector<PajeType*> ret;
  std::vector<PajeType*>::iterator it;
  ret = containedTypesForContainerType (container->type);
  for (it = ret.begin(); it != ret.end(); it++){
    PajeType *type = (*it);
    if (isContainerType(type) && nodeTypes.count (type->name)){
      std::vector<PajeContainer*> conts;
      std::vector<PajeContainer*>::iterator it2;
      conts = enumeratorOfContainersTypedInContainer (type, container);
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
  if (shouldBePresent (container)){
    VivaNode *node = new VivaNode (container, layout);
    nodes.push_back (node);
    nodeMap[container] = node;
  }
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

bool VivaGraph::shouldBePresent (PajeContainer *container)
{
  std::string name = container->type->name;
  if (nodeTypes.count (name)) return true;

  std::vector<PajeContainer*> vector;
  vector = enumeratorOfContainersInContainer (container);
  std::vector<PajeContainer*>::iterator it;
  for (it = vector.begin(); it != vector.end(); it++){
    if (shouldBePresent (*it)) return true;
  }
  return false;
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
