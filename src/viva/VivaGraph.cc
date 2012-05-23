#include "VivaGraph.h"
#include "PajeEntity.h"


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
  window = NULL;
  runner = NULL;

  //extract types for nodes and edges
  config_init (&config);
  config_read_file (&config, conffile.c_str());

  //get node types, put them in the nodeTypes set
  config_setting_t *node = config_lookup (&config, "node");
  if (!node || !config_setting_is_list (node)){
    //throw something
  }
  for (i = 0; i < config_setting_length (node); i++){
    const char *elem = config_setting_get_string_elem (node, i);
    nodeTypes.insert (std::string(elem));
  }
  config_setting_remove (config_setting_parent(node), "node");

  //get edge types, put them in the edgeTypes set
  config_setting_t *edge = config_lookup (&config, "edge");
  if (!edge || !config_setting_is_list (edge)){
    //throw something
  }
  for (i = 0; i < config_setting_length (edge); i++){
    const char *elem = config_setting_get_string_elem (edge, i);
    edgeTypes.insert (std::string(elem));
  }
  config_setting_remove (config_setting_parent(edge), "edge");

}

VivaGraph::~VivaGraph (void)
{
  layout_free (layout);
  layout = NULL;
  config_destroy (&config);
}

void VivaGraph::defineEdges (void)
{
  edges.clear ();
  defineEdges (rootInstance());
}

void VivaGraph::defineEdges (PajeContainer *container)
{
  std::vector<PajeType*> types = containedTypesForContainerType (container->type());
  std::vector<PajeType*>::iterator type;
  for (type = types.begin(); type != types.end(); type++){
    PajeLinkType *link_type = dynamic_cast<PajeLinkType*>(*type);
    if (link_type && edgeTypes.count (link_type->name)){
      std::vector<PajeEntity*> links;
      links = enumeratorOfEntitiesTypedInContainer (*type, container, startTime(), endTime());
      std::vector<PajeEntity*>::iterator link;
      for (link = links.begin(); link != links.end(); link++){
        PajeContainer *startContainer = (*link)->startContainer();
        PajeContainer *endContainer = (*link)->endContainer();
        edges[startContainer].insert (endContainer);
        edges[endContainer].insert (startContainer);
      }
    }else{
      //recurse if container type
      if (isContainerType(*type)){
        std::vector<PajeContainer*> subs = enumeratorOfContainersTypedInContainer (*type, container);
        std::vector<PajeContainer*>::iterator cont;
        for (cont = subs.begin(); cont != subs.end(); cont++){
          defineEdges (*cont);
        }
      }
    }
  }
}

void VivaGraph::layoutNodes (void)
{
  std::vector<VivaNode*>::iterator it;
  for (it = nodes.begin(); it != nodes.end(); it++){
    VivaNode *node = (*it);
    node->layout();
  }

  wxCommandEvent event (VivaGraphLayoutUpdated);
  wxPostEvent (view, event);
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

void VivaGraph::setWindow (GraphWindow *window)
{
  this->window = window;
}

void VivaGraph::defineMaxForConfigurations (void)
{
  //configure scale for configurations
  int i;
  config_setting_t *root = config_root_setting(&config);
  for (i = 0; i < config_setting_length (root); i++){
    config_setting_t *conf = config_setting_get_elem (root, i);
    config_setting_t *size = config_setting_get_member (conf, "size");
    std::string size_typename (config_setting_get_string (size));
    PajeType *size_type = entityTypeWithName (size_typename);
    std::map<std::string,double> values = spatialIntegrationOfContainer (rootInstance());

    std::string settingName = std::string(config_setting_name (conf));
    compositionsScale[settingName] = values[size_typename];
  }
}

void VivaGraph::timeLimitsChanged (void)
{
  //TODO
}

void VivaGraph::timeSelectionChanged (void)
{
  this->defineMaxForConfigurations ();
  this->layoutNodes ();
}

void VivaGraph::hierarchyChanged (void)
{
  //define edges
  defineEdges ();

  //clean-up nodes
  nodes.clear();

  //add the root
  PajeContainer *root = rootInstance();
  if (shouldBePresent (root)){
    addNode (root);
  }else{
    //user should know that her configuration is bad
  }
}

VivaNode *VivaGraph::getSelectedNodeByPosition (wxPoint p)
{
  tp_point point;
  point.x = (double)p.x/100;
  point.y = (double)p.y/100;

  tp_node *selected = layout_find_node_by_position (layout, point);
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
  if (container->container) return true;
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
    VivaNode *node = new VivaNode (this, container, config_root_setting(&config), layout);
    nodes.push_back (node);
    nodeMap[container] = node;

    //layout the node
    node->layout ();
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
    PajeContainer *parent = clickedContainer->container;

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

void VivaGraph::scaleSliderChanged (void)
{
  this->layoutNodes ();
}

double VivaGraph::maxForConfigurationWithName (std::string configurationName)
{
  return compositionsScale[configurationName];
}

double VivaGraph::userScaleForConfigurationWithName (std::string configurationName)
{
  if (window){
    return window->scaleSliderValue (configurationName);
  }else{
    return COMPOSITION_DEFAULT_USER_SCALE;
  }
}
