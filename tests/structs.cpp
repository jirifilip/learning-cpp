#include <gtest/gtest.h>
#include <gmock/gmock.h>


struct HelloWorld {
    int strength;
    int dexterity;
};


TEST(StructTests, SimpleStruct) {
    HelloWorld hw {18, 12};

    ASSERT_EQ(hw.dexterity, 12);
    ASSERT_EQ(hw.strength, 18);
}


std::string determineBuild(const HelloWorld& hw) {
    if (hw.strength > hw.dexterity) {
        return "Strength build!";
    }

    return "Dex build!";
}


TEST(StructTests, StructReference) {
    ASSERT_EQ("Strength build!", determineBuild(HelloWorld{18, 12}));
    ASSERT_EQ("Dex build!", determineBuild(HelloWorld{12, 13}));
}



HelloWorld testingCopy(int strength, int dexterity, HelloWorld* addressOut) {
    HelloWorld hw{strength, dexterity};
    addressOut = &hw;
    (void)addressOut;

    return hw;
}


TEST(StructTests, ReturningStructShouldMakeACopy) {
    HelloWorld* pointer{};

    HelloWorld hw = testingCopy(18, 12, pointer);

    ASSERT_EQ(hw.strength, 18);
    ASSERT_EQ(hw.dexterity, 12);
    ASSERT_NE(&hw, pointer);
    ASSERT_THAT(pointer, ::testing::IsNull());
}
