#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

TEST_CASE("catch hello_world", "[catch],[hello_world],[quick]") {
    REQUIRE(true);
}