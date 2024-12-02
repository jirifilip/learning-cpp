#include "gtest/gtest.h"
#include "gmock/gmock.h"


#include <list>


template<typename V, typename E>
class PathFinder {
private:
    std::vector<std::tuple<V, V, E>> vertices{}; 
public:
    void add(const V vertex1, const V vertex2, const E edge) {
        vertices.push_back(
            {vertex1, vertex2, edge}
        );
    }
    
    std::list<V> find(const V vertex1, const V vertex2, const std::function<bool(E)>& filter) {
        return std::list<V>();
    };
};





TEST(PathFinderTests, Instantiate) {
    PathFinder<std::string, int> pathfinder{};

    ASSERT_THAT(&pathfinder, ::testing::NotNull());
}


TEST(PathFinderTests, AddVertices) {
    PathFinder<std::string, int> pathfinder{};

    pathfinder.add("hello", "goodbe", 2);
    pathfinder.add("hello", "meowww", 30);
}