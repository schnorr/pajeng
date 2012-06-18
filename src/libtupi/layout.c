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
#include <pthread.h>

static pthread_mutex_t mutex;

void *layout_new (void)
{
  tp_layout *ret = (tp_layout*) malloc (sizeof(tp_layout));

  ret->k = 1.0;
  ret->K1 = 0.06;
  ret->K2 = 0.024;
  ret->force = 1.0;
  //viewZone = 5.0; calculated by the quality level
  ret->theta = 0.7;
  ret->quality = 0;
  ret->nodesPerCell = 10;
  ret->maxTreeDepth = 100;

  ret->stabilizationLimit = 0.9;

  tp_rect universe = tp_Rect (-100, -100, 200, 200);
  ret->box = box_new (ret->nodesPerCell, ret->maxTreeDepth, universe);

  ret->nodes = dict_new (10000);

  layout_reset_energies (ret);

  layout_set_quality (ret, ret->quality);
  pthread_mutex_init (&mutex, NULL);
  return ret;
}

void layout_free (void *layout)
{
  tp_layout *l = (tp_layout*)layout;
  box_free (l->box);
  dict_free(l->nodes);
  free (l);
  pthread_mutex_destroy (&mutex);
}

void layout_set_quality (void *layout, int quality)
{
  tp_layout *l = (tp_layout*)layout;

  //viewZone depends on the quality
  l->quality = quality;

  switch (l->quality) {
  case 0: l->viewZone = l->k; break;
  case 1: l->viewZone = 2 * l->k; break;
  case 2: l->viewZone = 5 * l->k; break;
  case 3: l->viewZone = 10 * l->k; break;
  case 4: l->viewZone = -1; break; //N2 algorithm
  default: l->viewZone = l->k; break;
  }
  fprintf (stderr, "quality set to %d, viewZone calculated to %f\n",
           l->quality, l->viewZone);
}

static void _layout_associate_particle_node (tp_particle *particle, tp_node *node)
{
  node_set_particle (node, particle);
  box_add_particle (particle->layout->box, particle);
  dict_insert_element (particle->layout->nodes, node->name, node);
}

void layout_add_node_with_point (void *layout, tp_node *node, tp_point point)
{
  pthread_mutex_lock (&mutex);
  tp_layout *l = (tp_layout*)layout;
  tp_particle *particle = particle_new_with_point (node->name,
                                        l,
                                        l->box,
                                        node,
                                        point);
  _layout_associate_particle_node (particle, node);
  pthread_mutex_unlock (&mutex);
}

void layout_add_node (void *layout, tp_node *node)
{
  pthread_mutex_lock (&mutex);
  tp_layout *l = (tp_layout*)layout;
  tp_particle *particle = particle_new (node->name,
                                        l,
                                        l->box,
                                        node);
  _layout_associate_particle_node (particle, node);
  pthread_mutex_unlock (&mutex);
}

tp_node *layout_find_node (void *layout, char *name)
{
  tp_layout *l = (tp_layout*)layout;
  return dict_get_element (l->nodes, name);
}

tp_node *layout_find_node_by_position (void *layout, tp_point point)
{
  tp_layout *l = (tp_layout*)layout;
  tp_particle *p = box_find_particle_by_position (l->box, point);
  if (p){
    return p->node;
  }else{
    return NULL;
  }
}

void layout_remove_node (void *layout, tp_node *node)
{
  pthread_mutex_lock (&mutex);
  tp_layout *l = (tp_layout*)layout;
  tp_particle *particle = node->particle;
  box_remove_particle (l->box, particle);
  particle_free (particle);

  dict_remove_element (l->nodes, node->name);
  pthread_mutex_unlock (&mutex);
}

void layout_move_node (void *layout, tp_node *node, tp_point point)
{
  pthread_mutex_lock (&mutex);
  tp_layout *l = (tp_layout*)layout;
  //remove current particle
  box_remove_particle (l->box, node->particle);
  particle_free (node->particle);
  node_set_particle (node, NULL);

  //add new particle at point
  tp_particle *particle = particle_new (node->name,
                                        l,
                                        l->box,
                                        node);
  particle->position = point;
  box_add_particle (l->box, particle);
  node_set_particle (node, particle);

  //particle is now frozen (it might be moving)
  ((tp_particle*)(node->particle))->frozen = 1;
  layout_reset_energies (l);
  pthread_mutex_unlock (&mutex);
}

void layout_compute (void *layout)
{
  pthread_mutex_lock (&mutex);
  tp_layout *l = (tp_layout*)layout;
  l->energy = 0;
  tp_rect area = l->box->rootCell->space;
  l->diagonalOfArea = tp_DiagonalRect (area);
  box_step (l->box);

  //add the newest energy accumulated to the energies vector
  l->energies_index = (l->energies_index+1)%TUPI_MAX_ENERGIES;
  l->energies_count++;
  if (l->energies_count > TUPI_MAX_ENERGIES) l->energies_count = TUPI_MAX_ENERGIES;
  l->energies[l->energies_index] = l->energy;
  pthread_mutex_unlock (&mutex);
}

double inline layout_stabilization_limit (void *layout)
{
  tp_layout *l = (tp_layout*)layout;
  return l->stabilizationLimit;
}

double layout_stabilization (void *layout)
{
  tp_layout *l = (tp_layout*)layout;
  if (l->energies_count < TUPI_MAX_ENERGIES){
    return 0;
  }

  int i;
  double average = 0;
  for (i = 0; i < TUPI_MAX_ENERGIES; i++){
    average += l->energies[i];
  }
  average /= TUPI_MAX_ENERGIES;

  //calculate the standard deviation
  double standard = 0;
  for (i = 0; i < TUPI_MAX_ENERGIES; i++){
    standard += pow (l->energies[i] - average, 2);
  }
  standard = sqrt (standard/TUPI_MAX_ENERGIES);
  return 1/standard;
}

void layout_reset_energies (void *layout)
{
  tp_layout *l = (tp_layout*)layout;
  l->energy = 0;
  l->energies_count = 0;
  l->energies_index = 0;
}

void layout_shake (void *layout)
{
  tp_layout *l = (tp_layout*)layout;
  box_shake (l->box);
}
