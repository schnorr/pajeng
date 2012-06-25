/*
    This file is part of Viva.

    Viva is free software: you can redistribute it and/or modify it
    under the terms of the GNU Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Viva is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Public License for more details.

    You should have received a copy of the GNU Public License
    along with Viva. If not, see <http://www.gnu.org/licenses/>.
*/
#include "VivaGraph.h"
#include "PajeEntity.h"

VivaGraph::VivaGraph (std::string conffile)
{
  int i;
  layout = layout_new ();
  view = NULL;
  runner = NULL;
  layoutDone = false;

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

void VivaGraph::setView (VivaGraphView *view)
{
  this->view = view;
}

void VivaGraph::draw (void)
{
  //draw edges, then the nodes
  std::vector<VivaNode*>::iterator it;
  for (it = nodes.begin(); it != nodes.end(); it++){
    (*it)->drawEdges();
  }
  for (it = nodes.begin(); it != nodes.end(); it++){
    (*it)->draw();
  }
}

void VivaGraph::defineEdges (void)
{
  edges.clear ();
  defineEdges (rootInstance());
}

void VivaGraph::addEdge (PajeContainer *a1, PajeContainer *a2)
{
  if (a1 == a2) return;
  edges[a1].insert (a2);
  edges[a2].insert (a1);
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
        PajeContainer *p, *a1, *a2;
        PajeContainer *startContainer = (*link)->startContainer();
        PajeContainer *endContainer = (*link)->endContainer();

        p = endContainer;
        while (p){
          addEdge (startContainer, p);
          p = p->container();
        }

        p = startContainer;
        while (p){
          addEdge (endContainer, p);
          p = p->container();
        }

        a1 = startContainer;
        a2 = endContainer;
        while (a1 && a2){
          addEdge (a1, a2);
          a1 = a1->container();
          a2 = a2->container();
        }
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
  layoutDone = false;

  std::vector<VivaNode*>::iterator it;
  for (it = nodes.begin(); it != nodes.end(); it++){
    VivaNode *node = (*it);
    node->layout();
  }

  layoutDone = true;
}

void VivaGraph::stop_runner (void)
{
  if (runner){
    runner->keepRunning = false;
    runner->wait();
    delete runner;
    runner = NULL;
  }
}

void VivaGraph::start_runner (void)
{
  stop_runner();
  if (!runner){
    if (!layout){
      throw "Unable to launch VivaRunner because layout is not defined";
    }
    layout_reset_energies(layout);
    runner = new VivaLayoutRunner (this, view, layout);
  }
}

void VivaGraph::go_bottom (void)
{
  this->stop_runner();
  //first we need to remove all nodes
  {
    std::vector<VivaNode*> toremove;
    std::vector<VivaNode*>::iterator it;
    for (it = nodes.begin(); it != nodes.end(); it++){
      toremove.push_back (*it);
    }

    for (it = toremove.begin(); it != toremove.end(); it++){
      deleteNode (*it);
    }
    nodes.clear();
  }

  //FIXME: should not access directly PajeContainer data structure

  //then we traverse the tree and add all nodes without children
  {
    std::vector<PajeContainer*> stack;
    stack.push_back (rootInstance());
    while (stack.size()){
      PajeContainer *container = stack.back();
      stack.pop_back ();
      std::vector<PajeContainer*> children = container->getChildren();
      if (children.size()){
        stack.insert (stack.end(), children.begin(), children.end());
      }else{
        addNode (container);
      }
    }
    interconnectNodes ();
  }
  layout_reset_energies (layout);
  this->start_runner();
}

void VivaGraph::go_down (void)
{
  //create a list of nodes to expand
  std::vector<VivaNode*> toexpand;
  std::vector<VivaNode*>::iterator it;
  for (it = nodes.begin(); it != nodes.end(); it++){
    VivaNode *node = (*it);
    if (hasChildren(node->container)){
      toexpand.push_back (node);
    }
  }

  if (toexpand.size() == 0) return;
  this->stop_runner ();
  {
    if (toexpand.size()){
      for (it = toexpand.begin(); it != toexpand.end(); it++){
        VivaNode *node = (*it);
        expandNode (node);
      }
      interconnectNodes();
    }
  }
  this->start_runner ();
}

void VivaGraph::go_up (void)
{
  //create a *set* of containers to collapse
  std::set<PajeContainer*> tocollapse, toremove;
  std::vector<VivaNode*>::iterator it;
  for (it = nodes.begin(); it != nodes.end(); it++){
    VivaNode *node = (*it);
    PajeContainer *parent = node->container->container();
    if (parent){
      tocollapse.insert (parent);
    }
  }

  //check if we have common ancestors among nodes to be collapsed
  std::set<PajeContainer*>::iterator i;
  for (i = tocollapse.begin(); i != tocollapse.end(); i++){
    std::set<PajeContainer*>::iterator j;
    PajeContainer *c1 = (*i);

    //check if c1 is ancestor of one of the nodes
    for (j = tocollapse.begin(); j != tocollapse.end(); j++){
      PajeContainer *c2 = (*j);
      if (c1->isAncestorOf (c2)){
        //if it is, mark the descendant for removal
        toremove.insert (c2);
      }
    }
  }

  //remove the selected descendants from the nodes to be collapsed
  for (i = toremove.begin(); i != toremove.end(); i++){
    tocollapse.erase (*i);
  }

  if (tocollapse.size() == 0) return;

  this->stop_runner ();
  {
    if (tocollapse.size()){
      std::set<PajeContainer*>::iterator i;
      for (i = tocollapse.begin(); i != tocollapse.end(); i++){
        PajeContainer *cont = (*i);
        collapseNode (cont);
      }
      interconnectNodes();
    }
  }
  this->start_runner ();
}

void VivaGraph::go_top (void)
{
  this->stop_runner ();
  {
    PajeContainer *root = rootInstance();

    //check if root is not already present
    if (nodeMap.count(root)) return;

    collapseNode (root);
    interconnectNodes ();

    //tell view that the graph changed
    emit graphChanged ();
  }
  this->start_runner ();
}

void VivaGraph::refresh (void)
{
  this->stop_runner ();
  layout_shake (layout);
  layout_reset_energies(layout);
  this->start_runner ();
}

// void VivaGraph::setView (GraphFrame *view)
// {
//   this->view = view;
// }

// void VivaGraph::setWindow (GraphWindow *window)
// {
//   this->window = window;
// }

void VivaGraph::defineMaxForConfigurations (void)
{
  //configure scale for configurations
  int i;
  config_setting_t *root = config_root_setting(&config);
  for (i = 0; i < config_setting_length (root); i++){
    config_setting_t *conf = config_setting_get_elem (root, i);
    config_setting_t *size = config_setting_get_member (conf, "size");
    if (size == NULL){
      throw "The 'size' field is not defined for configuration '"+std::string(config_setting_name(size))+"'";
    }
    if (config_setting_type (size) != CONFIG_TYPE_STRING){
      continue;
    }
    std::string size_typename = std::string(config_setting_get_string (size));
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

VivaNode *VivaGraph::getSelectedNodeByPosition (tp_point p)
{
  p.x /= 100;
  p.y /= 100;

  tp_node *selected = layout_find_node_by_position (layout, p);
  if (!selected) return NULL;
  return ((VivaNode*)selected->data);
}

bool VivaGraph::hasChildren (PajeContainer *container)
{
  //this should be customized according to graph configuration
  std::vector<PajeType*> ret;
  std::vector<PajeType*>::iterator it;
  ret = containedTypesForContainerType (container->type());
  for (it = ret.begin(); it != ret.end(); it++){
    PajeType *type = (*it);
    if (isContainerType(type)){
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
  if (container->container()) return true;
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
  ret = containedTypesForContainerType (container->type());
  bool expandWasCorrect = false;
  for (it = ret.begin(); it != ret.end(); it++){
    PajeType *type = (*it);
    if (isContainerType(type)){
      std::vector<PajeContainer*> conts;
      std::vector<PajeContainer*>::iterator it2;
      conts = enumeratorOfContainersTypedInContainer (type, container);
      for (it2 = conts.begin(); it2 != conts.end(); it2++){
        addNode (*it2);
        expandWasCorrect = true;
      }
    }
  }
  if (!expandWasCorrect){
    std::cout << "Expand not correct" << std::endl;
    exit(1);
  }
}

void VivaGraph::collapseNode (PajeContainer *container)
{
  //FIXME: should not access directly PajeContainer data structure

  //remove all expanded nodes below this container
  std::vector<PajeContainer*> stack;
  stack.push_back (container);
  while (stack.size()){
    PajeContainer *container = stack.back();
    stack.pop_back ();

    if (!nodeMap.count(container)){
      std::vector<PajeContainer*> children = container->getChildren();
      stack.insert (stack.end(), children.begin(), children.end());
    }else{
      deleteNode (nodeMap[container]);
    }
  }

  //add the parent container
  addNode (container);
}

void VivaGraph::addNode (PajeContainer *container)
{
  if (shouldBePresent (container)){
    tp_point point = tp_Point(0,0);
    if (positions.count(container)){
      point = positions[container];
    }else if (container->container() && positions.count(container->container())){
      point = positions[container->container()];
    }

    VivaNode *node = new VivaNode (this, container, config_root_setting(&config), layout, point);
    nodes.push_back (node);
    nodeMap[container] = node;

    //layout the node
    node->layout ();
  }
}

void VivaGraph::deleteNode (VivaNode *node)
{
  PajeContainer *container = node->container;
  //save position of this container for the future
  positions[container] = node->position();

  std::vector<VivaNode*>::iterator itx;
  itx = find (nodes.begin(), nodes.end(), node);
  nodes.erase (itx);
  nodeMap.erase (container);
  delete node;
}

bool VivaGraph::shouldBePresent (PajeContainer *container)
{
  std::string name = container->type()->name;
  if (nodeTypes.count (name)) return true;

  std::vector<PajeContainer*> vector;
  vector = enumeratorOfContainersInContainer (container);
  std::vector<PajeContainer*>::iterator it;
  for (it = vector.begin(); it != vector.end(); it++){
    if (shouldBePresent (*it)) return true;
  }
  return false;
}

void VivaGraph::interconnectNodes (void)
{
  std::vector<VivaNode*>::iterator it;
  for (it = nodes.begin(); it != nodes.end(); it++){
    VivaNode *node = (*it);

    std::vector<tp_node*> connected;
    std::set<PajeContainer*>::iterator it;
    for (it = edges[node->container].begin(); it != edges[node->container].end(); it++){
      PajeContainer *c = *it;
      if (nodeMap.count(c)){
        VivaNode *cn = nodeMap[c];
        connected.push_back (cn->node);
      }
    }
    node->setConnectedNodes (connected);
  }
}

void VivaGraph::leftMouseClicked (QPointF point)
{
  tp_point p = tp_Point (point.x(), point.y());
  VivaNode *clickedNode = getSelectedNodeByPosition (p);
  if (!clickedNode) return;
  if (!hasChildren(clickedNode->container)) return;

  this->stop_runner ();
  {
    PajeContainer *clickedContainer = clickedNode->container;

    //expand the clicked node
    expandNode (clickedNode);
    interconnectNodes ();

    //tell view that the graph changed
    emit graphChanged ();
  }
  this->start_runner ();
}

void VivaGraph::rightMouseClicked (QPointF point)
{
  tp_point p = tp_Point (point.x(), point.y());
  VivaNode *clickedNode = getSelectedNodeByPosition (p);
  if (!clickedNode) return;
  if (!hasParent(clickedNode->container)) return;

  this->stop_runner();
  {
    PajeContainer *clickedContainer = clickedNode->container;
    PajeContainer *parent = clickedContainer->container();

    //collapse the parent of the clicked node
    collapseNode (parent);
    interconnectNodes ();

    //tell view that the graph changed
    emit graphChanged ();
  }
  this->start_runner();
}

void VivaGraph::mouseOverPoint (QPointF point)
{
  tp_point p = tp_Point (point.x(), point.y());
  VivaNode *node = getSelectedNodeByPosition (p);
  if (node){
    emit highlightNode (node);
  }else{
    emit unhighlightNode ();
  }
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
  emit graphChanged ();
}

double VivaGraph::maxForConfigurationWithName (std::string configurationName)
{
  return compositionsScale[configurationName];
}

double VivaGraph::userScaleForConfigurationWithName (std::string configurationName)
{
  return COMPOSITION_DEFAULT_USER_SCALE;

  // if (window){
  //   return window->scaleSliderValue (configurationName);
  // }else{
  //   return COMPOSITION_DEFAULT_USER_SCALE;
  // }
}

void VivaGraph::layoutUpdated (void)
{
  emit graphChanged ();
}
