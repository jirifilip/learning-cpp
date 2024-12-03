#include "gtest/gtest.h"
#include "gmock/gmock.h"


#include <list>


template<typename V, typename E>
class PathFinder {
public:
    void add(const V vertex1, const V vertex2, const E edge) {
        vertices.push_back(
            {vertex1, vertex2, edge}
        );
    }
    
    std::vector<std::tuple<V, V, E>> find(const V& vertex1, const V& vertex2, const std::function<bool(E)>& filter) {
        (void)vertex1;
        (void)vertex2;
        auto prefilteredVertices = prefilterVertices(filter);

        return prefilteredVertices;
    };

private:
    std::vector<std::tuple<V, V, E>> vertices{}; 

    std::vector<std::tuple<V, V, E>> prefilterVertices(const std::function<bool(E)>& filter) {
        std::vector<std::tuple<V, V, E>> prefilteredVertices{};

        std::copy_if(
            vertices.begin(),
            vertices.end(),
            std::back_inserter(prefilteredVertices),
            [filter](std::tuple<V, V, E> input) {
                auto edge = std::get<2>(input);
                return filter(edge);
            }
        );

        return prefilteredVertices;
    }
};




TEST(PathFinderTests, Instantiate) {
    PathFinder<std::string, int> pathfinder{};

    ASSERT_THAT(&pathfinder, ::testing::NotNull());
}


TEST(PathFinderTests, AddVertices) {
    PathFinder<std::string, int> pathfinder{};

    pathfinder.add("hello", "goodbye", 2);
    pathfinder.add("hello", "meowww", 30);

    std::string text1 = "hello";

    pathfinder.find(
        text1,
        text1,
        [](int x) -> bool {
            (void)x;
            return true;
        }
    );

    ASSERT_THAT(
        pathfinder.find(
            text1,
            text1,
            [](int x) -> bool {
                return x > 15;
            }
        ),
        ::testing::ElementsAre(
            std::tuple{"hello", "meowww", 30}
        )
    );
}

