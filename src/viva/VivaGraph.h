#ifndef __VIVA_GRAPH_H
#define __VIVA_GRAPH_H
#include "PajeComponent.h"

class VivaGraph : public PajeComponent 
{
public:
  VivaGraph ();
  void inputEntity (PajeObject *data);

protected:
  void hierarchyChanged (void);
};

#endif
