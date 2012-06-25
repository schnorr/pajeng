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
#ifndef __VIVA_NODE_H
#define __VIVA_NODE_H
#include <GL/gl.h>
#include <libconfig.h>
#include "tupi.h"
#include "PajeComponent.h"

#define COMPOSITION_MAX_SIZE 200
#define COMPOSITION_DEFAULT_USER_SCALE 0.5

class VivaGraph;

class VivaComposition
{
public:
  double width;
  double height;
  std::map<PajeType*,double> proportion;
private:
  VivaGraph *filter;
  PajeContainer *container;
  PajeType *size_type;
  std::string name;
  std::vector<PajeType*> values_type;

public:
  VivaComposition (VivaGraph *filter, PajeContainer *container, config_setting_t *configuration);
  ~VivaComposition ();
  void layout (void);
  void draw (tp_point base);
};

class VivaNode
{
private:
  std::vector<VivaComposition*> compositions;

  void createCompositions (config_setting_t *conf);

public:
  double width;
  double height;
  tp_node *node;
  void *tupi_layout;
  PajeContainer *container;
  VivaGraph *filter;

  VivaNode (VivaGraph *filter, PajeContainer *container, config_setting_t *conf, void *layout);
  VivaNode (VivaGraph *filter, PajeContainer *container, config_setting_t *conf, void *layout, tp_point point);
  ~VivaNode ();

  tp_point position (void);
  void layout (void);
  void draw (void);
  void drawEdges (void);
  bool hasPoint (double x, double y);
  void setConnectedNodes (std::vector<tp_node*> connectedNodes);
};

/* bool operator!= (const VivaNode& t1, const VivaNode& t2); */
/* bool operator== (const VivaNode& t1, const VivaNode& t2); */

#include "VivaGraph.h"
#endif
