#include "PajeTreemap.h"
#include <float.h>

PajeTreemap::PajeTreemap (PajeTreemap *parent, PajeContainer *container, PajeTreemapView *filter)
{
  this->parent = parent;
  this->container = container;
  this->filter = filter;

  std::vector<PajeContainer*> subcontainers = filter->enumeratorOfContainersInContainer (container);
  std::vector<PajeContainer*>::iterator sub;
  for (sub = subcontainers.begin(); sub != subcontainers.end(); sub++){
    PajeTreemap *child = new PajeTreemap (this, *sub, filter);
    children.push_back (child);
  }
}

void PajeTreemap::recursiveTimeSelectionChanged (void)
{
  //clear and update the values because of the new time slice selection
  values.clear();
  values = filter->spatialIntegrationOfContainer (container);

  //recurse
  std::vector<PajeTreemap*>::iterator child;
  for (child = children.begin(); child != children.end(); child++){
    (*child)->recursiveTimeSelectionChanged();
  }
}

void PajeTreemap::recursiveSetTreemapValue (void)
{
  treemapValue = 0;

  std::map<PajeAggregatedType*,double>::iterator val;
  for (val = values.begin(); val != values.end(); val++){
    treemapValue += (*val).second;
  }

  std::vector<PajeTreemap*>::iterator child;
  for (child = children.begin(); child != children.end(); child++){
    (*child)->recursiveSetTreemapValue();
  }
}

bool myfunction (PajeTreemap *i, PajeTreemap *j)
{
  return (i->treemapValue < j->treemapValue);
}

void PajeTreemap::recursiveCalculateTreemapWithFactor (double factor)
{
  //clear all children bounding boxes
  std::vector<PajeTreemap*>::iterator it;
  for (it = children.begin(); it != children.end(); it++){
    (*it)->setBB(QRectF(0,0,0,0));
  }

  std::vector<PajeTreemap*> sorted = children;
  std::sort (sorted.begin(), sorted.end(), myfunction);
  std::reverse (sorted.begin(), sorted.end());

  //remove children with treemapValue equal to zero
  std::vector<PajeTreemap*>::iterator child;
  std::vector<PajeTreemap*>::iterator saved = sorted.begin();
  for (child = sorted.begin(); child != sorted.end(); child++){
    if ((*child)->treemapValue == 0) break;
  }
  sorted.erase (child, sorted.end());

  //calculate the smaller size
  double w = bb.width() < bb.height() ? bb.width() : bb.height();

  //squarify my children
  this->squarifyWithOrderedChildren (sorted, w, factor);

  //recurse
  for (child = children.begin(); child != children.end(); child++){
    (*child)->recursiveCalculateTreemapWithFactor(factor);
  }
}

void PajeTreemap::setBB (QRectF bb)
{
  this->bb = bb;
}

double PajeTreemap::worstf (std::vector<PajeTreemap*> list, double w, double factor)
{
  double rmax = 0, rmin = FLT_MAX, s = 0;
  std::vector<PajeTreemap*>::iterator it;
  for (it = list.begin(); it != list.end(); it++){
    PajeTreemap *child = *it;
    double childValue = child->treemapValue * factor;
    rmin = rmin < childValue ? rmin : childValue;
    rmax = rmax > childValue ? rmax : childValue;
    s += childValue;
  }
  s = s*s; w = w*w;
  double first = w*rmax/s, second = s/(w*rmin);
  return first > second ? first : second;

}

QRectF PajeTreemap::layoutRow (std::vector<PajeTreemap*> list, double w, QRectF r, double factor)
{
  double s = 0; // sum
  std::vector<PajeTreemap*>::iterator it;
  for (it = list.begin(); it != list.end(); it++){
    PajeTreemap *child = *it;
    s += child->treemapValue * factor;
  }
  double x = r.topLeft().x(), y = r.topLeft().y(), d = 0;
  double h = w==0 ? 0 : s/w;
  bool horiz = (w == r.width());

  for (it = list.begin(); it != list.end(); it++){
    PajeTreemap *child = *it;
    QRectF childRect;
    if (horiz){
      childRect.setTopLeft (QPointF(x+d, y));
    }else{
      childRect.setTopLeft (QPointF(x, y+d));
    }
    double nw = child->treemapValue * factor / h;
    if (horiz){
      childRect.setWidth (nw);
      childRect.setHeight (h);
      d += nw;
    }else{
      childRect.setWidth (h);
      childRect.setHeight (nw);
      d += nw;
    }
    child->setBB (childRect);
  }
  if (horiz){
    r = QRectF (x, y+h, r.width(), r.height()-h);
  }else{
    r = QRectF (x+h, y, r.width()-h, r.height());
  }
  return r;
}

void PajeTreemap::squarifyWithOrderedChildren (std::vector<PajeTreemap*> list, double w, double factor)
{
  std::vector<PajeTreemap*> row;
  double worst = FLT_MAX, nworst;
  QRectF r = bb;

  while (list.size()){
    // check if w is still valid
    if (w < 1){
      /* w should not be smaller than 1 pixel
         no space left for other children to appear */
      break;
    }

    row.insert (row.begin(), *(list.begin()));
    nworst = this->worstf (row, w, factor);
    if (nworst <= 1){
      /* nworst should not be smaller than ratio 1,
         which is the perfect square */
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
