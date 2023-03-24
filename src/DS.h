#pragma once

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

#define DS_ARRAY_DEC(name, TYPE)                                                                                       \
    typedef struct _##name##_array_t name##_array_t;                                                                   \
    name##_array_t *name##_array_new();                                                                                \
    size_t name##_array_size(const name##_array_t *arr);                                                               \
    bool name##_array_append(name##_array_t *arr, TYPE val);                                                           \
    TYPE name##_array_pop(name##_array_t *arr);                                                                        \
    bool name##_array_setat(name##_array_t *arr, TYPE val, const size_t index);                                        \
    TYPE name##_array_getat(const name##_array_t *arr, const size_t index);                                            \
    TYPE *name##_array_get_ptr_at(const name##_array_t *arr, const size_t index);                                      \
    bool name##_array_clear(name##_array_t *arr);

#define DS_LIST_DEC(name, TYPE)                                                                                        \
    typedef struct _##name##_list_t name##_list_t;                                                                     \
    name##_list_t *name##_list_new();                                                                                  \
    size_t name##_list_size(const name##_list_t *li);                                                                  \
    bool name##_list_append(name##_list_t *li, TYPE val);                                                              \
    TYPE name##_list_pop(name##_list_t *li);                                                                           \
    bool name##_list_setat(name##_list_t *li, TYPE val, const size_t index);                                           \
    TYPE name##_list_getat(const name##_list_t *li, const size_t index);                                               \
    TYPE *name##_list_get_ptr_at(const name##_list_t *li, const size_t index);                                         \
    bool name##_list_clear(name##_list_t *li);

#define DS_ARRAY_DEF(name, TYPE, delFunc)                                                                              \
    struct _##name##_array_t {                                                                                         \
        TYPE *array;                                                                                                   \
        size_t size;                                                                                                   \
        size_t capacity;                                                                                               \
    };                                                                                                                 \
    typedef struct _##name##_array_t name##_array_t;                                                                   \
                                                                                                                       \
    name##_array_t *name##_array_new() {                                                                               \
        name##_array_t *arr = malloc(sizeof(name##_array_t));                                                          \
        if (!arr) {                                                                                                    \
            errno = ENOMEM;                                                                                            \
            return NULL;                                                                                               \
        }                                                                                                              \
                                                                                                                       \
        arr->array = calloc(4, sizeof(TYPE));                                                                          \
        if (!arr->array) {                                                                                             \
            free(arr);                                                                                                 \
            errno = ENOMEM;                                                                                            \
            return NULL;                                                                                               \
        }                                                                                                              \
                                                                                                                       \
        arr->capacity = 4;                                                                                             \
        arr->size     = 0;                                                                                             \
        return arr;                                                                                                    \
    }                                                                                                                  \
                                                                                                                       \
    size_t name##_array_size(const name##_array_t *arr) {                                                              \
        return arr->size;                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    bool name##_array_append(name##_array_t *arr, TYPE val) {                                                          \
        if (arr->size < arr->capacity) {                                                                               \
            arr->array[arr->size] = val;                                                                               \
            arr->size++;                                                                                               \
            return true;                                                                                               \
        } else {                                                                                                       \
            arr->array = realloc(arr->array, (arr->capacity * 2 * sizeof(TYPE)));                                      \
            if (arr->array) {                                                                                          \
                arr->capacity *= 2;                                                                                    \
                arr->array[arr->size] = val;                                                                           \
                arr->size++;                                                                                           \
                return true;                                                                                           \
            }                                                                                                          \
            errno = ENOMEM;                                                                                            \
            return false;                                                                                              \
        }                                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    TYPE name##_array_pop(name##_array_t *arr) {                                                                       \
        if (arr->size == 0) {                                                                                          \
            errno = EINVAL;                                                                                            \
            return (TYPE){0};                                                                                          \
        }                                                                                                              \
                                                                                                                       \
        TYPE last_value = arr->array[--(arr->size)];                                                                   \
                                                                                                                       \
        if (arr->size * 2 < arr->capacity) {                                                                           \
            arr->array = realloc(arr->array, (arr->capacity / 2) * sizeof(TYPE));                                      \
            arr->capacity /= 2;                                                                                        \
        }                                                                                                              \
        return last_value;                                                                                             \
    }                                                                                                                  \
                                                                                                                       \
    bool name##_array_setat(name##_array_t *arr, TYPE val, const size_t index) {                                       \
        if (index < arr->size) {                                                                                       \
            if (delFunc) {                                                                                             \
                delFunc(arr->array + index);                                                                           \
            }                                                                                                          \
            arr->array[index] = val;                                                                                   \
            return true;                                                                                               \
        }                                                                                                              \
        errno = EINVAL;                                                                                                \
        return false;                                                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    TYPE name##_array_getat(const name##_array_t *arr, const size_t index) {                                           \
        if (index < arr->size) {                                                                                       \
            return arr->array[index];                                                                                  \
        }                                                                                                              \
        errno = EINVAL;                                                                                                \
        return (TYPE){0};                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    TYPE *name##_array_get_ptr_at(const name##_array_t *arr, const size_t index) {                                     \
        if (index < arr->size) {                                                                                       \
            return arr->array + index;                                                                                 \
        }                                                                                                              \
        errno = EINVAL;                                                                                                \
        return NULL;                                                                                                   \
    }                                                                                                                  \
                                                                                                                       \
    bool name##_array_clear(name##_array_t *arr) {                                                                     \
        if (delFunc) {                                                                                                 \
            for (size_t i = 0; i < arr->size; i++) {                                                                   \
                delFunc(arr->array + i);                                                                               \
            }                                                                                                          \
        }                                                                                                              \
        free(arr->array);                                                                                              \
        free(arr);                                                                                                     \
        return true;                                                                                                   \
    }

#define DS_ARRAY_FOREACH(arr, name)                                                                                    \
    typeof(*(arr->array)) name = arr->array[0];                                                                        \
    for (size_t _i = 0; _i++ < arr->size; name = arr->array[_i])

#define DS_ARRAY_PTR_FOREACH(arr, name)                                                                                \
    typeof(*(arr->array)) *name = arr->array + 0;                                                                      \
    for (size_t _i = 0; _i++ < arr->size; name = arr->array + _i)

#define DS_LIST_DEF(name, TYPE, delFunc)                                                                               \
    typedef struct _##name##_list_node name##_list_node;                                                               \
    struct _##name##_list_node {                                                                                       \
        TYPE element;                                                                                                  \
        name##_list_node *next;                                                                                        \
        name##_list_node *previous;                                                                                    \
    };                                                                                                                 \
                                                                                                                       \
    typedef struct _##name##_list_t name##_list_t;                                                                     \
    struct _##name##_list_t {                                                                                          \
        name##_list_node *first;                                                                                       \
        name##_list_node *last;                                                                                        \
        size_t size;                                                                                                   \
    };                                                                                                                 \
                                                                                                                       \
    name##_list_t *name##_list_new() {                                                                                 \
        name##_list_t *li = malloc(sizeof(name##_list_t));                                                             \
        if (!li) {                                                                                                     \
            errno = ENOMEM;                                                                                            \
            return NULL;                                                                                               \
        }                                                                                                              \
                                                                                                                       \
        li->first = NULL;                                                                                              \
        li->last  = NULL;                                                                                              \
        li->size  = 0;                                                                                                 \
        return li;                                                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    size_t name##_list_size(const name##_list_t *li) {                                                                 \
        return li->size;                                                                                               \
    }                                                                                                                  \
                                                                                                                       \
    bool name##_list_append(name##_list_t *li, TYPE val) {                                                             \
        name##_list_node *new = malloc(sizeof(name##_list_node));                                                      \
        if (!new) {                                                                                                    \
            errno = ENOMEM;                                                                                            \
            return false;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        new->element = val;                                                                                            \
        new->next    = NULL;                                                                                           \
                                                                                                                       \
        if (li->size > 0) {                                                                                            \
            name##_list_node *last = li->last;                                                                         \
            assert(last->next == NULL);                                                                                \
            last->next    = new;                                                                                       \
            new->previous = last;                                                                                      \
        } else {                                                                                                       \
            li->first     = new;                                                                                       \
            new->previous = NULL;                                                                                      \
        }                                                                                                              \
                                                                                                                       \
        li->last = new;                                                                                                \
        li->size++;                                                                                                    \
        return true;                                                                                                   \
    }                                                                                                                  \
                                                                                                                       \
    TYPE name##_list_pop(name##_list_t *li) {                                                                          \
        name##_list_node *last = li->last;                                                                             \
        last->previous->next   = NULL;                                                                                 \
        li->last               = last->previous;                                                                       \
        li->size--;                                                                                                    \
                                                                                                                       \
        TYPE element = last->element;                                                                                  \
        free(last);                                                                                                    \
        return element;                                                                                                \
    }                                                                                                                  \
                                                                                                                       \
    bool name##_list_setat(name##_list_t *li, TYPE val, const size_t index) {                                          \
        if (index >= li->size) {                                                                                       \
            errno = EINVAL;                                                                                            \
            return false;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        name##_list_node *node = li->first;                                                                            \
        for (size_t i = 0; i < index; i++) {                                                                           \
            node = node->next;                                                                                         \
        }                                                                                                              \
                                                                                                                       \
        if (delFunc) {                                                                                                 \
            delFunc(&(node->element));                                                                                 \
        }                                                                                                              \
                                                                                                                       \
        node->element = val;                                                                                           \
        return true;                                                                                                   \
    }                                                                                                                  \
                                                                                                                       \
    TYPE name##_list_getat(const name##_list_t *li, const size_t index) {                                              \
        if (index >= li->size) {                                                                                       \
            errno = EINVAL;                                                                                            \
            return (TYPE){0};                                                                                          \
        }                                                                                                              \
                                                                                                                       \
        name##_list_node *node = li->first;                                                                            \
        for (size_t i = 0; i < index; i++) {                                                                           \
            node = node->next;                                                                                         \
        }                                                                                                              \
        return node->element;                                                                                          \
    }                                                                                                                  \
                                                                                                                       \
    TYPE *name##_list_get_ptr_at(const name##_list_t *li, const size_t index) {                                        \
        if (index >= li->size) {                                                                                       \
            errno = EINVAL;                                                                                            \
            return NULL;                                                                                               \
        }                                                                                                              \
                                                                                                                       \
        name##_list_node *node = li->first;                                                                            \
        for (size_t i = 0; i < index; i++) {                                                                           \
            node = node->next;                                                                                         \
        }                                                                                                              \
        return &(node->element);                                                                                       \
    }                                                                                                                  \
                                                                                                                       \
    bool name##_list_clear(name##_list_t *li) {                                                                        \
        name##_list_node *node = li->first;                                                                            \
        for (size_t i = 0; i < li->size; i++) {                                                                        \
            name##_list_node *next = node->next;                                                                       \
            if (delFunc) {                                                                                             \
                delFunc(&(node->element));                                                                             \
            }                                                                                                          \
            free(node);                                                                                                \
            node = next;                                                                                               \
        }                                                                                                              \
        free(li);                                                                                                      \
        return true;                                                                                                   \
    }

#define DS_LIST_FOREACH(li, name)                                                                                      \
    typeof(li->first) name##_node   = li->first;                                                                       \
    typeof(li->first->element) name = name##_node->element;                                                            \
    for (size_t(name##_i) = 0, _i = 0; ++(name##_i) <= li->size; name##_node = name##_node->next,                      \
        name = name##_node ? name##_node->element : (typeof(li->first->element)){0}, _i++)

#define DS_LIST_PTR_FOREACH(li, name)                                                                                  \
    typeof(li->first) name##_node    = li->first;                                                                      \
    typeof(li->first->element) *name = &(name##_node->element);                                                        \
    for (size_t(name##_i) = 0, _i = 0; ++(name##_i) <= li->size;                                                       \
         name##_node = name##_node->next, name = name##_node ? &(name##_node->element) : NULL, _i++)
