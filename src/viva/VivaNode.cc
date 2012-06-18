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

void VivaComposition::draw (tp_point base)
{
  glTranslatef (base.x, base.y, 0);

  //fill my rectangle
  glColor3f (1.0, 1.0, 1.0);
  glRectf (0, 0, width, -height);

  std::map<PajeType*,double>::iterator it;
  double yaccum = 0;
  for (it = proportion.begin(); it != proportion.end(); it++){
    double size = (*it).second;
    PajeColor *color = filter->colorForEntityType ((*it).first);

    glColor3f (color->r, color->g, color->b);
    glRectf (0, yaccum, width, -height * size);

    yaccum -= size;
  }

  //dark thin border
  glColor3f (0, 0, 0);
  glBegin (GL_LINE_LOOP);
  glVertex2f (0, 0);
  glVertex2f (0, -height);
  glVertex2f (width, -height);
  glVertex2f (width, 0);
  glEnd ();

  glTranslatef (-base.x, -base.y, 0);
}

VivaNode::VivaNode (VivaGraph *filter, PajeContainer *container, config_setting_t *conf, void *layout)
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

VivaNode::VivaNode (VivaGraph *filter, PajeContainer *container, config_setting_t *conf, void *layout, tp_point point)
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
  tp_point pos = node_get_position (node);
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
  node_set_mask (node, mask);
}

void VivaNode::draw (void)
{
  tp_point position = this->position();
  tp_point translate = tp_Point (position.x - width/2,
                                 position.y + height/2);

  glTranslatef(translate.x, translate.y, 0);
  {
    tp_point base = tp_Point (0, 0);
    std::vector<VivaComposition*>::iterator it;
    for (it = compositions.begin(); it != compositions.end(); it++){
      VivaComposition *comp = (*it);
      comp->draw(base);
      base.x += comp->width;
    }
  }
  glTranslatef(-translate.x, -translate.y, 0);
}

void VivaNode::drawEdges ()
{
  tp_point position = this->position();
  int i, count = dynar_count(this->node->connected);
  for (i = 0; i < count; i++){
    tp_node *n = dynar_get_as (this->node->connected, tp_node*, i);
    tp_point nposition = ((VivaNode*)n->data)->position();

    glColor4f (0, 0, 0, 0.1);

    glBegin (GL_LINES);
    glVertex2f (position.x, position.y);
    glVertex2f (nposition.x, nposition.y);
    glEnd ();
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
