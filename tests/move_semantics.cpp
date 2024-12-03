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