#ifndef _VEC_H_
#define _VEC_H_

#define vec(type) type *

typedef void (*vec_elem_destructor_t) (void *elem);

typedef struct vec_metadata {
	size_t len;
	size_t capacity;
  vec_elem_destructor_t destructor;
} vec_metadata_t;

#define vec_metadata(vec) \
  (&((vec_metadata_t *)(vec))[-1])

/**
 * @return a pointer to the first element of the vector
 */
#define vec_begin(vec) (vec)

/**
 * @return a pointer to the last element of the vector
 */
#define vec_end(vec) ((vec) ? &((vec)[vec_len(vec)]) : NULL)

/**
 * @return the number of elements in the vector
 */
#define vec_len(vec) (vec ? vec_metadata(vec)->len : 0)

/**
 * @return the capacity of the vector
 */
#define vec_capacity(vec) (vec ? vec_metadata(vec)->capacity : 0) : 0)

/**
 * @return `true` if the vector is empty, `false` otherwise
 */
#define vec_is_empty(vec) (vec_len(vec) == 0)

/**
 * @return the destructor of each element in the vector
 */
#define vec_elem_destructor(vec) (vec_metadata(vec)->destructor)

/**
 * @brief Erase all elements in the vector
 */
#define vec_clear(src)                                           \
  do {                                                           \
    if (src) {                                                   \
      vec_elem_destructor_t destructor = vec_elem_destructor();  \
      if (destructor) {                                          \
        for (register size_t i = 0; i < vec_len(src); i++) {     \
          destructor(src[i]);                                    \
        }                                                        \
        vec_metadata(src)->len = 0;                              \
      }                                                          \
    }                                                            \
  } while (0)

#endif /* _VEC_H_ */
