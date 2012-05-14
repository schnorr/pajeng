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

tp_particle *particle_new (const char *name, tp_layout *layout, tp_box *box, tp_node *node)
{
  tp_particle *ret = (tp_particle*) malloc (sizeof(tp_particle));
  ret->name = strdup (name);
  ret->position.x = drand48() * 2 * layout->k - layout->k;
  ret->position.y = drand48() * 2 * layout->k - layout->k;
  ret->mask = tp_Rect (0,0,0,0);
  ret->weight = 1;
  ret->cell = NULL;
  ret->layout = layout;
  ret->box = box;
  ret->frozen = 0;
  ret->disp = tp_Point (0, 0);
  ret->len = 0;

  ret->node = node;
  return ret;
}

void particle_set_mask (tp_particle *particle, tp_rect mask)
{
  particle->mask = mask;
}

void particle_free (tp_particle *p)
{
  free (p->name);
  free (p);
}

int particle_compare (const void *p1, const void *p2)
{
  return strcmp (((tp_particle*)p1)->name, ((tp_particle*)p2)->name);
}

void particle_attraction (tp_particle *particle)
{
  int connected = dynar_count (particle->node->connected);
  if (connected == 0){
    return;
  }

  tp_layout *l = particle->layout;
  unsigned long i;
  for (i = 0; i < dynar_count(particle->node->connected); i++){
    tp_node *node = dynar_get_as (particle->node->connected, tp_node*, i);
    tp_particle *p = node->particle;
    if (p == NULL){
      TUPI_DIE("corresponding particle of graph node is not defined");
    }
    tp_point n1p = particle->position;
    tp_point n2p = p->position;
    tp_point dif = tp_SubtractPoints (n1p, n2p);
    tp_point normalized = tp_NormalizePoint (dif);
    double distance = tp_DistanceBetweenPoints (n1p, n2p);

    double factor = (l->K1 * (distance - l->k));
    factor /= connected * 0.1;

    particle->disp = tp_AddPoints (particle->disp,
                                   tp_MultiplyPoint (normalized, -factor));
    particle->layout->energy += factor;
  }
}

void particle_bary_repulsion (tp_particle *p1, tp_bary *bary)
{
  tp_layout *layout = p1->layout;

  tp_point n1p = p1->position;
  tp_point n2p = bary->center;
  tp_point dif = tp_SubtractPoints (n1p, n2p);
  tp_point normalized = tp_NormalizePoint (dif);
  double distance = tp_DistanceBetweenPoints (n1p, n2p);

  if (distance > 0) {
    if (distance < layout->k){
      distance = layout->k;
    }
  }
  double factor = distance != 0 ? ((layout->K2 / (distance*distance))) * bary->weight : 0.00001;
  normalized = tp_MultiplyPoint (normalized, factor);
  p1->disp = tp_AddPoints (p1->disp, normalized);
  p1->layout->energy += factor;
}

void particle_particle_repulsion (tp_particle *p1, tp_particle *p2)
{
  tp_layout *layout = p1->layout;

  tp_point n1p = p1->position;
  tp_point n2p = p2->position;
  tp_point dif = tp_SubtractPoints (n1p, n2p);
  tp_point normalized = tp_NormalizePoint (dif);
  double distance = tp_DistanceBetweenPoints (n1p, n2p);

  if (distance > 0){
    if (distance < p1->layout->k){
      distance = p1->layout->k;
    }
  }
  double factor = distance != 0 ? (layout->K2/(distance*distance)) * p2->weight : 0.00001;
  normalized = tp_MultiplyPoint (normalized, factor);
  p1->disp = tp_AddPoints (p1->disp, normalized);
  p1->layout->energy += factor;
}

void particle_repulsion_N2 (tp_particle *particle)
{
  tp_box *box = particle->box;

  unsigned long i;
  for (i = 0; i < dynar_count(box->particles_array); i++){
    tp_particle *p = dynar_get_as (box->particles_array, tp_particle*, i);
    if (particle_compare (particle, p) != 0){
      particle_particle_repulsion (particle, p);
    }
  }
}

int particle_intersection_with_cell (tp_particle *p, tp_cell *c)
{
  double k = p->layout->k;
  double vz = p->layout->viewZone;

  tp_rect grown = tp_GrowCenterPoint (p->position,
                                      p->layout->k * p->layout->viewZone);
  return tp_IntersectsRect (c->space, grown);
}

void particle_repulsion_with_cell (tp_particle *particle, tp_cell *cell)
{
  unsigned long i = 0;
  if (particle_intersection_with_cell (particle, cell)){
    //consider all my cell mates (and the closest cells mates - defined by viewZone and quality)

    if (cell->sub == NULL){ //isLeaf
      for (i = 0; i < dynar_count(cell->particles); i++){
        tp_particle *p = dynar_get_as (cell->particles, tp_particle*, i);
        if (particle_compare (particle, p) != 0){
          particle_particle_repulsion (particle, p);
        }
      }
    }else{
      if (dynar_count(cell->sub) > 4){
        TUPI_DIE("more divisions than expected");
      }
      for (i = 0; i < dynar_count(cell->sub); i++){
        tp_cell *subcell = dynar_get_as (cell->sub, tp_cell*, i);
        particle_repulsion_with_cell (particle, subcell);
      }
    }

  }else{

    if (particle->cell != cell){
      tp_bary *bary = cell->bary;
      if (bary == NULL){
        TUPI_DIE("bary is nil");
      }
      double distance = tp_DistanceBetweenPoints (particle->position, bary->center);
      double size = tp_DiagonalRect (cell->space);
      if (cell->sub == NULL /* isLeaf */ &&
          (size/distance) < particle->layout->theta){
        if (bary->weight != 0){
          particle_bary_repulsion (particle, bary);
        }
      }else{
        return;
        if (cell->sub != NULL){
          for (i = 0; i < dynar_count(cell->sub); i++){
            tp_cell *subcell = dynar_get_as (cell->sub, tp_cell*, i);
            particle_repulsion_with_cell (particle, subcell);
          }
        }
      }
    }else{
      TUPI_DIE("cell == c, but it shouldn't because this point is not intersecting c");
    }
  }
}

void particle_repulsion_NLOGN (tp_particle *particle)
{
  particle_repulsion_with_cell (particle, particle->box->rootCell);
}

void particle_move (tp_particle *particle)
{
  tp_box *box = particle->box;
  tp_layout *layout = particle->layout;

  if (!particle->frozen) {
    particle->disp = tp_Point (0, 0);

    if (layout->viewZone < 0){
      particle_repulsion_N2 (particle);
    }else{
      particle_repulsion_NLOGN (particle);
    }
    particle_attraction (particle);

    particle->disp = tp_MultiplyPoint (particle->disp, layout->force);
    particle->len = tp_LengthPoint (particle->disp);

    if (particle->len > (layout->diagonalOfArea / 2)) {
      particle->disp = tp_MultiplyPoint (particle->disp, (layout->diagonalOfArea/2)/particle->len);
      particle->len = layout->diagonalOfArea / 2;
    }
    //the displacement is registered in "disp"
    //but this should not be reflected yet to the particle's position
    //the nextStep method is used to commit the change
  }
}

void particle_commit (tp_particle *particle)
{
  tp_point zero = {0,0};
  if (tp_DistanceBetweenPoints (particle->disp, zero) != 0){
    particle->position = tp_AddPoints (particle->position, particle->disp);
  
    if (particle->cell == NULL){
      TUPI_DIE("No responsible cell?");
    }

    cell_move_particle (particle->cell, particle);
    particle->disp = zero;
  }
}

void particle_suicide (tp_particle *particle)
{
  box_remove_particle (particle->box, particle);
}
