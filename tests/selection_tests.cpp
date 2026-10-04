#include "app/Selection.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Selection keeps unique IDs in insertion order", "[selection]") {
    app::Selection selection;
    const sim::GameEntityId first{1};
    const sim::GameEntityId second{2};

    REQUIRE(selection.entities().empty());

    selection.add(second);
    selection.add(first);
    selection.add(second);
    selection.add({});

    REQUIRE(selection.entities().size() == 2);
    REQUIRE(selection.entities()[0] == second);
    REQUIRE(selection.entities()[1] == first);
    REQUIRE(selection.contains(first));
    REQUIRE(selection.contains(second));
    REQUIRE_FALSE(selection.contains({}));

    selection.remove(sim::GameEntityId{999});
    REQUIRE(selection.entities().size() == 2);

    selection.remove(second);
    selection.remove(second);
    REQUIRE(selection.entities().size() == 1);
    REQUIRE(selection.entities()[0] == first);

    selection.remove(first);
    REQUIRE(selection.entities().empty());
}

TEST_CASE("Selection supports replacement and clearing", "[selection]") {
    app::Selection selection;
    const sim::GameEntityId first{1};
    const sim::GameEntityId second{2};
    const sim::GameEntityId third{3};

    selection.add(first);
    selection.add(second);
    selection.select(third);

    REQUIRE(selection.entities().size() == 1);
    REQUIRE(selection.contains(third));
    REQUIRE_FALSE(selection.contains(first));
    REQUIRE_FALSE(selection.contains(second));

    selection.add(first);
    selection.clear();
    REQUIRE(selection.entities().empty());

    selection.add(first);
    selection.add(second);
    selection.select({});
    REQUIRE(selection.entities().empty());
}

