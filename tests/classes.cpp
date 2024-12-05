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


TEST(ClassesTest, TestReferenceRemoval) {
    ASSERT_TRUE((std::is_same<int, int>()).value);
    ASSERT_FALSE((std::is_same<int, int&>()).value);
    ASSERT_TRUE(
        (
            std::is_same<int, std::remove_reference<int&>::type>()
        ).value
    );
    ASSERT_TRUE(
        (
            std::is_same<int, std::remove_reference<int&&>::type>()
        ).value
    );
}


template <typename T>
class CustomUniquePointer {
private:
    T pointer;

public:
    CustomUniquePointer(T pointer): pointer(pointer) {}

    CustomUniquePointer(CustomUniquePointer& customUniquePointer) = delete;

    ~CustomUniquePointer() {
        delete pointer;
    }

    CustomUniquePointer(CustomUniquePointer&& customUniquePointer) {
        pointer = customUniquePointer.pointer;
        customUniquePointer.pointer = nullptr;
    }

    CustomUniquePointer& operator= (CustomUniquePointer& right) = delete;

    CustomUniquePointer& operator= (CustomUniquePointer&& right) {
        if (this == &right) {
            return *this;
        }

        delete pointer;

        pointer = right.pointer;
        right.pointer = nullptr;

        return *this;
    }

    bool isNull() {
        return pointer == nullptr;
    }
};


int functionExpectingValue(CustomUniquePointer<std::string*> u1) {
    (void) u1;
    return 99;
}

template <typename T>
std::remove_reference_t<T>&& customMove(T&& _Arg) noexcept {
    return static_cast<std::remove_reference_t<T>&&>(_Arg);
}


TEST(ClassesTest, TestRValueCasting) {
    CustomUniquePointer u1{ new std::string("hello") };

    ASSERT_EQ(
        functionExpectingValue(std::move(u1)),
        99
    );
    ASSERT_TRUE(u1.isNull());
    
    CustomUniquePointer u2{ new std::string("hello") };
    CustomUniquePointer u3 = std::move(u2);
    ASSERT_TRUE(u2.isNull());
    ASSERT_FALSE(u3.isNull());

    CustomUniquePointer u4 = std::move(u3);
    ASSERT_TRUE(u3.isNull());
    ASSERT_FALSE(u4.isNull());

    CustomUniquePointer u5 = customMove(u4);
    ASSERT_TRUE(u4.isNull());
    ASSERT_FALSE(u5.isNull());

    // custom move implemented by hand
    CustomUniquePointer<std::string*> uToMove{ new std::string("hello") };
    
    CustomUniquePointer uToMove2 {
        static_cast<std::remove_reference<CustomUniquePointer<std::string*>>::type&&>(uToMove)
    };
    ASSERT_TRUE(uToMove.isNull());
    ASSERT_FALSE(uToMove2.isNull());

    CustomUniquePointer uToMove3 {
        static_cast<CustomUniquePointer<std::string*>&&>(uToMove2)
    };
    ASSERT_TRUE(uToMove2.isNull());
    ASSERT_FALSE(uToMove3.isNull());

}


class ConstructorTypeConversion {
public:
    int a, b;
    ConstructorTypeConversion(int a, int b) :
        a { a },
        b { b } {}
};


TEST(ClassesTest, TypeConversion) {
    ConstructorTypeConversion c1(0.2, 0.2);
    
    // Narrowing conversion error:
    // ConstructorTypeConversion c2 { 0.2, 0.2 };

    ASSERT_EQ(c1.a, 0);
    ASSERT_EQ(c1.b, 0);

}


class IntPair {
public:
    int a, b;

    IntPair(int a, int b) : a { a }, b { b } {}; 
};


std::string pairToString(IntPair pair) {
    return "IntPair(" + std::to_string(pair.a) + ", " + std::to_string(pair.b) + ")";
}


std::string pairRefToString(IntPair& pair) {
    return "IntPair(" + std::to_string(pair.a) + ", " + std::to_string(pair.b) + ")";
}


TEST(ClassesTest, TemporaryObjects) {
    ASSERT_EQ(pairToString({ 2, 3}), "IntPair(2, 3)");
    ASSERT_EQ(pairToString(IntPair(2, 3)), "IntPair(2, 3)");
}


class CopyConstructor {
public:
    int a;
    int b;

    CopyConstructor(int a, int b) : a { a }, b { b } {}

    CopyConstructor(const CopyConstructor& other) : a { other.a }, b { other.b } {}
};


class ImplicitCopyConstructor {
public:
    int a;
    int b;

    ImplicitCopyConstructor(int a, int b) : a { a }, b { b + 1 } {}
};


TEST(ClassesTest, TestCopyConstructor) {
    CopyConstructor cp1 { 1, 2 };
    CopyConstructor cp2 { cp2 };

    ImplicitCopyConstructor i1 {1, 2};
    ImplicitCopyConstructor i2 { i1 };

    ASSERT_EQ(i1.a, 1);
    ASSERT_EQ(i1.b, 3);

    ASSERT_EQ(i2.a, 1);
    ASSERT_EQ(i2.b, 3);
}


class ImplicitConversion {
public:
    int x;

    ImplicitConversion(int x) : x(x) {}
};


std::string conversionToString(ImplicitConversion i) {
    return std::to_string(i.x);
}


TEST(ClassesTest, ImplicitConversion) {
    ASSERT_EQ(conversionToString(2), "2");
}