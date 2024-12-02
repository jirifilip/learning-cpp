#include <gtest/gtest.h>
#include <gmock/gmock.h>


#include <numeric>
#include <limits>
#include <list>
#include <optional>


TEST(CollectionsTest, BasicVector) {
    std::vector<int> v{1, 2, 3, 4, 5};
    ASSERT_EQ(v[0], 1);

    std::vector<int> length10vector( 10 );
    ASSERT_THAT(length10vector, ::testing::SizeIs(10));
}


TEST(CollectionsTest, FunctionalOperations) {
    std::vector<int> v{1, 2, 3, 4, 5};

    std::transform(
        v.begin(),
        v.end(),
        v.begin(), 
        [](int x) { 
            return x + 1; 
        }
    );   

    ASSERT_THAT(v, ::testing::ElementsAre(2, 3, 4, 5, 6));
}


TEST(CollectionsTest, TestReduce) {
    std::vector<int> v{2, 5, 3};

    int result = std::reduce(
        v.begin(),
        v.end(),
        0,
        [](int accumulator, int current) {
            return accumulator + current;
        }
    );

    ASSERT_EQ(result, 10);
}


TEST(CollectionsTest, TestReduceWithString) {
    std::vector<std::string> v{ "Hello", "World", "how", "are", "you", "?" };

    int shortest = std::accumulate(
        v.begin(),
        v.end(),
        std::numeric_limits<int>::max(),
        [](int lowestLengthYet, const std::string& current) {
            int textLength = static_cast<int>(current.length());

            return textLength < lowestLengthYet ? textLength : lowestLengthYet;
        }
    );

    ASSERT_EQ(shortest, 1);
}


TEST(CollectionsTest, TestFilter) {
    std::vector<int> v{1, 2, 3, 4, 5};
    std::vector<int> oddNumbers;

    std::copy_if(
        v.begin(),
        v.end(),
        std::back_inserter(oddNumbers),
        [](int x) {
            return x % 2 != 0;
        }
    );

    ASSERT_THAT(oddNumbers, ::testing::ElementsAre(1, 3, 5));
}


TEST(CollectionTest, TestBackInsertingIterator) {
    std::vector<int> v;

    auto inserter = std::back_inserter(v);

    for (int i = 0; i < 5; i++) {
        inserter = i;
        inserter++;
    }

    ASSERT_THAT(v, ::testing::ElementsAre(0, 1, 2, 3, 4));
}


TEST(CollectionTest, TestFrontInsertingIterator) {
    std::list<int> l;

    auto inserter = std::front_inserter(l);

    for (int i = 4; i > -1; i--) {
        inserter = i;
        inserter++;
    }

    ASSERT_THAT(l, ::testing::ElementsAre(0, 1, 2, 3, 4));
}


template<typename T>
class NotReallyIterator {

private:
    static const int MAX_SIZE = 10;
    std::array<std::optional<T>, MAX_SIZE> array{std::nullopt};
    int currentIndex = 0;

public:
    void operator ++() {
        currentIndex += 1;
    }

    void operator =(T& element) {
        this->array[currentIndex] = element;
    }

    std::vector<T> getNonEmpty() {
        std::vector<T> nonEmpty{};
        
        for (int i = 0; i < MAX_SIZE; i++) {
            if (array[i].has_value()) {
                nonEmpty.push_back(
                    array[i].value()
                );
            }
        }

        return nonEmpty;
    } 
};


TEST(CollectionTest, TestCustomIteratorLikeObject) {
    NotReallyIterator<std::string> iterator{};

    std::vector<std::string> elements = {"hello", "bye", "goodbye", "hello", "salut"};

    for (auto el : elements) {
        iterator = el;
        ++iterator;
        ++iterator;
    }

    ASSERT_THAT(
        iterator.getNonEmpty(),
        ::testing::ElementsAre("hello", "bye", "goodbye", "hello", "salut")
    );
}
