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

tp_dynar *dynar_new (const unsigned long elmsize, dynar_cmp_f compare)
{
  tp_dynar *ret = (tp_dynar*) malloc (sizeof(tp_dynar));
  ret->size = 0;
  ret->used = 0;
  ret->elmsize = elmsize;
  ret->data = NULL;
  ret->compare = compare;
  return ret;
}

void dynar_free (tp_dynar *dynar)
{
  free (dynar->data);
  free (dynar);
}

unsigned long dynar_count (const void *dynar)
{
  return ((tp_dynar*)dynar)->used;
}

static void _dynar_resize (tp_dynar *dynar, const unsigned long new_size)
{
  if (new_size != dynar->size) {
    dynar->size = new_size;
    dynar->data = (void*)realloc (dynar->data, new_size * dynar->elmsize);
  }
}

static void _dynar_expand (tp_dynar *dynar, const unsigned long nb)
{
  const unsigned long old_size = dynar->size;

  if (nb > old_size) {
    const unsigned long expand = 2 * (old_size + 1);
    _dynar_resize(dynar, (nb > expand ? nb : expand));
  }
}

static void *_dynar_elm (tp_dynar *dynar, const unsigned long idx)
{
  char *const data = (char *) dynar->data;
  const unsigned long elmsize = dynar->elmsize;

  return data + idx * elmsize;
}

static void *_dynar_insert_at_ptr (tp_dynar *dynar, const unsigned long idx)
{
  void *res;
  unsigned long old_used;
  unsigned long new_used;
  long nb_shift;

  old_used = dynar->used;
  new_used = old_used + 1;

  _dynar_expand(dynar, new_used);

  nb_shift = old_used - idx;

  if (nb_shift>0) {
    memmove(_dynar_elm(dynar, idx + 1),
            _dynar_elm(dynar, idx), nb_shift * dynar->elmsize);
  }

  dynar->used = new_used;
  res = _dynar_elm(dynar, idx);
  return res;
}

static void _dynar_remove_at_ptr (tp_dynar *dynar, const unsigned long idx)
{
  unsigned long nb_shift;
  unsigned long offset;

  nb_shift = dynar->used - 1 - idx;

  if (nb_shift) {
    offset = nb_shift * dynar->elmsize;
    memmove(_dynar_elm(dynar, idx), _dynar_elm(dynar, idx + 1), offset);
  }

  dynar->used--;
}

void *dynar_add_ptr (tp_dynar *dynar)
{
  return _dynar_insert_at_ptr(dynar, dynar->used);
}

void *dynar_get_ptr (void *dynar, const unsigned long idx)
{
  tp_dynar *d = (tp_dynar*)dynar;
  if (idx >= d->used){
    return NULL;
  }
  void *p = _dynar_elm (d, idx);
  return p;
}

void dynar_remove_idx (tp_dynar *dynar, const unsigned long idx)
{
  _dynar_remove_at_ptr (dynar, idx);
}

void dynar_remove (tp_dynar *dynar, const void *src)
{
  TUPI_DIE("unimplemented");
  /* unsigned long i; */
  /* for (i = 0; i < dynar->used; i++){ */
  /*   void *p = dynar_get_as (dynar, type, i); */
  /*   if (dynar->compare(p, src) == 0){ */
  /*     dynar_remove_idx (dynar, i); */
  /*     break; */
  /*   } */
  /* } */
}

int dynar_has_element (tp_dynar *dynar, const void *src)
{
  unsigned long i;
  for (i = 0; i < dynar->used; i++){
    void *p = dynar_get_ptr (dynar, i);
    if (p == src){
      return 1;
    }
  }
  return 0;
}
