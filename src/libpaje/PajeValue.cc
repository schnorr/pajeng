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
#include "PajeValue.h"

PajeValue::PajeValue (std::string name, std::string alias, PajeType *type, PajeColor *color)
{
  _color = color;
  _name = name;
  _alias = alias;
  _type = type;
}

PajeColor *PajeValue::color (void)
{
  return _color;
}

const std::string &PajeValue::name (void)
{
  return _name;
}

const std::string &PajeValue::alias (void)
{
  return _alias;
}

const std::string &PajeValue::identifier (void)
{
  if (alias().empty()){
    return _name;
  }else{
    return _alias;
  }
}

PajeType *PajeValue::type (void)
{
  return _type;
}
