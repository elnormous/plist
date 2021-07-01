#include <cstddef>
#include <string>
#include <vector>
#include "catch2/catch.hpp"
#include "plist.hpp"

TEST_CASE("Bool constructor", "[constructors]")
{
    const plist::Value d = true;
    REQUIRE(d.as<bool>() == true);
}

TEST_CASE("Integer constructor", "[constructors]")
{
    const plist::Value d = 1;
    REQUIRE(d.as<std::int64_t>() == 1);
}

TEST_CASE("String constructor", "[constructors]")
{
    const plist::Value d = std::string{"test"};
    REQUIRE(d.as<std::string>() == "test");
}

TEST_CASE("String literal constructor", "[constructors]")
{
    const plist::Value d = "test";
    REQUIRE(d.as<std::string>() == "test");
}

TEST_CASE("Array constructor", "[constructors]")
{
    const plist::Value d = plist::Array{plist::Value{0}, plist::Value{1}};
    REQUIRE(d.as<plist::Array>().size() == 2);
    REQUIRE(d[0].as<std::int64_t>() == 0);
    REQUIRE(d[1].as<std::int64_t>() == 1);
}

TEST_CASE("Dictionary constructor", "[constructors]")
{
    const plist::Value d = plist::Dictionary{{"0", plist::Value{0}}, {"1", plist::Value{1}}};
    REQUIRE(d.as<plist::Dictionary>().size() == 2);
    REQUIRE(d["0"].as<std::int64_t>() == 0);
    REQUIRE(d["1"].as<std::int64_t>() == 1);
}

TEST_CASE("Data constructor", "[constructors]")
{
    const plist::Value d = plist::Data{std::byte{0U}, std::byte{1U}};
    REQUIRE(d.as<plist::Data>().size() == 2);
    REQUIRE(d.as<plist::Data>()[0] == std::byte{0U});
    REQUIRE(d.as<plist::Data>()[1] == std::byte{1U});
}
