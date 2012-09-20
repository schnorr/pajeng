/*
    This file is part of PajeNG

    PajeNG is free software: you can redistribute it and/or modify
    it under the terms of the GNU Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PajeNG is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Public License for more details.

    You should have received a copy of the GNU Public License
    along with PajeNG. If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __PAJE_TREEMAP_H
#define __PAJE_TREEMAP_H
#include "WapajeTreemapView.h"

class WapajeTreemapView;

class WapajeTreemap
{
public:
  PajeContainer *container;

protected:
  double _treemapValue;
  QRectF bb;
  WapajeTreemap *_parent;
  WapajeTreemapView *filter;

public:
  WapajeTreemap (WapajeTreemap *parent, WapajeTreemapView *filter, PajeContainer *container);
  void setRect (QRectF bb) { this->bb = bb; };
  QRectF rect (void) { return bb; };
  double treemapValue (void) const { return _treemapValue; };
  virtual WapajeTreemap *parent (void) const { return _parent; };
  virtual std::vector<WapajeTreemap*> children (void) = 0;
  virtual std::vector<WapajeTreemap*> valueChildren (void) = 0;
  virtual void timeSelectionChanged (void) {};
  virtual void calculateTreemapWithFactor (double factor) {};
  virtual PajeAggregatedType *type (void) { return NULL; };
};

class WapajeTreemapValue : public WapajeTreemap
{
  PajeAggregatedType *_type;

public:
  WapajeTreemapValue (WapajeTreemap *parent, WapajeTreemapView *filter, PajeContainer *container, PajeAggregatedType *type, double value);
  std::vector<WapajeTreemap*> children (void);
  std::vector<WapajeTreemap*> valueChildren (void);
  PajeAggregatedType *type (void) { return _type; };
};


class WapajeTreemapNode : public WapajeTreemap
{
  PajeAggregatedDict values;
  std::vector<WapajeTreemap*> _children; //WapajeTreemapNode
  std::vector<WapajeTreemap*> _valueChildren; //WapajeTreemapValue

public:
  WapajeTreemapNode (WapajeTreemap *parent, WapajeTreemapView *filter, PajeContainer *container);
  void timeSelectionChanged (void);
  void calculateTreemapWithFactor (double factor);
  std::vector<WapajeTreemap*> children (void) { return _children; };
  std::vector<WapajeTreemap*> valueChildren (void) { return _valueChildren; };

protected:
  double worstf (std::vector<WapajeTreemap*> row, double w, double factor);
  QRectF layoutRow (std::vector<WapajeTreemap*> row, double w, QRectF r, double factor);
  std::vector<WapajeTreemap*> prepareChildren (std::vector<WapajeTreemap*> input);
  void squarifyWithOrderedChildren (std::vector<WapajeTreemap*> sorted, double w, double factor);
};

#endif
