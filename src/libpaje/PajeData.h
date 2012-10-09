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
#ifndef __PAJEDATA_H__
#define __PAJEDATA_H__
#include "PajeObject.h"
#include <stdio.h>
#include <stdlib.h>

class PajeData : public PajeObject {
public:
  char *bytes;
  int length;
  int capacity;

  PajeData (int capacity);
  ~PajeData (void);
  void increaseCapacityOf (int additionalCapacity);
};

#endif
