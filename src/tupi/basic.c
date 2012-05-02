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
#include "tupi_private.h"

tp_rect space_newsubcell_space (tp_rect universe, int index)
{
  double x = universe.origin.x;
  double y = universe.origin.y;
  double w = universe.size.width;
  double h = universe.size.height;
  switch (index){
  case 0:
    return tp_Rect(x, y, w/2, h/2);
    break;
  case 1:
    return tp_Rect(x+w/2, y, w/2, h/2);
    break;
  case 2:
    return tp_Rect(x, y+h/2, w/2, h/2);
    break;
  case 3:
    return tp_Rect(x+w/2, y+h/2, w/2, h/2);
    break;
  default: 
    TUPI_DIE("invalid sub division identifier provided");
    break;
  }
  TUPI_DIE("should not reach here");
  return tp_Rect(0,0,0,0);
}

void tupi_exception (const char *func,
                     const char *file,
                     const int line,
                     const char *error)
{
  fprintf (stderr, "TupiException at %s (%s:%d): [%s]\n",
           func, file, line, error);
  exit(1);
}
