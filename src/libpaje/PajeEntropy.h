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
#ifndef __PAJE_ENTROPY_H
#define __PAJE_ENTROPY_H
#include "PajeComponent.h"

class PajeEntropy : public PajeComponent {
public:
  PajeEntropy ();
  ~PajeEntropy ();

protected:
  std::pair<double,std::vector<PajeContainer*> > findBestAggregation (PajeContainer *root, PajeAggregatedType *type, double p);  
};

#endif
