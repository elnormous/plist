#include <cstddef>
#include <string>
#include <vector>
#include "catch2/catch.hpp"
#include "plist.hpp"

TEST_CASE("Bool constructor", "[constructors]")
{
    const plist::Value v = true;
    REQUIRE(v.is<bool>());
    REQUIRE(v.as<bool>() == true);
}

TEST_CASE("Integer constructor", "[constructors]")
{
    const plist::Value v = 1;
    REQUIRE(v.is<std::int64_t>());
    REQUIRE(v.as<std::int64_t>() == 1);
}

TEST_CASE("String constructor", "[constructors]")
{
    const plist::Value v = std::string{"test"};
    REQUIRE(v.is<std::string>());
    REQUIRE(v.as<std::string>() == "test");
}

TEST_CASE("String literal constructor", "[constructors]")
{
    const plist::Value v = "test";
    REQUIRE(v.is<std::string>());
    REQUIRE(v.as<std::string>() == "test");
}

TEST_CASE("Array constructor", "[constructors]")
{
    const plist::Value v = plist::Array{plist::Value{0}, plist::Value{1}};
    REQUIRE(v.is<plist::Array>());
    REQUIRE(v.getSize() == 2);
    REQUIRE(v[0].as<std::int64_t>() == 0);
    REQUIRE(v[1].as<std::int64_t>() == 1);
}

TEST_CASE("Dictionary constructor", "[constructors]")
{
    const plist::Value v = plist::Dictionary{{"0", plist::Value{0}}, {"1", plist::Value{1}}};
    REQUIRE(v.is<plist::Dictionary>());
    REQUIRE(v.as<plist::Dictionary>().size() == 2);
    REQUIRE(v.hasMember("0"));
    REQUIRE(v.hasMember("1"));
    REQUIRE(v["0"].as<std::int64_t>() == 0);
    REQUIRE(v["1"].as<std::int64_t>() == 1);
}

TEST_CASE("Data constructor", "[constructors]")
{
    const plist::Value v = plist::Data{std::byte{0U}, std::byte{1U}};
    REQUIRE(v.is<plist::Data>());
    REQUIRE(v.as<plist::Data>().size() == 2);
    REQUIRE(v.as<plist::Data>()[0] == std::byte{0U});
    REQUIRE(v.as<plist::Data>()[1] == std::byte{1U});
}

TEST_CASE("Date constructor", "[constructors]")
{
    const auto t = std::chrono::system_clock::now();
    const plist::Value v = t;
    REQUIRE(v.is<plist::Date>());
    REQUIRE(v.as<plist::Date>() == t);
}

TEST_CASE("Bool assignment", "[assignments]")
{
    plist::Value v;
    v = true;
    REQUIRE(v.is<bool>());
    REQUIRE(v.as<bool>() == true);
}

TEST_CASE("Integer assignment", "[assignments]")
{
    plist::Value v;
    v = 1;
    REQUIRE(v.is<std::int64_t>());
    REQUIRE(v.as<std::int64_t>() == 1);
}

TEST_CASE("String assignment", "[assignments]")
{
    plist::Value v;
    v = std::string{"test"};
    REQUIRE(v.is<std::string>());
    REQUIRE(v.as<std::string>() == "test");
}

TEST_CASE("String literal assignment", "[assignments]")
{
    plist::Value v;
    v = "test";
    REQUIRE(v.is<std::string>());
    REQUIRE(v.as<std::string>() == "test");
}

TEST_CASE("Array assignment", "[assignments]")
{
    plist::Value v;
    v = plist::Array{plist::Value{0}, plist::Value{1}};
    REQUIRE(v.is<plist::Array>());
    REQUIRE(v.getSize() == 2);
    REQUIRE(v[0].as<std::int64_t>() == 0);
    REQUIRE(v[1].as<std::int64_t>() == 1);
}

TEST_CASE("Dictionary assignment", "[assignments]")
{
    plist::Value v;
    v = plist::Dictionary{{"0", plist::Value{0}}, {"1", plist::Value{1}}};
    REQUIRE(v.is<plist::Dictionary>());
    REQUIRE(v.as<plist::Dictionary>().size() == 2);
    REQUIRE(v.hasMember("0"));
    REQUIRE(v.hasMember("1"));
    REQUIRE(v["0"].as<std::int64_t>() == 0);
    REQUIRE(v["1"].as<std::int64_t>() == 1);
}

TEST_CASE("Data assignment", "[assignments]")
{
    plist::Value v;
    v = plist::Data{std::byte{0U}, std::byte{1U}};
    REQUIRE(v.is<plist::Data>());
    REQUIRE(v.as<plist::Data>().size() == 2);
    REQUIRE(v.as<plist::Data>()[0] == std::byte{0U});
    REQUIRE(v.as<plist::Data>()[1] == std::byte{1U});
}

TEST_CASE("Date assignment", "[assignments]")
{
    const auto t = std::chrono::system_clock::now();
    plist::Value v;
    v = t;
    REQUIRE(v.is<plist::Date>());
    REQUIRE(v.as<plist::Date>() == t);
}

TEST_CASE("Array ranged loop", "[access]")
{
    plist::Value v = plist::Array{plist::Value{0}, plist::Value{1}};

    SECTION("mutable")
    {
        std::size_t counter = 0;
        for (const auto& e : v)
            REQUIRE(e.as<std::size_t>() == counter++);
        REQUIRE(counter == 2);
    }

    SECTION("const")
    {
        const auto c = v;
        std::size_t counter = 0;
        for (const auto& e : c)
            REQUIRE(e.as<std::size_t>() == counter++);
        REQUIRE(counter == 2);
    }
}

TEST_CASE("Integer encoding", "[encoding]")
{
    const plist::Value v = 1;

    SECTION("text")
    {
        const auto result = plist::encode(v, plist::Format::text);
        REQUIRE(result == "// !$*UTF8*$!\n1");
    }

    SECTION("xml")
    {
        const auto result = plist::encode(v, plist::Format::xml);
        REQUIRE(result == "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">"
                "<plist version=\"1.0\"><integer>1</integer></plist>");
    }
}

TEST_CASE("Float encoding", "[encoding]")
{
    const plist::Value v = 1.0F;

    SECTION("text")
    {
        const auto result = plist::encode(v, plist::Format::text);
        REQUIRE(result == "// !$*UTF8*$!\n1.000000");
    }

    SECTION("xml")
    {
        const auto result = plist::encode(v, plist::Format::xml);
        REQUIRE(result == "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">"
                "<plist version=\"1.0\"><real>1.000000</real></plist>");
    }
}

TEST_CASE("Bool false encoding", "[encoding]")
{
    const plist::Value v = false;

    SECTION("text")
    {
        const auto result = plist::encode(v, plist::Format::text);
        REQUIRE(result == "// !$*UTF8*$!\nNO");
    }

    SECTION("xml")
    {
        const auto result = plist::encode(v, plist::Format::xml);
        REQUIRE(result == "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">"
                "<plist version=\"1.0\"><false/></plist>");
    }
}

TEST_CASE("Bool true encoding", "[encoding]")
{
    const plist::Value v = true;

    SECTION("text")
    {
        const auto result = plist::encode(v, plist::Format::text);
        REQUIRE(result == "// !$*UTF8*$!\nYES");
    }

    SECTION("xml")
    {
        const auto result = plist::encode(v, plist::Format::xml);
        REQUIRE(result == "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">"
                "<plist version=\"1.0\"><true/></plist>");
    }
}

TEST_CASE("String encoding", "[encoding]")
{
    const plist::Value v = "a";

    SECTION("text")
    {
        const auto result = plist::encode(v, plist::Format::text);
        REQUIRE(result == "// !$*UTF8*$!\na");
    }

    SECTION("xml")
    {
        const auto result = plist::encode(v, plist::Format::xml);
        REQUIRE(result == "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">"
                "<plist version=\"1.0\"><string>a</string></plist>");
    }
}

TEST_CASE("String with space encoding", "[encoding]")
{
    const plist::Value v = "a b";

    SECTION("text")
    {
        const auto result = plist::encode(v, plist::Format::text);
        REQUIRE(result == "// !$*UTF8*$!\n\"a b\"");
    }

    SECTION("xml")
    {
        const auto result = plist::encode(v, plist::Format::xml);
        REQUIRE(result == "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">"
                "<plist version=\"1.0\"><string>a b</string></plist>");
    }
}

TEST_CASE("Empty array encoding", "[encoding]")
{
    const plist::Value v = plist::Array{};

    SECTION("text")
    {
        const auto result = plist::encode(v, plist::Format::text);
        REQUIRE(result == "// !$*UTF8*$!\n()");
    }

    SECTION("xml")
    {
        const auto result = plist::encode(v, plist::Format::xml);
        REQUIRE(result == "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">"
                "<plist version=\"1.0\"><array></array></plist>");
    }
}

TEST_CASE("Empty dictionary encoding", "[encoding]")
{
    const plist::Value v;

    SECTION("text")
    {
        const auto result = plist::encode(v, plist::Format::text);
        REQUIRE(result == "// !$*UTF8*$!\n{}");
    }

    SECTION("xml")
    {
        const auto result = plist::encode(v, plist::Format::xml);
        REQUIRE(result == "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">"
                "<plist version=\"1.0\"><dict></dict></plist>");
    }
}

TEST_CASE("Binary data encoding", "[encoding]")
{
    const plist::Value v = plist::Data{std::byte{0U}, std::byte{1U}};

    SECTION("text")
    {
        const auto result = plist::encode(v, plist::Format::text);
        REQUIRE(result == "// !$*UTF8*$!\n<0001>");
    }

    SECTION("text with white spaces")
    {
        const auto result = plist::encode(v, plist::Format::text, true);
        REQUIRE(result == "// !$*UTF8*$!\n<00 01>");
    }

    SECTION("xml")
    {
        const auto result = plist::encode(v, plist::Format::xml);
        REQUIRE(result == "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">"
                "<plist version=\"1.0\"><data>AAE=</data></plist>");
    }
}

TEST_CASE("Array encoding", "[encoding]")
{
    const plist::Value v = plist::Array{1, 2};

    SECTION("text")
    {
        const auto result = plist::encode(v, plist::Format::text);
        REQUIRE(result == "// !$*UTF8*$!\n(1,2)");
    }

    SECTION("text with white spaces")
    {
        const auto result = plist::encode(v, plist::Format::text, true);
        REQUIRE(result == "// !$*UTF8*$!\n(\n\t1,\n\t2\n)");
    }

    SECTION("xml")
    {
        const auto result = plist::encode(v, plist::Format::xml);
        REQUIRE(result == "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\"><plist version=\"1.0\">"
                "<array><integer>1</integer><integer>2</integer></array></plist>");
    }

    SECTION("xml with white spaces")
    {
        const auto result = plist::encode(v, plist::Format::xml, true);
        REQUIRE(result == "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
                "<plist version=\"1.0\">\n"
                "<array>\n\t<integer>1</integer>\n\t<integer>2</integer>\n</array>\n"
                "</plist>");
    }
}

TEST_CASE("Dictionary encoding", "[encoding]")
{
    const plist::Value v = plist::Dictionary{
        {"a", 1},
        {"b", 2}
    };

    SECTION("text")
    {
        const auto result = plist::encode(v, plist::Format::text);
        REQUIRE(result == "// !$*UTF8*$!\n{a=1;b=2;}");
    }

    SECTION("text with whites paces")
    {
        const auto result = plist::encode(v, plist::Format::text, true);
        REQUIRE(result == "// !$*UTF8*$!\n{\n\ta = 1;\n\tb = 2;\n}");
    }

    SECTION("xml")
    {
        const auto result = plist::encode(v, plist::Format::xml);
        REQUIRE(result == "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">"
                "<plist version=\"1.0\"><dict><key>a</key><integer>1</integer><key>b</key><integer>2</integer></dict></plist>");
    }

    SECTION("xml with whites paces")
    {
        const auto result = plist::encode(v, plist::Format::xml, true);
        REQUIRE(result == "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
                "<plist version=\"1.0\">\n"
                "<dict>\n"
                "\t<key>a</key>\n"
                "\t<integer>1</integer>\n"
                "\t<key>b</key>\n"
                "\t<integer>2</integer>\n"
                "</dict>\n"
                "</plist>");
    }
}
