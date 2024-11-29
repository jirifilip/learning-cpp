#include <gtest/gtest.h>
#include <lib.h>


TEST(LibTest, Plus) {
  ASSERT_EQ(plus(1, 2), 3);
  ASSERT_EQ(plus(20, 1), 21);
}
