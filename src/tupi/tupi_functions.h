/*
    This file is part of Tupi

    Tupi is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tupi is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Tupi.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __TUPI_FUNCTIONS_H
#define __TUPI_FUNCTIONS_H

static inline char *tp_PointDesc (tp_point p1, char *str, int len)
{
  snprintf (str, len, "{%f, %f}", p1.x, p1.y);
  return str;
}

static inline char *tp_RectDesc (tp_rect r, char *str, int len)
{
  snprintf (str, len, "{%f, %f, %f, %f}",
            r.origin.x, r.origin.y,
            r.size.width, r.size.height);
  return str;
}

static inline tp_point tp_CopyPoint (tp_point p1)
{
  tp_point res;
  res.x = p1.x;
  res.y = p1.y;
  return res;
}

static inline tp_point tp_Point (double x, double y)
{
  tp_point res;
  res.x = x;
  res.y = y;
  return res;
}

static inline tp_point tp_AddPoints (tp_point p1, tp_point p2)
{
  tp_point res;
  res.x = p1.x + p2.x;
  res.y = p1.y + p2.y;
  return res;
}

static inline tp_point tp_SubtractPoints (tp_point p1, tp_point p2)
{
  tp_point res;
  res.x = p1.x - p2.x;
  res.y = p1.y - p2.y;
  return res;
}

static inline double tp_DistanceBetweenPoints (tp_point p1, tp_point p2)
{
  double ptxd = p1.x - p2.x;
  double ptyd = p1.y - p2.y;
  double k = ptxd*ptxd + ptyd*ptyd;
  return sqrt(k);//ptxd*ptxd + ptyd*ptyd ); 
}

static inline double tp_LengthPoint (tp_point p)
{
  return sqrt (p.x*p.x + p.y*p.y);
}

static inline tp_point tp_NormalizePoint (tp_point p)
{
  tp_point res = tp_Point(0,0);
  double len = tp_LengthPoint (p);
  if (len != 0) {
    res.x = p.x/len;
    res.y = p.y/len;
  }else{
    res.x = 0;
    res.y = 0;
  }
  return res;
}

static inline tp_point tp_MultiplyPoint (tp_point p, double val)
{
  tp_point res;
  res.x = p.x * val;
  res.y = p.y * val;
  return res;
}

static inline tp_rect tp_CopyRect (tp_rect r1)
{
  tp_rect res2;
  res2.origin.x = r1.origin.x;
  res2.origin.y = r1.origin.y;
  res2.size.width = r1.size.width;
  res2.size.height = r1.size.height;
  return res2;
}

static inline tp_rect tp_Rect (double x, double y, double width, double height)
{
  tp_rect res;
  res.origin.x = x;
  res.origin.y = y;
  res.size.width = width;
  res.size.height = height;
  return res;
}

static inline double tp_DiagonalRect (tp_rect area)
{
  tp_point p1;
  tp_point p2;
  p1.x = area.origin.x;
  p1.y = area.origin.y;
  p2.x = area.origin.x + area.size.width;
  p2.y = area.origin.y + area.size.height;
  return tp_DistanceBetweenPoints (p1, p2);
}

static inline tp_rect tp_GrowCenterPoint (tp_point p, double val)
{
  tp_rect grown;
  grown.origin.x = p.x - val;
  grown.origin.y = p.y - val;
  grown.size.width = 2 * val;
  grown.size.height = 2 * val;
  return grown;
}

static inline double tp_MaxX (tp_rect a)
{
  return a.origin.x + a.size.width;
}

static inline double tp_MaxY (tp_rect a)
{
  return a.origin.y + a.size.height;
}

static inline double tp_MinX (tp_rect a)
{
  return a.origin.x;
}

static inline double tp_MinY (tp_rect a)
{
  return a.origin.y;
}

static inline tp_rect tp_UnionRect (tp_rect r1, tp_rect r2)
{
  tp_rect rect;
  rect = tp_Rect (fmin(tp_MinX(r1), tp_MinX(r2)),
                  fmin(tp_MinY(r1), tp_MinY(r2)),
                  0,
                  0);
  return tp_Rect (tp_MinX(rect),
                  tp_MinY(rect),
                  fmax(tp_MaxX(r1), tp_MaxX(r2)) - tp_MinX(rect),
                  fmax(tp_MaxY(r1), tp_MaxY(r2)) - tp_MinY(rect));
}

static inline int tp_IntersectsRect (tp_rect a, tp_rect b)
{
  return (tp_MaxX(a) <= tp_MinX(b)
          || tp_MaxX(b) <= tp_MinX(a)
          || tp_MaxY(a) <= tp_MinY(b)
          || tp_MaxY(b) <= tp_MinY(a)) ? 0 : 1;
}

static inline int tp_PointInRect (tp_point p, tp_rect r)
{
  tp_rect r2 = tp_CopyRect(r);
  tp_point p2 = tp_CopyPoint(p);
  int cond = ((float)p2.x >= (float)tp_MinX(r2) &&
              (float)p2.x <= (float)tp_MaxX(r2) &&
              (float)p2.y >= (float)tp_MinY(r2) &&
              (float)p2.y <= (float)tp_MaxY(r2)) ? 1 : 0;
  /* printf ("%s p(%f,%f) r(%f,%f,%f,%f) => %d\n", __FUNCTION__, */
  /*         p.x, p.y, r.origin.x, r.origin.y, r.size.width, r.size.height, cond); */
  return cond;
}


static inline int tp_PointInRect2 (tp_point p, tp_rect r)
{
  tp_rect r2 = tp_CopyRect(r);
  float minx = tp_MinX(r2);
  float maxx = tp_MaxX(r2);
  float miny = tp_MinY(r2);
  float maxy = tp_MaxY(r2);
  float x = p.x;
  float y = p.y;

  int cond2 = (x >= minx &&
               x <= maxx &&
               y >= miny &&
               y <= maxy) ? 1 : 0;

  int cond = (p.x >= tp_MinX(r2) &&
              p.x <= tp_MaxX(r2) &&
              p.y >= tp_MinY(r2) &&
              p.y <= tp_MaxY(r2)) ? 1 : 0;
  printf ("%s p(%f,%f) r(%f,%f,%f,%f) => %d\n", __FUNCTION__,
          p.x, p.y, r.origin.x, r.origin.y, r.size.width, r.size.height, cond2);
  return cond2;
}


static inline double gettime ()
{
  struct timeval tr;
  gettimeofday(&tr, NULL);
  return (double)tr.tv_sec+(double)tr.tv_usec/1000000;
}

#endif //__TUPI_FUNCTIONS_H
