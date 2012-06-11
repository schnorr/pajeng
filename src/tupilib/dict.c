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

tp_dict *dict_new (int capacity)
{
  tp_dict *ret = (tp_dict*) malloc (sizeof(tp_dict));
  bzero (&ret->hash, sizeof(struct hsearch_data));
  hcreate_r (capacity*1.2, &ret->hash);
  ret->count = 0;
  return ret;
}

void dict_free (tp_dict *dict)
{
  hdestroy_r (&dict->hash);
  free (dict);
}

int dict_has_element (tp_dict *dict, char *name)
{
  ENTRY e, *ep = NULL;
  e.key = name;
  e.data = NULL;
  hsearch_r (e, FIND, &ep, &dict->hash);
  if (ep == NULL) {
    return 0;
  }else{
    return 1;
  }
}

void *dict_get_element (tp_dict *dict, char *name)
{
  ENTRY e, *ep = NULL;
  e.key = name;
  e.data = NULL;
  hsearch_r (e, FIND, &ep, &dict->hash);
  if (ep == NULL) {
    return NULL;
  }else{
    return ep->data;
  }
}

void dict_insert_element (tp_dict *dict, char *name, void *object)
{
  if (dict_has_element (dict, name)){
    TUPI_DIE("element is already present");
  }

  ENTRY e, *ep = NULL;
  e.key = name;
  e.data = object;
  hsearch_r (e, ENTER, &ep, &dict->hash);
}

void dict_remove_element (tp_dict *dict, char *name)
{
  if (!dict_has_element (dict, name)){
    TUPI_DIE("trying to remove something that is not here");
  }

  ENTRY e, *ep = NULL;
  e.key = name;
  e.data = NULL;
  hsearch_r (e, FIND, &ep, &dict->hash);

  if (ep != NULL){
    ep->data = NULL;
    ep->key = "";
  }
}
