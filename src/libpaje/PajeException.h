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
#ifndef __PAJE_EXCEPTION_H__
#define __PAJE_EXCEPTION_H__
#include <iostream>
#include <exception>
#include "PajeObject.h"

class PajeException : public PajeObject
{
public:
  virtual const std::string reason () const throw () = 0;
  void report (void);
  void reportAndExit (void);
};

class PajeFileReadException : public PajeException
{
private:
  std::string filename;

public:
  PajeFileReadException (std::string filename);
  const std::string reason () const throw ();
};

class PajeDecodeException : public PajeException
{
private:
  std::string message;

protected:
  std::string exceptionName;

public:
  PajeDecodeException (std::string message);
  PajeDecodeException ();
  const std::string reason () const throw ();
};

class PajeContainerException : public PajeDecodeException
{
public:
  PajeContainerException (std::string message);
};

class PajeTypeException : public PajeDecodeException
{
public:
  PajeTypeException (std::string message);
};

class PajeLinkException : public PajeDecodeException
{
public:
  PajeLinkException (std::string message);
};

class PajeStateException : public PajeDecodeException
{
public:
  PajeStateException (std::string message);
};

class PajeVariableException : public PajeDecodeException
{
public:
  PajeVariableException (std::string message);
};

class PajeSimulationException : public PajeDecodeException
{
public:
  PajeSimulationException (std::string message);
};

class PajeProtocolException : public PajeDecodeException
{
public:
  PajeProtocolException (std::string message);
};

class PajeDefinitionsException : public PajeDecodeException
{
 public:
  PajeDefinitionsException (std::string message);
  PajeDefinitionsException ();
};

#endif
