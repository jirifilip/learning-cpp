#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <initializer_list>
#include <numeric>


class ConstClassTest {
public:
    int property;

    ConstClassTest(int x = 999) : property(x) {} 

    void changeProperty() {
        property = 20;
    }

    void notChangeProperty() const {}
};


TEST(ClassesTest, ConstClassTest) {
    const ConstClassTest c1 { 2 };
    ASSERT_EQ(c1.property, 2);

    c1.notChangeProperty();
    ASSERT_EQ(c1.property, 2);

    ConstClassTest c2 {};
    ASSERT_EQ(c2.property, 999);

    c2.changeProperty();
    ASSERT_EQ(c2.property, 20);
}


class OutsideMembers {
public:
    int plus(int a, int b);
};


int OutsideMembers::plus(int a, int b) {
    return a + b;
}


TEST(ClassesTest, OutsideMember) {
    OutsideMembers o1 {};
    
    ASSERT_EQ(o1.plus(1, 2), 3);
}


class NonConstOverload {
public:
    int property = 2;

    void changeProperty() const {}
    void changeProperty() {
        property = 99;
    }
};


TEST(ClassesTest, ConstMethodOverload) {
    NonConstOverload o1{};
    ASSERT_EQ(o1.property, 2);

    o1.changeProperty();
    ASSERT_EQ(o1.property, 99);

    const NonConstOverload o2{};
    ASSERT_EQ(o2.property, 2);

    o2.changeProperty();
    ASSERT_EQ(o2.property, 2);
}


class AccessingPrivateMembersOfSibling {
private:
    int property = 5;

public:
    void changePropertyOfOther(AccessingPrivateMembersOfSibling& other) {
        other.property = 99;
    }

    int getProperty() const {
        return property;
    }
};   


TEST(ClassesTest, ChangePrivateProperty) {
    AccessingPrivateMembersOfSibling p1 {};
    AccessingPrivateMembersOfSibling p2 {};

    ASSERT_EQ(p1.getProperty(), 5);
    ASSERT_EQ(p2.getProperty(), 5);

    p1.changePropertyOfOther(p2);

    ASSERT_EQ(p1.getProperty(), 5);
    ASSERT_EQ(p2.getProperty(), 99);
}


class GetterByReference {
public:
    std::string property = "hello world";

    const std::string& getProperty() const {
        return property;
    }

    const std::string getPropertyCopy() const {
        return property;
    }
};


TEST(ClassesTest, TestGettersByReference) {
    GetterByReference g {};

    ASSERT_EQ(g.property, "hello world");
    ASSERT_EQ(g.getProperty(), "hello world");
    ASSERT_EQ(&g.getProperty(), &g.property);

    std::string lvalue = g.getPropertyCopy(); 
    ASSERT_NE(&lvalue, &g.property);
    
    g.property = "bye bye";
    ASSERT_EQ(lvalue, "hello world");
}


class RValueDisposal {
private:
    std::string name;

public: 
    RValueDisposal(std::string name): name(name) {}

    void setName(std::string name) {
        this->name = name;
    }

    const std::string& getName() const {
        return name;
    }
};


RValueDisposal createByValue() {
    auto r = RValueDisposal("Frank");
    r.setName("Frankie");
    return r;
}


TEST(ClassesTest, TestRValueDisposal) {
    auto name { RValueDisposal("Frank").getName() };
    ASSERT_EQ(name, "Frank");

    auto name2 { createByValue().getName() };
    ASSERT_EQ(name2, "Frankie");
}


class CustomInitializerList {
public:
    int a;
    int sum;
    double mean;


    CustomInitializerList(std::initializer_list<int> initializer) {
        a = *initializer.begin();
        sum = std::reduce(initializer.begin(), initializer.end(), 0);
        mean = sum / initializer.size();
    }
};


TEST(ClassesTest, CustomInitializerList) {
    CustomInitializerList c {5, 6, 7, 8, 9};

    ASSERT_EQ(c.a, 5);
    ASSERT_EQ(c.sum, 35);
    ASSERT_EQ(c.mean, 7);
}