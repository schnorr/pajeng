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

VivaGraph::VivaGraph ()
{
  layout = layout_new ();

}

VivaGraph::~VivaGraph (void)
{
  layout_free (layout);
  layout = NULL;
}

void VivaGraph::setView (GraphView *view)
{
  this->view = view;
}

void VivaGraph::hierarchyChanged (void)
{
  std::cout << "VivaGraph " << __FUNCTION__ << std::endl;

  //clean-up nodes
  nodes.clear();

  //add the root
  VivaNode *node = new VivaNode (rootInstance(), layout);
  nodes.push_back (node);
}

void VivaGraph::mouseClicked (wxPoint p)
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
  if (!selected) return; //nothing was clicked

  VivaNode *clickedNode = (VivaNode*)selected->data;
  PajeContainer *clickedContainer = clickedNode->container;

  std::vector<VivaNode*>::iterator itx;
  itx = find (nodes.begin(), nodes.end(), clickedNode);
  nodes.erase (itx);
  delete clickedNode;

  wxCommandEvent event (VivaGraphChanged);
  wxPostEvent (view, event);


  std::vector<PajeType*> ret;
  std::vector<PajeType*>::iterator it;
  std::cout << __FUNCTION__ << std::endl;
  ret = containedTypesForContainerType (clickedContainer->type);
  std::cout << clickedContainer->type->identifier() << " "<<ret.size() << std::endl;
  for (it = ret.begin(); it != ret.end(); it++){
    if (isContainerType(*it)){
      std::vector<PajeContainer*> containers;
      std::vector<PajeContainer*>::iterator it2;
      std::cout << (*it)->name << std::endl;
      containers = enumeratorOfContainersTypedInContainer ((*it), clickedContainer);
      for (it2 = containers.begin(); it2 != containers.end(); it2++){
        std::cout << (*it2)->name << std::endl;
        VivaNode *node = new VivaNode (*it2, layout);
        nodes.push_back (node);
      }
    }
  }
}
