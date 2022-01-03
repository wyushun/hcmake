#include "gtest/gtest.h"
#include "hello_cmake.hpp"

TEST(TestHelloCMake, just_a_test) {
  HelloCMake t;
  EXPECT_EQ(t(), std::string("hello cmake!"));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
