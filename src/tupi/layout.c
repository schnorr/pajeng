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

tp_layout *layout_new (void)
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

void layout_free (tp_layout *l)
{
  box_free (l->box);
  dict_free(l->nodes);
  free (l);
  pthread_mutex_destroy (&mutex);
}

void layout_set_quality (tp_layout *layout, int quality)
{
  //viewZone depends on the quality
  layout->quality = quality;

  switch (layout->quality) {
  case 0: layout->viewZone = layout->k; break;
  case 1: layout->viewZone = 2 * layout->k; break;
  case 2: layout->viewZone = 5 * layout->k; break;
  case 3: layout->viewZone = 10 * layout->k; break;
  case 4: layout->viewZone = -1; break; //N2 algorithm
  default: layout->viewZone = layout->k; break;
  }
  fprintf (stderr, "quality set to %d, viewZone calculated to %f\n",
           layout->quality, layout->viewZone);
}

void layout_add_node (tp_layout *layout, tp_node *node)
{
  pthread_mutex_lock (&mutex);
  tp_particle *particle = particle_new (node->name,
                                        layout,
                                        layout->box,
                                        node);
  node_set_particle (node, particle);
  box_add_particle (layout->box, particle);

  dict_insert_element (layout->nodes, node->name, node);
  pthread_mutex_unlock (&mutex);
}

tp_node *layout_find_node (tp_layout *layout, char *name)
{
  return dict_get_element (layout->nodes, name);
}

tp_node *layout_find_node_by_position (tp_layout *layout, tp_point point, tp_rect mask)
{
  tp_particle *p = box_find_particle_by_position (layout->box, point, mask);
  if (p){
    return p->node;
  }else{
    return NULL;
  }
}

void layout_remove_node (tp_layout *layout, tp_node *node)
{
  pthread_mutex_lock (&mutex);
  //tp_particle *particle = box_find_particle (layout->box, nodeName);
  tp_particle *particle = node->particle;
  box_remove_particle (layout->box, particle);
  particle_free (particle);

  dict_remove_element (layout->nodes, node->name);
  pthread_mutex_unlock (&mutex);
}

void layout_move_node (tp_layout *layout, tp_node *node, tp_point point)
{
  pthread_mutex_lock (&mutex);
  //remove current particle
  box_remove_particle (layout->box, node->particle);
  particle_free (node->particle);
  node_set_particle (node, NULL);

  //add new particle at point
  tp_particle *particle = particle_new (node->name,
                                        layout,
                                        layout->box,
                                        node);
  particle->position = point;
  box_add_particle (layout->box, particle);
  node_set_particle (node, particle);

  //particle is now frozen (it might be moving)
  node->particle->frozen = 1;
  layout_reset_energies (layout);
  pthread_mutex_unlock (&mutex);
}

void layout_compute (tp_layout *layout)
{
  pthread_mutex_lock (&mutex);
  layout->energy = 0;
  tp_rect area = layout->box->rootCell->space;
  layout->diagonalOfArea = tp_DiagonalRect (area);
  box_step (layout->box);

  //add the newest energy accumulated to the energies vector
  layout->energies_index = (layout->energies_index+1)%TUPI_MAX_ENERGIES;
  layout->energies_count++;
  if (layout->energies_count > TUPI_MAX_ENERGIES) layout->energies_count = TUPI_MAX_ENERGIES;
  layout->energies[layout->energies_index] = layout->energy;
  pthread_mutex_unlock (&mutex);
}

double inline layout_stabilization_limit (tp_layout *layout)
{
  return layout->stabilizationLimit;
}

double layout_stabilization (tp_layout *layout)
{
  if (layout->energies_count < TUPI_MAX_ENERGIES){
    return 0;
  }

  int i;
  double average = 0;
  for (i = 0; i < TUPI_MAX_ENERGIES; i++){
    average += layout->energies[i];
  }
  average /= TUPI_MAX_ENERGIES;

  //calculate the standard deviation
  double standard = 0;
  for (i = 0; i < TUPI_MAX_ENERGIES; i++){
    standard += pow (layout->energies[i] - average, 2);
  }
  standard = sqrt (standard/TUPI_MAX_ENERGIES);
  return 1/standard;
}

void layout_reset_energies (tp_layout *ret)
{
  ret->energy = 0;
  ret->energies_count = 0;
  ret->energies_index = 0;
}
