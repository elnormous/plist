#include <cstddef>
#include <string>
#include <vector>
#include "catch2/catch.hpp"
#include "plist.hpp"

TEST_CASE("Bool constructor", "[constructors]")
{
    const plist::Value v = true;
    REQUIRE(v.as<bool>() == true);
}

TEST_CASE("Integer constructor", "[constructors]")
{
    const plist::Value v = 1;
    REQUIRE(v.as<std::int64_t>() == 1);
}

TEST_CASE("String constructor", "[constructors]")
{
    const plist::Value v = std::string{"test"};
    REQUIRE(v.as<std::string>() == "test");
}

TEST_CASE("String literal constructor", "[constructors]")
{
    const plist::Value v = "test";
    REQUIRE(v.as<std::string>() == "test");
}

TEST_CASE("Array constructor", "[constructors]")
{
    const plist::Value v = plist::Array{plist::Value{0}, plist::Value{1}};
    REQUIRE(v.as<plist::Array>().size() == 2);
    REQUIRE(v[0].as<std::int64_t>() == 0);
    REQUIRE(v[1].as<std::int64_t>() == 1);
}

TEST_CASE("Dictionary constructor", "[constructors]")
{
    const plist::Value v = plist::Dictionary{{"0", plist::Value{0}}, {"1", plist::Value{1}}};
    REQUIRE(v.as<plist::Dictionary>().size() == 2);
    REQUIRE(v["0"].as<std::int64_t>() == 0);
    REQUIRE(v["1"].as<std::int64_t>() == 1);
}

TEST_CASE("Data constructor", "[constructors]")
{
    const plist::Value v = plist::Data{std::byte{0U}, std::byte{1U}};
    REQUIRE(v.as<plist::Data>().size() == 2);
    REQUIRE(v.as<plist::Data>()[0] == std::byte{0U});
    REQUIRE(v.as<plist::Data>()[1] == std::byte{1U});
}

TEST_CASE("Bool assignment", "[assignments]")
{
    plist::Value v;
    v = true;
    REQUIRE(v.as<bool>() == true);
}

TEST_CASE("Integer assignment", "[assignments]")
{
    plist::Value v;
    v = 1;
    REQUIRE(v.as<std::int64_t>() == 1);
}

TEST_CASE("String assignment", "[assignments]")
{
    plist::Value v;
    v = std::string{"test"};
    REQUIRE(v.as<std::string>() == "test");
}

TEST_CASE("String literal assignment", "[assignments]")
{
    plist::Value v;
    v = "test";
    REQUIRE(v.as<std::string>() == "test");
}

TEST_CASE("Array assignment", "[assignments]")
{
    plist::Value v;
    v = plist::Array{plist::Value{0}, plist::Value{1}};
    REQUIRE(v.as<plist::Array>().size() == 2);
    REQUIRE(v[0].as<std::int64_t>() == 0);
    REQUIRE(v[1].as<std::int64_t>() == 1);
}

TEST_CASE("Dictionary assignment", "[assignments]")
{
    plist::Value v;
    v = plist::Dictionary{{"0", plist::Value{0}}, {"1", plist::Value{1}}};
    REQUIRE(v.as<plist::Dictionary>().size() == 2);
    REQUIRE(v["0"].as<std::int64_t>() == 0);
    REQUIRE(v["1"].as<std::int64_t>() == 1);
}

TEST_CASE("Data assignment", "[assignments]")
{
    plist::Value v;
    v = plist::Data{std::byte{0U}, std::byte{1U}};
    REQUIRE(v.as<plist::Data>().size() == 2);
    REQUIRE(v.as<plist::Data>()[0] == std::byte{0U});
    REQUIRE(v.as<plist::Data>()[1] == std::byte{1U});
}

TEST_CASE("Integer encoding", "[encoding]")
{
    const plist::Value v = 1;

    SECTION("ascii")
    {
        const auto result = plist::encode(v, plist::Format::ascii);
        REQUIRE(result == "// !$*UTF8*$!\n1");
    }

    SECTION("xml")
    {
        const auto result = plist::encode(v, plist::Format::xml);
        REQUIRE(result == "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\"><plist version=\"1.0\"><integer>1</integer></plist>");
    }
}

TEST_CASE("Float encoding", "[encoding]")
{
    const plist::Value v = 1.0F;

    SECTION("ascii")
    {
        const auto result = plist::encode(v, plist::Format::ascii);
        REQUIRE(result == "// !$*UTF8*$!\n1.000000");
    }

    SECTION("xml")
    {
        const auto result = plist::encode(v, plist::Format::xml);
        REQUIRE(result == "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\"><plist version=\"1.0\"><real>1.000000</real></plist>");
    }
}

TEST_CASE("Bool encoding", "[encoding]")
{
    const plist::Value v = true;

    SECTION("ascii")
    {
        const auto result = plist::encode(v, plist::Format::ascii);
        REQUIRE(result == "// !$*UTF8*$!\nYES");
    }

    SECTION("xml")
    {
        const auto result = plist::encode(v, plist::Format::xml);
        REQUIRE(result == "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\"><plist version=\"1.0\"><true/></plist>");
    }
}

TEST_CASE("String encoding", "[encoding]")
{
    const plist::Value v = "a";

    SECTION("ascii")
    {
        const auto result = plist::encode(v, plist::Format::ascii);
        REQUIRE(result == "// !$*UTF8*$!\na");
    }

    SECTION("xml")
    {
        const auto result = plist::encode(v, plist::Format::xml);
        REQUIRE(result == "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\"><plist version=\"1.0\"><string>a</string></plist>");
    }
}

TEST_CASE("String with space encoding", "[encoding]")
{
    const plist::Value v = "a b";

    SECTION("ascii")
    {
        const auto result = plist::encode(v, plist::Format::ascii);
        REQUIRE(result == "// !$*UTF8*$!\n\"a b\"");
    }

    SECTION("xml")
    {
        const auto result = plist::encode(v, plist::Format::xml);
        REQUIRE(result == "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\"><plist version=\"1.0\"><string>a b</string></plist>");
    }
}

TEST_CASE("Empty object encoding", "[encoding]")
{
    const plist::Value v;

    SECTION("ascii")
    {
        const auto result = plist::encode(v, plist::Format::ascii);
        REQUIRE(result == "// !$*UTF8*$!\n{}");
    }

    SECTION("xml")
    {
        const auto result = plist::encode(v, plist::Format::xml);
        REQUIRE(result == "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\"><plist version=\"1.0\"><dict></dict></plist>");
    }
}
