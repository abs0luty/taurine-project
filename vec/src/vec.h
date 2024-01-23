#ifndef _VEC_H_
#define _VEC_H_

#include <assert.h>
#include <stddef.h>

/**
 * @returns An empty vector.
 * @version 0.1.0
 */
#define new_empty_vec() ((void *)0)

#define init_vec(vec, capacity, elem_destructor_fn)                            \
  do {                                                                         \
    if (!(vec)) {                                                              \
      vec_reserve((vec), capacity);                                            \
      vec_set_elem_destructor((vec), elem_destructor_fn);                      \
    }                                                                          \
  } while (0)

/**
 * @brief   Sets the capacity of the vector.
 * @param   vec       The vector.
 * @param   capacity_ The new capacity of the vector.
 * @version 0.1.0
 */
#define vec_set_capacity(vec, capacity_)                                       \
  do {                                                                         \
    if ((vec)) {                                                               \
      vec_metadata((vec))->capacity = (capacity_);                             \
    }                                                                          \
  } while (0)

/**
 * @brief   Sets the length of the vector.
 * @param   vec  The vector.
 * @param   len_ The new length of the vector.
 * @version 0.1.0
 */
#define vec_set_len(vec, len_)                                                 \
  do {                                                                         \
    if ((vec)) {                                                               \
      vec_metadata((vec))->len = (len_);                                       \
    }                                                                          \
  } while (0)

/**
 * @brief      Sets the destructor of the elements in the vector.
 * @param      vec                   The vector.
 * @param      elem_destructor_fn    The new destructor function.
 * @version    0.1.0
 * @deprecated Use `vec_set_elem_destructor` instead.
 */
#define vec_set_destructor(vec, destructor_fn)                                 \
  vec_set_elem_destructor(vec, destructor_fn)

/**
 * @brief   Sets the destructor of the elements in the vector.
 * @param   vec                   The vector.
 * @param   elem_destructor_fn    The new destructor function.
 * @version 0.1.0
 */
#define vec_set_elem_destructor(vec, elem_destructor_fn)                       \
  do {                                                                         \
    if ((vec)) {                                                               \
      vec_metadata((vec))->destructor = elem_destructor_fn;                    \
    }                                                                          \
  } while (0)

/**
 * @brief   The vector type.
 * @param   type The type of elements in the vector.
 * @version 0.1.0
 */
#define vec(type) type *

/**
 * @brief      The vector type.
 * @param      type The type of elements in the vector.
 * @version    0.1.0
 * @deprecated Use `vec` instead.
 */
#define vec_type(type) type *

/**
 * @brief   The type of the destructor of each element
 *          in the vector.
 * @param   elem Pointer to the element in the vector.
 * @version 0.1.0
 */
typedef void (*vec_elem_destructor_t)(void *elem);

/**
 * @brief   For internal use, the metadata of the vector.
 * @version 0.1.0
 */
typedef struct vec_metadata {
  // The number of elements in the vector.
  size_t len;
  // The capacity of the vector.
  size_t capacity;
  // The function used to free the elements of the vector.
  vec_elem_destructor_t destructor;
} vec_metadata_t;

/**
 * @brief   For internal use, converts a vector to its metadata pointer.
 * @param   vec The vector.
 * @returns The vector's metadata pointer.
 * @version 0.1.0
 */
#define vec_metadata(vec) (&((vec_metadata_t *)(vec))[-1])

/**
 * @brief   For internal use, converts a metadata pointer to its vector
 *          type.
 * @param   ptr The vector's metadata pointer.
 * @returns The vector pointer.
 * @version 0.1.0
 */
#define vec_from_metadata_ptr(ptr) ((void *)&((vec_metadata_t *)(ptr))[1])

/**
 * @returns a pointer to the first element of the vector.
 * @version 0.1.0
 */
#define vec_begin(vec) (vec)

/**
 * @returns a pointer to the last element of the vector.
 * @version 0.1.0
 */
#define vec_end(vec) ((vec) ? &((vec)[vec_len(vec)]) : NULL)

/**
 * @returns the number of elements in the vector.
 * @version 0.1.0
 */
#define vec_len(vec) ((vec) ? vec_metadata(vec)->len : 0)

/**
 * @returns the capacity of the vector.
 * @version 0.1.0
 */
#define vec_capacity(vec) ((vec) ? vec_metadata(vec)->capacity : 0)

/**
 * @returns `true` if the vector is empty, `false` otherwise.
 * @version 0.1.0
 */
#define vec_is_empty(vec) (vec_len(vec) == 0)

/**
 * @returns the destructor of each element in the vector.
 * @version 0.1.0
 */
#define vec_elem_destructor(vec)                                               \
  ((vec) ? vec_metadata(vec)->destructor : (void *)0)

/**
 * @brief   Erases all elements in the vector.
 * @version 0.1.0
 */
#define vec_clear(src)                                                         \
  do {                                                                         \
    if (src) {                                                                 \
      void *metadata_ptr = vec_metadata(src);                                  \
      vec_elem_destructor_t destructor = vec_elem_destructor(src);             \
      if (destructor) {                                                        \
        for (register size_t i = 0; i < vec_len(src); i++) {                   \
          destructor(&(src)[i]);                                               \
        }                                                                      \
      }                                                                        \
      free(metadata_ptr);                                                      \
    }                                                                          \
  } while (0)

/**
 * @brief   For internal use, requests that the vector capacity be at least
 *          enough to contain `minimum_capacity` elements.
 * @param   vec      The vector.
 * @param   capacity The new capacity of the vector.
 * @version 0.1.0
 */
#define vec_reserve(vec, minimum_capacity)                                     \
  do {                                                                         \
    size_t capacity = vec_capacity(vec);                                       \
    if (capacity < minimum_capacity) {                                         \
      vec_grow(vec, minimum_capacity);                                         \
    }                                                                          \
  } while (0)

/**
 * @brief   For internal use, ensures that the vector is at least `capacity`
 * elements long.
 * @param   vec      The vector.
 * @param   capacity The new capacity of the vector.
 * @version 0.1.0
 */
#define vec_grow(vec, capacity)                                                \
  do {                                                                         \
    const size_t new_size =                                                    \
        (capacity) * sizeof(*(vec)) + sizeof(vec_metadata_t);                  \
    if ((vec)) {                                                               \
      void *metadata_ptr = vec_metadata(vec);                                  \
      void *new_vec_ptr = realloc(metadata_ptr, new_size);                     \
      assert(new_vec_ptr);                                                     \
      (vec) = vec_from_metadata_ptr(new_vec_ptr);                              \
    } else {                                                                   \
      void *new_vec_ptr = malloc(new_size);                                    \
      assert(new_vec_ptr);                                                     \
      (vec) = vec_from_metadata_ptr(new_vec_ptr);                              \
      vec_set_len((vec), 0);                                                   \
      vec_set_elem_destructor((vec), (void *)0);                               \
    }                                                                          \
    vec_set_capacity((vec), capacity);                                         \
  } while (0)

#ifdef VEC_LOGARITHMIC_GROWTH
#define vec_compute_next_grow(capacity) ((capacity) ? ((capacity) << 1) : 1)
#else
#ifdef VEC_LINEAR_GROWTH
#define vec_compute_next_grow(capacity) ((capacity) + 1)
#else
#error "`vec` library requires you to specify vector growth type.\
You must define either `VEC_LINEAR_GROWTH` or `VEC_LOGARITHMIC_GROWTH` macro."
#endif
#endif

#define vec_push_back(vec, value)                                              \
  do {                                                                         \
    size_t capacity = vec_capacity(vec);                                       \
    if (capacity <= vec_len(vec)) {                                            \
      vec_grow((vec), vec_compute_next_grow(capacity));                        \
    }                                                                          \
    (vec)[(vec_len(vec))] = value;                                             \
    vec_set_len((vec), vec_len(vec) + 1);                                      \
  } while (0)

#endif /* _VEC_H_ */
