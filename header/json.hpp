/****************************************************************************************
                                                             _____________________
  https://github.com/haiyyyyh                               |     by haiyyyyh     |
     only-json::DOM parser                                  |   version v0.4.0    |
                                                            ``````````````````````
                             _      _
 //|||\\  //\\    \\  ||     \\    //                \\  //||\\   //|||\\  //\\    \\
||    ||  || \\   ||  ||      \\  //                 ||  ||      ||    ||  || \\   ||
||    ||  ||  \\  ||  ||       \\//   ::::::::  _    ||  \\||\\  ||    ||  ||  \\  ||
||    ||  ||   \\ ||  ||        ||              \\   ||  _   ||  ||    ||  ||   \\ ||
\\\||//   \\    \\//  \\\||//   ||               \\\//   \\||//  \\|||//   \\    \\//

                                                                    ---- Bloated IS Sin
     =================
    [|      {↗}      |]
    [|   Only-Json   |]
    [| License : MIT |]
    [|  © haiyyyyh   |]
     ^^^^^^^^^^^^^^^^

    Nothing, but only json


 ===\ Source BookMark
    |     >> something like "P0: " in source code, for editor searching and quickly jumping
    |- Headers
    |- P0
    |- P1
    |- \- P1.0
    |  |- P1.1
    |  |- P1.2
    |  |- P1.3
    |  |- P1.4
    |  |- P1.5
    |  `- P1.6
    `- P2
       \- P2.0
       |- P2.1
       |- P2.2
       |- P2.3
       |- P2.4
       `- P2.5

*****************************************************************************************/


// MARK: Headers


#include <charconv>         // to_chars
#include <cmath>            // macro INFINITY
#include <cstdio>           // fprintf for panic output
#include <cstring>          // memcpy
#include <fstream>          // ifstring
#include <map>              // object_t
#include <memory_resource>  // poll
#include <string>           // string_t
#include <vector>           // array_t



namespace hai {


/*=======================================================
 * MARK: P0: private template tools,table and functions
 *======================================================*/


namespace {

namespace tools {

// SFINAE, Integer (no bool)

template <typename T>
constexpr bool IntegerOnly{false};

template <>
constexpr bool IntegerOnly<char>{true};

template <>
constexpr bool IntegerOnly<short>{true};

template <>
constexpr bool IntegerOnly<int>{true};

template <>
constexpr bool IntegerOnly<long>{true};

template <>
constexpr bool IntegerOnly<long long>{true};

template <>
constexpr bool IntegerOnly<unsigned char>{true};

template <>
constexpr bool IntegerOnly<unsigned short>{true};

template <>
constexpr bool IntegerOnly<unsigned int>{true};

template <>
constexpr bool IntegerOnly<unsigned long>{true};

template <>
constexpr bool IntegerOnly<unsigned long long>{true};

constexpr long long LL_MAX = 9223372036854775807LL;

// SFINAE, Float

template <typename T>
constexpr bool FloatOnly{false};

template <>
constexpr bool FloatOnly<float>{true};

template <>
constexpr bool FloatOnly<double>{true};

template <>
constexpr bool FloatOnly<long double>{true};

template <typename T1>
constexpr auto max_sizeof() -> unsigned long {
        return sizeof(T1);
}

// max
template <typename T1, typename T2, typename... Args>
constexpr auto max_sizeof() -> unsigned long {
        unsigned long others_max = max_sizeof<T2, Args...>();
        return sizeof(T1) > others_max ? sizeof(T1) : others_max;
}

template <typename T>
auto mut_memcast(void* mem) -> T& {
        return *reinterpret_cast<T*>(mem);
}

template <typename T>
auto memcast(const void* mem) -> const T& {
        return *reinterpret_cast<const T*>(mem);
}

void panic(const char* msg) {
        fprintf(stderr, "\e[31;1m[panic] \e[34;3monly-json\e[0m : %s\n", msg);
        abort();
}

void bad_using_panic(const char* func_name, const char* on_type) {
        fprintf(stderr,
                "\e[31;1m[panic] \e[34;3monly-json\e[0m : "
                "\e[33;1;4mtype check failed\e[0m"
                " (not '\e[36;1m%s\e[0m') when using '\e[32;3;1m%s\e[0m'\n",
                on_type, func_name);
        abort();
}

void bad_getting_panic(const char* msg) {
        fprintf(stderr,
                "\e[31;1m[panic] \e[34;3monly-json\e[0m : "
                "\e[33;1;4mtype check failed\e[0m"
                " when getting '\e[36;1m%s\e[0m'\n",
                msg);
        abort();
}

void bad_conversion_panic(const char* to_type) {
        fprintf(stderr,
                "\e[31;1m[panic] \e[34;3monly-json\e[0m : "
                "\e[33;1;4mbad conversion\e[0m"
                " to '\e[36;1m%s\e[0m'\n",
                to_type);
        abort();
}

// clang-format off

const char* table[201] = {
       "-100", "-99", "-98", "-97", "-96", "-95", "-94", "-93", "-92", "-91",
        "-90", "-89", "-88", "-87", "-86", "-85", "-84", "-83", "-82", "-81",
        "-80", "-79", "-78", "-77", "-76", "-75", "-74", "-73", "-72", "-71",
        "-70", "-69", "-68", "-67", "-66", "-65", "-64", "-63", "-62", "-61",
        "-60", "-59", "-58", "-57", "-56", "-55", "-54", "-53", "-52", "-51",
        "-50", "-49", "-48", "-47", "-46", "-45", "-44", "-43", "-42", "-41",
        "-40", "-39", "-38", "-37", "-36", "-35", "-34", "-33", "-32", "-31",
        "-30", "-29", "-28", "-27", "-26", "-25", "-24", "-23", "-22", "-21",
        "-20", "-19", "-18", "-17", "-16", "-15", "-14", "-13", "-12", "-11",
        "-10", "-9",  "-8",  "-7",  "-6",  "-5",  "-4",  "-3",  "-2",  "-1", "0",
        "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",  "10",
        "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
        "21", "22", "23", "24", "25", "26", "27", "28", "29", "30",
        "31", "32", "33", "34", "35", "36", "37", "38", "39", "40",
        "41", "42", "43", "44", "45", "46", "47", "48", "49", "50",
        "51", "52", "53", "54", "55", "56", "57", "58", "59", "60",
        "61", "62", "63", "64", "65", "66", "67", "68", "69", "70",
        "71", "72", "73", "74", "75", "76", "77", "78", "79", "80",
        "81", "82", "83", "84", "85", "86", "87", "88", "89", "90",
        "91", "92", "93", "94", "95", "96", "97", "98", "99", "100"
};

// clang-format on

char buf[22];

auto to_str(long long num) -> const char* {
        if (num <= 100 && num >= -100) {
                return table[num + 100];
        }
        auto [p, err] = std::to_chars(buf, buf + 20, num);
        *p = '\0';
        return buf;
}

auto to_str(unsigned long long num) -> const char* {
        if (num <= 100) {
                return table[num + 100];
        }
        auto [p, err] = std::to_chars(buf, buf + 20, num);
        *p = '\0';
        return buf;
}

auto to_str(double num) -> const char* {
        auto [p, err] = std::to_chars(buf, buf + 20, num);
        *p = '\0';
        return buf;
}

// clang-format off

const double pow_table[] {
    1e0,  1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,  1e8,  1e9, 
    1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19,
    1e20, 1e21, 1e22, 1e23, 1e24, 1e25, 1e26, 1e27, 1e28, 1e29,
    1e30, 1e31, 1e32, 1e33, 1e34, 1e35, 1e36, 1e37, 1e38, 1e39,
    1e40, 1e41, 1e42, 1e43, 1e44, 1e45, 1e46, 1e47, 1e48, 1e49,
    1e50, 1e51, 1e52, 1e53, 1e54, 1e55, 1e56, 1e57, 1e58, 1e59,
    1e60, 1e61, 1e62, 1e63, 1e64, 1e65, 1e66, 1e67, 1e68, 1e69,
    1e70, 1e71, 1e72, 1e73, 1e74, 1e75, 1e76, 1e77, 1e78, 1e79,
    1e80, 1e81, 1e82, 1e83, 1e84, 1e85, 1e86, 1e87, 1e88, 1e89,
    1e90, 1e91, 1e92, 1e93, 1e94, 1e95, 1e96, 1e97, 1e98, 1e99,
    1e100, 1e101, 1e102, 1e103, 1e104, 1e105, 1e106, 1e107, 1e108, 1e109, 
    1e110, 1e111, 1e112, 1e113, 1e114, 1e115, 1e116, 1e117, 1e118, 1e119,
    1e120, 1e121, 1e122, 1e123, 1e124, 1e125, 1e126, 1e127, 1e128, 1e129,
    1e130, 1e131, 1e132, 1e133, 1e134, 1e135, 1e136, 1e137, 1e138, 1e139,
    1e140, 1e141, 1e142, 1e143, 1e144, 1e145, 1e146, 1e147, 1e148, 1e149,
    1e150, 1e151, 1e152, 1e153, 1e154, 1e155, 1e156, 1e157, 1e158, 1e159,
    1e160, 1e161, 1e162, 1e163, 1e164, 1e165, 1e166, 1e167, 1e168, 1e169,
    1e170, 1e171, 1e172, 1e173, 1e174, 1e175, 1e176, 1e177, 1e178, 1e179,
    1e180, 1e181, 1e182, 1e183, 1e184, 1e185, 1e186, 1e187, 1e188, 1e189,
    1e190, 1e191, 1e192, 1e193, 1e194, 1e195, 1e196, 1e197, 1e198, 1e199,
    //
    1e200, 1e201, 1e202, 1e203, 1e204, 1e205, 1e206, 1e207, 1e208, 1e209,
    1e210, 1e211, 1e212, 1e213, 1e214, 1e215, 1e216, 1e217, 1e218, 1e219,
    1e220, 1e221, 1e222, 1e223, 1e224, 1e225, 1e226, 1e227, 1e228, 1e229,
    1e230, 1e231, 1e232, 1e233, 1e234, 1e235, 1e236, 1e237, 1e238, 1e239,
    1e240, 1e241, 1e242, 1e243, 1e244, 1e245, 1e246, 1e247, 1e248, 1e249,
    1e250, 1e251, 1e252, 1e253, 1e254, 1e255, 1e256, 1e257, 1e258, 1e259,
    1e260, 1e261, 1e262, 1e263, 1e264, 1e265, 1e266, 1e267, 1e268, 1e269,
    1e270, 1e271, 1e272, 1e273, 1e274, 1e275, 1e276, 1e277, 1e278, 1e279,
    1e280, 1e281, 1e282, 1e283, 1e284, 1e285, 1e286, 1e287, 1e288, 1e289,
    1e290, 1e291, 1e292, 1e293, 1e294, 1e295, 1e296, 1e297, 1e298, 1e299,
    1e300, 1e301, 1e302, 1e303, 1e304, 1e305, 1e306, 1e307, 1e308
};

const double neg_pow_table[] {
    1e-0,  1e-1,  1e-2,  1e-3,  1e-4,  1e-5,  1e-6,  1e-7,  1e-8,  1e-9, 
    1e-10, 1e-11, 1e-12, 1e-13, 1e-14, 1e-15, 1e-16, 1e-17, 1e-18, 1e-19,
    1e-20, 1e-21, 1e-22, 1e-23, 1e-24, 1e-25, 1e-26, 1e-27, 1e-28, 1e-29,
    1e-30, 1e-31, 1e-32, 1e-33, 1e-34, 1e-35, 1e-36, 1e-37, 1e-38, 1e-39,
    1e-40, 1e-41, 1e-42, 1e-43, 1e-44, 1e-45, 1e-46, 1e-47, 1e-48, 1e-49,
    1e-50, 1e-51, 1e-52, 1e-53, 1e-54, 1e-55, 1e-56, 1e-57, 1e-58, 1e-59,
    1e-60, 1e-61, 1e-62, 1e-63, 1e-64, 1e-65, 1e-66, 1e-67, 1e-68, 1e-69,
    1e-70, 1e-71, 1e-72, 1e-73, 1e-74, 1e-75, 1e-76, 1e-77, 1e-78, 1e-79,
    1e-80, 1e-81, 1e-82, 1e-83, 1e-84, 1e-85, 1e-86, 1e-87, 1e-88, 1e-89,
    1e-90, 1e-91, 1e-92, 1e-93, 1e-94, 1e-95, 1e-96, 1e-97, 1e-98, 1e-99,
    1e-100, 1e-101, 1e-102, 1e-103, 1e-104, 1e-105, 1e-106, 1e-107, 1e-108, 1e-109, 
    1e-110, 1e-111, 1e-112, 1e-113, 1e-114, 1e-115, 1e-116, 1e-117, 1e-118, 1e-119,
    1e-120, 1e-121, 1e-122, 1e-123, 1e-124, 1e-125, 1e-126, 1e-127, 1e-128, 1e-129,
    1e-130, 1e-131, 1e-132, 1e-133, 1e-134, 1e-135, 1e-136, 1e-137, 1e-138, 1e-139,
    1e-140, 1e-141, 1e-142, 1e-143, 1e-144, 1e-145, 1e-146, 1e-147, 1e-148, 1e-149,
    1e-150, 1e-151, 1e-152, 1e-153, 1e-154, 1e-155, 1e-156, 1e-157, 1e-158, 1e-159,
    1e-160, 1e-161, 1e-162, 1e-163, 1e-164, 1e-165, 1e-166, 1e-167, 1e-168, 1e-169,
    1e-170, 1e-171, 1e-172, 1e-173, 1e-174, 1e-175, 1e-176, 1e-177, 1e-178, 1e-179,
    1e-180, 1e-181, 1e-182, 1e-183, 1e-184, 1e-185, 1e-186, 1e-187, 1e-188, 1e-189,
    1e-190, 1e-191, 1e-192, 1e-193, 1e-194, 1e-195, 1e-196, 1e-197, 1e-198, 1e-199,
    //
    1e-200, 1e-201, 1e-202, 1e-203, 1e-204, 1e-205, 1e-206, 1e-207, 1e-208, 1e-209,
    1e-210, 1e-211, 1e-212, 1e-213, 1e-214, 1e-215, 1e-216, 1e-217, 1e-218, 1e-219,
    1e-220, 1e-221, 1e-222, 1e-223, 1e-224, 1e-225, 1e-226, 1e-227, 1e-228, 1e-229,
    1e-230, 1e-231, 1e-232, 1e-233, 1e-234, 1e-235, 1e-236, 1e-237, 1e-238, 1e-239,
    1e-240, 1e-241, 1e-242, 1e-243, 1e-244, 1e-245, 1e-246, 1e-247, 1e-248, 1e-249,
    1e-250, 1e-251, 1e-252, 1e-253, 1e-254, 1e-255, 1e-256, 1e-257, 1e-258, 1e-259,
    1e-260, 1e-261, 1e-262, 1e-263, 1e-264, 1e-265, 1e-266, 1e-267, 1e-268, 1e-269,
    1e-270, 1e-271, 1e-272, 1e-273, 1e-274, 1e-275, 1e-276, 1e-277, 1e-278, 1e-279,
    1e-280, 1e-281, 1e-282, 1e-283, 1e-284, 1e-285, 1e-286, 1e-287, 1e-288, 1e-289,
    1e-290, 1e-291, 1e-292, 1e-293, 1e-294, 1e-295, 1e-296, 1e-297, 1e-298, 1e-299,
    1e-300, 1e-301, 1e-302, 1e-303, 1e-304, 1e-305, 1e-306, 1e-307, 1e-308, 1e-309,
    1e-310, 1e-311, 1e-312, 1e-313, 1e-314, 1e-315, 1e-316, 1e-317, 1e-318, 1e-319,
    1e-320, 1e-321, 1e-322, 1e-323
};

constexpr int pow_table_len = sizeof(pow_table) / sizeof(pow_table[0]);
constexpr int neg_pow_table_len = sizeof(neg_pow_table) / sizeof(neg_pow_table[0]);

const char* indent_table[] {
        "",
        "    ",
        "        ",
        "            ",
        "                ",
        "                    ",
        "                        ",
        "                            ",
        "                                ",
        "                                    ",
        "                                        ",
        "                                            ",
        "                                                ",
        "                                                    ",
        "                                                        ",
        "                                                            ",
        "                                                                ",
        "                                                                    ",
        "                                                                        ",
        "                                                                            ",
        "                                                                                ",
        "                                                                                    ",
        "                                                                                        ",
        "                                                                                            ",
        "                                                                                                ",
        "                                                                                                    ",
        "                                                                                                        ",
        "                                                                                                            ",
        "                                                                                                                ",
        "                                                                                                                    ",
        "                                                                                                                        ",
};

const char* escape_table[32] {
        "\\u0000", "\\u0001", "\\u0002", "\\u0003",
        "\\u0004", "\\u0005", "\\u0006", "\\u0007",
        "\\b",     "\\t",     "\\n",     "\\u000B",
        "\\f",     "\\r",     "\\u000E", "\\u000F",
        "\\u0010", "\\u0011", "\\u0012", "\\u0013",
        "\\u0014", "\\u0015", "\\u0016", "\\u0017",
        "\\u0018", "\\u0019", "\\u001A", "\\u001B",
        "\\u001C", "\\u001D", "\\u001E", "\\u001F"
};

// clang-format on

template <typename... Args>
class tuple;

template <>
class tuple<> {
public:
        tuple() = default;
        template <int idx>
        auto get() {
                static_assert(false, "tuple get<> index out of bounds");
        }
};

template <typename T0, typename... Args>
class tuple<T0, Args...> {
private:
        T0 _0;
        tuple<Args...> _other;

private:
        template <int idx>
        struct Tag {};

        template <int idx>
        auto& get_item(Tag<idx>) {
                return _other.template get<idx - 1>();
        }

        auto& get_item(Tag<0>) { return _0; }

public:
        tuple() {}
        tuple(T0 _t0, Args... _other_t) : _0(_t0), _other(_other_t...) {}
        template <int idx>
        auto& get() {
                return this->get_item(Tag<idx>{});
        }
};

template <typename any_string>
void u32ch_decode_dump_to_u8string(unsigned int ch, any_string& str) {
        if (ch <= 0x7F) {
                // 1字节：0xxxxxxx
                str += (char)ch;
        } else if (ch <= 0x7FF) {
                // 2字节：110xxxxx 10xxxxxx
                str += (char)(0xC0 | ((ch >> 6) & 0x1F));  // 第1字节：110 + 高5位
                str += (char)(0x80 | (ch & 0x3F));         // 第2字节：10 + 低6位
        } else if (ch <= 0xFFFF) {
                // 3字节：1110xxxx 10xxxxxx 10xxxxxx
                str += (char)(0xE0 | ((ch >> 12) & 0x0F));  // 第1字节：1110 + 高4位
                str += (char)(0x80 | ((ch >> 6) & 0x3F));   // 第2字节：10 + 中间6位
                str += (char)(0x80 | (ch & 0x3F));          // 第3字节：10 + 低6位
        } else {
                // 4字节：11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
                str += (char)(0xF0 | ((ch >> 18) & 0x07));  // 第1字节：11110 + 高3位
                str += (char)(0x80 | ((ch >> 12) & 0x3F));  // 第2字节：10 + 次高6位
                str += (char)(0x80 | ((ch >> 6) & 0x3F));   // 第3字节：10 + 中间6位
                str += (char)(0x80 | (ch & 0x3F));          // 第4字节：10 + 低6位
        }
}


// try to improve speed
template <typename K, typename V, typename Alloc>
class linear_map {
private:
        using dataT = std::pair<K, V>;
        std::vector<dataT, Alloc> _data;

public:
        linear_map() = default;
        linear_map(const linear_map&) = default;
        linear_map(linear_map&& other) noexcept : _data(std::move(other._data)) {}
        linear_map(std::initializer_list<dataT> init) : _data(init) {}

        auto operator[](K key) noexcept -> V& {
                for (auto& [_k, _v] : _data) {
                        if (_k == key) {
                                return _v;
                        }
                }
                _data.push_back({std::move(key), {}});
                return _data.back().second();
        }

        auto check_insert(const dataT& new_data) noexcept -> linear_map& {
                for (auto& [_k, _v] : _data) {
                        if (_k == new_data.first) {
                                _v = new_data.second;
                                return *this;
                        }
                }
                _data.push_back(new_data);
                return *this;
        }

        auto check_insert(dataT&& new_data) noexcept -> linear_map& {
                for (auto& [_k, _v] : _data) {
                        if (_k == new_data.first) {
                                _v = std::move(new_data.second);
                                return *this;
                        }
                }
                _data.push_back(std::move(new_data));
                return *this;
        }

        auto insert(const dataT& new_data) noexcept -> linear_map& {
                _data.push_back(new_data);
                return *this;
        }

        auto insert(dataT&& new_data) noexcept -> linear_map& {
                _data.push_back(std::move(new_data));
                return *this;
        }

        auto size() const noexcept -> size_t { return _data.size(); }

        auto data() const noexcept -> std::vector<dataT, Alloc>& { return _data; }

        auto begin() const noexcept { return _data.begin(); }

        auto end() const noexcept { return _data.end(); }

        auto at(size_t idx) const noexcept -> V& { return _data[idx].second(); }
};


// string, require one argument, the allocator
template <typename Alloc>
using std_string_wrapping = std::basic_string<char, std::char_traits<char>, Alloc>;

// object, require three arguments: keytype, valuetype, allocater
template <typename k, typename v, typename all>
using std_map_wrapping = std::map<k, v, std::less<k>, all>;


}  // namespace tools

}  // namespace



/*=======================================================
 * MARK: P1: json class definition start
 *======================================================*/


// clang-format off

// stringT: require 1 argument, <allocator>
// arrayT: require 2 argument, <item_type, allocator>
// objectT: require 3 argument, <key_type, value_type, allocator>
// AllocT: require 1 argument, <data_type>
template <
        template <typename> typename stringT, // ofjaas
        template <typename, typename...> typename arrayT,
        template <typename, typename, typename...> typename objectT,
        template <typename> typename AllocT
>
class basic_json {
        // MARK: P1.0: 内部类型和私有成员
        // inside container,type and private members
public:
        // [DEV]: remove in release
        // using string_t = std::string;
        // using array_t = std::vector<basic_json>;
        // using object_t = std::map<string_t, basic_json>;
        using string_t = stringT<AllocT<char>>;
        using array_t = arrayT<basic_json, AllocT<basic_json>>;
        using object_t = objectT<string_t, basic_json, AllocT<std::pair<const string_t, basic_json>>>;
        enum class types {
                Integer,
                Unsign,
                Float,
                Boolean,
                Null,
                String,
                Array,
                Object
        };
        // clang-format on

private:
        // 利用char固定1字节的特性, 申请一块足够大的栈内存来手动管理
        // 这属于高危操作, 我们应知道自己在做什么
        alignas(std::max_align_t) char mem_block[tools::max_sizeof<string_t, array_t, object_t>()];
        types Type;

        void free() noexcept {
                // literal type, quickly skip
                if (Type < types::String) {
                        return;
                }
                // RAII类型需要析构
                switch (Type) {
                case types::String:
                        tools::mut_memcast<string_t>(mem_block).~string_t();
                        break;
                case types::Array:
                        tools::mut_memcast<array_t>(mem_block).~array_t();
                        break;
                case types::Object:
                        tools::mut_memcast<object_t>(mem_block).~object_t();
                        break;
                default:
                        break;
                }
        }


        // MARK: P1.1 构造函数和析构函数
        // constructor and destructor
        // (a constructor that passed parameter std::ifstream is at P1.7)
public:
        basic_json() { Type = types::Null; }
        // Integer match this function
        template <typename T>
                requires(tools::IntegerOnly<T>)
        basic_json(T i_v) noexcept {
                if (i_v > tools::LL_MAX) {
                        new (mem_block) unsigned long long(i_v);
                        Type = types::Unsign;
                } else {
                        new (mem_block) long long(i_v);
                        Type = types::Integer;
                }
        }

        // Floating match this function
        template <typename T>
                requires(tools::FloatOnly<T>)
        basic_json(T f_v) noexcept {
                new (mem_block) double(f_v);
                Type = types::Float;
        }

        // bool only
        basic_json(bool b_v) noexcept {
                new (mem_block) bool(b_v);
                Type = types::Boolean;
        }

        // null
        basic_json(decltype(nullptr)) noexcept { Type = types::Null; }

        // string
        basic_json(const string_t& str_v) noexcept {
                new (mem_block) string_t(str_v);
                Type = types::String;
        }
        basic_json(string_t&& str_v) noexcept {
                new (mem_block) string_t((string_t&&)str_v);
                Type = types::String;
        }
        basic_json(const char* constr) noexcept {
                new (mem_block) string_t(constr);
                Type = types::String;
        }

        // list
        basic_json(const array_t& arr_v) noexcept {
                new (mem_block) array_t(arr_v);
                Type = types::Array;
        }
        basic_json(array_t&& arr_v) noexcept {
                new (mem_block) array_t((array_t&&)arr_v);
                Type = types::Array;
        }

        template <typename = std::enable_if<true>>
        basic_json(std::initializer_list<basic_json> initial_list) noexcept {
                new (mem_block) array_t(initial_list);
                Type = types::Array;
        }

        // 嵌套对象
        basic_json(const object_t& obj_v) noexcept {
                new (mem_block) object_t(obj_v);
                Type = types::Object;
        }
        basic_json(object_t&& obj_v) noexcept {
                new (mem_block) object_t((object_t&&)obj_v);
                Type = types::Object;
        }
        // another initializer_list constructor, constructor of 'json' is ambiguous, property using this, else use
        // array initializer
        basic_json(std::initializer_list<std::pair<const string_t, basic_json>> initial_list) noexcept {
                new (mem_block) object_t(initial_list);  // aka std::map<string, json>
                Type = types::Object;
        }

        // copy constructor
        basic_json(const basic_json& other) noexcept {
                if (other.Type < types::Boolean) {
                        memcpy(mem_block, other.mem_block, 8);
                } else {
                        switch (other.Type) {
                        case types::Boolean:
                                new (mem_block) bool(tools::memcast<bool>(other.mem_block));
                                break;
                        case types::String:
                                new (mem_block) string_t(tools::memcast<string_t>(other.mem_block));
                                break;
                        case types::Array:
                                new (mem_block) array_t(tools::memcast<array_t>(other.mem_block));
                                break;
                        case types::Object:
                                new (mem_block) object_t(tools::memcast<object_t>(other.mem_block));
                                break;
                        default:
                                break;
                        }
                }
                Type = other.Type;
        }
        // move constructor
        basic_json(basic_json&& other) noexcept {
                switch (other.Type) {
                case types::String:
                        new (mem_block) string_t((string_t&&)tools::mut_memcast<string_t>(other.mem_block));
                        break;
                case types::Array:
                        new (mem_block) array_t((array_t&&)tools::mut_memcast<array_t>(other.mem_block));
                        break;
                case types::Object:
                        new (mem_block) object_t((object_t&&)tools::mut_memcast<object_t>(other.mem_block));
                        break;
                default:
                        memcpy(mem_block, other.mem_block, sizeof(mem_block));
                        break;
                }
                Type = other.Type;
                other.Type = types::Null;
        }
        ~basic_json() noexcept { free(); }


        // MARK: P1.2 等于号重载
        // assignment
        // (a assignment function that passed parameter std::ifstream is at P1.7)
public:
        // Integer match this function
        template <typename T>
                requires(tools::IntegerOnly<T>)
        auto operator=(T i_v) noexcept -> basic_json& {
                free();
                if (i_v > tools::LL_MAX) {
                        new (mem_block) unsigned long long(i_v);
                        Type = types::Unsign;
                } else {
                        new (mem_block) long long(i_v);
                        Type = types::Integer;
                }
                return *this;
        }

        // Floating match this function
        template <typename T>
                requires(tools::FloatOnly<T>)
        auto operator=(T f_v) noexcept -> basic_json& {
                free();
                new (mem_block) double(f_v);
                Type = types::Float;
                return *this;
        }

        // bool only
        auto operator=(bool b_v) noexcept -> basic_json& {
                free();
                new (mem_block) bool(b_v);
                Type = types::Boolean;
                return *this;
        }

        // null
        auto operator=(decltype(nullptr)) noexcept -> basic_json& {
                free();
                Type = types::Null;
                return *this;
        }

        // string
        auto operator=(const string_t& str_v) noexcept -> basic_json& {
                free();
                new (mem_block) string_t(str_v);
                Type = types::String;
                return *this;
        }
        auto operator=(string_t&& str_v) noexcept -> basic_json& {
                free();
                new (mem_block) string_t((string_t&&)str_v);
                Type = types::String;
                return *this;
        }
        auto operator=(const char* constr) noexcept -> basic_json& {
                free();
                new (mem_block) string_t{constr};
                Type = types::String;
                return *this;
        }

        // list
        auto operator=(const array_t& arr_v) noexcept -> basic_json& {
                free();
                new (mem_block) array_t(arr_v);
                Type = types::Array;
                return *this;
        }
        auto operator=(array_t&& arr_v) noexcept -> basic_json& {
                free();
                new (mem_block) array_t((array_t&&)arr_v);
                Type = types::Array;
                return *this;
        }

        template <typename = std::enable_if<true>>
        auto operator=(std::initializer_list<basic_json> initial_list) noexcept -> basic_json& {
                free();
                new (mem_block) array_t(initial_list);
                Type = types::Array;
                return *this;
        }

        // dict
        auto operator=(const object_t& obj_v) noexcept -> basic_json& {
                free();
                new (mem_block) object_t(obj_v);
                Type = types::Object;
                return *this;
        }
        auto operator=(object_t&& obj_v) noexcept -> basic_json& {
                free();
                new (mem_block) object_t((object_t&&)obj_v);
                Type = types::Object;
                return *this;
        }
        auto operator=(std::initializer_list<std::pair<const string_t, basic_json>> initial_list) noexcept
                -> basic_json& {
                free();
                new (mem_block) object_t(initial_list);
                Type = types::Object;
                return *this;
        }

        // copy
        auto operator=(const basic_json& other) noexcept -> basic_json& {
                free();
                if (other.Type < types::Boolean) {
                        memcpy(mem_block, other.mem_block, 8);
                } else {
                        switch (other.Type) {
                        case types::Boolean:
                                memcpy(mem_block, other.mem_block, sizeof(bool));
                                break;
                        case types::String:
                                new (mem_block) string_t(tools::memcast<string_t>(other.mem_block));
                                break;
                        case types::Array:
                                new (mem_block) array_t(tools::memcast<array_t>(other.mem_block));
                                break;
                        case types::Object:
                                new (mem_block) object_t(tools::memcast<object_t>(other.mem_block));
                                break;
                        default:
                                break;
                        }
                }
                Type = other.Type;
                return *this;
        }
        // move
        auto operator=(basic_json&& other) noexcept -> basic_json& {
                free();
                switch (other.Type) {
                case types::String:
                        new (mem_block) string_t((string_t&&)tools::mut_memcast<string_t>(other.mem_block));
                        break;
                case types::Array:
                        new (mem_block) array_t((array_t&&)tools::mut_memcast<array_t>(other.mem_block));
                        break;
                case types::Object:
                        new (mem_block) object_t((object_t&&)tools::mut_memcast<object_t>(other.mem_block));
                        break;
                default:
                        memcpy(mem_block, other.mem_block, sizeof(mem_block));
                        break;
                }
                Type = other.Type;
                other.Type = types::Null;
                return *this;
        }


        // MARK: P1.3 显式/隐式 转换
        // convert
public:
        template <typename T>
        operator T*() const noexcept {
                if (Type != types::Null) {
                        tools::bad_conversion_panic("null type");
                }
                return nullptr;
        }

        // not const

        template <typename T>
                requires(tools::IntegerOnly<T> || tools::FloatOnly<T>)
        operator T() noexcept {
                if (Type > types::Float) {
                        tools::bad_conversion_panic("number");
                }
                switch (Type) {
                case types::Integer:
                        return (T)tools::mut_memcast<long long>(mem_block);
                case types::Float:
                        return (T)tools::mut_memcast<double>(mem_block);
                default:
                        return (T)tools::mut_memcast<unsigned long long>(mem_block);
                }
        }

        operator bool() noexcept {
                if (Type != types::Boolean) {
                        tools::bad_conversion_panic("boolean");
                }
                return tools::mut_memcast<bool>(mem_block);
        }

        operator string_t() noexcept {
                if (Type != types::String) {
                        tools::bad_conversion_panic("string");
                }
                return tools::mut_memcast<string_t>(mem_block);
        }

        operator array_t() noexcept {
                if (Type != types::Array) {
                        tools::bad_conversion_panic("array");
                }
                return tools::mut_memcast<array_t>(mem_block);
        }

        operator object_t() noexcept {
                if (Type != types::Object) {
                        tools::bad_conversion_panic("object");
                }
                return tools::mut_memcast<object_t>(mem_block);
        }

        // const

        template <typename T>
                requires(tools::IntegerOnly<T> || tools::FloatOnly<T>)
        operator const T() const noexcept {
                if (Type > types::Float) {
                        tools::bad_conversion_panic("number");
                }
                switch (Type) {
                case types::Integer:
                        return (T)tools::memcast<long long>(mem_block);
                case types::Float:
                        return (T)tools::memcast<double>(mem_block);
                default:
                        return (T)tools::memcast<unsigned long long>(mem_block);
                }
        }

        operator const bool() const noexcept {
                if (Type != types::Boolean) {
                        tools::bad_conversion_panic("boolean");
                }
                return tools::memcast<bool>(mem_block);
        }

        operator const string_t() const noexcept {
                if (Type != types::String) {
                        tools::bad_conversion_panic("string");
                }
                return tools::memcast<string_t>(mem_block);
        }

        operator const array_t() const noexcept {
                if (Type != types::Array) {
                        tools::bad_conversion_panic("array");
                }
                return tools::memcast<array_t>(mem_block);
        }

        operator const object_t() const noexcept {
                if (Type != types::Object) {
                        tools::bad_conversion_panic("object");
                }
                return tools::memcast<object_t>(mem_block);
        }


        // MARK: P1.4 获取内部对象的函数
        // get
public:
        auto type() const noexcept -> types { return Type; }

        // not const

        auto number_integer() noexcept -> long long& {
                if (Type != types::Integer) {
                        tools::bad_getting_panic("integer number");
                }
                return tools::mut_memcast<long long>(mem_block);
        }

        auto number_unsign() noexcept -> unsigned long long& {
                if (Type != types::Integer) {
                        tools::bad_getting_panic("unsigned number");
                }
                return tools::mut_memcast<unsigned long long>(mem_block);
        }

        auto number_floating() noexcept -> double& {
                if (Type != types::Float) {
                        tools::bad_getting_panic("floating-point number");
                }
                return tools::mut_memcast<double>(mem_block);
        }

        auto boolean() noexcept -> bool& {
                if (Type != types::Boolean) {
                        tools::bad_getting_panic("boolean");
                }
                return tools::mut_memcast<bool>(mem_block);
        }

        auto string() noexcept -> string_t& {
                if (Type != types::String) {
                        tools::bad_getting_panic("string");
                }
                return tools::mut_memcast<string_t>(mem_block);
        }

        auto array() noexcept -> array_t& {
                if (Type != types::Array) {
                        tools::bad_getting_panic("array");
                }
                return tools::mut_memcast<array_t>(mem_block);
        }

        auto object() noexcept -> object_t& {
                if (Type != types::Object) {
                        tools::bad_getting_panic("object");
                }
                return tools::mut_memcast<object_t>(mem_block);
        }

        // const

        auto number_integer() const noexcept -> const long long& {
                if (Type != types::Integer) {
                        tools::bad_getting_panic("integer number");
                }
                // tools::memcast & memcast or use const_cast<> at here all cause errors
                // so use C-style unsafe conversion
                return tools::memcast<long long>(mem_block);
        }

        auto number_unsign() const noexcept -> const unsigned long long& {
                if (Type != types::Integer) {
                        tools::bad_getting_panic("unsigned number");
                }
                return tools::memcast<unsigned long long>(mem_block);
        }

        auto number_floating() const noexcept -> const double& {
                if (Type != types::Float) {
                        tools::bad_getting_panic("floating-point number");
                }
                return tools::memcast<double>(mem_block);
        }

        auto boolean() const noexcept -> const bool& {
                if (Type != types::Boolean) {
                        tools::bad_getting_panic("boolean");
                }
                return tools::memcast<bool>(mem_block);
        }

        auto string() const noexcept -> const string_t& {
                if (Type != types::String) {
                        tools::bad_getting_panic("string");
                }
                return tools::memcast<string_t>(mem_block);
        }

        auto array() const noexcept -> const array_t& {
                if (Type != types::Array) {
                        tools::bad_getting_panic("array");
                }
                return tools::memcast<array_t>(mem_block);
        }

        auto object() const noexcept -> const object_t& {
                if (Type != types::Object) {
                        tools::bad_getting_panic("object");
                }
                return tools::memcast<object_t>(mem_block);
        }


        // MARK: P1.5 容器功能函数
        // other operator overload
public:
        auto operator[](unsigned long idx) noexcept -> basic_json& {
                if (Type == types::Null) {
                        new (mem_block) array_t(idx + 1);
                        Type = types::Array;
                } else if (Type != types::Array) {
                        tools::bad_using_panic("operator[](size_t)", "array");
                }
                // undefined behavior (depend on the array implement)
                return (tools::mut_memcast<array_t>(mem_block))[idx];
        }

        auto operator[](unsigned long idx) const noexcept -> const basic_json& {
                if (Type != types::Array) {
                        tools::bad_using_panic("operator[](size_t)", "array");
                }
                // undefined behavior (depend on the array implement)
                return (tools::memcast<array_t>(mem_block))[idx];
        }

        auto operator[](const string_t& key) noexcept -> basic_json& {
                if (Type == types::Null) {
                        new (mem_block) object_t();
                        Type = types::Object;
                } else if (Type != types::Object) {
                        tools::bad_using_panic("operator[](string key)", "object");
                }
                // undefined behavior (depend on the dict implement)
                return (tools::mut_memcast<object_t>(mem_block))[key];
        }

        auto operator[](const char (&key)[]) noexcept -> basic_json& {
                if (Type == types::Null) {
                        new (mem_block) object_t();
                        Type = types::Object;
                } else if (Type != types::Object) {
                        tools::bad_using_panic("operator[](string key)", "object");
                }
                // undefined behavior (depend on the dict implement)
                return (tools::mut_memcast<object_t>(mem_block))[key];
        }

        auto operator+=(std::pair<string_t, basic_json> insert_pair) noexcept -> basic_json& {
                if (Type != types::Object) {
                        tools::bad_using_panic("operator+=(pair)", "object");
                }
                tools::mut_memcast<object_t>(mem_block).insert(std::move(insert_pair));
                return *this;
        }

        template <typename = std::enable_if<true>>
        auto operator+=(basic_json other) noexcept {
                if (Type != types::Array) {
                        tools::bad_using_panic("operator+=(json item)", "array");
                }
                tools::mut_memcast<array_t>(mem_block).push_back(std::move(other));
        }

        bool operator==(const basic_json& other) const noexcept {
                if (this->Type != other.Type)
                        return false;
                switch (Type) {
                case types::Integer:
                        return tools::memcast<long long>(mem_block) == tools::memcast<long long>(other.mem_block);
                case types::Unsign:
                        return tools::memcast<unsigned long long>(mem_block) ==
                               tools::memcast<unsigned long long>(other.mem_block);
                case types::Float:
                        return tools::memcast<double>(mem_block) == tools::memcast<double>(other.mem_block);
                case types::Boolean:
                        return tools::memcast<bool>(mem_block) == tools::memcast<bool>(other.mem_block);
                case types::Null:
                        return true;
                case types::String:
                        return tools::memcast<string_t>(mem_block) == tools::memcast<string_t>(other.mem_block);
                case types::Array:
                        return tools::memcast<array_t>(mem_block) == tools::memcast<array_t>(other.mem_block);
                case types::Object:
                        return tools::memcast<object_t>(mem_block) == tools::memcast<object_t>(other.mem_block);
                }
        }


        // MARK: P1.6 json的序列化
        // for dumping function ↓↓↓
private:
        static void string_decode_dump(const std::string& in_str, std::string& to_str) {
                to_str += '"';
                for (const auto& ch : in_str) {
                        if (ch >= 0 && ch < 32) {
                                to_str += tools::escape_table[(unsigned)ch];
                                continue;
                        }
                        switch (ch) {
                        case '"':
                                to_str += R"(\")";
                                break;
                        case '\\':
                                to_str += R"(\\)";
                                break;
                        // this can be remove
                        // case '/':
                        //         to_str += R"(\/)";
                        //         break;
                        case 0x7F:
                                to_str += R"(\u007F)";
                                break;
                        default:
                                to_str += ch;
                        }
                }
                to_str += '"';
        }

        // dumping
private:
        // dump to string
        void dump(std::string& str, int indent) const noexcept {
                switch (Type) {
                case types::Integer:
                        str += tools::to_str(tools::memcast<long long>(mem_block));
                        return;
                case types::Unsign:
                        str += tools::to_str(tools::memcast<unsigned long long>(mem_block));
                        return;
                case types::Float:
                        str += tools::to_str(tools::memcast<double>(mem_block));
                        return;
                case types::Boolean:
                        str += tools::memcast<bool>(mem_block) ? "true" : "false";
                        return;
                case types::Null:
                        str += "null";
                        return;
                case types::String:
                        string_decode_dump(tools::memcast<string_t>(mem_block), str);
                        return;
                        // clang-format off
                case types::Array: {
                        bool line_break = false;
                        auto &this_array = tools::memcast<array_t>(mem_block);
                        if(this_array.empty()){
                                str+="[]";
                                return;
                        }
                        std::vector<std::string> fields;
                        for (auto &item : this_array) {
                                if (!line_break && item.Type > types::String) {
                                        line_break = true;
                                }
                                fields.push_back({});
                                item.dump(fields.back(), indent + 1);
                                fields.back() += ", ";
                        }
                        str += '[';
                        if (!line_break) {
                                for (auto &item_str : fields) {
                                        str += item_str;
                                }
                                str.resize(str.length() - 2);
                        } else {
                                for (auto &item_str : fields) {
                                        str += '\n';
                                        str += tools::indent_table[indent + 1];
                                        str += item_str;
                                }
                                str.resize(str.length() - 2);
                                str += '\n';
                                str += tools::indent_table[indent];
                        }
                        str += ']';
                        break;
                }
                case types::Object: {
                        const object_t &obj = tools::memcast<object_t>(mem_block);
                        if(obj.empty()) {
                                str += "{}";
                                return;
                        }
                        str += "{\n";
                        for (auto &[key, val] : obj) {
                                str += tools::indent_table[indent + 1];
                                string_decode_dump(key, str);
                                str += ": ";
                                val.dump(str, indent + 1);
                                str += ", \n";
                        }
                        str.resize(str.length() - 3);
                        str += '\n';
                        str += tools::indent_table[indent];
                        str += '}';
                }
                // clang-format on
                default:
                        break;
                }
                return;
        }  // function `dump`

        // no pretty dumping (no indent and line break)
        void fast_dump(std::string& str) const noexcept {
                switch (Type) {
                case types::Integer:
                        str += tools::to_str(tools::memcast<long long>(mem_block));
                        return;
                case types::Unsign:
                        str += tools::to_str(tools::memcast<unsigned long long>(mem_block));
                        return;
                case types::Float:
                        str += tools::to_str(tools::memcast<double>(mem_block));
                        return;
                case types::Boolean:
                        str += tools::memcast<bool>(mem_block) ? "true" : "false";
                        return;
                case types::Null:
                        str += "null";
                        return;
                case types::String:
                        string_decode_dump(tools::memcast<string_t>(mem_block), str);
                        return;
                        // clang-format off
                case types::Array: {
                        auto &this_array = tools::memcast<array_t>(mem_block);
                        str += '[';
                        for (auto &item : this_array) {
                                item.fast_dump(str);
                                str += ", ";
                        }
                        if(!this_array.empty())
                                str.resize(str.length() - 2);
                        str += ']';
                        break;
                }
                case types::Object:{
                        const object_t &obj = tools::memcast<object_t>(mem_block);
                        str += '{';
                        for (auto &[key, val] : obj) {
                                string_decode_dump(key, str);
                                str += ": ";
                                val.fast_dump(str);
                                str += ", ";
                        }
                        if(!obj.empty())
                                str.resize(str.length() - 2);
                        str += '}';
                }
                // clang-format on
                default:
                        break;
                }
                return;
        }  // function `fast_dump`

public:
        auto dump() const noexcept -> std::string {
                std::string ret;
                dump(ret, 0);
                return static_cast<std::string>(ret);
        }

        auto fast_dump() const noexcept -> std::string {
                std::string ret;
                fast_dump(ret);
                return static_cast<std::string>(ret);
        }



        /*=======================================================
         * MARK: P2: 反序列化解析逻辑
         *======================================================*/


private:
        // clang-format off
        using err_string_t = std::string;
        // MARK: P2.0 调度函数
        // private parsing function, switch and call some parse_xxx function, be call by public parse
        // and parse_xxx return <json object : error massage>
        static auto parsing_func(const std::string &str, size_t &idx) -> std::pair<basic_json, err_string_t> {
                basic_json result;
                char ch = str[idx];
                switch (ch) {
                // number
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case '+':
                case '-': {
                        auto ret = parse_number(str, idx);
                        switch (ret.template get<1>()) {
                        case types::Null:
                                return {nullptr, ret.template get<2>()};
                        case types::Integer:
                                return {ret.template get<0>(), ""};
                        case types::Unsign:
                                result = tools::mut_memcast<unsigned long long>(&ret.template get<0>());
                                return {std::move(result), ""};
                        case types::Float:
                                result = tools::mut_memcast<double>(&ret.template get<0>());
                                return {std::move(result), ""};
                        default:
                                tools::panic("development bug, something bad");  // [remove in release]
                        }
                        // 不会运行至此处
                }
                case '"': {
                        auto [string, err] = parse_string(str, idx);
                        if (!err.empty()) {
                                return {nullptr, std::move(err)};
                        }
                        return {std::move(string), ""};
                }
                case '[': {
                        auto [array, err] = parse_array(str, idx);
                        if (!err.empty()) {
                                return {nullptr, std::move(err)};
                        }
                        return {std::move(array), ""};
                }
                case '{': {
                        auto [object, err] = parse_object(str, idx);
                        if(!err.empty()) {
                                return {nullptr, std::move(err)};
                        }
                        return {std::move(object), ""};
                }
                case 'n': {
                        // is it 'null' ?
                        if(idx+3>=str.length() || str[idx+1]!='u' || str[idx+2]!='l' || str[idx+3]!='l'){
                                return {nullptr, "未预期的字符'n', 未能匹配到关键字'null', 错误"};
                        }
                        idx+=3;
                        return {nullptr, ""};
                }
                case 't': {
                        // is it 'true' ?
                        if(idx+3>=str.length() || str[idx+1]!='r' || str[idx+2]!='u' || str[idx+3]!='e'){
                                return {nullptr, "未预期的字符't', 未能匹配到关键字'true', 错误"};
                        }
                        idx+=3;
                        return {true, ""};
                }
                case 'f': {
                        // is it 'false' ?
                        if(idx+4>=str.length() || str[idx+1]!='a' || str[idx+2]!='l' || str[idx+3]!='s' || str[idx+4]!='e'){
                                return {nullptr, "未预期的字符'f', 未能匹配到关键字'false', 错误"};
                        }
                        idx+=4;
                        return {false, ""};
                }
                default:
                        return {nullptr, "未预期的字符, 语法错误, 非法的json格式"};
                }
        } // end function `parsing_func`

        // MARK: P2.1 字符串解析
        static auto parse_string(const std::string &str, size_t &idx) -> std::pair<string_t, err_string_t> {
                static const auto get_16base_ch_num = [](char ch) -> int8_t {
                        switch (ch) {
                        case '0':
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                        case '8':
                        case '9':
                                return ch-'0'+1; // actual value increased by 1
                        case 'A':
                        case 'B':
                        case 'C':
                        case 'D':
                        case 'E':
                        case 'F':
                                ch+=32;
                        case 'a':
                        case 'b':
                        case 'c':
                        case 'd':
                        case 'e':
                        case 'f':
                                return ch-'a'+10+1;
                        default:
                                return 0; // actual -1
                        }
                };
                bool is_low_zone = false;
                unsigned int point = 0;
                string_t ret;
                ++idx;  // we make sure that the first character is '"'
                size_t start = idx;
                bool is_view = false;
                for (; idx<str.length(); ++idx) {
                        char ch=str[idx];
                        switch (ch) {
                        case '"':
                                if(is_view) {
                                        ret = str.substr(start, idx-start);
                                }
                                return {std::move(ret), ""};
                        case '\n':
                                return {"", "'\"'未闭合, 找到行尾'\n', 非法的`string`"};
                        case '\\': {
                                if(!is_view) {
                                        ret = str.substr(start, idx-start);
                                        is_view = true;
                                }
                                auto i = idx+1; // declear a temp index variable, if failed, idx doesn't change, success, then idx = temp_idx
                                if(i>=str.length()){
                                        return {"", "'\\'后的EOF, 非法转义"};
                                }
                                switch(str[i]){
                                case '"':
                                        ret+='"';
                                        break;
                                case '\\':
                                        ret+='\\';
                                        break;
                                case '/':
                                        ret+='/';
                                        break;
                                case 'b':
                                        ret+='\b';
                                        break;
                                case 'f':
                                        ret+='\f';
                                        break;
                                case 'n':
                                        ret+='\n';
                                        break;
                                case 'r':
                                        ret+='\r';
                                        break;
                                case 't':
                                        ret+='\t';
                                        break;
                                case 'u': {
                                        ++i; // move to first bit (0~9 A~F a~f)
                                        if(i+3>=str.length()) {
                                                return {"", "非法的UTF-16转义, 遇到EOF"};
                                        }
                                        int8_t bit1=get_16base_ch_num(str[i]),
                                               bit2=get_16base_ch_num(str[i+1]),
                                               bit3=get_16base_ch_num(str[i+2]),
                                               bit4=get_16base_ch_num(str[i+3]);
                                        // 但凡有一个0, 全部为0
                                        if(bit1*bit2*bit3*bit4 == 0){
                                                return {"", "非法的UTF-16转义"};
                                        }
                                        unsigned int this_point = ((bit1-1)<<12) + ((bit2-1)<<8) + ((bit3-1)<<4) + (bit4-1);
                                        if (this_point>=0xD800 && this_point<=0xDBFF) { // high-half zone
                                                if(is_low_zone) {
                                                        return {"", "出现在低位的高代理, 非法的UTF-16转义"};
                                                }
                                                point = this_point;
                                                is_low_zone = true;
                                        } else if (this_point>=0xDC00 && this_point<=0xDFFF) { // low-half zone
                                                if(!is_low_zone) {
                                                        return {"", "出现在高位的低代理, 非法的UTF-16转义"};
                                                }
                                                point = ((point-0xD800) << 10 | (this_point-0xDC00)) + 0x10000;
                                                tools::u32ch_decode_dump_to_u8string(point, ret);
                                                is_low_zone = false;
                                        } else {
                                                tools::u32ch_decode_dump_to_u8string(this_point, ret);
                                        }
                                        i+=3;
                                        break;
                                }
                                case '\n':
                                        return {"", "'\\'转义换行符'\\n'是非标准的"};
                                default:
                                        return {"", "'\\'转义非法"};
                                } // end switch case of character after '\'
                                idx=i;
                                break;
                        }
                        default:
                                if(!is_view) {
                                        ret += ch;
                                }
                        }
                }
                return {"", "'\"'未闭合, 找到EOF, 非法的`string`"};
        } // end finction `parse_string`

        // MARK: P2.2 数字解析状态机
        static auto parse_number(const std::string &str, size_t &idx) -> tools::tuple<long long, types, err_string_t> {
                typedef unsigned long long ULL;
                typedef long long LL;
                bool is_negative = false, neg_e = false;
                constexpr int8_t _none{0}, _dot{1}, _e{2}, _dot_and_e{3};
                int8_t flag{_none};  // 0:none, 1:find_dot, 2:find_e, 3:find dot and e
                ULL int_part=0, dec_part=0, e_part=0;
                int dec_digits=0;
                char ch = str[idx];
                switch (ch) {
                case '0':
                        if (idx+1 >= str.length()) {
                                return {0, types::Integer, ""};
                        }
                        switch (str[idx+1]) {
                        case '.':
                                flag = _dot;
                                break;
                        case 'E':
                        case 'e':
                                flag = _e;
                                break;
                        default:
                                return {0, types::Integer, ""};
                        }
                        idx += 2;
                        break;
                case '+':
                        is_negative = true;  // 后续取反
                case '-':
                        if (idx+1 >= str.length()) {
                                return {0, types::Null, "孤立的+/-号, 发现EOF, 非法的`number`"};
                        }
                        ch = str[idx+1];
                        if (ch<'0' || ch>'9') {
                                return {0, types::Null, "孤立的+/-号, 非法的`number`"};
                        }
                        is_negative = !is_negative;  // true->false, false->true
                        ++idx;
                default:
                        break;
                }
                for (; idx < str.length(); ++idx) {
                        ch = str[idx];
                        switch (ch) {
                        case '0':
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                        case '8':
                        case '9':
                                switch (flag) {
                                case _none:
                                        int_part = int_part*10 + (ch-'0');
                                        break;
                                case _dot:
                                        dec_part = dec_part*10 + (ch-'0');
                                        ++dec_digits;
                                        break;
                                case _e:
                                case _dot_and_e:
                                        e_part = e_part*10 + (ch-'0');
                                default:
                                        break;
                                }
                                continue;
                        case '\n':
                                goto parse_string_to_number;
                                break;
                        case '.':
                                if (flag>_none) {
                                        return {0, types::Null, "多余的'.', 非法的`number`"};
                                }
                                flag = _dot;
                                break;
                        case 'E':
                        case 'e':
                                if (flag>_dot) {
                                        return {0, types::Null, "多余的'e', 非法的`number`"};
                                }
                                if (str[idx-1] == '.') {
                                        return {0, types::Null, "'.'后跟随的'e', 非法的number"};
                                }
                                flag += 2;  // none+2=e, dot+2=dot_and_e
                                break;
                        case '+':
                                neg_e = true;  // 之后取反
                        case '-':
                                if (str[idx-1] != 'e') {
                                        return {0, types::Null, "未预期的+/-号, 非法的`number`"};
                                }
                                neg_e = !neg_e;
                                break;
                        default:
                                // 这里可以直接做格式检查的, 但会让状态机的流程变乱, 多解析一次数字也没什么开销
                                goto parse_string_to_number;
                        }
                }
parse_string_to_number:
                --idx;  // idx need to stop at the last number_part character, 1.45e100
                        //                                                            ^
                // check
                switch (str[idx]) {
                case '.':
                        return {0, types::Null, "数字以'.'结尾, 非法的`number`"};
                case 'e':
                        return {0, types::Null, "数字以'e'结尾, 非法的`number`"};
                case '+':
                        return {0, types::Null, "数字以'+'结尾, 非法的`number`"};
                case '-':
                        return {0, types::Null, "数字以'-'结尾, 非法的`number`"};
                default:
                        break;
                }
                LL res_mem;
                if (flag != _none) {
                        double &res = tools::mut_memcast<double>(&res_mem);
                        res = (double)int_part;
                        switch (flag) {
                        case _dot:
                                res += (double)dec_part * tools::neg_pow_table[dec_digits];
                                break;
                        case _dot_and_e:
                                res += (double)dec_part * tools::neg_pow_table[dec_digits];
                        case _e:
                                res *= neg_e ?
                                  e_part < tools::neg_pow_table_len ?
                                    tools::neg_pow_table[e_part] :
                                    0.0
                                  : e_part < tools::pow_table_len ?
                                    tools::pow_table[e_part] :
                                    INFINITY;
                        default:
                                break;
                        }
                        if (is_negative) {
                                res = -res;
                        }
                        return {res_mem, types::Float, ""};
                } else {
                        if (int_part > tools::LL_MAX) {
                                if (is_negative) {
                                        double &res = tools::mut_memcast<double>(&res_mem);
                                        res = -(double)int_part;
                                        return {res_mem, types::Float, ""};
                                } else {
                                        ULL &res = tools::mut_memcast<ULL>(&res_mem);
                                        res = int_part;
                                        return {res_mem, types::Unsign, ""};
                                }
                        } else {
                                LL &res = tools::mut_memcast<LL>(&res_mem);
                                res = (LL)int_part;
                                return {res_mem, types::Integer, ""};
                        }
                }
        } // end function `parse_number`

        // MARK: P2.3 数组解析
        static auto parse_array(const std::string &str, size_t &idx) -> std::pair<array_t, err_string_t> {  // [completed]
                ++idx;  // make sure that the first character is '['
                array_t result;
                bool need_comma=false;
                for (; idx<str.length(); ++idx) {
                        char ch = str[idx];
                        switch (ch) {
                        case ' ':
                        case '\t':
                        case '\n':
                                continue;
                        case ',':
                                if (need_comma) {
                                        need_comma=false;
                                } else {
                                        return {{}, "预期为'值', 发现冗余的',', 非法的`array`"};
                                }
                                break;
                        case ']':
                                if (!need_comma && result.size()!=0) {
                                        return {{}, "尾随',', 非法的`array`"};
                                }
                                return {std::move(result), {}};
                        default:  // 任何其他字符的情况
                                if (need_comma) {
                                        return {{}, "发现未预期的字符, 预期为',', 非法的`array`"};
                                }
                                auto [val, err] = parsing_func(str, idx);
                                if (!err.empty()) {
                                        return {{}, std::move(err)};
                                }
                                result.push_back(std::move(val));
                                need_comma = true;
                        }
                }
                return {{}, "未闭合的']', 发现EOF, 非法的`array`"};
        } // end function `parse_array`

        // MARK: P2.4 对象解析
        static auto parse_object(const std::string &str, size_t &idx) -> std::pair<object_t, err_string_t> {
                ++idx;  // first character must be '{'
                object_t result;
                constexpr int8_t part_key{0}, part_colon{1}, part_val{2}, comma_or_end{3};
                int8_t at_part {part_key};  // [0]key, [1]colon, [2]value, [3]comma or '}' ending
                std::pair<string_t, basic_json> temp;
                for (; idx<str.length(); ++idx) {
                        char ch = str[idx];
                        switch (ch) {
                        case ' ':
                        case '\t':
                        case '\n':
                                continue;
                        case '}':
                                if (at_part!=comma_or_end) {
                                        if (at_part==part_key && result.size()==0) {
                                                return {std::move(result), ""};  // empty json object, like "{}"
                                        } else {
                                                return {{}, "结构不完整, 未预期的'}', 非法的`object`"};
                                        }
                                }
                                return {std::move(result), ""};
                        }
                        switch (at_part) {
                        case part_key: {
                                if (ch!='"') {
                                        return {{}, "预期为'键', 发现异常字符, 非法的`object`"};
                                }
                                auto [key, err] = parse_string(str, idx);
                                if (!err.empty()) {
                                        return {{}, std::move(err)};
                                }
                                temp.first = std::move(key);
                                at_part = part_colon;
                                break;
                        }
                        case part_colon:
                                if (ch!=':') {
                                        return {{}, "预期为':', 发现异常字符, 非法的`object`"};
                                }
                                at_part = part_val;
                                break;
                        case part_val: {
                                auto [val, err] = parsing_func(str, idx);
                                if (!err.empty()) {
                                        return {{}, std::move(err)};
                                }
                                temp.second = std::move(val);
                                result.insert(std::move(temp));
                                at_part = comma_or_end;
                                break;
                        }
                        case comma_or_end:
                                if (ch!=',') {
                                        return {{}, "预期为',', 发现异常字符, 非法的`object`"};
                                }
                                at_part = part_key;
                                break;
                        }
                }  // end loop for
                return {{}, "结构不完整, 发现EOF, 非法的`object`"};
        } // end function `parse_object`

// end parser
        // clang-format on


        // MARK: P2.5: 给外部调用的解析相关操作
        // parsing API
public:
        // parse json
        static auto parse(const std::string& json_text) -> std::tuple<basic_json, err_string_t, size_t> {
                size_t idx = 0;
                // skip the space at the start
                for (; idx < json_text.length(); ++idx) {
                        switch (json_text[idx]) {
                        case ' ':
                        case '\t':
                        case '\n':
                                continue;
                        }
                        break;
                }
                if (idx == json_text.length()) {
                        return {nullptr, "", idx};
                }
                auto [json_obj, err] = parsing_func(json_text, idx);
                if (!err.empty()) {
                        return {nullptr, std::move(err), idx};
                } else {
                        ++idx;  // at a new character we don't parsed
                        // skip the space at the end
                        for (; idx < json_text.length(); ++idx) {
                                switch (json_text[idx]) {
                                case ' ':
                                case '\t':
                                case '\n':
                                        continue;
                                }
                                break;
                        }
                        // not at the end, some redundant characters
                        if (idx < json_text.length()) {
                                return {nullptr, "完整结构后的多余字符, 非法的json", idx};
                        }
                        return {std::move(json_obj), "", idx};
                }
        }

        static auto parse(std::ifstream file) -> std::tuple<basic_json, err_string_t, size_t> {
                file.seekg(0, std::ios::end);
                size_t json_len = (size_t)file.tellg();
                file.seekg(0, std::ios::beg);
                std::string str;
                str.resize(json_len);
                file.read(str.data(), json_len);
                return parse(str);
        }

        basic_json(std::ifstream file) noexcept {
                auto [temp, err, idx] = parse(std::move(file));
                if (!err.empty()) {
                        Type = types::Null;
                        return;
                }
                *this = std::move(temp);
        }

        auto operator=(std::ifstream file) noexcept -> basic_json& {
                auto [temp, err, idx] = parse(std::move(file));
                if (!err.empty()) {
                        Type = types::Null;
                } else {
                        *this = std::move(temp);
                }
                return *this;
        }

        auto parse_from(const std::string& str) noexcept -> std::pair<err_string_t, size_t> {
                auto [temp, err, idx] = parse(str);
                if (!err.empty()) {
                        return {std::move(err), idx};
                }
                *this = std::move(temp);
                return {"", idx};
        }

        auto parse_from(std::ifstream file) noexcept -> std::pair<err_string_t, size_t> {
                auto [temp, err, idx] = parse(std::move(file));
                if (!err.empty()) {
                        return {std::move(err), idx};
                }
                *this = std::move(temp);
                return {"", idx};
        }


};  // class basic_json



/*=======================================================
 * MARK: P3: sundries
 *======================================================*/


// for only-json basic_json template arguments contract
// wrapped some STL container

using json = basic_json<tools::std_string_wrapping, std::vector, tools::std_map_wrapping, std::allocator>;

template <template <typename> typename Alc>
using json_with_pool = basic_json<tools::std_string_wrapping, std::vector, tools::std_map_wrapping, Alc>;


// syntactic sugar
inline auto operator""_json(const char* constr, size_t) -> json {
        auto [temp, err, _] = json::parse(constr);
        if (!err.empty()) {
                return nullptr;
        }
        return std::move(temp);
}


// 错误检查器, 通过给定错误点下标idx, 观察一定范围内的内容以方便查错, 与json无关
// parsing json may failed, and it return a massage and index
// this function can check the content near the failed index
inline auto check_failed_part(const std::string& json_text_str, size_t idx, short view_offset = 20)
        -> std::string {
        auto start_idx = idx >= (size_t)view_offset ? idx - view_offset : 0;
        auto end_idx = idx + view_offset < json_text_str.length() ? idx + view_offset : json_text_str.length() - 1;
        return json_text_str.substr(start_idx, end_idx - start_idx + 1);
}


template <class T>
class pool {
private:
        // 全局静态内存池
        inline static std::pmr::monotonic_buffer_resource s_pool;

public:
        using value_type = T;

        pool() = default;
        pool(const pool&) noexcept = default;

        template <class U>
        pool(const pool<U>&) noexcept {}

        T* allocate(std::size_t n) { return static_cast<T*>(s_pool.allocate(n * sizeof(T))); }

        void deallocate(T* p, std::size_t n) noexcept { s_pool.deallocate(p, n * sizeof(T)); }

        template <class U>
        bool operator==(const pool<U>&) const noexcept {
                return true;
        }

        template <class U>
        bool operator!=(const pool<U>&) const noexcept {
                return false;
        }
};


}  // namespace hai