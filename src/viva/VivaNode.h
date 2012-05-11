#ifndef __VIVA_NODE_H
#define __VIVA_NODE_H
#include <wx/wx.h>
#include "tupi_private.h"
#include "PajeComponent.h"

class VivaNode
{
public:
  tp_node *node;
  tp_layout *tupi_layout;
  PajeContainer *container;

  VivaNode (PajeContainer *container, tp_layout *layout);
  ~VivaNode ();

  wxPoint position (void);
};

/* bool operator!= (const VivaNode& t1, const VivaNode& t2); */
/* bool operator== (const VivaNode& t1, const VivaNode& t2); */

#endif
