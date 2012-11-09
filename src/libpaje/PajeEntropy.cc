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
#include "PajeEntropy.h"

PajeEntropy::PajeEntropy ()
{
}

PajeEntropy::~PajeEntropy ()
{
}

std::pair<double,std::vector<PajeContainer*> > PajeEntropy::findBestAggregation (PajeContainer *root, PajeAggregatedType *type, double p, bool falseGain)
{
  double maxRIC;
  std::pair<double,std::vector<PajeContainer*> > bestAgg;

  PajeAggregatedDict::iterator git;
  double g = 0;
  if (falseGain){
    g = root->gain[type];
  }else{
    for (git = root->gain.begin(); git != root->gain.end(); git++){
      PajeAggregatedType *type = (*git).first;
      g += root->gain[type];
    }
  }
  double d = 0;
  for (git = root->div.begin(); git != root->div.end(); git++){
    PajeAggregatedType *type = (*git).first;
    d += root->div[type];
  }

  //calculate for root
  std::pair<double,std::vector<PajeContainer*> > rootAgg;
  rootAgg.first = p * g - (1 - p) * d;
  rootAgg.second.push_back(root);

  //recursion control
  std::vector<PajeContainer*>::iterator it;
  std::vector<PajeContainer*> children = root->getChildren();
  if (children.empty()){
    bestAgg = rootAgg;
  }else{
    std::pair<double,std::vector<PajeContainer*> > childrenBestAgg;
    for (it = children.begin(); it != children.end(); it++){
      PajeContainer *child = *it;
      std::pair<double,std::vector<PajeContainer*> > result = findBestAggregation (child, type, p, falseGain);
      childrenBestAgg.first += result.first;
      childrenBestAgg.second.insert (childrenBestAgg.second.end(), result.second.begin(), result.second.end());
    }

    if (rootAgg.first  > childrenBestAgg.first){
      bestAgg = rootAgg;
    }else{
      bestAgg = childrenBestAgg;
    }
  }
  return bestAgg;
}
