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

tp_box *box_new (int particles_per_cell, int max_tree_depth, tp_rect universe)
{
  tp_box *ret = (tp_box*) malloc (sizeof(tp_box));
  ret->particles = dict_new (10000);
  ret->particles_array = dynar_new (sizeof(tp_particle*), &particle_compare);
  ret->time = 0;

  ret->rootCell = cell_new (-1, NULL, universe, ret);

  ret->mode = Resize;
  ret->numberOfParticlesPerCell = particles_per_cell;
  ret->maxTreeDepth = max_tree_depth;
  ret->needResize = 0;
  ret->minPointForResize = tp_Point (0, 0);
  ret->maxPointForResize = tp_Point (0, 0);
  return ret;
}

void box_free (tp_box *box)
{
  dict_free (box->particles);
  dynar_free (box->particles_array);
  cell_free (box->rootCell);
  free (box);
}

void box_check_divisions (tp_box *box)
{
  if (box->needResize){
    tp_rect bb = tp_CopyRect (box->rootCell->space);
    tp_point min = tp_CopyPoint (box->minPointForResize);
    tp_point max = tp_CopyPoint (box->maxPointForResize);

    double xdif = (max.x - min.x);
    double ydif = (max.y - min.y);

    tp_rect minMaxRect = tp_Rect (min.x - (xdif*0.1),
                                  min.y - (ydif*0.1),
                                  xdif*1.1,
                                  ydif*1.1);

    tp_rect doubleRect = tp_Rect (bb.origin.x - bb.size.width/2,
                                  bb.origin.y - bb.size.height/2,
                                  bb.size.width*2,
                                  bb.size.height*2);

    tp_rect unionRect = tp_UnionRect (minMaxRect, doubleRect);
    cell_resize_to (box->rootCell, unionRect);
    box->needResize = 0;
  }else{
    cell_recompute (box->rootCell);
  }

  if (!cell_is_valid (box->rootCell)){
    TUPI_DIE("tree not valid");
  }


}

void box_delete (tp_box *box, tp_particle *particle)
{
  particle_suicide (particle);
}

void box_resize (tp_box *box, tp_particle *particle)
{
  tp_point pos = particle->position;
  tp_point min = box->minPointForResize;
  tp_point max = box->maxPointForResize;

  min.x = pos.x <= min.x ? pos.x : min.x;
  min.y = pos.y <= min.y ? pos.y : min.y;

  max.x = pos.x >= max.x ? pos.x : max.x;
  max.y = pos.y >= max.y ? pos.y : max.y;

  box->minPointForResize = min;
  box->maxPointForResize = max;
  box->needResize = 1;
}

void box_handle_out_particle (tp_box *box, tp_particle *particle)
{
  switch(box->mode){
  case Delete:
    box_delete (box, particle);
    break;
  case Resize:
    box_resize (box, particle);
    break;
  default:
    TUPI_DIE("unknown mode");
    break;
  }
}

void box_add_particle (tp_box *box, tp_particle *particle)
{
  /* if (dict_has_element (box->particles, particle->name)){ */
  /*   TUPI_DIE("a particle with the same identifier already exists"); */
  /* } */

  if (!tp_PointInRect (particle->position, box->rootCell->space)){
    box_handle_out_particle (box, particle);
    box_check_divisions (box);
    if (!tp_PointInRect (particle->position, box->rootCell->space)){
      TUPI_DIE("even after handling out, new particle is not within the rootCell universe");
    }
  }
  cell_add_particle (box->rootCell, particle);
//  dict_insert_element (box->particles, particle->name, particle);
  dynar_add_as (box->particles_array, tp_particle*, particle);
  particle->box = box;
}

void box_remove_particle (tp_box *box, tp_particle *particle)
{
  tp_cell *cell = particle->cell;
  cell_remove_particle (cell, particle);
//  dict_remove_element (box->particles, particle->name);
  dynar_remove_as (box->particles_array, tp_particle*, particle);
  particle->box = NULL;
}

void box_remove_all_particles (tp_box *box)
{
  while (dynar_count(box->particles_array) != 0){
    tp_particle *p = dynar_get_as (box->particles_array, tp_particle*, 0);
    tp_cell *cell = p->cell;
    if (cell == NULL){
      TUPI_DIE("removing a particle that is not in the tree, it has no cell");
    }
    box_remove_particle (box, p);

    if (cell_has_particle(cell, p)){
      TUPI_DIE("the cell from which the particle was removed still contains the particle");
    }
  }
}

tp_particle *box_find_particle (tp_box *box, const char *name)
{
  unsigned long i;
  for (i = 0; i < dynar_count(box->particles_array); i++){
    tp_particle *p = dynar_get_as (box->particles_array, tp_particle*, i);
    if (strcmp (p->name, name) == 0){
      return p;
    }
  }
  return NULL;
}

tp_particle *box_find_particle_by_position (tp_box *box, tp_point point)
{
  return cell_find_particle_by_position (box->rootCell, point);
}

void box_step (tp_box *box)
{
  int debug = 0;
  double tt1, tt2;
  unsigned long i;

  if (debug){
    tt1 = gettime();
  }

  //this is the most expensive operation
  for (i = 0; i < dynar_count(box->particles_array); i++){
    tp_particle *p = dynar_get_as (box->particles_array, tp_particle*, i);
    particle_move (p);
  }

  if (debug){
    tt2 = gettime();
    fprintf (stderr, "%f (%f) ",
             tt2-tt1, (tt2-tt1)/dynar_count(box->particles_array));
    tt1 = gettime();
  }

  for (i = 0; i < dynar_count(box->particles_array); i++){
    tp_particle *p = dynar_get_as (box->particles_array, tp_particle*, i);
    particle_commit (p);
  }

  if (debug){
    tt2 = gettime();
    fprintf (stderr, "%f ",
             tt2-tt1);
    tt1 = gettime();
  }

  box_check_divisions (box);

  if (debug){
    tt2 = gettime();
    fprintf (stderr, "%f\n", tt2-tt1);
  }

  box->time++;
}

void box_shake (tp_box *box)
{
  unsigned int i;

  for (i = 0; i < dynar_count(box->particles_array); i++){
    tp_particle *p = dynar_get_as (box->particles_array, tp_particle*, i);
    particle_shake (p);
  }
}
