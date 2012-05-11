#ifndef __VIVA_NODE_H
#define __VIVA_NODE_H
#include <wx/wx.h>
#include <libconfig.h>
#include "tupi_private.h"
#include "PajeComponent.h"

class VivaGraph;

class VivaComposition
{
private:
  wxRect bb;
  VivaGraph *filter;
  PajeContainer *container;
  PajeType *size_type;
  std::vector<PajeType*> values_type;

public:
  VivaComposition (VivaGraph *filter, PajeContainer *container, config_setting_t *configuration);
  ~VivaComposition ();
  wxRect layout (void);
  void draw (wxDC& dc, wxPoint base);
};

class VivaNode
{
private:
  std::vector<VivaComposition*> compositions;

public:
  tp_node *node;
  tp_layout *tupi_layout;
  PajeContainer *container;
  VivaGraph *filter;

  VivaNode (VivaGraph *filter, PajeContainer *container, config_setting_t *conf, tp_layout *layout);
  ~VivaNode ();

  wxPoint position (void);
  wxRect layout (void);
  void draw (wxDC& dc);
};

/* bool operator!= (const VivaNode& t1, const VivaNode& t2); */
/* bool operator== (const VivaNode& t1, const VivaNode& t2); */

#include "VivaGraph.h"
#endif
