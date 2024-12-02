#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <optional>


TEST(MemoryTest, TestReferences) {
    const int origin{ 4 };
    const int& originReference{ origin };

    ASSERT_EQ(origin, originReference);
    ASSERT_EQ(&origin, &originReference);
}


TEST(MemoryTest, TestModifyingReference) {
    int origin{ 4 };
    int& originReference{ origin };

    ASSERT_EQ(originReference, 4);
    
    originReference = 5;
    ASSERT_EQ(origin, 5);
    ASSERT_EQ(origin, originReference);
}


int& getDanglingReference() {
    int origin = 5;
    int& reference = origin;
    return reference;
} 


TEST(MemoryTest, DanglingReference) {
    //int& reference = getDanglingReference();    

    // Sometimes works, sometimes not
    // ASSERT_TRUE(reference != 5);
}


TEST(MemoryTest, ConstReference) {
    int origin{ 42 };
    const int& reference{ origin };

    origin++;

    ASSERT_EQ(reference, 43);    
    ASSERT_EQ(origin, 43);
}


TEST(MemoryTest, ConstReferenceDifferentTypes) {
    short origin{ 42 };
    const int& reference{ origin };

    origin++;

    ASSERT_EQ(reference, 42);    
    ASSERT_EQ(origin, 43);
}


std::string& sampleFunction(std::string& someString) {
    return someString;
}

TEST(MemoryTest, PassingStringByReference) {
    std::string sampleString{ "Some text" };

    ASSERT_EQ(&sampleString, &sampleFunction(sampleString));
}


TEST(MemoryTest, StringComparison) {
    std::string text1{ "text" };
    std::string text2{ "text" };

    ASSERT_TRUE(text1 == text1);
    ASSERT_TRUE(text1 == text2);
}


std::string& alterString(std::string& text) {
    text = "hello";
    return text;
}


TEST(MemoryTest, PassByReferenceAndAlterValue) {
    std::string text = "goodbye";

    alterString(text);

    ASSERT_EQ(text, "hello");
}


const double& passByDifferentType(const double& x) {
    return x;
}


TEST(MemoryTest, PassByReferenceButDifferentType) {
    int x = 2;

    const double y = passByDifferentType(x);
    x++;
    
    ASSERT_EQ(x, 3);
    ASSERT_EQ(y, 2);
}


TEST(MemoryTest, StringView) {
    std::string_view x{ "Hello world" };
    std::string differentX{ "Hello world" };

    std::string_view y{ x.substr(0, 5) };
    
    ASSERT_EQ(y, "Hello");
    ASSERT_EQ(x.data(), y.data());
    ASSERT_NE(x.data(), differentX.data());
}


TEST(MemoryTest, InitVar) {
    char c1 {};
    int i1 {};

    ASSERT_EQ(c1, '\0');
    ASSERT_EQ(i1, 0);
}


TEST(MemoryTest, DereferencingArray) {
    int x {2};
    int array[] { 1, 2, 3, 4, 5};

    ASSERT_EQ(*(&x), 2);

    ASSERT_THAT(array, ::testing::ElementsAre(1, 2, 3, 4, 5));
    ASSERT_EQ(*array, 1);
    ASSERT_EQ(*(array + 4), 5);
}


void changingWithPointer(std::string* stringPointer) {
    *stringPointer = "Goodbye";    
}


TEST(MemoryTest, ChangingStringByPointer) {
    std::string testString = "Hello";

    changingWithPointer(&testString);
    
    ASSERT_EQ(testString, "Goodbye");
}


int testOptionalArguments(int x = 4) {
    return x;
}


TEST(MemoryTest, TestOptionalArguments) {
    ASSERT_EQ(testOptionalArguments(), 4);
    ASSERT_EQ(testOptionalArguments(5), 5);
}


std::string& returnByReference() {
    static std::string someString{ "John went to work" };

    return someString;
}


TEST(MemoryTest, ReturnByReference) {
    ASSERT_EQ(returnByReference(), "John went to work");
}


std::optional<int> divide(int x, int y) {
    if (y == 0) {
        return std::nullopt;
    }

    return x / y;
}


TEST(MemoryTest, TestOptionalType) {
    std::optional<int> result = divide(5, 0);

    ASSERT_EQ(result.value_or(0), 0);
}


class Employee {
public:
    Employee(std::string name) : name(name) {} 
    std::string name;
};


std::string getName(std::optional<std::reference_wrapper<Employee>> employee = std::nullopt) {
    if (employee.has_value()) {
        return employee.value().get().name;
    } 

    return "no-name";
}


TEST(MemoryTest, TestOptionalTypeWithReference) {
    Employee john{ "John" };

    ASSERT_EQ(john.name, "John");
    ASSERT_EQ(getName(john), "John");
    ASSERT_EQ(getName(), "no-name");
}

