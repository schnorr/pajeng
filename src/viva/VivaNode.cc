#include "VivaNode.h"

VivaComposition::VivaComposition (VivaGraph *filter, PajeContainer *container, config_setting_t *configuration)
{
  bb = tp_Rect(0,0,0,0);
  this->filter = filter;
  this->container = container;

  //get configuration
  config_setting_t *type = config_setting_get_member (configuration, "type");
  if (!type || config_setting_type (type) != CONFIG_TYPE_STRING){
    //throw something
  }
  config_setting_t *size = config_setting_get_member (configuration, "size");
  if (!size || config_setting_type (size) != CONFIG_TYPE_STRING){
    //throw something
  }
  config_setting_t *values = config_setting_get_member (configuration, "values");
  if (!values || config_setting_type (values) != CONFIG_TYPE_LIST){
    //throw something
  }

  //transform configuration
  std::string size_typename (config_setting_get_string (size));
  size_type = filter->entityTypeWithName (size_typename);

  int i;
  for (i = 0; i < config_setting_length (values); i++){
    config_setting_t *value = config_setting_get_elem (values, i);
    if (!value || config_setting_type (value) != CONFIG_TYPE_STRING){
      //throw something
    }
    std::string value_typename (config_setting_get_string (value));
    PajeType *value_type = filter->entityTypeWithName (value_typename);
    if (value_type){
      values_type.push_back (value_type);
    }
  }
}

VivaComposition::~VivaComposition (void)
{
  values_type.clear();
}

tp_rect VivaComposition::layout (void)
{
  if (size_type == NULL){
    bb = tp_Rect (0, 0, 0, 0);
  }else{
    bb = tp_Rect (0, 0, 30, 30);
  }
  return bb;
}

void VivaComposition::draw (wxDC& dc, tp_point base)
{
  wxColour black;
  black.Set(wxT("#000000"));
  dc.SetPen(wxPen(black));

  bb.origin.x = base.x;
  bb.origin.y = base.y;

  wxRect rect = wxRect (bb.origin.x, bb.origin.y, bb.size.width, bb.size.height);
  dc.DrawRectangle(rect);
}

VivaNode::VivaNode (VivaGraph *filter, PajeContainer *container, config_setting_t *conf, tp_layout *layout)
{
  this->node = node_new (container->name.c_str(), this);
  this->tupi_layout = layout;
  this->container = container;
  this->filter = filter;
  layout_add_node (layout, this->node);

  //use configuration to create compositions
  int i;
  for (i = 0; i < config_setting_length (conf); i++){
    config_setting_t *child = config_setting_get_elem (conf, i);
    VivaComposition *comp = new VivaComposition (filter, container, child);
    compositions.push_back (comp);
  }
}

VivaNode::~VivaNode ()
{
  layout_remove_node (tupi_layout, this->node);
  node_free (this->node);
  this->node = NULL;
}

tp_point VivaNode::position ()
{
  tp_point pos = node->particle->position;
  pos.x *= 100;
  pos.y *= 100;
  return pos;
}

tp_rect VivaNode::layout ()
{
  bb = tp_Rect(0,0,0,0);
  tp_rect region = bb;

  std::vector<VivaComposition*>::iterator it;
  for (it = compositions.begin(); it != compositions.end(); it++){
    VivaComposition *comp = (*it);
    tp_rect compRect = comp->layout();
    region = tp_UnionRect (region, compRect);
  }
  bb = region;

  tp_rect mask;
  mask = tp_Rect (position().x/100 - bb.size.width/2,
                  position().y/100 - bb.size.height/2,
                  bb.size.width,
                  bb.size.height);
  particle_set_mask (node->particle, mask);
  return bb;
}

void VivaNode::draw (wxDC& dc)
{
  tp_point position = this->position();
  std::vector<VivaComposition*>::iterator it;
  tp_point point = position;
  point.x -= bb.size.width/2;
  point.y -= bb.size.height/2;

  for (it = compositions.begin(); it != compositions.end(); it++){
    VivaComposition *comp = (*it);
    comp->draw(dc, point);
    point.x += comp->bb.size.width;
  }

  dc.DrawText (wxString(node->name, wxConvUTF8), position.x, position.y);
  dc.DrawPoint (position.x, position.y);
}

// bool operator!= (const VivaNode& t1, const VivaNode& t2)
// {
//   return t1.container->name != t2.container->name;
// }

// bool operator== (const VivaNode& t1, const VivaNode& t2)
// {
//   return t1.container->name == t2.container->name;
// }
