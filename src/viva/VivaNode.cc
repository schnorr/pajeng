#include "VivaNode.h"

VivaComposition::VivaComposition (VivaGraph *filter, PajeContainer *container, config_setting_t *configuration)
{
  width = 0;
  height = 0;
  this->filter = filter;
  this->container = container;

  //get configuration name
  name = std::string (config_setting_name (configuration));

  //get configuration
  config_setting_t *type = config_setting_get_member (configuration, "type");
  if (!type){
    throw "The 'type' field is not defined for configuration '"+name+"'";
  }
  if (config_setting_type (type) != CONFIG_TYPE_STRING){
    throw "The 'type' field is not a string in configuration '"+name+"'";
  }

  config_setting_t *size = config_setting_get_member (configuration, "size");
  if (!size){
    throw "The 'size' field is not defined for configuration '"+name+"'";
  }
  if (config_setting_type (size) != CONFIG_TYPE_STRING){
    throw "The 'size' field is not a string in configuration '"+name+"'";
  }

  config_setting_t *values = config_setting_get_member (configuration, "values");
  if (!values){
    throw "The 'values' field is not defined for configuration '"+name+"'";
  }
  if (config_setting_type (values) != CONFIG_TYPE_LIST){
    throw "The 'values' field is not a list in configuration '"+name+"'";
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
    if (config_setting_type (value) != CONFIG_TYPE_STRING){
      continue;
    }
    std::string value_typename (config_setting_get_string (value));
    PajeType *value_type = filter->entityTypeWithName (value_typename);
    if (!value_type){
      continue;
    }
    values_type.push_back (value_type);
  }
}

VivaComposition::~VivaComposition (void)
{
  values_type.clear();
  proportion.clear();
  this->filter = NULL;
  this->container = NULL;
  this->size_type = NULL;
  name = std::string ("deleted composition");
}

void VivaComposition::layout (void)
{
  width = 0;
  height = 0;

  if (!filter || !size_type || !container) return;
  std::map<std::string,double> values = filter->spatialIntegrationOfContainer (container);
  if (values.size() == 0) return;

  double max = filter->maxForConfigurationWithName (name);
  double userScale = filter->userScaleForConfigurationWithName (name) * 100; //use a 100 magnification
  double size_var = 2 * COMPOSITION_MAX_SIZE * values[size_type->name]/max;
  double size = sqrt (userScale * size_var);

  //update the new width and height
  width = size;
  height = size;

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

void VivaComposition::draw (cairo_t *cr, tp_point base)
{
  cairo_translate (cr, base.x, base.y);

  //fill my rectangle
  cairo_set_source_rgb (cr, 1, 1, 1);
  cairo_rectangle (cr, 0, 0, width, height);
  cairo_fill (cr);

  std::map<PajeType*,double>::iterator it;
  double yaccum = 0;
  for (it = proportion.begin(); it != proportion.end(); it++){
    double size = (*it).second;
    PajeColor *color = filter->colorForEntityType ((*it).first);

    cairo_set_source_rgb (cr, color->r, color->g, color->b);
    cairo_rectangle (cr, 0, yaccum, width, height * size);
    cairo_fill (cr);

    yaccum += size;
  }

  //dark thin border
  cairo_set_source_rgb (cr, 0, 0, 0);
  cairo_rectangle (cr, 0, 0, width, height);
  cairo_set_line_width (cr, 1);
  cairo_stroke (cr);

  cairo_translate (cr, -base.x, -base.y);
}

VivaNode::VivaNode (VivaGraph *filter, PajeContainer *container, config_setting_t *conf, tp_layout *layout)
{
  this->width = 0;
  this->height = 0;
  this->node = node_new (container->name().c_str(), this);
  this->tupi_layout = layout;
  this->container = container;
  this->filter = filter;
  layout_add_node (layout, this->node);
  this->createCompositions (conf);
}

VivaNode::VivaNode (VivaGraph *filter, PajeContainer *container, config_setting_t *conf, tp_layout *layout, tp_point point)
{
  this->width = 0;
  this->height = 0;
  this->node = node_new (container->name().c_str(), this);
  this->tupi_layout = layout;
  this->container = container;
  this->filter = filter;
  //transform point to tupi space
  point.x /= 100;
  point.y /= 100;
  std::cout << __FUNCTION__ << " " << container->name()  << " (" << point.x << ", " << point.y << ")" << std::endl;
  layout_add_node_with_point (layout, this->node, point);
  this->createCompositions (conf);
}

VivaNode::~VivaNode ()
{
  //delete all compositions
  std::vector<VivaComposition*>::iterator it;
  for (it = compositions.begin(); it != compositions.end(); it++){
    VivaComposition *comp = (*it);
    delete comp;
  }
  compositions.clear();

  //remove node from the particle system
  layout_remove_node (tupi_layout, this->node);

  //free the node of the particle system
  node_free (this->node);

  //set to NULL
  this->node = NULL;
  this->tupi_layout = NULL;
  this->container = NULL;
  this->filter = NULL;
  this->width = -1;
  this->height = -1;
}

void VivaNode::createCompositions (config_setting_t *conf)
{
  //use configuration to create compositions
  int i;
  for (i = 0; i < config_setting_length (conf); i++){
    config_setting_t *child = config_setting_get_elem (conf, i);
    VivaComposition *comp = new VivaComposition (filter, container, child);
    compositions.push_back (comp);
  }
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
  width = 0;
  height = 0;

  std::vector<VivaComposition*>::iterator it;
  for (it = compositions.begin(); it != compositions.end(); it++){
    VivaComposition *comp = (*it);
    comp->layout ();
    if (comp->height > height) height = comp->height;
    width += comp->width;
  }

  tp_rect mask = tp_Rect (0, 0, width/100, height/100);
  particle_set_mask (node->particle, mask);
}

void VivaNode::draw (cairo_t *cr)
{
  tp_point position = this->position();
  tp_point translate = tp_Point (position.x - width/2,
                                 position.y - height/2);

  //apply node transformation matrix
  cairo_translate (cr, translate.x, translate.y);

  tp_point base = tp_Point (0, 0);

  std::vector<VivaComposition*>::iterator it;
  for (it = compositions.begin(); it != compositions.end(); it++){
    VivaComposition *comp = (*it);
    comp->draw(cr, base);
    base.x += comp->width;
  }

  // TODO, FIXME: draw the name of the node

  //unde node transformation matrix
  cairo_translate (cr, -translate.x, -translate.y);
}

void VivaNode::drawEdges (cairo_t *cr)
{
  tp_point myposition = this->position();
  int i, count = dynar_count(this->node->connected);
  for (i = 0; i < count; i++){
    tp_node *n = dynar_get_as (this->node->connected, tp_node*, i);
    tp_point nposition = ((VivaNode*)n->data)->position();

    cairo_move_to (cr, myposition.x, myposition.y);
    cairo_line_to (cr, nposition.x, nposition.y);
    cairo_set_source_rgba (cr, 0, 0, 0, 0.1);
    cairo_set_line_width (cr, 0.5);
    cairo_stroke (cr);
  }
}

void VivaNode::setConnectedNodes (std::vector<tp_node*> connectedNodes)
{
  //clear current connections
  node_connect_clear (node);

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
