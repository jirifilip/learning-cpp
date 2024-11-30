#include <gtest/gtest.h>
#include <gmock/gmock.h>


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