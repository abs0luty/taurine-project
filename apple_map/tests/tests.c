#include "apple_map/apple_map.h"
#include "tau/tau.h"

TEST(apple_map_tests, apple_map_insert_and_get) {
  apple_map_t *map = apple_map_new();
  apple_map_insert(map, "foo", 3, 1);

  uintptr_t value;
  CHECK(apple_map_get(map, "foo", 3, &value));
  CHECK_EQ(value, 1);

  CHECK(!apple_map_get(map, "bar", 3, &value));

  apple_map_free(map);
}

TEST(apple_map_tests, apple_map_insert_and_remove) {
  apple_map_t *map = apple_map_new();
  apple_map_insert(map, "foo", 3, 1);

  uintptr_t value;
  CHECK(apple_map_get(map, "foo", 3, &value));
  CHECK_EQ(value, 1);

  apple_map_remove(map, "foo", 3);

  CHECK(!apple_map_get(map, "foo", 3, &value));

  apple_map_free(map);
}

void empty_callback(void *key, size_t key_size, uintptr_t value, void *user) {}

TEST(apple_map_tests, apple_map_insert_and_soft_insert) {
  apple_map_t *map = apple_map_new();
  apple_map_insert(map, "foo", 3, 1);

  uintptr_t value;
  CHECK(apple_map_get(map, "foo", 3, &value));
  CHECK_EQ(value, 1);

  apple_map_soft_insert(map, "foo", 3, 2, empty_callback, NULL);
  CHECK(apple_map_get(map, "foo", 3, &value));
  CHECK_EQ(value, 2);

  apple_map_free(map);
}

TEST(apple_map_tests, apple_map_len) {
  apple_map_t *map = apple_map_new();
  apple_map_insert(map, "foo", 3, 1);
  CHECK_EQ(apple_map_len(map), 1);

  apple_map_insert(map, "bar", 3, 2);
  CHECK_EQ(apple_map_len(map), 2);

  apple_map_free(map);
}

void free_callback(void *key, size_t key_size, uintptr_t value, void *user) {
  free((char *)value);
}

TEST(apple_map_tests, apple_map_free) {
  apple_map_t *map = apple_map_new();
  char *value = (char *)malloc(4);
  strcpy(value, "foo\0");
  apple_map_insert(map, "foo", 3, (uintptr_t)value);

  char *extracted_value;
  CHECK(apple_map_get(map, "foo", 3, (uintptr_t *)&extracted_value));
  CHECK_EQ(strcmp(extracted_value, "foo\0"), 0);

  apple_map_remove_free(map, "foo", 3, free_callback, NULL);

  CHECK(!apple_map_get(map, "foo", 3, (uintptr_t *)&extracted_value));

  apple_map_free(map);
}

TAU_MAIN()
