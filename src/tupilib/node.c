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

tp_node *node_new (const char *name, const void *data)
{
  tp_node *ret = (tp_node*) malloc (sizeof(tp_node));
  ret->name = strdup (name);
  ret->data = data;
  ret->connected = dynar_new (sizeof(tp_particle*), &node_compare);
  ret->particle = NULL;
  return ret;
}

void node_free (tp_node *node)
{
  free (node->name);
  dynar_free (node->connected);
  free (node);
}

int node_compare (const void *p1, const void *p2)
{
  return strcmp (((tp_node*)p1)->name, ((tp_node*)p2)->name);
}

static int node_is_connected (tp_node *n1, tp_node *n2)
{
  int i;
  int count = dynar_count(n1->connected);
  int found = 0;
  for (i = 0; i < count; i++){
    tp_node *node = dynar_get_as (n1->connected, tp_node*, i);
    if (strcmp (node->name, n2->name) == 0){
      found = 1;
      break;
    }
  }
  return found;
}

void node_connect (tp_node *n1, tp_node *n2)
{
  //search n2 on n1
  if (!node_is_connected (n1, n2)) dynar_add_as (n1->connected, tp_node*, n2);

  //search n1 on n2
  if (!node_is_connected (n2, n1)) dynar_add_as (n2->connected, tp_node*, n1);
}

void node_connect_clear (tp_node *n)
{
  dynar_free (n->connected);
  n->connected = dynar_new (sizeof(tp_particle*), &node_compare);
}

void node_set_particle (tp_node *n1, tp_particle *p)
{
  n1->particle = p;
}
