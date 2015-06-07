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
#ifndef __PAJE_VALUE_H
#define __PAJE_VALUE_H
#include "PajeObject.h"
#include "PajeType.h"

class PajeType;

class PajeValue : public PajeObject {
private:
  PajeColor *_color;
  std::string _name;
  std::string _alias;
  PajeType *_type; //associated type
public:
  PajeValue (std::string name, std::string alias, PajeType *type, PajeColor *color);
  PajeColor *color (void);
  const std::string &name (void);
  const std::string &alias (void);
  const std::string &identifier (void);
  PajeType *type (void);
};

#endif
