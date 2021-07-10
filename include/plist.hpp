// Ouzel by Elviss Strazdins

#ifndef OUZEL_FORMATS_PLIST_HPP
#define OUZEL_FORMATS_PLIST_HPP

#include <chrono>
#include <map>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

namespace plist
{
    class TypeError final: public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    class RangeError final: public std::range_error
    {
    public:
        using std::range_error::range_error;
    };

    class Value final
    {
        using Dictionary = std::map<std::string, Value>;
        using Array = std::vector<Value>;
        using Data = std::vector<std::byte>;
        using String = std::string;
        using Date = std::chrono::system_clock::time_point;
    public:
        Value() = default;
        Value(const Dictionary& v):value{v} {}
        Value(const Array& v):value(v) {}
        Value(bool v):value{v} {}
        template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
        Value(T v):value{static_cast<double>(v)} {}
        template <typename T, typename std::enable_if_t<std::is_integral_v<T> && !std::is_same_v<T, bool>>* = nullptr>
        Value(T v):value{static_cast<std::int64_t>(v)} {}
        Value(const String& v):value{v} {}
        Value(const char* v):value{v} {}
        Value(const Data& v):value{v} {}
        Value(const Date& v):value{v} {}

        Value& operator=(const Dictionary& v)
        {
            value = v;
            return *this;
        }

        Value& operator=(const Array& v)
        {
            value = v;
            return *this;
        }

        Value& operator=(bool v)
        {
            value = v;
            return *this;
        }

        template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
        Value& operator=(T v)
        {
            value = static_cast<double>(v);
            return *this;
        }

        template <typename T, typename std::enable_if_t<std::is_integral_v<T> && !std::is_same_v<T, bool>>* = nullptr>
        Value& operator=(T v)
        {
            value = static_cast<std::int64_t>(v);
            return *this;
        }

        Value& operator=(const String& v)
        {
            value = v;
            return *this;
        }

        Value& operator=(const char* v)
        {
            value = std::string{v};
            return *this;
        }

        Value& operator=(const Data& v)
        {
            value = v;
            return *this;
        }

        template <typename T, typename std::enable_if_t<std::is_same_v<T, bool>>* = nullptr>
        bool is() const noexcept
        {
            return std::holds_alternative<bool>(value);
        }

        template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
        bool is() const noexcept
        {
            return std::holds_alternative<double>(value);
        }

        template <typename T, typename std::enable_if_t<std::is_integral_v<T> && !std::is_same_v<T, bool>>* = nullptr>
        bool is() const noexcept
        {
            return std::holds_alternative<std::int64_t>(value);
        }

        template <typename T, typename std::enable_if_t<
            std::is_same_v<T, String> ||
            std::is_same_v<T, const char*>
        >* = nullptr>
        bool is() const noexcept
        {
            return std::holds_alternative<String>(value);
        }

        template <typename T, typename std::enable_if_t<std::is_same_v<T, Dictionary>>* = nullptr>
        bool is() const noexcept
        {
            return std::holds_alternative<Dictionary>(value);
        }

        template <typename T, typename std::enable_if_t<std::is_same_v<T, Array>>* = nullptr>
        bool is() const noexcept
        {
            return std::holds_alternative<Array>(value);
        }

        template <typename T, typename std::enable_if_t<std::is_same_v<T, Data>>* = nullptr>
        bool is() const noexcept
        {
            return std::holds_alternative<Data>(value);
        }

        template <typename T, typename std::enable_if_t<std::is_same_v<T, Date>>* = nullptr>
        bool is() const noexcept
        {
            return std::holds_alternative<Date>(value);
        }

        template <typename T, typename std::enable_if_t<std::is_same_v<T, bool>>* = nullptr>
        T as() const
        {
            if (const auto b = std::get_if<bool>(&value))
                return *b;
            else if (const auto d = std::get_if<double>(&value))
                return *d != 0.0;
            else if (const auto i = std::get_if<std::int64_t>(&value))
                return *i != 0;
            else
                throw TypeError{"Wrong type"};
        }

        template <typename T, typename std::enable_if_t<
            std::is_arithmetic_v<T> &&
            !std::is_same_v<T, bool>
        >* = nullptr>
        T as() const
        {
            if (const auto d = std::get_if<double>(&value))
                return static_cast<T>(*d);
            else if (const auto i = std::get_if<std::int64_t>(&value))
                return static_cast<T>(*i);
            else if (const auto b = std::get_if<bool>(&value))
                return *b ? T(1.0) : T(0.0);
            else
                throw TypeError{"Wrong type"};
        }

        template <typename T, typename std::enable_if_t<
            std::is_same_v<T, String> ||
            std::is_same_v<T, Dictionary> ||
            std::is_same_v<T, Array> ||
            std::is_same_v<T, Data> ||
            std::is_same_v<T, Date>
        >* = nullptr>
        T& as()
        {
            if (const auto p = std::get_if<T>(&value))
                return *p;
            else
                throw TypeError{"Wrong type"};
        }

        template <typename T, typename std::enable_if_t<std::is_same_v<T, const char*>>* = nullptr>
        T as() const
        {
            if (const auto p = std::get_if<String>(&value))
                return p->c_str();
            else
                throw TypeError{"Wrong type"};
        }

        template <typename T, typename std::enable_if_t<
            std::is_same_v<T, String> ||
            std::is_same_v<T, Dictionary> ||
            std::is_same_v<T, Array> ||
            std::is_same_v<T, Data> ||
            std::is_same_v<T, Date>
        >* = nullptr>
        const T& as() const
        {
            if (const auto p = std::get_if<T>(&value))
                return *p;
            else
                throw TypeError{"Wrong type"};
        }

        auto begin()
        {
            if (const auto p = std::get_if<Array>(&value))
                return p->begin();
            else
                throw TypeError{"Wrong type"};
        }

        auto end()
        {
            if (const auto p = std::get_if<Array>(&value))
                return p->end();
            else
                throw TypeError{"Wrong type"};
        }

        auto begin() const
        {
            if (const auto p = std::get_if<Array>(&value))
                return p->begin();
            else
                throw TypeError{"Wrong type"};
        }

        auto end() const
        {
            if (const auto p = std::get_if<Array>(&value))
                return p->end();
            else
                throw TypeError{"Wrong type"};
        }

        auto hasMember(const std::string& member) const
        {
            if (const auto p = std::get_if<Dictionary>(&value))
                return p->find(member) != p->end();
            else
                throw TypeError{"Wrong type"};
        }

        Value& operator[](const std::string& member) &
        {
            if (const auto p = std::get_if<Dictionary>(&value))
            {
                if (const auto iterator = p->find(member); iterator != p->end())
                    return iterator->second;
                else
                {
                    const auto [newIterator, success] = p->insert({std::string{member}, Value{}});
                    (void)success;
                    return newIterator->second;
                }
            }
            else
                throw TypeError{"Wrong type"};
        }

        const Value& operator[](const std::string& member) const&
        {
            if (const auto p = std::get_if<Dictionary>(&value))
            {
                if (const auto iterator = p->find(member); iterator != p->end())
                    return iterator->second;
                else
                    throw RangeError{"Member does not exist"};
            }
            else
                throw TypeError{"Wrong type"};
        }

        Value& operator[](std::size_t index) &
        {
            if (const auto p = std::get_if<Array>(&value))
            {
                if (index >= p->size()) p->resize(index + 1);
                return (*p)[index];
            }
            else
                throw TypeError{"Wrong type"};
        }

        const Value& operator[](std::size_t index) const&
        {
            if (const auto p = std::get_if<Array>(&value))
            {
                if (index < p->size())
                    return (*p)[index];
                else
                    throw RangeError{"Index out of range"};
            }
            else
                throw TypeError{"Wrong type"};
        }

        bool isEmpty() const
        {
            if (const auto p = std::get_if<Array>(&value))
                return p->empty();
            else
                throw TypeError{"Wrong type"};
        }

        std::size_t getSize() const
        {
            if (const auto p = std::get_if<Array>(&value))
                return p->size();
            else
                throw TypeError{"Wrong type"};
        }

        void resize(std::size_t size) &
        {
            if (const auto p = std::get_if<Array>(&value))
                return p->resize(size);
            else
                throw TypeError{"Wrong type"};
        }

        void pushBack(const Value& v) &
        {
            if (const auto p = std::get_if<Array>(&value))
                return p->push_back(v);
            else
                throw TypeError{"Wrong type"};
        }

        void pushBack(std::byte v)
        {
            if (const auto p = std::get_if<Data>(&value))
                return p->push_back(v);
            else
                throw TypeError{"Wrong type"};
        }

        auto& getValue() const noexcept { return value; }

    private:
        std::variant<Dictionary, Array, String, double, std::int64_t, bool, Data, Date> value;
    };

    enum class Format
    {
        text,
        xml
    };

    using Dictionary = std::map<std::string, Value>;
    using Array = std::vector<Value>;
    using Data = std::vector<std::byte>;
    using String = std::string;
    using Date = std::chrono::system_clock::time_point;

    inline std::string encode(const Value& value, Format format, bool whitespaces = false)
    {
        class TextEncoder final
        {
        public:
            static std::string encode(const Value& value, bool whitespaces)
            {
                std::string result = "// !$*UTF8*$!\n";
                encode(value, result, whitespaces);
                return result;
            }

        private:
            static void encode(const std::string& s, std::string& result)
            {
                if (!s.empty())
                {
                    bool hasSpecialChars = false;
                    for (const auto c : s)
                        if ((c < 'a' || c > 'z') &&
                            (c < 'A' || c > 'Z') &&
                            (c < '0' || c > '9') &&
                            c != '_' && c != '$' && c != '/' &&
                            c != ':' && c != '.' && c != '-')
                        {
                            hasSpecialChars = true;
                            break;
                        }

                    if (hasSpecialChars) result.push_back('"');
                    for (const auto c : s)
                    {
                        if (c == '"' || c == '\\') result += '\\';
                        result += c;
                    }
                    if (hasSpecialChars) result.push_back('"');
                }
                else
                    result += "\"\"";
            }

            static void encode(const Value& value, std::string& result, bool whitespaces, size_t level = 0)
            {
                if (auto dictionary = std::get_if<Dictionary>(&value.getValue()))
                {
                    result.push_back('{');
                    for (const auto& [key, entryValue] : *dictionary)
                    {
                        if (whitespaces) result.push_back('\n');
                        if (whitespaces) result.insert(result.end(), level + 1, '\t');
                        encode(key, result);
                        if (whitespaces) result.push_back(' ');
                        result.push_back('=');
                        if (whitespaces) result.push_back(' ');
                        encode(entryValue, result, whitespaces, level + 1);
                        result.push_back(';'); // trailing semicolon is mandatory
                    }
                    if (whitespaces) result.push_back('\n');
                    if (whitespaces) result.insert(result.end(), level, '\t');
                    result += "}";
                }
                else if (auto array = std::get_if<Array>(&value.getValue()))
                {
                    result.push_back('(');
                    std::size_t count = 0;
                    for (const auto& child : *array)
                    {
                        if (count++) result.push_back(','); // trailing comma is optional
                        if (whitespaces) result.push_back('\n');
                        if (whitespaces) result.insert(result.end(), level + 1, '\t');
                        encode(child, result, whitespaces, level + 1);
                    }
                    if (whitespaces) result.push_back('\n');
                    if (whitespaces) result.insert(result.end(), level, '\t');
                    result += ')';
                }
                else if (auto string = std::get_if<String>(&value.getValue()))
                {
                    encode(*string, result);
                }
                else if (auto real = std::get_if<double>(&value.getValue()))
                {
                    result += std::to_string(*real);
                }
                else if (auto integer = std::get_if<std::int64_t>(&value.getValue()))
                {
                    result += std::to_string(*integer);
                }
                else if (auto boolean = std::get_if<bool>(&value.getValue()))
                {
                    result += *boolean ? "YES" : "NO";
                }
                else if (auto data = std::get_if<Data>(&value.getValue()))
                {
                    result += '<';
                    std::size_t count = 0;
                    for (const auto b : *data)
                    {
                        if (whitespaces && count++) result.push_back(' ');
                        constexpr char digits[] = "0123456789ABCDEF";
                        result += digits[(static_cast<std::size_t>(b) >> 4) & 0x0F];
                        result += digits[static_cast<std::size_t>(b) & 0x0F];
                    }
                    result += '>';
                }
                else if (auto date = std::get_if<Date>(&value.getValue()))
                {
                    (void)date;
                    throw std::runtime_error("Date fields are not supported");
                }
                else
                    throw std::runtime_error("Unsupported format");
            }
        };

        class XmlEncoder final
        {
        public:
            static std::string encode(const Value& value, bool whitespaces)
            {
                std::string result = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
                if (whitespaces) result.push_back('\n');
                result += "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">";
                if (whitespaces) result.push_back('\n');
                result += "<plist version=\"1.0\">";
                if (whitespaces) result.push_back('\n');
                encode(value, result, whitespaces);
                if (whitespaces) result.push_back('\n');
                result += "</plist>";
                return result;
            }

        private:
            static void encode(const std::string& s, std::string& result)
            {
                for (const auto c : s)
                    if (c == '<') result += "&lt;";
                    else if (c == '>') result += "&gt;";
                    else if (c == '&') result += "&amp;";
                    else result.push_back(c);
            }

            static void encode(const Value& value, std::string& result, bool whitespaces, size_t level = 0)
            {
                if (auto dictionary = std::get_if<Dictionary>(&value.getValue()))
                {
                    result += "<dict>";
                    if (whitespaces) result.push_back('\n');
                    for (const auto& [key, entryValue] : *dictionary)
                    {
                        if (whitespaces) result.insert(result.end(), level + 1, '\t');
                        result += "<key>";
                        encode(key, result);
                        result += "</key>";
                        if (whitespaces) result += '\n';
                        if (whitespaces) result.insert(result.end(), level + 1, '\t');
                        encode(entryValue, result, whitespaces, level + 1);
                        if (whitespaces) result += '\n';
                    }
                    result.insert(result.end(), level, '\t');
                    result += "</dict>";
                }
                else if (auto array = std::get_if<Array>(&value.getValue()))
                {
                    result += "<array>";
                    if (whitespaces) result.push_back('\n');
                    for (const auto& child : *array)
                    {
                        if (whitespaces) result.insert(result.end(), level + 1, '\t');
                        encode(child, result, whitespaces, level + 1);
                        if (whitespaces) result.push_back('\n');
                    }
                    if (whitespaces) result.insert(result.end(), level, '\t');
                    result += "</array>";
                }
                else if (auto string = std::get_if<String>(&value.getValue()))
                {
                    result += "<string>";
                    encode(*string, result);
                    result += "</string>";
                }
                else if (auto real = std::get_if<double>(&value.getValue()))
                {
                    result += "<real>";
                    result += std::to_string(*real);
                    result += "</real>";
                }
                else if (auto integer = std::get_if<std::int64_t>(&value.getValue()))
                {
                    result += "<integer>";
                    result += std::to_string(*integer);
                    result += "</integer>";
                }
                else if (auto boolean = std::get_if<bool>(&value.getValue()))
                {
                    result += *boolean ? "<true/>" : "<false/>";
                }
                else if (auto data = std::get_if<Data>(&value.getValue()))
                {
                    result += "<data>";
                    constexpr char chars[] = {
                        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
                    };
                    std::size_t c = 0;
                    std::uint8_t charArray[3];
                    for (const auto b : *data)
                    {
                        charArray[c++] = static_cast<std::uint8_t>(b);
                        if (c == 3)
                        {
                            result += chars[static_cast<std::uint8_t>((charArray[0] & 0xFC) >> 2)];
                            result += chars[static_cast<std::uint8_t>(((charArray[0] & 0x03) << 4) + ((charArray[1] & 0xF0) >> 4))];
                            result += chars[static_cast<std::uint8_t>(((charArray[1] & 0x0F) << 2) + ((charArray[2] & 0xC0) >> 6))];
                            result += chars[static_cast<std::uint8_t>(charArray[2] & 0x3f)];
                            c = 0;
                        }
                    }

                    if (c)
                    {
                        result += chars[static_cast<std::uint8_t>((charArray[0] & 0xFC) >> 2)];

                        if (c == 1)
                            result += chars[static_cast<std::uint8_t>((charArray[0] & 0x03) << 4)];
                        else // c == 2
                        {
                            result += chars[static_cast<std::uint8_t>(((charArray[0] & 0x03) << 4) + ((charArray[1] & 0xF0) >> 4))];
                            result += chars[static_cast<std::uint8_t>((charArray[1] & 0x0F) << 2)];
                        }

                        while (++c < 4) result += '=';
                    }
                    result += "</data>";
                }
                else if (auto date = std::get_if<Date>(&value.getValue()))
                {
                    (void)date;
                    throw std::runtime_error("Date fields are not supported");
                }
                else
                    throw std::runtime_error("Unsupported format");
            }
        };

        switch (format)
        {
            case Format::text: return TextEncoder::encode(value, whitespaces);
            case Format::xml: return XmlEncoder::encode(value, whitespaces);
        }

        throw std::runtime_error("Unsupported format");
    }
}

#endif // OUZEL_FORMATS_PLIST_HPP
