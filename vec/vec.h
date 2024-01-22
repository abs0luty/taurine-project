#ifndef _VEC_H_
#define _VEC_H_

#include <stddef.h>

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
#define vec_len(vec) (vec ? vec_metadata(vec)->len : 0)

/**
 * @returns the capacity of the vector.
 * @version 0.1.0
 */
#define vec_capacity(vec) (vec ? vec_metadata(vec)->capacity : 0)

/**
 * @returns `true` if the vector is empty, `false` otherwise.
 * @version 0.1.0
 */
#define vec_is_empty(vec) (vec_len(vec) == 0)

/**
 * @returns the destructor of each element in the vector.
 * @version 0.1.0
 */
#define vec_elem_destructor(vec) (vec_metadata(vec)->destructor)

/**
 * @brief   Erases all elements in the vector.
 * @version 0.1.0
 */
#define vec_clear(src)                                                         \
  do {                                                                         \
    if (src) {                                                                 \
      vec_elem_destructor_t destructor = vec_elem_destructor();                \
      if (destructor) {                                                        \
        for (register size_t i = 0; i < vec_len(src); i++) {                   \
          destructor(src[i]);                                                  \
        }                                                                      \
        vec_metadata(src)->len = 0;                                            \
      }                                                                        \
    }                                                                          \
  } while (0)

#endif /* _VEC_H_ */
