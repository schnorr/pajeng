#include "VivaNode.h"

VivaComposition::VivaComposition (VivaGraph *filter, PajeContainer *container, config_setting_t *configuration)
{
  bb = tp_Rect(0,0,0,0);
  this->filter = filter;
  this->container = container;

  //get configuration name
  name = std::string (config_setting_name (configuration));

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

void VivaComposition::layout (void)
{
  bb = tp_Rect (0, 0, 0, 0);
  if (!filter || !size_type || !container) return;
  std::map<std::string,double> values = filter->spatialIntegrationOfContainer (container);
  if (values.size() == 0) return;

  double max = filter->maxForConfigurationWithName (name);
  double userScale = filter->userScaleForConfigurationWithName (name) * 100; //use a 100 magnification
  double size_var = 2 * COMPOSITION_MAX_SIZE * values[size_type->name]/max;
  double size = sqrt (userScale * size_var);
  bb = tp_Rect (0, 0, size, size);

  proportion.clear();
  std::vector<PajeType*>::iterator it;
  for (it = values_type.begin(); it != values_type.end(); it++){
    PajeType *t = (*it);
    double s = values[t->name];
    if (s){
      proportion[t] = s/values[size_type->name];
    }
  }
}

void VivaComposition::draw (wxDC& dc, tp_point base)
{
  wxColour black;
  black.Set(wxT("#000000"));
  dc.SetPen(wxPen(black));

  bb.origin.x = base.x;
  bb.origin.y = base.y;

  wxRect rect = wxRect (bb.origin.x, bb.origin.y, bb.size.width, bb.size.height);
  dc.SetPen(wxPen(*wxBLACK));
  dc.SetBrush(wxBrush(*wxWHITE));
  dc.DrawRectangle(rect);

  std::map<PajeType*,double>::iterator it;
  for (it = proportion.begin(); it != proportion.end(); it++){
    PajeColor *color = filter->colorForEntityType ((*it).first);
    wxColour c;
    c.Set (255*color->r, 255*color->g, 255*color->b, 255*color->a);
    dc.SetPen(wxPen(c));
    dc.SetBrush(wxBrush(c));
    double size = (*it).second;
    wxRect r = wxRect (bb.origin.x, bb.origin.y, bb.size.width, bb.size.height * size);
    dc.DrawRectangle(r);
  }

  dc.SetPen(wxPen(*wxBLACK));
  dc.SetBrush(*wxTRANSPARENT_BRUSH);
  dc.DrawRectangle(rect);
}

VivaNode::VivaNode (VivaGraph *filter, PajeContainer *container, config_setting_t *conf, tp_layout *layout)
{
  this->node = node_new (container->name().c_str(), this);
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

void VivaNode::layout (void)
{
  bb = tp_Rect(0,0,0,0);

  std::vector<VivaComposition*>::iterator it;
  for (it = compositions.begin(); it != compositions.end(); it++){
    VivaComposition *comp = (*it);
    comp->layout ();
    if (comp->bb.size.height > bb.size.height) bb.size.height = comp->bb.size.height;
    bb.size.width += comp->bb.size.width;
  }

  tp_rect mask = tp_Rect (0, 0, bb.size.width/100, bb.size.height/100);
  particle_set_mask (node->particle, mask);
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

void VivaNode::drawEdges (wxDC& dc)
{
  tp_point myposition = this->position();
  int i, count = dynar_count(this->node->connected);
  for (i = 0; i < count; i++){
    tp_node *n = dynar_get_as (this->node->connected, tp_node*, i);
    tp_point nposition = ((VivaNode*)n->data)->position();
    dc.DrawLine (myposition.x, myposition.y, nposition.x, nposition.y);
  }
}

void VivaNode::setConnectedNodes (std::vector<tp_node*> connectedNodes)
{

  std::vector<tp_node*>::iterator it;
  for (it = connectedNodes.begin(); it != connectedNodes.end(); it++){
    tp_node *n = *it;
    node_connect (node, n);
  }
}

// bool operator!= (const VivaNode& t1, const VivaNode& t2)
// {
//   return t1.container->name != t2.container->name;
// }

// bool operator== (const VivaNode& t1, const VivaNode& t2)
// {
//   return t1.container->name == t2.container->name;
// }
