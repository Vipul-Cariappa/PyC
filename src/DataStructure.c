#include "CppTypeInfo.h"
#include "DataStructures.h"
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* array_long_long_t functions & methods */

/*
  creates and returns a new array_long_long_t pointer
*/
array_long_long_t *array_long_long_new() {
  array_long_long_t *arr = malloc(sizeof(array_long_long_t));
  if (!arr) {
    errno = ENOMEM;
    return NULL;
  }

  arr->array = calloc(4, sizeof(long long));
  if (!arr->array) {
    free(arr);
    errno = ENOMEM;
    return NULL;
  }

  arr->capacity = 4;
  arr->size = 0;

  return arr;
}

/*
  returns the size of the array
*/
size_t array_long_long_size(array_long_long_t *arr) { return arr->size; }

/*
  appends val to the end of the array arr
*/
bool array_long_long_append(array_long_long_t *arr, long long val) {
  if (arr->size < arr->capacity) {
    arr->array[arr->size] = val;
    arr->size++;
    return true;
  } else {
    arr->array = realloc(arr->array, ((arr->capacity + 4) * sizeof(long long)));

    if (arr->array) {
      arr->capacity += 4;
      arr->array[arr->size] = val;
      arr->size++;
      return true;
    }
    errno = ENOMEM;
    return false;
  }
}

/*
  removes and returns the last value in the array arr
*/
long long array_long_long_pop(array_long_long_t *arr) {
  if (arr->size == 0) {
    errno = EINVAL;
    return 0;
  }

  long long last_value = arr->array[--(arr->size)];

  if (arr->size + 4 < arr->capacity) {
    arr->array = realloc(arr->array, ((arr->capacity - 4) * sizeof(long long)));
    arr->capacity -= 4;
  }

  return last_value;
}

/*
  sets val at index in array arr
*/
bool array_long_long_setat(array_long_long_t *arr, long long val,
                           size_t index) {
  if (index < arr->size) {
    arr->array[index] = val;
    return true;
  }
  errno = ENOMEM;
  return false;
}

/*
  returns the value at index in array arr
*/
long long array_long_long_getat(array_long_long_t *arr, size_t index) {
  if (index < arr->size) {
    return arr->array[index];
  }
  errno = EINVAL;
  return 0;
}

/*
  clear the arr and frees all allocated memory
*/
bool array_long_long_clear(array_long_long_t *arr) {
  free(arr->array);
  free(arr);
  return true;
}

/* array_size_t_t functions & methods */

/*
  creates and returns a new array_size_t_t pointer
*/
array_size_t_t *array_size_t_new() {
  array_size_t_t *arr = malloc(sizeof(array_size_t_t));
  if (!arr) {
    errno = ENOMEM;
    return NULL;
  }

  arr->array = calloc(4, sizeof(size_t));
  if (!arr->array) {
    free(arr);
    errno = ENOMEM;
    return NULL;
  }

  arr->capacity = 4;
  arr->size = 0;

  return arr;
}

/*
  returns the size of the array
*/
size_t array_size_t_size(array_size_t_t *arr) { return arr->size; }

/*
  appends val to the end of the array arr
*/
bool array_size_t_append(array_size_t_t *arr, size_t val) {
  if (arr->size < arr->capacity) {
    arr->array[arr->size] = val;
    arr->size++;
    return true;
  } else {
    arr->array = realloc(arr->array, ((arr->capacity + 4) * sizeof(size_t)));

    if (arr->array) {
      arr->capacity += 4;
      arr->array[arr->size] = val;
      arr->size++;
      return true;
    }
    errno = ENOMEM;
    return false;
  }
}

/*
  removes and returns the last value in the array arr
*/
size_t array_size_t_pop(array_size_t_t *arr) {
  if (arr->size == 0) {
    errno = EINVAL;
    return 0;
  }

  size_t last_value = arr->array[--(arr->size)];

  if (arr->size + 4 < arr->capacity) {
    arr->array = realloc(arr->array, ((arr->capacity - 4) * sizeof(size_t)));
    arr->capacity -= 4;
  }

  return last_value;
}

/*
  sets val at index in array arr
*/
bool array_size_t_setat(array_size_t_t *arr, size_t val, size_t index) {
  if (index < arr->size) {
    arr->array[index] = val;
    return true;
  }
  errno = ENOMEM;
  return false;
}

/*
  returns the value at index in array arr
*/
size_t array_size_t_getat(array_size_t_t *arr, size_t index) {
  if (index < arr->size) {
    return arr->array[index];
  }
  errno = EINVAL;
  return 0;
}

/*
  clear the arr and frees all allocated memory
*/
bool array_size_t_clear(array_size_t_t *arr) {
  free(arr->array);
  free(arr);
  return true;
}

/* array_str_t functions & methods */

/*
  creates and returns a new array_str_t pointer
*/
array_str_t *array_str_new() {
  array_str_t *arr = malloc(sizeof(array_str_t));
  if (!arr) {
    errno = ENOMEM;
    return NULL;
  }

  arr->array = calloc(4, sizeof(char *));
  if (!arr->array) {
    free(arr);
    errno = ENOMEM;
    return NULL;
  }

  memset(arr->array, 0, 4 * sizeof(char *));
  arr->capacity = 4;
  arr->size = 0;

  return arr;
}

/*
  returns the size of the array
*/
size_t array_str_size(array_str_t *arr) { return arr->size; }

/*
  appends val to the end of the array arr
*/
bool array_str_append(array_str_t *arr, char *val) {
  if (arr->size < arr->capacity) {
    size_t len = strlen(val);
    char *cpy_val = calloc(len + 1, sizeof(char));
    if (cpy_val) {
      strcpy(cpy_val, val);
      arr->array[arr->size] = cpy_val;
      arr->size++;
      return true;
    } else {
      errno = ENOMEM;
      return false;
    }
  } else {
    arr->array = realloc(arr->array, ((arr->capacity + 4) * sizeof(char *)));

    if (arr->array) {
      memset(arr->array + arr->size, 0, 4 * sizeof(char *));
      arr->capacity += 4;

      size_t len = strlen(val);
      char *cpy_val = calloc(len + 1, sizeof(char));
      if (cpy_val) {
        strcpy(cpy_val, val);
        arr->array[arr->size] = cpy_val;
        arr->size++;
        return true;
      } else {
        errno = ENOMEM;
        return false;
      }
    }
    errno = ENOMEM;
    return false;
  }
}

/*
  removes and returns the last value in the array arr
*/
char *array_str_pop(array_str_t *arr) {
  if (arr->size == 0) {
    errno = EINVAL;
    return 0;
  }

  char *last_value = arr->array[--(arr->size)];
  arr->array[arr->size] = NULL;

  if (arr->size + 4 < arr->capacity) {
    arr->array = realloc(arr->array, ((arr->capacity + 4) * sizeof(char *)));
    arr->capacity -= 4;
  }

  return last_value;
}

/*
  sets val at index in array arr
*/
bool array_str_setat(array_str_t *arr, char *val, size_t index) {
  if (index < arr->size) {
    assert(arr->array[index] != NULL);
    free(arr->array[index]);

    size_t len = strlen(val);
    char *cpy_val = calloc(len + 1, sizeof(char));
    if (cpy_val) {
      strcpy(cpy_val, val);
      arr->array[index] = cpy_val;
      arr->size++;
      return true;
    } else {
      errno = ENOMEM;
      return false;
    }
  }
  errno = ENOMEM;
  return false;
}

/*
  returns the value at index in array arr
*/
char *array_str_getat(array_str_t *arr, size_t index) {
  if (index < arr->size) {
    return arr->array[index];
  }
  errno = EINVAL;
  return 0;
}

/*
  clear the arr and frees all allocated memory
*/
bool array_str_clear(array_str_t *arr) {
  for (size_t i = 0; i < arr->size; i++) {
    free(arr->array[i]);
  }

  free(arr->array);
  free(arr);
  return true;
}

/* array_p_void_t functions & methods */

/*
  creates and returns a new array_p_void_t pointer
*/
array_p_void_t *array_p_void_new() {
  array_p_void_t *arr = malloc(sizeof(array_p_void_t));
  if (!arr) {
    errno = ENOMEM;
    return NULL;
  }

  arr->array = calloc(4, sizeof(void *));
  if (!arr->array) {
    free(arr);
    errno = ENOMEM;
    return NULL;
  }

  memset(arr->array, 0, 4 * sizeof(void *));
  arr->capacity = 4;
  arr->size = 0;

  return arr;
}

/*
  returns the size of the array
*/
size_t array_p_void_size(array_p_void_t *arr) { return arr->size; }

/*
  appends val to the end of the array arr
*/
bool array_p_void_append(array_p_void_t *arr, void *val) {
  if (arr->size < arr->capacity) {
    arr->array[arr->size++] = val;
    return true;
  } else {
    arr->array = realloc(arr->array, ((arr->capacity + 4) * sizeof(void *)));

    if (arr->array) {
      memset(arr->array + arr->size, 0, 4 * sizeof(void *));
      arr->capacity += 4;

      arr->array[arr->size++] = val;
      return true;
    }
    errno = ENOMEM;
    return false;
  }
}

/*
  removes and returns the last value in the array arr
*/
void *array_p_void_pop(array_p_void_t *arr) {
  if (arr->size == 0) {
    errno = EINVAL;
    return 0;
  }

  void *last_value = arr->array[--(arr->size)];
  arr->array[arr->size] = NULL;

  if (arr->size + 4 < arr->capacity) {
    arr->array = realloc(arr->array, ((arr->capacity - 4) * sizeof(void *)));
    arr->capacity -= 4;
  }

  return last_value;
}

/*
  sets val at index in array arr
*/
bool array_p_void_setat(array_p_void_t *arr, void *val, size_t index) {
  if (index < arr->size) {
    arr->array[index] = val;
    return true;
  }
  errno = ENOMEM;
  return false;
}

/*
  returns the value at index in array arr
*/
void *array_p_void_getat(array_p_void_t *arr, size_t index) {
  if (index < arr->size) {
    return arr->array[index];
  }
  errno = EINVAL;
  return 0;
}

/*
  clear the arr and frees all allocated memory
*/
bool array_p_void_clear(array_p_void_t *arr) {
  free(arr->array);
  free(arr);
  return true;
}

/* array_p_ffi_type_t functions & methods */

/*
  creates and returns a new array_p_ffi_type_t pointer
*/
array_p_ffi_type_t *array_p_ffi_type_new() {
  array_p_ffi_type_t *arr = malloc(sizeof(array_p_ffi_type_t));
  if (!arr) {
    errno = ENOMEM;
    return NULL;
  }

  arr->array = calloc(4, sizeof(ffi_type *));
  if (!arr->array) {
    free(arr);
    errno = ENOMEM;
    return NULL;
  }

  memset(arr->array, 0, 4 * sizeof(ffi_type *));
  arr->capacity = 4;
  arr->size = 0;

  return arr;
}

/*
  returns the size of the array
*/
size_t array_p_ffi_type_size(array_p_ffi_type_t *arr) { return arr->size; }

/*
  appends val to the end of the array arr
*/
bool array_p_ffi_type_append(array_p_ffi_type_t *arr, ffi_type *val) {
  if (arr->size < arr->capacity) {
    arr->array[arr->size++] = val;
    return true;
  } else {
    arr->array =
        realloc(arr->array, ((arr->capacity + 4) * sizeof(ffi_type *)));

    if (arr->array) {
      memset(arr->array + arr->size, 0, 4 * sizeof(ffi_type *));
      arr->capacity += 4;

      arr->array[arr->size++] = val;
      return true;
    }
    errno = ENOMEM;
    return false;
  }
}

/*
  removes and returns the last value in the array arr
*/
ffi_type *array_p_ffi_type_pop(array_p_ffi_type_t *arr) {
  if (arr->size == 0) {
    errno = EINVAL;
    return 0;
  }

  ffi_type *last_value = arr->array[--(arr->size)];
  arr->array[arr->size] = NULL;

  if (arr->size + 4 < arr->capacity) {
    arr->array =
        realloc(arr->array, ((arr->capacity - 4) * sizeof(ffi_type *)));
    arr->capacity -= 4;
  }

  return last_value;
}

/*
  sets val at index in array arr
*/
bool array_p_ffi_type_setat(array_p_ffi_type_t *arr, ffi_type *val,
                            size_t index) {
  if (index < arr->size) {
    arr->array[index] = val;
    return true;
  }
  errno = ENOMEM;
  return false;
}

/*
  returns the value at index in array arr
*/
ffi_type *array_p_ffi_type_getat(array_p_ffi_type_t *arr, size_t index) {
  if (index < arr->size) {
    return arr->array[index];
  }
  errno = EINVAL;
  return 0;
}

/*
  clear the arr and frees all allocated memory
*/
bool array_p_ffi_type_clear(array_p_ffi_type_t *arr) {
  free(arr->array);
  free(arr);
  return true;
}

/* array_CXTypeKind_t functions & methods */

/*
  creates and returns a new array_CXTypeKind_t pointer
*/
array_CXTypeKind_t *array_CXTypeKind_new() {
  array_CXTypeKind_t *arr = malloc(sizeof(array_CXTypeKind_t));
  if (!arr) {
    errno = ENOMEM;
    return NULL;
  }

  arr->array = calloc(4, sizeof(long long));
  if (!arr->array) {
    free(arr);
    errno = ENOMEM;
    return NULL;
  }

  arr->capacity = 4;
  arr->size = 0;

  return arr;
}

/*
  returns the size of the array
*/
size_t array_CXTypeKind_size(array_CXTypeKind_t *arr) { return arr->size; }

/*
  appends val to the end of the array arr
*/
bool array_CXTypeKind_append(array_CXTypeKind_t *arr, enum CXTypeKind val) {
  if (arr->size < arr->capacity) {
    arr->array[arr->size] = val;
    arr->size++;
    return true;
  } else {
    arr->array =
        realloc(arr->array, ((arr->capacity + 4) * sizeof(enum CXTypeKind)));

    if (arr->array) {
      arr->capacity += 4;
      arr->array[arr->size] = val;
      arr->size++;
      return true;
    }
    errno = ENOMEM;
    return false;
  }
}

/*
  removes and returns the last value in the array arr
*/
enum CXTypeKind array_CXTypeKind_pop(array_CXTypeKind_t *arr) {
  if (arr->size == 0) {
    errno = EINVAL;
    return 0;
  }

  enum CXTypeKind last_value = arr->array[--(arr->size)];

  if (arr->size + 4 < arr->capacity) {
    arr->array =
        realloc(arr->array, ((arr->capacity - 4) * sizeof(enum CXTypeKind)));
    arr->capacity -= 4;
  }

  return last_value;
}

/*
  sets val at index in array arr
*/
bool array_CXTypeKind_setat(array_CXTypeKind_t *arr, enum CXTypeKind val,
                            size_t index) {
  if (index < arr->size) {
    arr->array[index] = val;
    return true;
  }
  errno = ENOMEM;
  return false;
}

/*
  returns the value at index in array arr
*/
enum CXTypeKind array_CXTypeKind_getat(array_CXTypeKind_t *arr, size_t index) {
  if (index < arr->size) {
    return arr->array[index];
  }
  errno = EINVAL;
  return 0;
}

/*
  clear the arr and frees all allocated memory
*/
bool array_CXTypeKind_clear(array_CXTypeKind_t *arr) {
  free(arr->array);
  free(arr);
  return true;
}

/* array_Structure_t functions & methods */

/*
  creates and returns a new array_Structure_t pointer
*/
array_Structure_t *array_Structure_new() {
  array_Structure_t *arr = malloc(sizeof(array_Structure_t));
  if (!arr) {
    errno = ENOMEM;
    return NULL;
  }

  arr->array = calloc(4, sizeof(Structure));
  if (!arr->array) {
    free(arr);
    errno = ENOMEM;
    return NULL;
  }

  arr->capacity = 4;
  arr->size = 0;

  return arr;
}

/*
  returns the size of the array
*/
size_t array_Structure_size(array_Structure_t *arr) { return arr->size; }

/*
  appends val to the end of the array arr
*/
bool array_Structure_append(array_Structure_t *arr, Structure val) {
  if (arr->size < arr->capacity) {
    arr->array[arr->size] = val;
    arr->size++;
    return true;
  } else {
    arr->array = realloc(arr->array, ((arr->capacity + 4) * sizeof(Structure)));

    if (arr->array) {
      arr->capacity += 4;
      arr->array[arr->size] = val;
      arr->size++;
      return true;
    }
    errno = ENOMEM;
    return false;
  }
}

/*
  removes and returns the last value in the array arr
*/
Structure array_Structure_pop(array_Structure_t *arr) {
  if (arr->size == 0) {
    errno = EINVAL;
    return (Structure){0};
  }

  Structure last_value = arr->array[--(arr->size)];

  if (arr->size + 4 < arr->capacity) {
    arr->array = realloc(arr->array, ((arr->capacity - 4) * sizeof(Structure)));
    arr->capacity -= 4;
  }

  return last_value;
}

/*
  sets val at index in array arr
*/
bool array_Structure_setat(array_Structure_t *arr, Structure val,
                           size_t index) {
  if (index < arr->size) {
    if ((arr->array[index]).name) {
      Symbols_clearStructure(arr->array + index);
    }

    arr->array[index] = val;
    return true;
  }
  errno = EINVAL;
  return false;
}

/*
  returns the value at index in array arr
*/
Structure array_Structure_getat(array_Structure_t *arr, size_t index) {
  if (index < arr->size) {
    return arr->array[index];
  }
  errno = EINVAL;
  return (Structure){0};
}

/*
  returns the pointer to value at index in array arr
*/
Structure *array_Structure_get_ptr_at(array_Structure_t *arr, size_t index) {
  if (index < arr->size) {
    return &arr->array[index];
  }
  errno = EINVAL;
  return NULL;
}

/*
  clear the arr and frees all allocated memory
*/
bool array_Structure_clear(array_Structure_t *arr) {
  for (size_t i = 0; i < arr->size; i++) {
    Symbols_clearStructure(arr->array + i);
  }

  free(arr->array);
  free(arr);
  return true;
}

/* list_Structure_t functions & methods */

/*
  creates and returns a new list_Structure_t pointer
*/
list_Structure_t *list_Structure_new() {
  list_Structure_t *li = malloc(sizeof(list_Structure_t));
  if (!li) {
    errno = ENOMEM;
    return false;
  }

  li->first = NULL;
  li->last = NULL;
  li->size = 0;
  return li;
}

/*
  returns the size of the list
*/
size_t list_Structure_size(list_Structure_t *li) { return li->size; }

/*
  appends val to the end of the list
*/
bool list_Structure_append(list_Structure_t *li, Structure val) {
  node_list_Structure *new = malloc(sizeof(node_list_Structure));
  if (!new) {
    errno = ENOMEM;
    return false;
  }

  new->element = val;
  new->next = NULL;

  if (li->size > 0) {
    node_list_Structure *last = li->last;
    assert(last->next == NULL);
    last->next = new;

    new->previous = last;

  } else {
    li->first = new;
    new->previous = NULL;
  }

  li->last = new;
  li->size++;

  return true;
}

/*
  removes and returns the last value in the list
*/
Structure list_Structure_pop(list_Structure_t *li) {
  node_list_Structure *last = li->last;
  last->previous->next = NULL;
  li->last = last->previous;
  li->size--;

  Structure s = last->element;
  free(last);
  return s;
}
/*
  sets val at index in list
*/
bool list_Structure_setat(list_Structure_t *li, Structure val, size_t index) {
  if (index >= li->size) {
    errno = EINVAL;
    return false;
  }

  node_list_Structure *node = li->first;

  for (size_t i = 0; i < index; i++) {
    node = node->next;
  }

  Symbols_clearStructure(&(node->element));
  node->element = val;
  return true;
}

/*
  returns the value at index in list
*/
Structure list_Structure_getat(list_Structure_t *li, size_t index) {
  if (index >= li->size) {
    errno = EINVAL;
    return (Structure){0};
  }

  node_list_Structure *node = li->first;

  for (size_t i = 0; i < index; i++) {
    node = node->next;
  }

  return node->element;
}

/*
  returns the pointer to value at index in list
*/
Structure *list_Structure_get_ptr_at(list_Structure_t *li, size_t index) {
  if (index >= li->size) {
    errno = EINVAL;
    return NULL;
  }

  node_list_Structure *node = li->first;

  for (size_t i = 0; i < index; i++) {
    node = node->next;
  }

  return &node->element;
}

/*
  clear the list and frees all allocated memory
*/
bool list_Structure_clear(list_Structure_t *li) {
  node_list_Structure *node = li->first;

  for (size_t i = 0; i < li->size; i++) {
    node_list_Structure *next = node->next;
    Symbols_clearStructure(&(node->element));
    free(node);
    node = next;
  }

  free(li);
  return true;
}

/* array_p_Structure_t functions & methods */

/*
  creates and returns a new array_p_Structure_t pointer
*/
array_p_Structure_t *array_p_Structure_new() {
  array_p_Structure_t *arr = malloc(sizeof(array_p_Structure_t));
  if (!arr) {
    errno = ENOMEM;
    return NULL;
  }

  arr->array = calloc(4, sizeof(Structure *));
  if (!arr->array) {
    free(arr);
    errno = ENOMEM;
    return NULL;
  }

  memset(arr->array, 0, 4 * sizeof(Structure *));
  arr->capacity = 4;
  arr->size = 0;

  return arr;
}

/*
  returns the size of the array
*/
size_t array_p_Structure_size(array_p_Structure_t *arr) { return arr->size; }

/*
  appends val to the end of the array arr
*/
bool array_p_Structure_append(array_p_Structure_t *arr, Structure *val) {
  if (arr->size < arr->capacity) {
    arr->array[arr->size++] = val;
    return true;
  } else {
    arr->array =
        realloc(arr->array, ((arr->capacity + 4) * sizeof(Structure *)));

    if (arr->array) {
      memset(arr->array + arr->size, 0, 4 * sizeof(Structure *));
      arr->capacity += 4;

      arr->array[arr->size++] = val;
      return true;
    }
    errno = ENOMEM;
    return false;
  }
}

/*
  removes and returns the last value in the array arr
*/
Structure *array_p_Structure_pop(array_p_Structure_t *arr) {
  if (arr->size == 0) {
    errno = EINVAL;
    return 0;
  }

  Structure *last_value = arr->array[--(arr->size)];
  arr->array[arr->size] = NULL;

  if (arr->size + 4 < arr->capacity) {
    arr->array =
        realloc(arr->array, ((arr->capacity - 4) * sizeof(Structure *)));
    arr->capacity -= 4;
  }

  return last_value;
}

/*
  sets val at index in array arr
*/
bool array_p_Structure_setat(array_p_Structure_t *arr, Structure *val,
                             size_t index) {
  if (index < arr->size) {
    arr->array[index] = val;
    return true;
  }
  errno = ENOMEM;
  return false;
}

/*
  returns the value at index in array arr
*/
Structure *array_p_Structure_getat(array_p_Structure_t *arr, size_t index) {
  if (index < arr->size) {
    return arr->array[index];
  }
  errno = EINVAL;
  return 0;
}

/*
  clear the arr and frees all allocated memory
*/
bool array_p_Structure_clear(array_p_Structure_t *arr) {
  free(arr->array);
  free(arr);
  return true;
}

/* array_Union_t functions & methods */

/*
  creates and returns a new array_Union_t pointer
*/
array_Union_t *array_Union_new() {
  array_Union_t *arr = malloc(sizeof(array_Union_t));
  if (!arr) {
    errno = ENOMEM;
    return NULL;
  }

  arr->array = calloc(4, sizeof(Union));
  if (!arr->array) {
    free(arr);
    errno = ENOMEM;
    return NULL;
  }

  arr->capacity = 4;
  arr->size = 0;

  return arr;
}

/*
  returns the size of the array
*/
size_t array_Union_size(array_Union_t *arr) { return arr->size; }

/*
  appends val to the end of the array arr
*/
bool array_Union_append(array_Union_t *arr, Union val) {
  if (arr->size < arr->capacity) {
    arr->array[arr->size] = val;
    arr->size++;
    return true;
  } else {
    arr->array = realloc(arr->array, ((arr->capacity + 4) * sizeof(Union)));

    if (arr->array) {
      arr->capacity += 4;
      arr->array[arr->size] = val;
      arr->size++;
      return true;
    }
    errno = ENOMEM;
    return false;
  }
}

/*
  removes and returns the last value in the array arr
*/
Union array_Union_pop(array_Union_t *arr) {
  if (arr->size == 0) {
    errno = EINVAL;
    return (Union){0};
  }

  Union last_value = arr->array[--(arr->size)];

  if (arr->size + 4 < arr->capacity) {
    arr->array = realloc(arr->array, ((arr->capacity - 4) * sizeof(Union)));
    arr->capacity -= 4;
  }

  return last_value;
}

/*
  sets val at index in array arr
*/
bool array_Union_setat(array_Union_t *arr, Union val, size_t index) {
  if (index < arr->size) {
    if ((arr->array[index]).name) {
      Symbols_clearUnion(arr->array + index);
    }

    arr->array[index] = val;
    return true;
  }
  errno = ENOMEM;
  return false;
}

/*
  returns the value at index in array arr
*/
Union array_Union_getat(array_Union_t *arr, size_t index) {
  if (index < arr->size) {
    return arr->array[index];
  }
  errno = EINVAL;
  return (Union){0};
}

/*
  returns the pointer to value at index in array arr
*/
Union *array_Union_get_ptr_at(array_Union_t *arr, size_t index) {
  if (index < arr->size) {
    return &arr->array[index];
  }
  errno = EINVAL;
  return NULL;
}

/*
  clear the arr and frees all allocated memory
*/
bool array_Union_clear(array_Union_t *arr) {
  for (size_t i = 0; i < arr->size; i++) {
    Symbols_clearUnion(arr->array + i);
  }

  free(arr->array);
  free(arr);
  return true;
}

/* list_Union_t functions & methods */

/*
  creates and returns a new list_Union_t pointer
*/
list_Union_t *list_Union_new() {
  list_Union_t *li = malloc(sizeof(list_Union_t));
  if (!li) {
    errno = ENOMEM;
    return false;
  }

  li->first = NULL;
  li->last = NULL;
  li->size = 0;
  return li;
}

/*
  returns the size of the list
*/
size_t list_Union_size(list_Union_t *li) { return li->size; }

/*
  appends val to the end of the list
*/
bool list_Union_append(list_Union_t *li, Union val) {
  node_list_Union *new = malloc(sizeof(node_list_Union));
  if (!new) {
    errno = ENOMEM;
    return false;
  }

  new->element = val;
  new->next = NULL;

  if (li->size > 0) {
    node_list_Union *last = li->last;
    assert(last->next == NULL);
    last->next = new;

    new->previous = last;

  } else {
    li->first = new;
    new->previous = NULL;
  }

  li->last = new;
  li->size++;

  return true;
}

/*
  removes and returns the last value in the list
*/
Union list_Union_pop(list_Union_t *li) {
  node_list_Union *last = li->last;
  last->previous->next = NULL;
  li->last = last->previous;
  li->size--;

  Union s = last->element;
  free(last);
  return s;
}
/*
  sets val at index in list
*/
bool list_Union_setat(list_Union_t *li, Union val, size_t index) {
  if (index >= li->size) {
    errno = EINVAL;
    return false;
  }

  node_list_Union *node = li->first;

  for (size_t i = 0; i < index; i++) {
    node = node->next;
  }

  Symbols_clearUnion(&(node->element));
  node->element = val;
  return true;
}

/*
  returns the value at index in list
*/
Union list_Union_getat(list_Union_t *li, size_t index) {
  if (index >= li->size) {
    errno = EINVAL;
    return (Union){0};
  }

  node_list_Union *node = li->first;

  for (size_t i = 0; i < index; i++) {
    node = node->next;
  }

  return node->element;
}

/*
  returns the pointer to value at index in list
*/
Union *list_Union_get_ptr_at(list_Union_t *li, size_t index) {
  if (index >= li->size) {
    errno = EINVAL;
    return NULL;
  }

  node_list_Union *node = li->first;

  for (size_t i = 0; i < index; i++) {
    node = node->next;
  }

  return &node->element;
}

/*
  clear the list and frees all allocated memory
*/
bool list_Union_clear(list_Union_t *li) {
  node_list_Union *node = li->first;

  for (size_t i = 0; i < li->size; i++) {
    node_list_Union *next = node->next;
    Symbols_clearUnion(&(node->element));
    free(node);
    node = next;
  }

  free(li);
  return true;
}

/* array_p_Union_t functions & methods */

/*
  creates and returns a new array_p_Union_t pointer
*/
array_p_Union_t *array_p_Union_new() {
  array_p_Union_t *arr = malloc(sizeof(array_p_Union_t));
  if (!arr) {
    errno = ENOMEM;
    return NULL;
  }

  arr->array = calloc(4, sizeof(Union *));
  if (!arr->array) {
    free(arr);
    errno = ENOMEM;
    return NULL;
  }

  memset(arr->array, 0, 4 * sizeof(Union *));
  arr->capacity = 4;
  arr->size = 0;

  return arr;
}

/*
  returns the size of the array
*/
size_t array_p_Union_size(array_p_Union_t *arr) { return arr->size; }

/*
  appends val to the end of the array arr
*/
bool array_p_Union_append(array_p_Union_t *arr, Union *val) {
  if (arr->size < arr->capacity) {
    arr->array[arr->size++] = val;
    return true;
  } else {
    arr->array = realloc(arr->array, ((arr->capacity + 4) * sizeof(Union *)));

    if (arr->array) {
      memset(arr->array + arr->size, 0, 4 * sizeof(Union *));
      arr->capacity += 4;

      arr->array[arr->size++] = val;
      return true;
    }
    errno = ENOMEM;
    return false;
  }
}

/*
  removes and returns the last value in the array arr
*/
Union *array_p_Union_pop(array_p_Union_t *arr) {
  if (arr->size == 0) {
    errno = EINVAL;
    return 0;
  }

  Union *last_value = arr->array[--(arr->size)];
  arr->array[arr->size] = NULL;

  if (arr->size + 4 < arr->capacity) {
    arr->array = realloc(arr->array, ((arr->capacity - 4) * sizeof(Union *)));
    arr->capacity -= 4;
  }

  return last_value;
}

/*
  sets val at index in array arr
*/
bool array_p_Union_setat(array_p_Union_t *arr, Union *val, size_t index) {
  if (index < arr->size) {
    arr->array[index] = val;
    return true;
  }
  errno = ENOMEM;
  return false;
}

/*
  returns the value at index in array arr
*/
Union *array_p_Union_getat(array_p_Union_t *arr, size_t index) {
  if (index < arr->size) {
    return arr->array[index];
  }
  errno = EINVAL;
  return 0;
}

/*
  clear the arr and frees all allocated memory
*/
bool array_p_Union_clear(array_p_Union_t *arr) {
  free(arr->array);
  free(arr);
  return true;
}

/* array_Global_t functions & methods */

/*
  creates and returns a new array_Global_t pointer
*/
array_Global_t *array_Global_new() {
  array_Global_t *arr = malloc(sizeof(array_Global_t));
  if (!arr) {
    errno = ENOMEM;
    return NULL;
  }

  arr->array = calloc(4, sizeof(Global));
  if (!arr->array) {
    free(arr);
    errno = ENOMEM;
    return NULL;
  }

  arr->capacity = 4;
  arr->size = 0;

  return arr;
}

/*
  returns the size of the array
*/
size_t array_Global_size(array_Global_t *arr) { return arr->size; }

/*
  appends val to the end of the array arr
*/
bool array_Global_append(array_Global_t *arr, Global val) {
  if (arr->size < arr->capacity) {
    arr->array[arr->size] = val;
    arr->size++;
    return true;
  } else {
    arr->array = realloc(arr->array, ((arr->capacity + 4) * sizeof(Global)));

    if (arr->array) {
      arr->capacity += 4;
      arr->array[arr->size] = val;
      arr->size++;
      return true;
    }
    errno = ENOMEM;
    return false;
  }
}

/*
  removes and returns the last value in the array arr
*/
Global array_Global_pop(array_Global_t *arr) {
  if (arr->size == 0) {
    errno = EINVAL;
    return (Global){0};
  }

  Global last_value = arr->array[--(arr->size)];

  if (arr->size + 4 < arr->capacity) {
    arr->array = realloc(arr->array, ((arr->capacity - 4) * sizeof(Global)));
    arr->capacity -= 4;
  }

  return last_value;
}

/*
  sets val at index in array arr
*/
bool array_Global_setat(array_Global_t *arr, Global val, size_t index) {
  if (index < arr->size) {
    if ((arr->array[index]).name) {
      Symbols_clearGlobal(arr->array + index);
    }

    arr->array[index] = val;
    return true;
  }
  errno = ENOMEM;
  return false;
}

/*
  returns the value at index in array arr
*/
Global array_Global_getat(array_Global_t *arr, size_t index) {
  if (index < arr->size) {
    return arr->array[index];
  }
  errno = EINVAL;
  return (Global){0};
}

/*
  returns the pointer to value at index in array arr
*/
Global *array_Global_get_ptr_at(array_Global_t *arr, size_t index) {
  if (index < arr->size) {
    return &arr->array[index];
  }
  errno = EINVAL;
  return NULL;
}

/*
  clear the arr and frees all allocated memory
*/
bool array_Global_clear(array_Global_t *arr) {
  for (size_t i = 0; i < arr->size; i++) {
    Symbols_clearGlobal(arr->array + i);
  }

  free(arr->array);
  free(arr);
  return true;
}

/* list_Global_t functions & methods */

/*
  creates and returns a new list_Global_t pointer
*/
list_Global_t *list_Global_new() {
  list_Global_t *li = malloc(sizeof(list_Global_t));
  if (!li) {
    errno = ENOMEM;
    return false;
  }

  li->first = NULL;
  li->last = NULL;
  li->size = 0;
  return li;
}

/*
  returns the size of the list
*/
size_t list_Global_size(list_Global_t *li) { return li->size; }

/*
  appends val to the end of the list
*/
bool list_Global_append(list_Global_t *li, Global val) {
  node_list_Global *new = malloc(sizeof(node_list_Global));
  if (!new) {
    errno = ENOMEM;
    return false;
  }

  new->element = val;
  new->next = NULL;

  if (li->size > 0) {
    node_list_Global *last = li->last;
    assert(last->next == NULL);
    last->next = new;

    new->previous = last;

  } else {
    li->first = new;
    new->previous = NULL;
  }

  li->last = new;
  li->size++;

  return true;
}

/*
  removes and returns the last value in the list
*/
Global list_Global_pop(list_Global_t *li) {
  node_list_Global *last = li->last;
  last->previous->next = NULL;
  li->last = last->previous;
  li->size--;

  Global s = last->element;
  free(last);
  return s;
}
/*
  sets val at index in list
*/
bool list_Global_setat(list_Global_t *li, Global val, size_t index) {
  if (index >= li->size) {
    errno = EINVAL;
    return false;
  }

  node_list_Global *node = li->first;

  for (size_t i = 0; i < index; i++) {
    node = node->next;
  }

  Symbols_clearGlobal(&(node->element));
  node->element = val;
  return true;
}

/*
  returns the value at index in list
*/
Global list_Global_getat(list_Global_t *li, size_t index) {
  if (index >= li->size) {
    errno = EINVAL;
    return (Global){0};
  }

  node_list_Global *node = li->first;

  for (size_t i = 0; i < index; i++) {
    node = node->next;
  }

  return node->element;
}

/*
  returns the pointer to value at index in list
*/
Global *list_Global_get_ptr_at(list_Global_t *li, size_t index) {
  if (index >= li->size) {
    errno = EINVAL;
    return NULL;
  }

  node_list_Global *node = li->first;

  for (size_t i = 0; i < index; i++) {
    node = node->next;
  }

  return &node->element;
}

/*
  clear the list and frees all allocated memory
*/
bool list_Global_clear(list_Global_t *li) {
  node_list_Global *node = li->first;

  for (size_t i = 0; i < li->size; i++) {
    node_list_Global *next = node->next;
    Symbols_clearGlobal(&(node->element));
    free(node);
    node = next;
  }

  free(li);
  return true;
}

/* array_TypeDef_t functions & methods */

/*
  creates and returns a new array_TypeDef_t pointer
*/
array_TypeDef_t *array_TypeDef_new() {
  array_TypeDef_t *arr = malloc(sizeof(array_TypeDef_t));
  if (!arr) {
    errno = ENOMEM;
    return NULL;
  }

  arr->array = calloc(4, sizeof(TypeDef));
  if (!arr->array) {
    free(arr);
    errno = ENOMEM;
    return NULL;
  }

  arr->capacity = 4;
  arr->size = 0;

  return arr;
}

/*
  returns the size of the array
*/
size_t array_TypeDef_size(array_TypeDef_t *arr) { return arr->size; }

/*
  appends val to the end of the array arr
*/
bool array_TypeDef_append(array_TypeDef_t *arr, TypeDef val) {
  if (arr->size < arr->capacity) {
    arr->array[arr->size] = val;
    arr->size++;
    return true;
  } else {
    arr->array = realloc(arr->array, ((arr->capacity + 4) * sizeof(TypeDef)));

    if (arr->array) {
      arr->capacity += 4;
      arr->array[arr->size] = val;
      arr->size++;
      return true;
    }
    errno = ENOMEM;
    return false;
  }
}

/*
  removes and returns the last value in the array arr
*/
TypeDef array_TypeDef_pop(array_TypeDef_t *arr) {
  if (arr->size == 0) {
    errno = EINVAL;
    return (TypeDef){0};
  }

  TypeDef last_value = arr->array[--(arr->size)];

  if (arr->size + 4 < arr->capacity) {
    arr->array = realloc(arr->array, ((arr->capacity - 4) * sizeof(TypeDef)));
    arr->capacity -= 4;
  }

  return last_value;
}

/*
  sets val at index in array arr
*/
bool array_TypeDef_setat(array_TypeDef_t *arr, TypeDef val, size_t index) {
  if (index < arr->size) {
    if ((arr->array[index]).name) {
      Symbols_clearTypedef(arr->array + index);
    }

    arr->array[index] = val;
    return true;
  }
  errno = ENOMEM;
  return false;
}

/*
  returns the value at index in array arr
*/
TypeDef array_TypeDef_getat(array_TypeDef_t *arr, size_t index) {
  if (index < arr->size) {
    return arr->array[index];
  }
  errno = EINVAL;
  return (TypeDef){0};
}

/*
  returns the pointer to value at index in array arr
*/
TypeDef *array_TypeDef_get_ptr_at(array_TypeDef_t *arr, size_t index) {
  if (index < arr->size) {
    return &arr->array[index];
  }
  errno = EINVAL;
  return NULL;
}

/*
  clear the arr and frees all allocated memory
*/
bool array_TypeDef_clear(array_TypeDef_t *arr) {
  for (size_t i = 0; i < arr->size; i++) {
    Symbols_clearTypedef(arr->array + i);
  }

  free(arr->array);
  free(arr);
  return true;
}

/* list_TypeDef_t functions & methods */

/*
  creates and returns a new list_TypeDef_t pointer
*/
list_TypeDef_t *list_TypeDef_new() {
  list_TypeDef_t *li = malloc(sizeof(list_TypeDef_t));
  if (!li) {
    errno = ENOMEM;
    return false;
  }

  li->first = NULL;
  li->last = NULL;
  li->size = 0;
  return li;
}

/*
  returns the size of the list
*/
size_t list_TypeDef_size(list_TypeDef_t *li) { return li->size; }

/*
  appends val to the end of the list
*/
bool list_TypeDef_append(list_TypeDef_t *li, TypeDef val) {
  node_list_TypeDef *new = malloc(sizeof(node_list_TypeDef));
  if (!new) {
    errno = ENOMEM;
    return false;
  }

  new->element = val;
  new->next = NULL;

  if (li->size > 0) {
    node_list_TypeDef *last = li->last;
    assert(last->next == NULL);
    last->next = new;

    new->previous = last;

  } else {
    li->first = new;
    new->previous = NULL;
  }

  li->last = new;
  li->size++;

  return true;
}

/*
  removes and returns the last value in the list
*/
TypeDef list_TypeDef_pop(list_TypeDef_t *li) {
  node_list_TypeDef *last = li->last;
  last->previous->next = NULL;
  li->last = last->previous;
  li->size--;

  TypeDef s = last->element;
  free(last);
  return s;
}
/*
  sets val at index in list
*/
bool list_TypeDef_setat(list_TypeDef_t *li, TypeDef val, size_t index) {
  if (index >= li->size) {
    errno = EINVAL;
    return false;
  }

  node_list_TypeDef *node = li->first;

  for (size_t i = 0; i < index; i++) {
    node = node->next;
  }

  Symbols_clearTypedef(&(node->element));
  node->element = val;
  return true;
}

/*
  returns the value at index in list
*/
TypeDef list_TypeDef_getat(list_TypeDef_t *li, size_t index) {
  if (index >= li->size) {
    errno = EINVAL;
    return (TypeDef){0};
  }

  node_list_TypeDef *node = li->first;

  for (size_t i = 0; i < index; i++) {
    node = node->next;
  }

  return node->element;
}

/*
  returns the pointer to value at index in list
*/
TypeDef *list_TypeDef_get_ptr_at(list_TypeDef_t *li, size_t index) {
  if (index >= li->size) {
    errno = EINVAL;
    return NULL;
  }

  node_list_TypeDef *node = li->first;

  for (size_t i = 0; i < index; i++) {
    node = node->next;
  }

  return &node->element;
}

/*
  clear the list and frees all allocated memory
*/
bool list_TypeDef_clear(list_TypeDef_t *li) {
  node_list_TypeDef *node = li->first;

  for (size_t i = 0; i < li->size; i++) {
    node_list_TypeDef *next = node->next;
    Symbols_clearTypedef(&(node->element));
    free(node);
    node = next;
  }

  free(li);
  return true;
}

/* array_FunctionType_t functions & methods */

/*
  creates and returns a new array_FunctionType_t pointer
*/
array_FunctionType_t *array_FunctionType_new() {
  array_FunctionType_t *arr = malloc(sizeof(array_FunctionType_t));
  if (!arr) {
    errno = ENOMEM;
    return NULL;
  }

  arr->array = calloc(4, sizeof(FunctionType));
  if (!arr->array) {
    free(arr);
    errno = ENOMEM;
    return NULL;
  }

  arr->capacity = 4;
  arr->size = 0;

  return arr;
}

/*
  returns the size of the array
*/
size_t array_FunctionType_size(array_FunctionType_t *arr) { return arr->size; }

/*
  appends val to the end of the array arr
*/
bool array_FunctionType_append(array_FunctionType_t *arr, FunctionType val) {
  if (arr->size < arr->capacity) {
    arr->array[arr->size] = val;
    arr->size++;
    return true;
  } else {
    arr->array =
        realloc(arr->array, ((arr->capacity + 4) * sizeof(FunctionType)));

    if (arr->array) {
      arr->capacity += 4;
      arr->array[arr->size] = val;
      arr->size++;
      return true;
    }
    errno = ENOMEM;
    return false;
  }
}

/*
  removes and returns the last value in the array arr
*/
FunctionType array_FunctionType_pop(array_FunctionType_t *arr) {
  if (arr->size == 0) {
    errno = EINVAL;
    return (FunctionType){0};
  }

  FunctionType last_value = arr->array[--(arr->size)];

  if (arr->size + 4 < arr->capacity) {
    arr->array =
        realloc(arr->array, ((arr->capacity + 4) * sizeof(FunctionType)));
    arr->capacity -= 4;
  }

  return last_value;
}

/*
  sets val at index in array arr
*/
bool array_FunctionType_setat(array_FunctionType_t *arr, FunctionType val,
                              size_t index) {
  if (index < arr->size) {
    if ((arr->array[index]).mangledName) {
      Symbols_clearFunctionType(arr->array + index);
    }

    arr->array[index] = val;
    return true;
  }
  errno = ENOMEM;
  return false;
}

/*
  returns the value at index in array arr
*/
FunctionType array_FunctionType_getat(array_FunctionType_t *arr, size_t index) {
  if (index < arr->size) {
    return arr->array[index];
  }
  errno = EINVAL;
  return (FunctionType){0};
}

/*
  returns the pointer to value at index in array arr
*/
FunctionType *array_FunctionType_get_ptr_at(array_FunctionType_t *arr,
                                            size_t index) {
  if (index < arr->size) {
    return &arr->array[index];
  }
  errno = EINVAL;
  return NULL;
}

/*
  clear the arr and frees all allocated memory
*/
bool array_FunctionType_clear(array_FunctionType_t *arr) {
  for (size_t i = 0; i < arr->size; i++) {
    Symbols_clearFunctionType(arr->array + i);
  }

  free(arr->array);
  free(arr);
  return true;
}

/* list_FunctionType_t functions & methods */

/*
  creates and returns a new list_FunctionType_t pointer
*/
list_FunctionType_t *list_FunctionType_new() {
  list_FunctionType_t *li = malloc(sizeof(list_FunctionType_t));
  if (!li) {
    errno = ENOMEM;
    return false;
  }

  li->first = NULL;
  li->last = NULL;
  li->size = 0;
  return li;
}

/*
  returns the size of the list
*/
size_t list_FunctionType_size(list_FunctionType_t *li) { return li->size; }

/*
  appends val to the end of the list
*/
bool list_FunctionType_append(list_FunctionType_t *li, FunctionType val) {
  node_list_FunctionType *new = malloc(sizeof(node_list_FunctionType));
  if (!new) {
    errno = ENOMEM;
    return false;
  }

  new->element = val;
  new->next = NULL;

  if (li->size > 0) {
    node_list_FunctionType *last = li->last;
    assert(last->next == NULL);
    last->next = new;

    new->previous = last;

  } else {
    li->first = new;
    new->previous = NULL;
  }

  li->last = new;
  li->size++;

  return true;
}

/*
  removes and returns the last value in the list
*/
FunctionType list_FunctionType_pop(list_FunctionType_t *li) {
  node_list_FunctionType *last = li->last;
  last->previous->next = NULL;
  li->last = last->previous;
  li->size--;

  FunctionType s = last->element;
  free(last);
  return s;
}
/*
  sets val at index in list
*/
bool list_FunctionType_setat(list_FunctionType_t *li, FunctionType val,
                             size_t index) {
  if (index >= li->size) {
    errno = EINVAL;
    return false;
  }

  node_list_FunctionType *node = li->first;

  for (size_t i = 0; i < index; i++) {
    node = node->next;
  }

  Symbols_clearFunctionType(&(node->element));
  node->element = val;
  return true;
}

/*
  returns the value at index in list
*/
FunctionType list_FunctionType_getat(list_FunctionType_t *li, size_t index) {
  if (index >= li->size) {
    errno = EINVAL;
    return (FunctionType){0};
  }

  node_list_FunctionType *node = li->first;

  for (size_t i = 0; i < index; i++) {
    node = node->next;
  }

  return node->element;
}

/*
  returns the pointer to value at index in list
*/
FunctionType *list_FunctionType_get_ptr_at(list_FunctionType_t *li,
                                           size_t index) {
  if (index >= li->size) {
    errno = EINVAL;
    return NULL;
  }

  node_list_FunctionType *node = li->first;

  for (size_t i = 0; i < index; i++) {
    node = node->next;
  }

  return &node->element;
}

/*
  clear the list and frees all allocated memory
*/
bool list_FunctionType_clear(list_FunctionType_t *li) {
  node_list_FunctionType *node = li->first;

  for (size_t i = 0; i < li->size; i++) {
    node_list_FunctionType *next = node->next;
    Symbols_clearFunctionType(&(node->element));
    free(node);
    node = next;
  }

  free(li);
  return true;
}

/* array_Function_t functions & methods */

/*
  creates and returns a new array_Function_t pointer
*/
array_Function_t *array_Function_new() {
  array_Function_t *arr = malloc(sizeof(array_Function_t));
  if (!arr) {
    errno = ENOMEM;
    return NULL;
  }

  arr->array = calloc(4, sizeof(Function));
  if (!arr->array) {
    free(arr);
    errno = ENOMEM;
    return NULL;
  }

  arr->capacity = 4;
  arr->size = 0;

  return arr;
}

/*
  returns the size of the array
*/
size_t array_Function_size(array_Function_t *arr) { return arr->size; }

/*
  appends val to the end of the array arr
*/
bool array_Function_append(array_Function_t *arr, Function val) {
  if (arr->size < arr->capacity) {
    arr->array[arr->size] = val;
    arr->size++;
    return true;
  } else {
    arr->array = realloc(arr->array, ((arr->capacity + 4) * sizeof(Function)));

    if (arr->array) {
      arr->capacity += 4;
      arr->array[arr->size] = val;
      arr->size++;
      return true;
    }
    errno = ENOMEM;
    return false;
  }
}

/*
  removes and returns the last value in the array arr
*/
Function array_Function_pop(array_Function_t *arr) {
  if (arr->size == 0) {
    errno = EINVAL;
    return (Function){0};
  }

  Function last_value = arr->array[--(arr->size)];

  if (arr->size + 4 < arr->capacity) {
    arr->array = realloc(arr->array, ((arr->capacity - 4) * sizeof(Function)));
    arr->capacity -= 4;
  }

  return last_value;
}

/*
  sets val at index in array arr
*/
bool array_Function_setat(array_Function_t *arr, Function val, size_t index) {
  if (index < arr->size) {
    if ((arr->array[index]).name) {
      Symbols_clearFunction(arr->array + index);
    }

    arr->array[index] = val;
    return true;
  }
  errno = ENOMEM;
  return false;
}

/*
  returns the value at index in array arr
*/
Function array_Function_getat(array_Function_t *arr, size_t index) {
  if (index < arr->size) {
    return arr->array[index];
  }
  errno = EINVAL;
  return (Function){0};
}

/*
  returns the pointer to value at index in array arr
*/
Function *array_Function_get_ptr_at(array_Function_t *arr, size_t index) {
  if (index < arr->size) {
    return &arr->array[index];
  }
  errno = EINVAL;
  return NULL;
}

/*
  clear the arr and frees all allocated memory
*/
bool array_Function_clear(array_Function_t *arr) {
  for (size_t i = 0; i < arr->size; i++) {
    Symbols_clearFunction(arr->array + i);
  }

  free(arr->array);
  free(arr);
  return true;
}

/* list_Function_t functions & methods */

/*
  creates and returns a new list_Function_t pointer
*/
list_Function_t *list_Function_new() {
  list_Function_t *li = malloc(sizeof(list_Function_t));
  if (!li) {
    errno = ENOMEM;
    return false;
  }

  li->first = NULL;
  li->last = NULL;
  li->size = 0;
  return li;
}

/*
  returns the size of the list
*/
size_t list_Function_size(list_Function_t *li) { return li->size; }

/*
  appends val to the end of the list
*/
bool list_Function_append(list_Function_t *li, Function val) {
  node_list_Function *new = malloc(sizeof(node_list_Function));
  if (!new) {
    errno = ENOMEM;
    return false;
  }

  new->element = val;
  new->next = NULL;

  if (li->size > 0) {
    node_list_Function *last = li->last;
    assert(last->next == NULL);
    last->next = new;

    new->previous = last;

  } else {
    li->first = new;
    new->previous = NULL;
  }

  li->last = new;
  li->size++;

  return true;
}

/*
  removes and returns the last value in the list
*/
Function list_Function_pop(list_Function_t *li) {
  node_list_Function *last = li->last;
  last->previous->next = NULL;
  li->last = last->previous;
  li->size--;

  Function s = last->element;
  free(last);
  return s;
}
/*
  sets val at index in list
*/
bool list_Function_setat(list_Function_t *li, Function val, size_t index) {
  if (index >= li->size) {
    errno = EINVAL;
    return false;
  }

  node_list_Function *node = li->first;

  for (size_t i = 0; i < index; i++) {
    node = node->next;
  }

  Symbols_clearFunction(&(node->element));
  node->element = val;
  return true;
}

/*
  returns the value at index in list
*/
Function list_Function_getat(list_Function_t *li, size_t index) {
  if (index >= li->size) {
    errno = EINVAL;
    return (Function){0};
  }

  node_list_Function *node = li->first;

  for (size_t i = 0; i < index; i++) {
    node = node->next;
  }

  return node->element;
}

/*
  returns the pointer to value at index in list
*/
Function *list_Function_get_ptr_at(list_Function_t *li, size_t index) {
  if (index >= li->size) {
    errno = EINVAL;
    return NULL;
  }

  node_list_Function *node = li->first;

  for (size_t i = 0; i < index; i++) {
    node = node->next;
  }

  return &node->element;
}

/*
  clear the list and frees all allocated memory
*/
bool list_Function_clear(list_Function_t *li) {
  node_list_Function *node = li->first;

  for (size_t i = 0; i < li->size; i++) {
    node_list_Function *next = node->next;
    Symbols_clearFunction(&(node->element));
    free(node);
    node = next;
  }
  free(li);
  return true;
}

/* array_Class_t functions & methods */

/*
  creates and returns a new array_Class_t pointer
*/
array_Class_t *array_Class_new() {
  array_Class_t *arr = malloc(sizeof(array_Class_t));
  if (!arr) {
    errno = ENOMEM;
    return NULL;
  }

  arr->array = calloc(4, sizeof(Class));
  if (!arr->array) {
    free(arr);
    errno = ENOMEM;
    return NULL;
  }

  arr->capacity = 4;
  arr->size = 0;

  return arr;
}

/*
  returns the size of the array
*/
size_t array_Class_size(array_Class_t *arr) { return arr->size; }

/*
  appends val to the end of the array arr
*/
bool array_Class_append(array_Class_t *arr, Class val) {
  if (arr->size < arr->capacity) {
    arr->array[arr->size] = val;
    arr->size++;
    return true;
  } else {
    arr->array = realloc(arr->array, ((arr->capacity + 4) * sizeof(Class)));

    if (arr->array) {
      arr->capacity += 4;
      arr->array[arr->size] = val;
      arr->size++;
      return true;
    }
    errno = ENOMEM;
    return false;
  }
}

/*
  removes and returns the last value in the array arr
*/
Class array_Class_pop(array_Class_t *arr) {
  if (arr->size == 0) {
    errno = EINVAL;
    return (Class){0};
  }

  Class last_value = arr->array[--(arr->size)];

  if (arr->size + 4 < arr->capacity) {
    arr->array = realloc(arr->array, ((arr->capacity - 4) * sizeof(Class)));
    arr->capacity -= 4;
  }

  return last_value;
}

/*
  sets val at index in array arr
*/
bool array_Class_setat(array_Class_t *arr, Class val, size_t index) {
  // TODO: clean allocated memory pointed by each Class in array
  if (index < arr->size) {
    arr->array[index] = val;
    return true;
  }
  errno = ENOMEM;
  return false;
}

/*
  returns the value at index in array arr
*/
Class array_Class_getat(array_Class_t *arr, size_t index) {
  if (index < arr->size) {
    return arr->array[index];
  }
  errno = EINVAL;
  return (Class){0};
}

/*
  returns the pointer to value at index in array arr
*/
Class *array_Class_get_ptr_at(array_Class_t *arr, size_t index) {
  if (index < arr->size) {
    return &arr->array[index];
  }
  errno = EINVAL;
  return NULL;
}

/*
  clear the arr and frees all allocated memory
*/
bool array_Class_clear(array_Class_t *arr) {
  // TODO: clean allocated memory pointed by each Class in array
  free(arr->array);
  free(arr);
  return true;
}

/* list_Class_t Classs & methods */

/*
  creates and returns a new list_Class_t pointer
*/
list_Class_t *list_Class_new() {
  list_Class_t *li = malloc(sizeof(list_Class_t));
  if (!li) {
    errno = ENOMEM;
    return false;
  }

  li->first = NULL;
  li->last = NULL;
  li->size = 0;
  return li;
}

/*
  returns the size of the list
*/
size_t list_Class_size(list_Class_t *li) { return li->size; }

/*
  appends val to the end of the list
*/
bool list_Class_append(list_Class_t *li, Class val) {
  node_list_Class *new = malloc(sizeof(node_list_Class));
  if (!new) {
    errno = ENOMEM;
    return false;
  }

  new->element = val;
  new->next = NULL;

  if (li->size > 0) {
    node_list_Class *last = li->last;
    assert(last->next == NULL);
    last->next = new;

    new->previous = last;

  } else {
    li->first = new;
    new->previous = NULL;
  }

  li->last = new;
  li->size++;

  return true;
}

/*
  removes and returns the last value in the list
*/
Class list_Class_pop(list_Class_t *li) {
  node_list_Class *last = li->last;
  last->previous->next = NULL;
  li->last = last->previous;
  li->size--;

  Class s = last->element;
  free(last);
  return s;
}
/*
  sets val at index in list
*/
bool list_Class_setat(list_Class_t *li, Class val, size_t index) {
  if (index >= li->size) {
    errno = EINVAL;
    return false;
  }

  node_list_Class *node = li->first;

  for (size_t i = 0; i < index; i++) {
    node = node->next;
  }

  // Symbols_clearClass(&(node->element));
  node->element = val;
  return true;
}

/*
  returns the value at index in list
*/
Class list_Class_getat(list_Class_t *li, size_t index) {
  if (index >= li->size) {
    errno = EINVAL;
    return (Class){0};
  }

  node_list_Class *node = li->first;

  for (size_t i = 0; i < index; i++) {
    node = node->next;
  }

  return node->element;
}

/*
  returns the pointer to value at index in list
*/
Class *list_Class_get_ptr_at(list_Class_t *li, size_t index) {
  if (index >= li->size) {
    errno = EINVAL;
    return NULL;
  }

  node_list_Class *node = li->first;

  for (size_t i = 0; i < index; i++) {
    node = node->next;
  }

  return &node->element;
}

/*
  clear the list and frees all allocated memory
*/
bool list_Class_clear(list_Class_t *li) {
  node_list_Class *node = li->first;

  for (size_t i = 0; i < li->size; i++) {
    node_list_Class *next = node->next;
    free(node);
    node = next;
  }

  free(li);
  return true;
}
