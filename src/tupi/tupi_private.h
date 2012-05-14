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
#ifndef __TUPI_PRIVATE_H
#define __TUPI_PRIVATE_H
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif //__GNU_SOURCE
#ifndef __USE_GNU
#define __USE_GNU
#endif //__USE_GNU
#include <search.h>

#define TUPI_MAX_ENERGIES 256

#if defined(__cplusplus)
extern "C" {
#endif
void tupi_exception (const char *func,
                     const char *file,
                     const int line,
                     const char *error);
#define TUPI_DIE(str) tupi_exception(__FUNCTION__, __FILE__, __LINE__, str)

/* data structures */
typedef int (*dynar_cmp_f)(const void *p1, const void *p2);
typedef struct m_tp_dynar {
  unsigned long size;
  unsigned long used;
  unsigned long elmsize;
  void *data;
  dynar_cmp_f compare;
} tp_dynar;

typedef struct m_tp_dict {
  struct hsearch_data hash;
  unsigned long count;
} tp_dict;

/* geometric and particle system types */
typedef struct m_tp_point {
  double x;
  double y;
} tp_point;

typedef struct m_tp_size {
  double width;
  double height;
} tp_size;

typedef struct m_tp_rect {
  tp_point origin;
  tp_size size;
} tp_rect;

typedef struct m_tp_cell {
  //the cell hierarchical structure
  struct m_tp_cell *parent;
  tp_dynar *sub;
  int depth;
  int index; //in the parent universe

  //the cell characteristics
  tp_rect space;
  struct m_tp_bary *bary;
  struct m_tp_box *box;

  //the particles within this cell
  tp_dynar *particles;
  int population;
} tp_cell;

typedef struct m_tp_bary {
  double weight;
  tp_point center;
  tp_cell *cell;
} tp_bary;

typedef struct m_tp_particle {
  char *name;
  tp_point position;
  tp_rect mask; //used to search by point
  tp_cell *cell;
  double weight;

  int frozen;
  tp_point disp; // Displacement vector.
  double len; // Last computed displacement vector length.

  struct m_tp_layout *layout;
  struct m_tp_box *box;

  struct m_tp_node *node; //the node that this particle represents
} tp_particle;

typedef struct m_tp_layout {
  double k; // optimal distance between nodes.
  double K1; // default attraction.
  double K2; // default repulsion.
  double force; // global force strength in [0..1] that is used to scale moves
  double viewZone; // view distance at which the cells of the n-tree are explored exhaustively, after this the poles are used. This is a multiple of k.
  double theta; //Barnes/Hut theta threshold to know if we use a pole or not.
  int quality; // quality level.
  int nodesPerCell; // number of nodes per space-cell.
  int maxTreeDepth;
  double stabilizationLimit;

  //used by the particles
  double diagonalOfArea; // the diagonal of the graph area at the current step.

  struct m_tp_box *box;

  tp_dict *nodes; //dictionary of nodes that this layout is handling

  /* to calculate the stabilization of the particle system */
  double energy; //the energy of the last loop (an accumulator)
  double energies[TUPI_MAX_ENERGIES];
  int energies_index;
  int energies_count;
} tp_layout;


enum OutOfUniverseMode {Delete, Resize};
typedef struct m_tp_box {
  tp_dict *particles;
  tp_dynar *particles_array;
  unsigned long time;
  tp_cell *rootCell;

  //parameters migrated from NTree class
  enum OutOfUniverseMode mode;
  int numberOfParticlesPerCell;
  int maxTreeDepth;
  int needResize;
  tp_point minPointForResize;
  tp_point maxPointForResize;
} tp_box;


/* graph type */
typedef struct m_tp_node {
  char *name;
  const void *data; //user data
  tp_dynar *connected;
  tp_particle *particle; //the particle that represents this node
} tp_node;

tp_dynar *dynar_new (const unsigned long elmsize, dynar_cmp_f compare);
void dynar_free (tp_dynar *dynar);
unsigned long dynar_count (const tp_dynar *dynar);
void *dynar_add_ptr (tp_dynar *dynar);
void *dynar_get_ptr (tp_dynar *dynar, const unsigned long idx);
void dynar_remove (tp_dynar *dynar, const void *src);
void dynar_remove_idx (tp_dynar *dynar, const unsigned long idx);
int dynar_has_element (tp_dynar *dynar, const void *src);
#define dynar_add_as(dynar,type,value) \
  (*(type*)dynar_add_ptr(dynar)=value)
#define dynar_get_as(dynar,type,idx) \
  (*(type*)dynar_get_ptr(dynar,idx))
#define dynar_remove_as(dynar,type,value)  \
  unsigned long i; \
  for (i = 0; i < dynar->used; i++){ \
    if (dynar->compare(dynar_get_as (dynar,type,i), value) == 0){ \
      dynar_remove_idx (dynar, i); \
    } \
  }

tp_dict *dict_new (int capacity);
void dict_free (tp_dict *dict);
void *dict_get_ptr (tp_dict *dict, unsigned long idx);
int dict_has_element (tp_dict *dict, char *name);
void dict_insert_element (tp_dict *dict, char *name, void *object);
void dict_remove_element (tp_dict *dict, char *name);
void *dict_get_element (tp_dict *dict, char *name);

tp_particle *particle_new (const char *name, tp_layout *layout, tp_box *box, tp_node *node);
void particle_set_mask (tp_particle *particle, tp_rect mask);
void particle_free (tp_particle *p);
int particle_compare (const void *p1, const void *p2);
void particle_move (tp_particle *particle);
void particle_commit (tp_particle *particle);
void particle_suicide (tp_particle *particle);

tp_cell *cell_new (int index, tp_cell *parent, tp_rect space, tp_box *box);
void cell_free (tp_cell *cell);
int cell_compare (const void *c1, const void *c2);
int cell_has_particle (tp_cell *cell, tp_particle *particle);
void cell_remove_particle (tp_cell *cell, tp_particle *particle);
void cell_add_particle (tp_cell *cell, tp_particle *particle);
void cell_move_particle (tp_cell *cell, tp_particle *particle);
int cell_is_valid (tp_cell *cell);
void cell_resize_to (tp_cell *cell, tp_rect space);
void cell_recompute (tp_cell *cell);
tp_particle *cell_find_particle_by_position (tp_cell *cell, tp_point point);

tp_bary *bary_new (tp_cell *cell);
void bary_free (tp_bary *bary);
void bary_recompute (tp_bary *bary);

tp_rect space_newsubcell_space (tp_rect universe, int index);

tp_box *box_new (int particles_per_cell, int max_tree_depth, tp_rect universe);
void box_free (tp_box *box);
void box_add_particle (tp_box *box, tp_particle *particle);
void box_remove_particle (tp_box *box, tp_particle *particle);
void box_remove_all_particles (tp_box *box);
void box_handle_out_particle (tp_box *box, tp_particle *particle);
tp_particle *box_find_particle (tp_box *box, const char *name);
tp_particle *box_find_particle_by_position (tp_box *box, tp_point point);
void box_step (tp_box *box);

tp_layout *layout_new (void);
void layout_free (tp_layout *l);
void layout_set_quality (tp_layout *layout, int quality);
void layout_add_node (tp_layout *layout, tp_node *node);
tp_node *layout_find_node (tp_layout *layout, char *name);
tp_node *layout_find_node_by_position (tp_layout *layout, tp_point point);
void layout_remove_node (tp_layout *layout, tp_node *node);
void layout_move_node (tp_layout *layout, tp_node *node, tp_point point);
void layout_compute (tp_layout *layout);
double layout_stabilization_limit (tp_layout *layout);
double layout_stabilization (tp_layout *layout);
void layout_reset_energies (tp_layout *ret);

tp_node *node_new (const char *name, const void *data);
void node_free (tp_node *node);
int node_compare (const void *p1, const void *p2);
void node_connect (tp_node *n1, tp_node *n2);
void node_set_particle (tp_node *n1, tp_particle *p);

#include "tupi_functions.h"


#if defined(__cplusplus)
}
#endif

#endif //__TUPI_PRIVATE_H
