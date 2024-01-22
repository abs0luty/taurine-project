#include "tau/tau.h"
#include "vec/vec.h"

TEST(vec_tests, empty_vec) {
  vec(int) test = new_empty_vec();
  CHECK(vec_is_empty(test));
  CHECK_EQ(vec_len(test), 0);
  CHECK_EQ(vec_capacity(test), 0);
  CHECK_EQ((void *)vec_elem_destructor(test), (void *)0);

  vec_clear(test);
}

TEST(vec_tests, int_vec) {
  vec(int) test = new_empty_vec();
  vec_push_back(test, 5);

  CHECK(!vec_is_empty(test));
  CHECK_EQ(vec_len(test), 1);
  CHECK_EQ(vec_capacity(test), 1);
  CHECK_EQ(*vec_begin(test), 5);
  CHECK_EQ(*(vec_end(test) - 1), 5);

  vec_push_back(test, 6);
  vec_push_back(test, 4);

  CHECK(!vec_is_empty(test));
  CHECK_EQ(vec_len(test), 3);
  CHECK_EQ(vec_capacity(test), 3);
  CHECK_EQ(*vec_begin(test), 5);
  CHECK_EQ(*(vec_end(test) - 1), 4);
  CHECK_EQ(*(vec_end(test) - 2), 6);

  CHECK_EQ((void *)vec_elem_destructor(test), (void *)0);

  vec_clear(test);
}

TAU_MAIN()
