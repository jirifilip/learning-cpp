#include <gtest/gtest.h>
#include <gmock/gmock.h>


template <typename T>
class AutoPointer {
private:
    T* pointer {};

public:
    AutoPointer(T* pointer = nullptr) 
        : pointer(pointer)
        {}

    AutoPointer(AutoPointer& other) {
        pointer = other.pointer;
        other.pointer = nullptr;        
    }

    ~AutoPointer() {
        delete pointer;
    }

    AutoPointer& operator=(AutoPointer& other) {
        if (&other == this) {
            return *this;
        }

        delete pointer;
        pointer = other.pointer;
        other.pointer = nullptr;
        return *this;
    }

    T& operator*() const { 
        return *pointer;
    }
    T* operator->() const {
        return pointer;
    }

    bool isNull() const {
        return pointer == nullptr;
    }
};


class Resource {
public:
    int x;

    Resource() {
        x = 5;
        std::cout << "Resource acquired" << std::endl;
    }

    ~Resource() {
        std::cout << "Resource destroyed" << std::endl;
    }
};


TEST(MoveSemantics, CustomAutoPointer) {
    AutoPointer<Resource> resource{ new Resource() };
    AutoPointer<Resource> resource2 { resource };
    AutoPointer<Resource> resource3 = resource2;

    ASSERT_TRUE(resource.isNull());
    ASSERT_TRUE(resource2.isNull());
    ASSERT_FALSE(resource3.isNull());

    Resource* nullResource = nullptr;
    (void)(*nullResource);
    delete nullResource;
}


template <typename T>
class RValueMove {
private:
    T* pointer;

public:
    RValueMove(T* pointer): pointer(pointer) {}

    RValueMove(RValueMove& right) = delete;

    RValueMove(RValueMove&& right) noexcept : pointer(right.pointer) {
        right.pointer = nullptr;
    }
    
    ~RValueMove() {
        delete pointer;
    }

    T& operator*() const {
        return *pointer;
    }

    RValueMove& operator= (RValueMove& right) = delete;

    RValueMove& operator= (RValueMove&& right) noexcept {
        if (&right = this) {
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


RValueMove<Resource> generateResource() {
    RValueMove<Resource> r = { new Resource() };
    return r;
}


TEST(MoveSemantics, BasicRValues) {
    int&& rInt { 5 };    

    ASSERT_EQ(rInt, 5);
}


TEST(MoveSemantics, MovableClass) {
    RValueMove<Resource> r1 { generateResource() };

    ASSERT_FALSE(r1.isNull());
}


template <typename T>
std::remove_reference_t<T>&& myMove(T&& input) {
    return static_cast<std::remove_reference_t<T>&&>(input);
}


TEST(MoveSemantics, MyMove) {
    RValueMove<Resource> r1 { generateResource() };
    RValueMove<Resource> r2 { myMove(r1) };

    ASSERT_TRUE(r1.isNull());
    ASSERT_FALSE(r2.isNull());
}


TEST(MoveSemantics, PlayingWithConversions) {
    double d { 3.5 };

    int i { static_cast<int>(d) };
    int iAssignment = d;

    ASSERT_EQ(i, 3);
    ASSERT_EQ(iAssignment, 3);
}


TEST(MoveSemantics, ConvertToRValue) {
    int i1 = 4;
    ASSERT_EQ(i1, 4);

    int i2{ 4 };
    ASSERT_EQ(i2, 4);

    int i3{ 5 };
    int i4{ i3 };
    ASSERT_EQ(i4, 5);

    int i1Ref{ i1 };
    ASSERT_EQ(i1Ref, 4);

    ASSERT_EQ(static_cast<int&&>(i1), 4);
}


TEST(MoveSemantics, ChangingRRefsValues) {
    int&& rref{ 10 };
    ASSERT_EQ(rref, 10);

    rref = 20;
    ASSERT_EQ(rref, 20);

    int&& i = 5;
    ASSERT_EQ(i, 5);    
}


std::string func(const int& lref) {
    (void) lref;
    return "lvalue";
}


std::string func(const int&& rref) {
    (void) rref;
    return "rvalue";
}


TEST(MoveSemantics, PassingLRValuesToFunctions) {
    int x { 5 };
    ASSERT_EQ(func(x), "lvalue");

    ASSERT_EQ(func(5), "rvalue");

    int &&y { 5 };
    ASSERT_EQ(func(y), "lvalue");
}
