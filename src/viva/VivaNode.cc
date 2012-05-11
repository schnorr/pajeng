#include "VivaNode.h"


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
