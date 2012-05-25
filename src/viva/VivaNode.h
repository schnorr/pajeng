#ifndef __VIVA_NODE_H
#define __VIVA_NODE_H
#include <wx/wx.h>
#include <libconfig.h>
#include "tupi_private.h"
#include "PajeComponent.h"

#define COMPOSITION_MAX_SIZE 200
#define COMPOSITION_DEFAULT_USER_SCALE 0.5

class VivaGraph;

class VivaComposition
{
public:
  tp_rect bb;
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
  void draw (wxDC& dc, tp_point base);
};

class VivaNode
{
private:
  std::vector<VivaComposition*> compositions;

public:
  tp_rect bb;
  tp_node *node;
  tp_layout *tupi_layout;
  PajeContainer *container;
  VivaGraph *filter;

  VivaNode (VivaGraph *filter, PajeContainer *container, config_setting_t *conf, tp_layout *layout);
  ~VivaNode ();

  tp_point position (void);
  void layout (void);
  void draw (wxDC& dc);
  void drawEdges (wxDC& dc);
  bool hasPoint (double x, double y);
  void setConnectedNodes (std::vector<tp_node*> connectedNodes);
};

/* bool operator!= (const VivaNode& t1, const VivaNode& t2); */
/* bool operator== (const VivaNode& t1, const VivaNode& t2); */

#include "VivaGraph.h"
#endif
