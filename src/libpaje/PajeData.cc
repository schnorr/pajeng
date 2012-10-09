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
#include "PajeData.h"

PajeData::PajeData (int capacity)
{
  this->bytes = (char*) malloc (capacity * sizeof(char));
  this->capacity = capacity;
  this->length = 0;
}

PajeData::~PajeData (void)
{
  free (bytes);
}

void PajeData::increaseCapacityOf (int additionalCapacity)
{
  this->capacity += additionalCapacity;
  this->bytes = (char*) realloc (this->bytes, capacity * sizeof(char));
}
