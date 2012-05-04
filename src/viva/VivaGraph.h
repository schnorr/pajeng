#ifndef __VIVA_GRAPH_H
#define __VIVA_GRAPH_H
#include <wx/wx.h> //to VivaNode
#include <algorithm>
#include "graphview.h"
#include "PajeComponent.h"
#include "tupi_private.h"

class GraphView;

class VivaNode
{
public:
  tp_node *node;
  tp_layout *layout;
  PajeContainer *container;

  VivaNode (PajeContainer *container, tp_layout *layout);
  ~VivaNode ();

  wxPoint position (void);
};

/* bool operator!= (const VivaNode& t1, const VivaNode& t2); */
/* bool operator== (const VivaNode& t1, const VivaNode& t2); */

class VivaGraph : public PajeComponent 
{
private:
  GraphView *view;
  tp_layout *layout;

public:
  std::vector<VivaNode*> nodes;

public:
  VivaGraph ();
  ~VivaGraph ();
  void setView (GraphView *view);
  void mouseClicked (wxPoint point);

protected:
  void hierarchyChanged (void);
};

#endif
