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
#ifndef __PAJEOBJECT_H__
#define __PAJEOBJECT_H__
#include <iostream>

/* abstract class for all Paje objects */
class PajeVirtualObject {
  virtual bool isPajeObject (void) = 0;
};

class PajeObject : PajeVirtualObject {
  bool isPajeObject (void);
};

#endif
