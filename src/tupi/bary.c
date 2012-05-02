/*
    This file is part of Tupi

    Tupi is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tupi is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Tupi.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "tupi_private.h"

tp_bary *bary_new (tp_cell *cell)
{
  tp_bary *ret = (tp_bary*) malloc (sizeof(tp_bary));
  ret->weight = 0;
  ret->center = tp_Point(0,0);
  ret->cell = cell;
  return ret;
}

void bary_free (tp_bary *bary)
{
  free (bary);
}

void bary_recompute (tp_bary *bary)
{
  unsigned long i;
  tp_point new_center = tp_Point(0,0);
  double new_weight = 0;
  tp_cell *cell = bary->cell;

  if (cell == NULL){
    TUPI_DIE("bary with no cell");
  }

  if (cell->sub == NULL){

    for (i = 0; i < dynar_count(cell->particles); i++){
      tp_particle *p = dynar_get_as (cell->particles, tp_particle*, i);
      new_center = tp_AddPoints (new_center, p->position);
      new_weight += p->weight;
    }

    if (dynar_count(cell->particles) > 0){
      new_center = tp_MultiplyPoint (new_center,
                                     1.0/dynar_count(cell->particles));
    }

  }else{

    int subcellPopCount = 0;
    for (i = 0; i < dynar_count(cell->sub); i++){
      tp_cell *subcell = dynar_get_as (cell->sub, tp_cell*, i);
      tp_point p = tp_MultiplyPoint (subcell->bary->center,
                                     subcell->population);
      new_center = tp_AddPoints (new_center, p);
      new_weight += bary->weight;
      subcellPopCount += subcell->population;
    }

    if (cell->population != subcellPopCount){
      TUPI_DIE("Discrepancy in population counts ?");
    }
    if (cell->population > 0){
      new_center = tp_MultiplyPoint (new_center, 1.0/cell->population);
    }
  }
  bary->weight = new_weight;
  bary->center.x = new_center.x;
  bary->center.y = new_center.y;
}

