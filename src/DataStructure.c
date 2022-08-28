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
    if (arr->array[index]) {
      free(arr->array[index]);
    }

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
  // TODO: clean allocated memory pointed by the Structure
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
  // TODO: clean allocated memory pointed by the existing Structure
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
  // TODO: clean allocated memory pointed by each Structure in array
  free(arr->array);
  free(arr);
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
  // TODO: clean allocated memory pointed by the Union
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
  // TODO: clean allocated memory pointed by each Union in array
  free(arr->array);
  free(arr);
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
  // TODO: clean allocated memory pointed by the Global
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
  // TODO: clean allocated memory pointed by each Global in array
  free(arr->array);
  free(arr);
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
  // TODO: clean allocated memory pointed by the TypeDef
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
  // TODO: clean allocated memory pointed by each TypeDef in array
  free(arr->array);
  free(arr);
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
  // TODO: clean allocated memory pointed by the FunctionType
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
  // TODO: clean allocated memory pointed by each FunctionType in array
  free(arr->array);
  free(arr);
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
  // TODO: clean allocated memory pointed by the Function
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
  // TODO: clean allocated memory pointed by each Function in array
  free(arr->array);
  free(arr);
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
  // TODO: clean allocated memory pointed by the Class
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