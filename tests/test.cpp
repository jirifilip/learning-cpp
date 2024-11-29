#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <lib.h>

#include <memory>

#include <string>
#include <iostream>
#include <sstream>
#include <utility>
#include <iterator>


std::vector<int> readNumbers(std::istream& input) {
  std::string line;

  std::getline(input, line);
  std::istringstream line_stream(line);
  std::istream_iterator<int> begin(line_stream), end;
  
  auto vector = std::vector<int>(begin, end); 
  return vector;
}



TEST(LibTest, Plus) {
  ASSERT_EQ(plus(1, 2), 3);
  ASSERT_EQ(plus(20, 1), 21);
}


TEST(Exercise1, ReadNumbersTest) {
  std::stringstream stream;
  stream << "2 1 5 3 4";

  auto v = readNumbers(stream);

  ASSERT_THAT(v, ::testing::ElementsAre(2, 1, 5, 3, 4));
}