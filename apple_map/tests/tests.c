#include "map.h"
#include "tau.h"

TEST(apple_map_tests, apple_map_insert_and_get) {
  apple_map_t *map = apple_map_new();
  apple_map_insert(map, "foo", 3, 1);

  uintptr_t value;
  CHECK(apple_map_get(map, "foo", 3, &value));
  CHECK_EQ(value, 1);

  CHECK(!apple_map_get(map, "bar", 3, &value));

  apple_map_free(map);
}

TAU_MAIN()
