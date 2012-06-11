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

tp_cell *cell_new (int index, tp_cell *parent, tp_rect space, tp_box *box)
{
  tp_cell *ret = (tp_cell*) malloc (sizeof(tp_cell));
  ret->parent = parent;
  ret->sub = NULL;
  ret->depth = parent != NULL ? parent->depth+1 : 0;
  ret->index = index;
  ret->space = tp_CopyRect(space);
  ret->bary = bary_new (ret);
  ret->bary->cell = ret;
  ret->box = box;
  ret->particles = dynar_new (sizeof(tp_particle*), &particle_compare);
  ret->population = 0;
  return ret;
}

void cell_free (tp_cell *cell)
{
  if (cell->sub && dynar_count (cell->sub) != 0){
    unsigned long i;
    for (i = 0; i < dynar_count(cell->sub); i++){
      tp_cell *subcell = dynar_get_as (cell->sub, tp_cell*, i);
      cell_free (subcell);
    }
    dynar_free(cell->sub);
  }
  bary_free (cell->bary);
  dynar_free (cell->particles);
  free (cell);
}

int cell_compare (const void *c1, const void *c2)
{
  TUPI_DIE("not implemented");
  return 0;
}

int cell_has_particle (tp_cell *cell, tp_particle *particle)
{
  return dynar_has_element (cell->particles, particle);
}

void cell_remove_particle (tp_cell *cell, tp_particle *particle)
{
  cell->population--;

  if (cell->sub == NULL){
    dynar_remove_as (cell->particles, tp_particle*, particle);
    if (cell->population != dynar_count (cell->particles)){
      TUPI_DIE("population != [children count]");
    }
    particle->cell = NULL;
  }

  if (cell->parent != NULL){
    cell_remove_particle (cell->parent, particle);
  }
}

void cell_add_particle (tp_cell *cell, tp_particle *particle)
{
  unsigned long i;
  cell->population++;

  if (cell->sub != NULL){
    int k = 0;
    for (i = 0; i < dynar_count(cell->sub); i++){
      tp_cell *subcell = dynar_get_as (cell->sub, tp_cell*, i);
      if (tp_PointInRect (particle->position, subcell->space)){
        if (k == 0){
          cell_add_particle (subcell, particle);
        }
        k++;
      }
    }

    if (k == 0){
      TUPI_DIE("no subcell to add particle");
    }

    if (k != 1){
      char str[100];
      fprintf (stderr, "particle position: %s\n",
               tp_PointDesc(particle->position, str, 100));
      for (i = 0; i < dynar_count(cell->sub); i++){
        tp_cell *subcell = dynar_get_as (cell->sub, tp_cell*, i);
        fprintf (stderr, "\tsubcell rect %s within %d\n",
                 tp_RectDesc(subcell->space, str, 100),
                 tp_PointInRect (particle->position, subcell->space));
      }
      fprintf (stderr,
               "too many subcells to add particle => use first choice\n");
    }

  }else{
    if (dynar_has_element (cell->particles, particle)){
      TUPI_DIE("Particle added in the cell already exists");
    }
    dynar_add_as (cell->particles, tp_particle*, particle);
    particle->cell = cell;

    if (cell->population != dynar_count(cell->particles)){
      TUPI_DIE("Discrepancy in population count");
    }
  }
}

void cell_move_particle (tp_cell *cell, tp_particle *particle)
{
  if (cell->sub != NULL){ //!isLeaf
    TUPI_DIE("particle moved event in non-leaf cell");
  }

  // If it moved out of me, reposition it.
  if (!tp_PointInRect (particle->position, cell->space)){
    tp_box *box = cell->box;
    tp_cell *rootCell = box->rootCell;
    if (tp_PointInRect (particle->position, rootCell->space)){
      cell_remove_particle (cell, particle);
      cell_add_particle (rootCell, particle);
    }else{
      box_handle_out_particle (box, particle);
    }
  }
}

void cell_mitosis (tp_cell *cell)
{
  unsigned long i, j;

  if (cell->sub != NULL){
    TUPI_DIE("sub needs to be nil here");
  }

  if (dynar_count(cell->particles) <= cell->box->numberOfParticlesPerCell){
    TUPI_DIE("no subdivision needed ?");
  }

  cell->sub = dynar_new (sizeof(tp_cell*), &cell_compare);
  for (j = 0; j < 4; j++){
    tp_rect subcell_space = space_newsubcell_space (cell->space, j);
    tp_cell *subcell = cell_new (j,
                                 cell,
                                 subcell_space,
                                 cell->box);

    //see those particles that will belong to the subcell
    tp_dynar *bucket = dynar_new (sizeof(tp_particle*), &particle_compare);
    for (i = 0; i < dynar_count(cell->particles); i++){
      tp_particle *p = dynar_get_as (cell->particles, tp_particle*, i);
      if (tp_PointInRect (p->position, subcell->space)){
        dynar_add_as (bucket, tp_particle*, p);
      }
    }

    int particles_to_be_removed = dynar_count(bucket);
    int particles_total = dynar_count(cell->particles);
    int expected_particles = particles_total - particles_to_be_removed;

    //add the selected particles to the subcell
    for (i = 0; i < dynar_count(bucket); i++){
      tp_particle *p = dynar_get_as (bucket, tp_particle*, i);
      dynar_remove_as (cell->particles, tp_particle*, p);
      cell_add_particle (subcell, p);
    }

    if (dynar_count(cell->particles) != expected_particles){
      TUPI_DIE("some particles marked to be removed were not");
    }

    dynar_free (bucket);

    //add the newly created subcell to the children of current cell
    dynar_add_as (cell->sub, tp_cell*, subcell);
  }

  if (dynar_count(cell->particles)){
    char str[100];
    printf ("debug messages:\n");
    for (i = 0; i < dynar_count(cell->particles); i++){
      tp_particle *p = dynar_get_as (cell->particles, tp_particle*, i);

      printf ("particle %p remains with position %s inside the cell %d\n",
              p, tp_PointDesc (p->position, str, 100),
              tp_PointInRect (p->position, cell->space));

      for (j = 0; j < dynar_count(cell->sub); j++){
        tp_cell *subcell = dynar_get_as (cell->sub, tp_cell*, j);

        printf("p %p against subcell %p rect %s inside %d\n",
               p, subcell, tp_RectDesc (subcell->space, str, 100), 
               tp_PointInRect (p->position, subcell->space));
        tp_PointInRect2 (p->position, subcell->space);
      }

    }

    for (i = 0; i < dynar_count(cell->sub); i++){
      tp_cell *subcell = dynar_get_as (cell->sub, tp_cell*, i);
      printf ("subcell %p pop %d\n", subcell, subcell->population);
    }
    TUPI_DIE("there are unclassified particles after mitosis ?");
  }
}

void cell_fusion (tp_cell *cell)
{
  unsigned long i = 0;
  if (dynar_count(cell->particles) != 0){
    TUPI_DIE("when fusioning cells, no particles should be in the destination cell");
  }

  for (i = 0; i < dynar_count(cell->sub); i++){
    tp_cell *subcell = dynar_get_as (cell->sub, tp_cell*, i);
    if (subcell->sub != NULL){
      TUPI_DIE("Fusion of non leaf-subcells");
    }

    while (dynar_count(subcell->particles) != 0){
      tp_particle *p = dynar_get_as (subcell->particles, tp_particle*, 0);
      p->cell = cell;
      dynar_add_as (cell->particles, tp_particle*, p);
      dynar_remove_idx (subcell->particles, 0);
    }
  }

  while (dynar_count(cell->sub)){
    tp_cell *subcell = dynar_get_as (cell->sub, tp_cell*, 0);
    cell_free(subcell);
    dynar_remove_idx(cell->sub, 0);
  }
  dynar_free (cell->sub);
  cell->sub = NULL;
}


void cell_recompute (tp_cell *cell)
{
  tp_cell *subcell = NULL;
  unsigned long i = 0;
  tp_box *box = cell->box;


  if (cell->sub == NULL){
    if (cell->population != dynar_count(cell->particles)){
      TUPI_DIE("population != [children count]");
    }

    if (cell->depth < box->maxTreeDepth &&
        cell->population > box->numberOfParticlesPerCell){

      cell_mitosis (cell);

      for (i = 0; i < dynar_count(cell->sub); i++){
        tp_cell *subcell = dynar_get_as (cell->sub, tp_cell*, i);
        cell_recompute (subcell);
      }
    }
  }else{
    int leafs = 0;
    int divisions = 4;

    for (i = 0; i < dynar_count(cell->sub); i++){
      tp_cell *subcell = dynar_get_as (cell->sub, tp_cell*, i);
      cell_recompute (subcell);
      if (subcell->sub == NULL){
        leafs++;
      }
    }

    if (leafs == divisions &&
        cell->population <= box->numberOfParticlesPerCell){

      cell_fusion (cell);

    }
  }

  bary_recompute (cell->bary);
}

void cell_resize_to (tp_cell *cell, tp_rect space)
{
  unsigned long i;
  if(cell->parent != NULL){
    TUPI_DIE("can only resize the root cell");
  }

  // 1. We are sure we are in the root cell.
  // 2. We remove all particles from the particle box.

  int oldPopulation = cell->population;
  tp_dynar *allParticles = dynar_new (sizeof(tp_particle*), &particle_compare);
  for (i = 0; i < dynar_count(cell->box->particles_array); i++){
    tp_particle *p = dynar_get_as (cell->box->particles_array, tp_particle*, i);
    dynar_add_as (allParticles, tp_particle*, p);
  }
  box_remove_all_particles (cell->box);

  // 3. Recompute the tree to remove all children by fusion. We do this
  //    to trigger removal events.

  if (cell->population != 0){
    TUPI_DIE("after removal of all particles the root cell still contains particles");
  }

  if (dynar_count(cell->particles) != 0){
    TUPI_DIE("after removal of all particles the root cell still contains #2");
  }

  cell_recompute (cell);

  if (cell->sub != NULL || cell->parent != NULL){
    TUPI_DIE("after particles removal the mama cell should be root and leaf");
  }

  // 4. We resize this root cell.

  cell->space = space;
		
  // 5. Re-insert all particles in this root cell.

  for (i = 0; i < dynar_count(allParticles); i++){
    tp_particle *p = dynar_get_as (allParticles, tp_particle*, i);
    box_add_particle (cell->box, p);
  }

  // 6. Recompute the tree to subdivide it. We use recompute to trigger
  //    mitosis events.

  cell_recompute (cell);

  if (cell->population != dynar_count (cell->box->particles_array)){
    TUPI_DIE("discrepancy when resinserting particles during mama resize");
  }

  if (oldPopulation != cell->population){
    TUPI_DIE("after resize new population size != old");
  }
  dynar_free (allParticles);
}

int cell_is_valid (tp_cell *cell)
{
  int divisions = 4;
  int pop = 0;
  unsigned long i = 0;

  if (cell->sub != NULL){
    if (divisions != dynar_count(cell->sub)){
      return 0;
    }

    for (i = 0; i < dynar_count(cell->sub); i++){
      tp_cell *subcell = dynar_get_as (cell->sub, tp_cell*, i);
      if (cell_is_valid (subcell) == 0) return 0;
      pop += subcell->population;
      if (subcell->depth != cell->depth+1) return 0;
    }
    if (pop != cell->population) return 0;
  }

  return 1;
}

tp_particle *cell_find_particle_by_position (tp_cell *cell,
                                             tp_point point)
{
  int i;
  if (cell->sub != NULL){
    //recurse only on the single children that contains point
    for (i = 0; i < dynar_count(cell->sub); i++){
      tp_cell *subcell = dynar_get_as (cell->sub, tp_cell*, i);
      if (tp_PointInRect (point, subcell->space)){
        return cell_find_particle_by_position (subcell,
                                               point);
      }
    }
  }else{
    //search locally
    for (i = 0; i < dynar_count(cell->particles); i++){
      tp_particle *p = dynar_get_as (cell->particles, tp_particle*, i);
      tp_point position = p->position;
      tp_rect pmask = p->mask;
      pmask.origin.x = p->position.x - pmask.size.width/2;
      pmask.origin.y = p->position.y - pmask.size.height/2;
      if (tp_PointInRect (point, pmask)){
        return p;
      }
    }
  }
  return NULL;
}
