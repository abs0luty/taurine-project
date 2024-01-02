#ifndef _APPLE_MAP_H_
#define _APPLE_MAP_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief      Hashmap is a data structure, that maps keys to values. Values in the
 *             apple map implementation are pointer values or integral types.
 *
 * @version    0.1.0
 */
typedef struct apple_map apple_map_t;

/**
 * @brief            Callback type for iterating through the hashmap entries.
 *
 * @param key        Current entry key.
 * @param key_size   Current entry key size.
 * @param value      Current entry value.
 * @param user       User pointer is a pointer that you can pass through `apple_map_iter`.
 *
 * @version          0.1.0
 */
typedef void (*apple_map_callback)(void *key, size_t key_size, uintptr_t value, void *user);

/**
 * @brief      Creates a new empty hashmap.
 * @returns    A newly allocated empty hashmap.
 *
 * @version    0.1.0
 */
apple_map_t *apple_map_new(void);

/**
 * @brief            Inserts a key-value pair into the hashmap.
 * @details          Function doesn't copy a key, so you should guarantee its lifetime.
 *
 * @param map        The hashmap, into which the key-value pair will be inserted.
 * @param key        The key, to insert into the hashmap.
 * @param key_size   The size of the key.
 * @param value      The value, to insert into the hashmap.
 *
 * @version          0.1.0
 */
void apple_map_insert(apple_map_t *map, const void *key, size_t key_size, uintptr_t value);

/**
 * @brief              Resolves a key-value pair from the hashmap.
 *
 * @param map          The hashmap, from which the key-value pair will be resolved.
 * @param key          The key to resolve.
 * @param key_size     The size of the key.
 * @param out_value    The reference to a value to store the resolved value.
 *
 * @returns            The resolved value.
 *
 * @version            0.1.0
 */
bool apple_map_get(apple_map_t *map, const void *key, size_t key_size, uintptr_t *out_value);

/**
 * @brief              Tries to resolve a key-value pair from the hashmap. If it fails, it adds the
 *                     entry into the hashmap, with the value read from `out_in`. If it doesn't, `out_in`
 *                     will be set to the value from the hashmap entry successfully resolved by key.
 *
 * @param map          The hashmap, from which the key-value pair will be resolved.
 * @param key          The key to resolve.
 * @param key_size     The size of the key.
 * @param out_in       The reference to a value to store it in a new hashmap entry or the
 *                     value that will be set to the value of successfully resolved entry.
 *
 * @returns            `true` if the key-value pair already exists.
 *                     `false` otherwise.
 *
 * @version            0.1.0
 */
bool apple_map_get_or_insert(apple_map_t *map, const void *key, size_t key_size, uintptr_t *out_in);

/**
 * @brief              Similiar to `apple_map_insert`, but when trying to overwrite a hashmap entry,
 *                     it will free the old entry's data via callback using `callback`.
 * @param map          The hashmap, into which the key-value pair will be soft-inserted.
 * @param key          The key, to insert into the hashmap.
 * @param key_size     The size of the key.
 * @param value        The value, to insert into the hashmap.
 * @param callback     The callback, that will be called when the key-value pair already exists.
 * @param user         User pointer is a pointer that you can use in the `callback`.
 *
 * @version            0.1.0
 */
void apple_map_soft_insert(apple_map_t *map, const void *key, size_t key_size,
                            uintptr_t value, apple_map_callback callback, void *user);

/**
 * @brief            Removes a key-value pair resolved by key from the hashmap.
 * @param map        The hashmap, from which the key-value pair will be removed.
 * @param  key       The key, via which entry is resolved.
 * @param  key_size  The size of the key.
 *
 * @version          0.1.0
 */
void apple_map_remove(apple_map_t *map, const void *key, size_t key_size);

/**
 * @brief              Removes a key-value pair resolved by key from the hashmap. But unlike
 *                     `apple_map_remove`, it will free the key-value pair's data via the `callback`.
 * @param map          The hashmap, from which the key-value pair will be removed.
 * @param  key         The key, via which entry is resolved.
 * @param  key_size    The size of the key.
 * @param  callback    The callback, that will be called to free the key-value pair.
 * @param  user        User pointer is a pointer that you can use in the `callback`.
 *
 * @version            0.1.0
 */
void apple_map_remove_free(apple_map_t *map, const void *key, size_t key_size,
                            apple_map_callback callback, void *user);

/**
 * @brief            Returns the number of entries in the hashmap.
 * @returns          The number of entries in the hashmap.
 *
 * @version          0.1.0
 */
size_t apple_map_len(apple_map_t *map);

/**
 * @brief              Resizes the hashmap to a new capacity when the hashmap is full.
 * @param map          The hashmap to iterate.
 *
 * @version            0.1.0
 */
void apple_map_resize(apple_map_t *map);

/**
 * @brief              Iterates through the hashmap, using the `callback`.
 * @param map          The hashmap to iterate.
 * @param callback     The callback, that will be called on each entry.
 * @param user         User pointer is a pointer that you can use in the `callback`.
 *
 * @version            0.2.0
 */
void apple_map_iter(apple_map_t *map, apple_map_callback callback, void *user);

/**
 * @brief              Frees the hashmap object and inner buckets.
 * @details            This function doesn't free the key-value pairs. You can achieve that by
 *                     using `apple_map_iter` with callback freeing the hashmap entries.
 * @param map          The hashmap object to free.
 *
 * @version            0.1.0
 */
void apple_map_free(apple_map_t *map);

#ifdef __cplusplus
}
#endif

#endif /* _APPLE_MAP_H_ */
