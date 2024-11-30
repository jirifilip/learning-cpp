#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <lib.h>

#include <memory>

#include <string>
#include <iostream>
#include <sstream>
#include <utility>
#include <iterator>

#include "basic_memory.cpp"


std::vector<int> readNumbers(std::istream& input) {
  std::string line;

  std::getline(input, line);
  std::istringstream line_stream(line);
  std::istream_iterator<int> begin(line_stream), end;
  
  auto vector = std::vector<int>(begin, end); 
  std::cout << "Address: " << &vector << std::endl;
  return vector;
}


TEST(LibTest, Plus) {
  ASSERT_EQ(plus(1, 2), 3);
  ASSERT_EQ(plus(20, 1), 21);
}


// https://www.hackerrank.com/challenges/vector-sort/problem
TEST(Exercise1, ReadNumbersTest) {
  std::stringstream stream;
  stream << "2 1 5 3 4";

  auto vector = readNumbers(stream);
  std::cout << "Address: " << &vector << std::endl;

  auto vector2 = std::vector<int>(vector.begin(), vector.end());
  std::cout << "Address: " << &vector2 << std::endl;


  ASSERT_THAT(vector, ::testing::ElementsAre(2, 1, 5, 3, 4));
}


TEST(MessingAround, TestingHowStreamsWork) {
  std::stringstream stream;

  stream << "2 1.0 ahoj";

  int numberFromStream;
  float floatFromStream;
  std::string stringFromStream;

  stream >> numberFromStream;
  stream >> floatFromStream;
  stream >> stringFromStream;

  ASSERT_EQ(numberFromStream, 2);
  ASSERT_EQ(floatFromStream, 1.0);
  ASSERT_EQ(stringFromStream, "ahoj");
}


TEST(MessingAround, TestingHowIteratorsWork) {
  std::vector<int> vec{1, 2, 3, 4};

  for (auto number: vec) {
    std::cout << number << " "; 
  }

  std::vector<int>::iterator iter = vec.begin();
  
  int i = 1;
  while (iter != vec.end()) {
    ASSERT_EQ(i, *iter);
    i++;
    iter++;
  }
}


class CustomDereference {
  public:
  std::string operator *() {
    return "This is my custom dereference operator";
  } 
};

std::ostream& operator <<(std::ostream &os, const CustomDereference &instance) {
  return os << "CustomDereference()" << &instance;
}


TEST(MessingAround, CustomDereference) {
  CustomDereference custom{};

  std::stringstream stream;

  stream << custom;

  ASSERT_EQ(*custom, "This is my custom dereference operator");
  ASSERT_THAT(stream.str(), ::testing::StartsWith("CustomDereference()"));
}


