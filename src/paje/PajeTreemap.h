#ifndef __PAJE_TREEMAP_H
#define __PAJE_TREEMAP_H
#include "PajeTreemapView.h"

class PajeTreemapView;

class PajeTreemap
{
public:
  PajeTreemap *parent;
  PajeTreemapView *filter;
  std::vector<PajeTreemap*> children;
  std::map<PajeAggregatedType*,double> values;

public:
  PajeContainer *container;
  double treemapValue;

public:
  PajeTreemap (PajeTreemap *parent, PajeContainer *container, PajeTreemapView *filter);
  void recursiveTimeSelectionChanged (void);
  void recursiveSetTreemapValue (void); //bottom-up
  void recursiveCalculateTreemapWithFactor (double factor);
  void setBB (QRectF bb);
  QRectF rect (void) { return bb; };

protected:
  QRectF bb;
  double worstf (std::vector<PajeTreemap*> row, double w, double factor);
  QRectF layoutRow (std::vector<PajeTreemap*> row, double w, QRectF r, double factor);
  void squarifyWithOrderedChildren (std::vector<PajeTreemap*> sorted, double w, double factor);
};

#endif
