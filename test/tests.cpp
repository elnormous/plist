#include <cstddef>
#include <string>
#include <vector>
#include "catch2/catch.hpp"
#include "plist.hpp"

TEST_CASE("Integer")
{
    const plist::Value d = 1;
    REQUIRE(d.as<std::int64_t>() == 1);
}

TEST_CASE("String")
{
    const plist::Value d = "test";
    REQUIRE(d.as<std::string>() == "test");
}
