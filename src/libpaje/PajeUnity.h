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
#ifndef __PAJEUNITY_H__
#define __PAJEUNITY_H__
#include "PajeComponent.h"
#include "PajeEventDecoder.h"
#include "PajeSimulator.h"
#include "PajeEventDecoder.h"

class PajeUnity : public PajeComponent {
private:
  PajeComponent *reader;
  PajeEventDecoder *decoder;
  PajeSimulator *simulator;
  PajeDefinitions *definitions;
  bool flexReader;
  double t1, t2;

public:
  PajeUnity (bool flexReader, bool strictHeader, std::string tracefilename, double stopat, int ignoreIncompleteLinks, char *probabilistic);
  ~PajeUnity ();
  double getTime ();
  void report ();
  void reportDot ();
  void reportContainer ();
};

#endif
