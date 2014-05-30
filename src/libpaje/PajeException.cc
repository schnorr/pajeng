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
#include "PajeException.h"
#include <stdlib.h>
#include <typeinfo>

void PajeException::report (void)
{
  std::cout << reason() << std::endl;
}

void PajeException::reportAndExit (void)
{
  report ();
  exit(1);
}

PajeFileReadException::PajeFileReadException (std::string filename)
{
  this->filename = filename;
}

const std::string PajeFileReadException::reason (void) const throw()
{
  std::string exceptionName (typeid(this).name());
  return std::string(exceptionName + ": Could not open '") + filename.c_str() + std::string("' for reading.");
}

PajeDecodeException::PajeDecodeException (std::string message)
{
  this->message = message;
  exceptionName = std::string ("PajeDecodeException");
}

PajeDecodeException::PajeDecodeException ()
{
  exceptionName = std::string ("PajeDecodeException");
}

const std::string PajeDecodeException::reason (void) const throw ()
{
  return std::string(exceptionName + ": " + message);
}

PajeContainerException::PajeContainerException (std::string message) : PajeDecodeException (message)
{
  exceptionName = std::string ("PajeContainerException");
}

PajeTypeException::PajeTypeException (std::string message) : PajeDecodeException (message)
{
  exceptionName = std::string ("PajeTypeException");
}

PajeLinkException::PajeLinkException (std::string message) : PajeDecodeException (message)
{
  exceptionName = std::string ("PajeLinkException");
}

PajeStateException::PajeStateException (std::string message) : PajeDecodeException (message)
{
  exceptionName = std::string ("PajeStateException");
}

PajeVariableException::PajeVariableException (std::string message) : PajeDecodeException (message)
{
  exceptionName = std::string ("PajeVariableException");
}

PajeSimulationException::PajeSimulationException (std::string message) : PajeDecodeException (message)
{
  exceptionName = std::string ("PajeSimulationException");
}

PajeProtocolException::PajeProtocolException (std::string message) : PajeDecodeException (message)
{
  exceptionName = std::string ("PajeProtocolException");
}

PajeDefinitionsException::PajeDefinitionsException (std::string message) : PajeDecodeException (message)
{
  exceptionName = std::string ("PajeDefinitionsException");
}

PajeDefinitionsException::PajeDefinitionsException ()
{
  exceptionName = std::string ("PajeDefinitionsException");
}
