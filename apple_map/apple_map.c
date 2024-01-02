#include "apple_map.h"

#include <stdlib.h>
#include <string.h>

typedef struct bucket_t bucket;

struct apple_map
{
  bucket_t *buckets;

  bucket_t *first, *last;

  size_t capacity;
  size_t len;
  size_t tombstone_len;
};

typedef struct bucket
{
  bucket_t *next;

  const void *key;
  size_t key_size;
  uint32_t hash;
  uintptr_t value;
} bucket_t;

static bucket_t *resolve(apple_map_t *map, const void *key, size_t key_size, uint32_t hash);

static inline uint32_t fnv_1a_hash(const unsigned char *data, size_t size);

static bucket_t *resize_entry(apple_map_t *map, bucket_t *entry);

const size_t DEFAULT_CAPACITY = 30;

apple_map_t *apple_map_new(void)
{
  apple_map_t *map = malloc(sizeof(apple_map_t));

  if (map == NULL)
  {
    return NULL;
  }

  map->buckets = calloc(DEFAULT_CAPACITY, sizeof(bucket_t));
  map->first = NULL;
  map->last = (bucket_t *)&map->first;

  map->capacity = DEFAULT_CAPACITY;
  map->len = 0;
  map->tombstone_len = 0;

  return map;
}

inline void apple_map_free(apple_map_t *map)
{
  free(map->buckets);
  free(map);
}

bool apple_map_get(apple_map_t *map, const void *key, size_t key_size, uintptr_t *out_value)
{
  uint32_t hash = fnv_1a_hash(key, key_size);
  bucket_t *entry = resolve(map, key, key_size, hash);

  *out_value = entry->value;

  return entry->key != NULL;
}

static bucket_t *resolve(apple_map_t *map, const void *key, size_t key_size, uint32_t hash)
{
  uint32_t index = hash % map->capacity;

  while (true)
  {
    bucket_t *entry = &map->buckets[index];

    bool null_key = entry->key == NULL;
    bool null_value = entry->value == 0;

    if ((null_key && null_value) ||
        (!null_key &&
         entry->key_size == key_size &&
         entry->hash == hash &&
         memcmp(entry->key, key, key_size) == 0))
    {
      return entry;
    }

    index = (index + 1) % map->capacity;
  }
}

static inline uint32_t fnv_1a_hash(const unsigned char *data, size_t size)
{
  size_t blocks_count = size / 8;
  uint64_t hash = 2166136261u;

  for (size_t i = 0; i < blocks_count; i++)
  {
    hash ^= (uint64_t)data[0] << 0 | (uint64_t)data[1] << 8 |
            (uint64_t)data[2] << 16 | (uint64_t)data[3] << 24 |
            (uint64_t)data[4] << 32 | (uint64_t)data[5] << 40 |
            (uint64_t)data[6] << 48 | (uint64_t)data[7] << 56;
    hash *= 0xbf58476d1ce4e5b9;
    data += 8;
  }

  uint64_t last = size & 0xff;
  switch (size % 8)
  {
  case 7:
    last |= (uint64_t)data[6] << 56;
  case 6:
    last |= (uint64_t)data[5] << 48;
  case 5:
    last |= (uint64_t)data[4] << 40;
  case 4:
    last |= (uint64_t)data[3] << 32;
  case 3:
    last |= (uint64_t)data[2] << 24;
  case 2:
    last |= (uint64_t)data[1] << 16;
  case 1:
    last |= (uint64_t)data[0] << 8;
    hash ^= last;
    hash *= 0xd6e8feb86659fd93;
  }

  return (uint32_t)(hash ^ hash >> 32);
}

const float MAX_CAPACITY_PERCENTAGE = 0.75;

void apple_map_insert(apple_map_t *map, const void *key, size_t key_size, uintptr_t value)
{
  if (map->len + 1 > MAX_CAPACITY_PERCENTAGE * map->capacity)
  {
    apple_map_resize(map);
  }

  uint32_t hash = fnv_1a_hash(key, key_size);
  bucket_t *entry = resolve(map, key, key_size, hash);

  if (entry->key == NULL)
  {
    map->last->next = entry;
    map->last = entry;

    entry->next = NULL;

    map->len++;

    entry->key = key;
    entry->key_size = key_size;

    entry->hash = hash;
  }

  entry->value = value;
}

bool apple_map_get_or_insert(apple_map_t *map, const void *key, size_t key_size, uintptr_t *out_in)
{
  if (map->len + 1 > MAX_CAPACITY_PERCENTAGE * map->capacity)
  {
    apple_map_resize(map);
  }

  uint32_t hash = fnv_1a_hash(key, key_size);
  bucket_t *entry = resolve(map, key, key_size, hash);

  if (entry->key == NULL)
  {
    map->last->next = entry;
    map->last = entry;
    entry->next = NULL;

    map->len++;

    entry->value = *out_in;
    entry->key = key;
    entry->key_size = key_size;
    entry->hash = hash;

    return false;
  }

  *out_in = entry->value;

  return true;
}

void apple_map_remove(apple_map_t *map, const void *key, size_t key_size)
{
  uint32_t hash = fnv_1a_hash(key, key_size);
  bucket_t *entry = resolve(map, key, key_size, hash);

  if (entry->key != NULL)
  {
    entry->key = NULL;
    entry->value = 0xDEAD;

    map->tombstone_len++;
  }
}

void apple_map_remove_free(apple_map_t *map, const void *key, size_t key_size,
                           apple_map_callback callback, void *user)
{
  uint32_t hash = fnv_1a_hash(key, key_size);
  bucket_t *entry = resolve(map, key, key_size, hash);

  if (entry->key != NULL)
  {
    callback((void *)entry->key, entry->key_size, entry->value, user);

    entry->key = NULL;
    entry->value = 0xDEAD;

    map->tombstone_len++;
  }
}

void apple_map_soft_insert(apple_map_t *map, const void *key, size_t key_size,
                           uintptr_t value, apple_map_callback callback, void *user)
{
  if (map->len + 1 > MAX_CAPACITY_PERCENTAGE * map->capacity)
  {
    apple_map_resize(map);
  }

  uint32_t hash = fnv_1a_hash(key, key_size);
  bucket_t *entry = resolve(map, key, key_size, hash);

  if (entry->key == NULL)
  {
    map->last->next = entry;
    map->last = entry;
    entry->next = NULL;

    map->len++;

    entry->key = key;
    entry->key_size = key_size;
    entry->value = value;
    entry->hash = hash;

    return;
  }

  callback((void *)entry->key, key_size, entry->value, user);

  entry->key = key;
  entry->value = value;
}

inline size_t apple_map_len(apple_map_t *map)
{
  return map->len - map->tombstone_len;
}

const float RESIZE_FACTOR_PERCENTAGE = 2;

void apple_map_resize(apple_map_t *map)
{
  bucket_t *old_buckets = map->buckets;

  map->capacity *= RESIZE_FACTOR_PERCENTAGE;
  map->buckets = calloc(map->capacity, sizeof(bucket_t));

  map->last = (bucket_t *)&map->first;

  map->len -= map->tombstone_len;
  map->tombstone_len = 0;

  do
  {
    bucket_t *current = map->last->next;

    if (current->key == NULL)
    {
      map->last->next = current->next;
      continue;
    }

    map->last->next = resize_entry(map, map->last->next);
    map->last = map->last->next;
  } while (map->last->next != NULL);

  free(old_buckets);
}

static bucket_t *resize_entry(apple_map_t *map, bucket_t *entry)
{
  uint32_t idx = entry->hash % map->capacity;

  while (true)
  {
    bucket_t *new_entry = &map->buckets[idx];

    if (entry->key == NULL)
    {
      *new_entry = *entry;
      return new_entry;
    }

    idx = (idx + 1) % map->capacity;
  }
}

void apple_map_iter(apple_map_t *map, apple_map_callback callback, void *user)
{
  bucket_t *current = map->first;

  while (current != NULL)
  {
    if (current->key != NULL)
      callback((void *)current->key, current->key_size, current->value, user);

    current = current->next;
  }
}
