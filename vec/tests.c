#include "vec/vec.h"
#include "tau/tau.h"

TEST(vec_tests, empty_vec) {
  vec(int) test = (void*) 0;

  CHECK(vec_is_empty(test));
  CHECK_EQ(vec_len(test), 0);
  CHECK_EQ(vec_capacity(test), 0);
}

TAU_MAIN()
