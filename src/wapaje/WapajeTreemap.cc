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
#include "WapajeTreemap.h"
#include <float.h>

WapajeTreemap::WapajeTreemap (WapajeTreemap *parent, WapajeTreemapView *filter, PajeContainer *container)
{
  this->_parent = parent;
  this->filter = filter;
  this->container = container;
  _treemapValue = 0;
}

WapajeTreemapValue::WapajeTreemapValue (WapajeTreemap *parent, WapajeTreemapView *filter, PajeContainer *container, PajeAggregatedType *type, double value)
  : WapajeTreemap (parent, filter, container)
{
  this->_type = type;
  _treemapValue = value;
}

std::vector<WapajeTreemap*> WapajeTreemapValue::children (void)
{
  throw "should not be called for WapajeTreemapValue";
}

std::vector<WapajeTreemap*> WapajeTreemapValue::valueChildren (void)
{
  throw "should not be called for WapajeTreemapValue";
}

WapajeTreemapNode::WapajeTreemapNode (WapajeTreemap *parent, WapajeTreemapView *filter, PajeContainer *container)
  : WapajeTreemap (parent, filter, container)
{
  std::vector<PajeContainer*> subcontainers = filter->enumeratorOfContainersInContainer (container);
  std::vector<PajeContainer*>::iterator sub;
  for (sub = subcontainers.begin(); sub != subcontainers.end(); sub++){
    WapajeTreemapNode *child = new WapajeTreemapNode (this, filter, *sub);
    _children.push_back (child);
  }
}

void WapajeTreemapNode::timeSelectionChanged (void)
{
  //clear and update the values because of the new time slice selection
  values.clear();
  values = filter->spatialIntegrationOfContainer (container);

  //update the treemapValue
  _treemapValue = 0;
  PajeAggregatedDict::iterator val;
  for (val = values.begin(); val != values.end(); val++){
    _treemapValue += (*val).second;
  }

  //update aggregatedChildren since aggregated values have changed
  _valueChildren.clear();
  PajeAggregatedDict::iterator it;
  for (it = values.begin(); it != values.end(); it++){
    PajeAggregatedType *type = (*it).first;
    double value = (*it).second;
    WapajeTreemapValue *valueChild = new WapajeTreemapValue (this, filter, container, type, value);
    _valueChildren.push_back (valueChild);
  }

  //recurse
  std::vector<WapajeTreemap*>::iterator child;
  for (child = _children.begin(); child != _children.end(); child++){
    (*child)->timeSelectionChanged();
  }
}

void WapajeTreemapNode::calculateTreemapWithFactor (double factor)
{
  std::vector<WapajeTreemap*> sortedChildren = this->prepareChildren (_children);
  std::vector<WapajeTreemap*> sortedAggregatedChildren = this->prepareChildren (_valueChildren);

  //calculate the smaller size
  double w = bb.width() < bb.height() ? bb.width() : bb.height();

  //squarify my children
  this->squarifyWithOrderedChildren (sortedChildren, w, factor);
  this->squarifyWithOrderedChildren (sortedAggregatedChildren, w, factor);

  //recurse
  std::vector<WapajeTreemap*>::iterator child;
  for (child = _children.begin(); child != _children.end(); child++){
    (*child)->calculateTreemapWithFactor(factor);
  }
}

bool myfunction (WapajeTreemap *i, WapajeTreemap *j)
{
  return (i->treemapValue() < j->treemapValue());
}

std::vector<WapajeTreemap*> WapajeTreemapNode::prepareChildren (std::vector<WapajeTreemap*> input)
{
  //clear all children bounding boxes
  std::vector<WapajeTreemap*>::iterator it;
  for (it = input.begin(); it != input.end(); it++){
    (*it)->setRect(QRectF(0,0,0,0));
  }

  std::vector<WapajeTreemap*> sorted = input;
  std::sort (sorted.begin(), sorted.end(), myfunction);
  std::reverse (sorted.begin(), sorted.end());

  //remove children with treemapValue equal to zero
  std::vector<WapajeTreemap*>::iterator child;
  std::vector<WapajeTreemap*>::iterator saved = sorted.begin();
  for (child = sorted.begin(); child != sorted.end(); child++){
    if ((*child)->treemapValue() == 0) break;
  }
  sorted.erase (child, sorted.end());
  return sorted;
}


double WapajeTreemapNode::worstf (std::vector<WapajeTreemap*> list, double w, double factor)
{
  double rmax = 0, rmin = FLT_MAX, s = 0;
  std::vector<WapajeTreemap*>::iterator it;
  for (it = list.begin(); it != list.end(); it++){
    WapajeTreemap *child = *it;
    double childValue = child->treemapValue() * factor;
    rmin = rmin < childValue ? rmin : childValue;
    rmax = rmax > childValue ? rmax : childValue;
    s += childValue;
  }
  s = s*s; w = w*w;
  double first = w*rmax/s, second = s/(w*rmin);
  return first > second ? first : second;

}

QRectF WapajeTreemapNode::layoutRow (std::vector<WapajeTreemap*> list, double w, QRectF r, double factor)
{
  double s = 0; // sum
  std::vector<WapajeTreemap*>::iterator it;
  for (it = list.begin(); it != list.end(); it++){
    WapajeTreemap *child = *it;
    s += child->treemapValue() * factor;
  }
  double x = r.topLeft().x(), y = r.topLeft().y(), d = 0;
  double h = w==0 ? 0 : s/w;
  bool horiz = (w == r.width());

  for (it = list.begin(); it != list.end(); it++){
    WapajeTreemap *child = *it;
    QRectF childRect;
    if (horiz){
      childRect.setTopLeft (QPointF(x+d, y));
    }else{
      childRect.setTopLeft (QPointF(x, y+d));
    }
    double nw = child->treemapValue() * factor / h;
    if (horiz){
      childRect.setWidth (nw);
      childRect.setHeight (h);
      d += nw;
    }else{
      childRect.setWidth (h);
      childRect.setHeight (nw);
      d += nw;
    }
    child->setRect (childRect);
  }
  if (horiz){
    r = QRectF (x, y+h, r.width(), r.height()-h);
  }else{
    r = QRectF (x+h, y, r.width()-h, r.height());
  }
  return r;
}

void WapajeTreemapNode::squarifyWithOrderedChildren (std::vector<WapajeTreemap*> list, double w, double factor)
{
  std::vector<WapajeTreemap*> row;
  double worst = FLT_MAX, nworst;
  QRectF r = bb;

  while (list.size()){
    // check if w is still valid
    if (w < 1){
      // w should not be smaller than 1 pixel
      // no space left for other children to appear
      break;
    }

    row.insert (row.begin(), *(list.begin()));
    nworst = this->worstf (row, w, factor);
    if (nworst <= 1){
      // nworst should not be smaller than ratio 1,
      //  which is the perfect square
      break;
    }
    if (nworst <= worst){
      list.erase (list.begin());
      worst = nworst;
    }else{
      row.erase (row.begin());
      r = this->layoutRow (row, w, r, factor);//layout current row
      w = r.width() < r.height() ? r.width() : r.height();
      row.clear();
      worst = FLT_MAX;
    }
  }

  if (row.size()){
    r = this->layoutRow (row, w, r, factor);
    row.clear();
  }
}
