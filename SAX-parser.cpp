/****************************************************************************************
                                                             _____________________
  https://github.com/haiyyyyh                               |     by haiyyyyh     |
     only-json SAX parser                                   | version v0.7.0 DEV  |
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

*****************************************************************************************/

#include <cmath>    // macro INFINITY
#include <fstream>  // ifstream


namespace hai {


namespace {

namespace tools {

constexpr long long LL_MAX = 0x7fffffffffffffffLL;
constexpr unsigned long long ULL_MAX = 0xffffffffffffffffULL;

template <typename T>
auto mut_memcast(void* mem) -> T& {
        return *reinterpret_cast<T*>(mem);
}

template <typename T>
auto memcast(const void* mem) -> const T& {
        return *reinterpret_cast<const T*>(mem);
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


static const bool str_skip_ch_table[256] = {
        0, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 0, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 0, 1, 1, 1, 1, 0,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 0, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1
};

static const bool skip_space_ch_table[256] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0
};


template <typename...Args>
struct tuple;

template <typename T>
struct tuple<T> {
        T _1;
};

template <typename T1, typename T2>
struct tuple<T1, T2> {
        T1 _1; T2 _2;
};

template <typename T1, typename T2, typename T3>
struct tuple<T1, T2, T3> {
        T1 _1; T2 _2; T3 _3;
};

template <typename T1, typename T2, typename T3, typename T4>
struct tuple<T1, T2, T3, T4> {
        T1 _1; T2 _2; T3 _3; T4 _4;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5>
struct tuple<T1, T2, T3, T4, T5> {
        T1 _1; T2 _2; T3 _3; T4 _4; T5 _5;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
struct tuple<T1, T2, T3, T4, T5, T6> {
        T1 _1; T2 _2; T3 _3; T4 _4; T5 _5; T6 _6;
};

// clang-format on


#ifndef likely
#define likely(x) __builtin_expect((x), 1)
#endif
#ifndef unlikely
#define unlikely(x) __builtin_expect((x), 0)
#endif

#define likely_if(x) if (__builtin_expect((x), 1))
#define unlikely_if(x) if (__builtin_expect((x), 0))

// 强内联
#define INLINE __attribute__((always_inline)) inline
// #define INLINE
// 热函数
#define HOT __attribute__((hot))


template <typename any_handler>
INLINE void u32ch_decode_push_to_handler(unsigned int ch, any_handler& handler) {
        if (ch <= 0x7F) {
                // 1字节：0xxxxxxx
                handler.push_char((char)ch);
        } else if (ch <= 0x7FF) {
                // 2字节：110xxxxx 10xxxxxx
                handler.push_char((char)(0xC0 | ((ch >> 6) & 0x1F)));  // 第1字节：110 + 高5位
                handler.push_char((char)(0x80 | (ch & 0x3F)));         // 第2字节：10 + 低6位
        } else if (ch <= 0xFFFF) {
                // 3字节：1110xxxx 10xxxxxx 10xxxxxx
                handler.push_char((char)(0xE0 | ((ch >> 12) & 0x0F)));  // 第1字节：1110 + 高4位
                handler.push_char((char)(0x80 | ((ch >> 6) & 0x3F)));   // 第2字节：10 + 中间6位
                handler.push_char((char)(0x80 | (ch & 0x3F)));          // 第3字节：10 + 低6位
        } else {
                // 4字节：11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
                handler.push_char((char)(0xF0 | ((ch >> 18) & 0x07)));  // 第1字节：11110 + 高3位
                handler.push_char((char)(0x80 | ((ch >> 12) & 0x3F)));  // 第2字节：10 + 次高6位
                handler.push_char((char)(0x80 | ((ch >> 6) & 0x3F)));   // 第3字节：10 + 中间6位
                handler.push_char((char)(0x80 | (ch & 0x3F)));          // 第4字节：10 + 低6位
        }
}

}  // namespace tools


typedef long long Int64;
typedef unsigned long long Uint64;
// compiler extention
typedef __int128_t Int128;
typedef __uint128_t Uint128;


}  // namespace



/* #define CHECKER_GENERATION(name)\
template <typename HandleT, typename...MethodArgs>\
class check_has_##name{\
        template <typename T>\
        static auto test(int) -> decltype(\
                std::is_same<void, decltype(std::declval<T>().name(std::declval<MethodArgs>()...))>()\
        );\
        template <typename U>\
        static auto test(...) -> std::false_type;\
public:\
        static constexpr bool value = decltype(test<HandleT>(0))::value;\
}; */


// MARK: Istream
class Istream {
private:
        std::string str;
        size_t size_cache;
        char* iter;
        char* start;
        char* end;

public:
        Istream() = delete;

        Istream(std::string str_)
                : str(str_),
                  size_cache(str.length()),
                  iter(&str[0]),
                  start(&str[0]),
                  end(&str.back() + 1) {}

        Istream(std::ifstream&& file) {
                file.seekg(0, std::ios::end);
                size_t json_len = (size_t)file.tellg();
                file.seekg(0, std::ios::beg);
                str.resize(json_len);
                file.read(str.data(), json_len);
                size_cache = str.length();
                iter = &str[0];
                start = &str[0];
                end = &str.back() + 1;
        }

        Istream(Istream&& other) = default;

        HOT INLINE char peek() { return *iter; }

        HOT INLINE void seek() {
                likely_if (iter!=end) ++iter;
        }

        HOT INLINE char get() {
                unlikely_if (iter==end) return '\0';
                char ret = *(iter++);
                return ret;
        }

        INLINE size_t pos() const { return iter - start; }

        INLINE size_t length() const { return size_cache; }

        inline void reset() { iter = start; }
};


constexpr unsigned int flag_parse_number_as_str = 0b00000001;
constexpr unsigned int flag_enable_following_comma = 0b00000010;
constexpr unsigned int flag_enable_escape_line_break = 0b00000100;
constexpr unsigned int flag_no_parse_escape_sequence = 0b00001000;
constexpr unsigned int flag_enable_single_quot = 0b00010000;
constexpr unsigned int flag_enable_comment = 0b00100000;
constexpr unsigned int flag_enable_extention_value = 0b01000000;


// MARK: parser
template <class IstreamT, class HandlerT, unsigned int flag=0b00000000>
class Parser {
public:
        IstreamT& istream;
        HandlerT& handler;
        Parser(IstreamT& ref_istream, HandlerT& ref_handler) : istream(ref_istream), handler(ref_handler) {}

        Parser(Parser&& other) : istream(other.istream), handler(other.handler) {}

        // clang-format off
private:

// MARK: skip space
// if at the end return `true`
INLINE bool eat_space() {
        char ch;
        while(true) {
                ch = istream.peek();
                /* crash bug: 这里的ch曾经是转成Uint64,后因越界访问被ASAN拦截;开启优化,
                关闭ASAN后UB不被拦截,但char转Uint64出现超级大数,会产生bug,并且可造成CPU
                缓存失效和分支预测失败的问题,但由于压测数据整个无空格,bug未暴露,但在perf
                插桩性能测试中eat_space()出现不正常的17.8%开销被发现,
                此前一直是带着bug和性能黑洞跑的还没发现!!! */
                likely_if (!tools::skip_space_ch_table[(unsigned char)ch]) break;
                istream.seek();
        }
        unlikely_if (ch=='\0') return true;
        return false;
}

// MARK: comment
// skip space and comment,if allow comment
INLINE bool eat_space_and_comment(const char* (&err)) {
_start:
        char ch = istream.peek();
        if (ch=='/') goto _skip_comment;
        else if (tools::skip_space_ch_table[(unsigned char)ch]) {
                istream.seek(); // skip this space
                goto _skip_space;
        }
        else if (ch=='\0') return true;
        else return false;
_skip_comment:
        // skip comment if start with '/'
        while (ch=='/') {
                istream.seek(); // skip this '/'
                ch = istream.get();
                if (ch=='/') { // line comment
                        while (true) {
                                ch = istream.get();
                                if (ch=='\n') {
                                        break;
                                } else if (ch=='\0') {
                                        return true;
                                }
                        }
                } else if (ch=='*') { // multiple line comment
                        while (true) {
                                ch = istream.get();
                                // maybe "*/" close
                                if (ch=='*') {
                                        ch = istream.get();
                                        if (ch=='/') {
                                                ch = istream.peek();
                                                break;
                                        }
                                }
                                if (ch=='\0') {
                                        err = "未闭合的注释/*, 发现EOF";
                                        return true;
                                }
                        }
                } else {
                        err = "非法的注释格式,发现'/'";
                        return false;
                }
        }
        goto _start;
_skip_space:
        // skip space
        while (true) {
                ch = istream.peek();
                likely_if (!tools::skip_space_ch_table[(unsigned char)ch]) break;
                istream.seek();
        }
        unlikely_if (ch=='\0') return true;
        goto _start;
}

INLINE static int8_t get_16base_ch_num(char ch) {
        switch (ch) {
        case '0': case '1': case '2': case '3':
        case '4': case '5': case '6': case '7':
        case '8': case '9':
                return ch-'0'+1;  // actual value increased by 1
        case 'A': case 'B': case 'C':
        case 'D': case 'E': case 'F':
                ch += 32;
        case 'a': case 'b': case 'c':
        case 'd': case 'e': case 'f':
                return ch-'a'+10+1;
        default:
                return 0;  // actual is -1
        }
}

INLINE unsigned int read_4bit_of_unicode() {
        int8_t bit1 = get_16base_ch_num(istream.get()),
               bit2 = get_16base_ch_num(istream.get()),
               bit3 = get_16base_ch_num(istream.get()),
               bit4 = get_16base_ch_num(istream.get());
        // 但凡有一个0, 全部为0
        unlikely_if (bit1*bit2*bit3*bit4==0) {
                return 0;
        }
        return ((bit1 - 1) << 12)
                + ((bit2 - 1) << 8)
                + ((bit3 - 1) << 4)
                + (bit4 - 1) + 1;
}

// MARK: parse_string()
template <char quot='"'>
INLINE auto parse_string() -> const char* {
        istream.seek(); // skip the quotation " or '
        char ch;
        // most of the string doesn't have escape sequence
        while(true) {
_eat_ascii:
                ch = istream.get();
                likely_if (tools::str_skip_ch_table[(unsigned char)ch]) {
                        handler.push_char(ch);
                        continue;
                }
                switch (ch) {
                case quot:
                        return "";
                case '\0':
                        return "未闭合的'\"', 发现EOF";
                case '\n':
                        return "未闭合的'\"', 发现换行符";
                case '\\':
                        if constexpr (flag & flag_no_parse_escape_sequence) {
                                // escaped quotation only
                                if (istream.peek()==quot) {
                                        handler.push_char(quot);
                                        istream.seek();
                                        continue;
                                }
                        } else {
                                goto _had_escape;
                        }
                default:
                        handler.push_char(ch);
                }
        }
_had_escape:
        // now we meet a '\' character, read ptr at the char after '\'
        ch = istream.get();
        switch (ch) {
        case '\0':
                return "'\\'后的EOF, 非法转义";
        case quot: handler.push_char(quot); break;
        case '\\': handler.push_char('\\'); break;
        case 'b':  handler.push_char('\b'); break;
        case 'f':  handler.push_char('\f'); break;
        case 'n':  handler.push_char('\n'); break;
        case 'r':  handler.push_char('\r'); break;
        case 't':  handler.push_char('\t'); break;
        case '/':  handler.push_char('/');  break;
        case 'u': {
                unlikely_if (istream.pos()+4 >= istream.length()) {
                        return "非法不完整的UTF-16转义";
                }
                auto point = read_4bit_of_unicode();
                unlikely_if (point==0) return "未预期的字符, 非法UTF-16转义";
                --point;
                if (point >= 0xD800 && point <= 0xDBFF) { // high-half zone
                        unlikely_if (istream.get() != '\\' || istream.get() != 'u') {
                                return "孤立的高代理, 非法UTF-16转义";
                        }
                        unlikely_if (istream.pos()+4 >= istream.length()) {
                                return "非法不完整的UTF-16转义, 位于低代理";
                        }
                        auto low_point = read_4bit_of_unicode();
                        unlikely_if (low_point==0) return "未预期的字符, 非法UTF-16转义";
                        --low_point;
                        unlikely_if (low_point < 0xDC00 || low_point > 0xDFFF) { // low-half zone not low range
                                return "低位代理值异常, 非法UTF-16";
                        }
                        point = ((point-0xD800)<<10 | (low_point-0xDC00)) + 0x10000;
                        tools::u32ch_decode_push_to_handler(point, handler);
                } else unlikely_if(point >= 0xDC00 && point <= 0xDFFF) { // low-half zone
                        return "独立的低代理, 非法UTF-16转义";
                } else {
                        tools::u32ch_decode_push_to_handler(point, handler);
                }
                break;
        }
        default:
                if constexpr (flag & flag_enable_escape_line_break) {
                        // check \r\n
                        if (ch=='\r') {
                                ch = istream.get(); // skip '\r', expect '\n'
                        }
                        unlikely_if (ch!='\n') {
                                return "非法转义";
                        }
                } else {
                        return "非法转义";
                }
        }
        goto _eat_ascii;
}


// MARK: parse_number()
INLINE auto parse_number(bool check_first_ch) -> const char* {
        Uint128 int_num=0;
        Uint128 dec_num=0;
        Uint128 exp_num=0;
        int count_dec_digits=0;
        bool is_neg = false;
        bool exp_part_is_neg = false;
        unlikely_if(check_first_ch) switch (istream.peek()) {
        case '0':
                istream.seek();
                switch (istream.peek()) {
                case '.':
                        istream.seek(); // skip '.'
                        goto _dec_part;
                case 'E':
                case 'e':
                        istream.seek(); // skip 'E' or 'e'
                        goto _exp_part;
                default:
                        handler.push_number((long long)0);
                        return "";
                }
        case '+':
                is_neg = true; // 后续取反
                // fall through
        case '-':
                istream.seek();
                char next_ch = istream.peek();
                unlikely_if (next_ch<'0' || next_ch>'9') return "+/-后的未预期字符";
                is_neg = !is_neg; // true -> false, false -> true
                goto _int_part;
        }
_int_part:
        // if allow NaN and Inf
        if constexpr (flag == flag_enable_extention_value) {
                char ch = istream.peek();
                if (ch=='N') {
                        istream.seek(); // skip this 'N'
                        unlikely_if (istream.pos()+2 >= istream.length() ||
                                        istream.get() != 'a' ||
                                        istream.get() != 'N' ){
                                return "未预期的'N'";
                        }
                        handler.push_number(NAN);
                        return "";
                } else if (ch=='I') {
                        istream.seek(); // skip this 'I'
                        unlikely_if (istream.pos()+2 >= istream.length() ||
                                        istream.get() != 'n' ||
                                        istream.get() != 'f' ){
                                return "未预期的'I'";
                        }
                        handler.push_number(is_neg ? INFINITY : -INFINITY);
                        return "";
                }
        }
        while (true) {
                char ch = istream.peek();
                likely_if (ch>='0' && ch<='9') {
                        int_num = int_num*10 + (ch-'0');
                        istream.seek();
                        continue;
                }
                switch (ch) {
                case '.':
                        istream.seek(); // skip this '.'
                        goto _dec_part;
                case 'E':
                case 'e':
                        istream.seek(); // skip this 'e'/'E'
                        goto _exp_part;
                default:
                        goto _end_by_int;
                }
        }
_dec_part: {
        char ch = istream.get();
        likely_if (ch>='0' && ch<='9') {
                ++count_dec_digits;
                dec_num = ch-'0';
        } else {
                return "'.'后的未预期字符";
        }
        while (true) {
                ch = istream.peek();
                likely_if (ch>='0' && ch<='9') {
                        dec_num = dec_num*10 + (ch-'0');
                        ++count_dec_digits;
                        istream.seek();
                        continue;
                }
                if (ch=='e' || ch=='E') {
                        istream.seek();
                        goto _exp_part;
                }
                goto _end_by_float;
        }
}
_exp_part: {
        char ch = istream.get();
        likely_if (ch>='0' && ch<='9') exp_num = ch-'0';
        else if (ch=='-') exp_part_is_neg = true;
        else unlikely_if (ch!='+') return "'e'或'E'后未预期的字符";
        while (true) {
                ch = istream.peek();
                likely_if (ch>='0' && ch<='9') {
                        exp_num = exp_num*10 + (ch-'0');
                        istream.seek();
                        continue;
                }
                goto _end_by_float;
        }
}
_end_by_int:
        if (is_neg) {
                unlikely_if (int_num > tools::LL_MAX) {
                        handler.push_number(-(double)int_num); // negative and too big, conversion to double
                        return "";
                }
                handler.push_number(-(Int64)int_num); // else, negative long long (signed Int64)
                return "";
        }
        unlikely_if (int_num > tools::LL_MAX) {
                unlikely_if (int_num > tools::ULL_MAX) {
                        handler.push_number((double)int_num); // even bigger than ULL max, use double
                } else {
                        handler.push_number((Uint64)int_num); // too big, Uint64
                }
        } else {
                handler.push_number((Int64)int_num); // else, just a long long (Int64)
        }
        return "";
_end_by_float:
        double res = (double)int_num  +  (double)dec_num * tools::neg_pow_table[count_dec_digits];
        res *= exp_part_is_neg ?
                exp_num < tools::neg_pow_table_len ?
                  tools::neg_pow_table[exp_num] : 0.0
                :exp_num < tools::pow_table_len ?
                  tools::pow_table[exp_num] : INFINITY;
        handler.push_number(is_neg ? -res : res);
        return "";
}  // end function `parse_number`


// MARK: parse_number_to_str()
INLINE auto parse_number_to_str(bool check_first_ch) -> const char* {
        char ch = istream.peek();
        unlikely_if(check_first_ch) switch (ch) {
        case '0':
                handler.push_num_ch('0');
                istream.seek(); // skip this '0'
                switch (istream.peek()) {
                case '.':
                        handler.push_num_ch('.');
                        istream.seek(); // skip '.'
                        goto _dec_part;
                case 'E':
                case 'e':
                        handler.push_num_ch('e');
                        istream.seek(); // skip 'E' or 'e'
                        goto _exp_part;
                default:
                        return "";
                }
        case '+':
        case '-':
                handler.push_num_ch(ch);
                istream.seek(); // skip sig
                char next_ch = istream.peek();
                unlikely_if (next_ch<'0' || next_ch>'9') return "+/-后的未预期字符";
                goto _int_part;
        }
_int_part:
        // if allow NaN and Inf
        if constexpr (flag == flag_enable_extention_value) {
                 ch = istream.peek();
                if (ch=='N') {
                        istream.seek(); // skip this 'N'
                        unlikely_if (istream.pos()+2 >= istream.length() ||
                                        istream.get() != 'a' ||
                                        istream.get() != 'N' ){
                                return "未预期的'N'";
                        }
                        handler.start_num();
                        handler.push_num_ch('N');
                        handler.push_num_ch('a');
                        handler.push_num_ch('N');
                        handler.end_num();
                        return "";
                } else if (ch=='I') {
                        istream.seek(); // skip this 'I'
                        unlikely_if (istream.pos()+2 >= istream.length() ||
                                        istream.get() != 'n' ||
                                        istream.get() != 'f' ){
                                return "未预期的'I'";
                        }
                        handler.start_num();
                        handler.push_num_ch('I');
                        handler.push_num_ch('n');
                        handler.push_num_ch('f');
                        handler.end_num();
                        return "";
                }
        }
        while (true) {
                ch = istream.peek();
                likely_if (ch>='0' && ch<='9') {
                        handler.push_num_ch(ch);
                        istream.seek();
                        continue;
                }
                switch (ch) {
                case '.':
                        handler.push_num_ch('.');
                        istream.seek(); // skip this '.'
                        goto _dec_part;
                case 'E':
                case 'e':
                        handler.push_num_ch('e');
                        istream.seek(); // skip this 'e'/'E'
                        goto _exp_part;
                default:
                        return "";
                }
        }
_dec_part: {
        ch = istream.get();
        likely_if (ch>='0' && ch<='9') {
                handler.push_num_ch(ch);
        } else {
                return "'.'后的未预期字符";
        }
        while (true) {
                ch = istream.peek();
                likely_if (ch>='0' && ch<='9') {
                        handler.push_num_ch(ch);
                        istream.seek();
                        continue;
                }
                if (ch=='e' || ch=='E') {
                        handler.push_num_ch('e');
                        istream.seek();
                        goto _exp_part;
                }
                return "";
        }
}
_exp_part: {
        char ch = istream.get();
        likely_if ((ch>='0' && ch<='9') || ch=='-' || ch=='+') {
                handler.push_num_ch(ch);
        } else{
                return "'e'或'E'后未预期的字符";
        }
        while (true) {
                ch = istream.peek();
                likely_if (ch>='0' && ch<='9') {
                        handler.push_num_ch(ch);
                        istream.seek();
                        continue;
                }
                return "";
        }
}
}  // end function `parse_number`


public:

// MARK: parse()
auto parse() -> const char* {
        // skip space definition
        #define skip_space(ret) \
                if constexpr (flag & flag_enable_comment) { \
                        const char* err="";                           \
                        unlikely_if (eat_space_and_comment(err))      \
                                return ret;                           \
                        else unlikely_if (err[0]!='\0')               \
                                return err;                           \
                } else {                                              \
                        unlikely_if (eat_space()) {                   \
                                return ret;                           \
                        }                                             \
                }
        // 所有标签, 均为'_'开头
        // all of the goto lable start with '_'
        enum class state {
                none,
                in_arr,
                obj_k,
                obj_v
        };
        state stack[1024]; state* top = stack;
        bool check_number_first_char = false;
        skip_space("empty json");
_start_val:
        char temp_ch = istream.peek();
        switch (temp_ch) {
        case '+': case '-': case '0':
                check_number_first_char = true;
        case '1': case '2': case '3':
        case '4': case '5': case '6':
        case '7': case '8': case '9':
                goto _parse_num;
        case '"':
                goto _parse_str;
        case '[':
                goto _arr_start;
        case '{':
                goto _obj_start;
        case 't':
                goto _parse_true;
        case 'f':
                goto _parse_false;
        case 'n':
                goto _parse_null;
        default:
                // single quotation '
                if constexpr (flag & flag_enable_single_quot) {
                        if (temp_ch=='\'') {
                                goto _parse_str;
                        }
                } else if constexpr (flag & flag_enable_extention_value) {
                        if (temp_ch=='N' || temp_ch=='I') { // NaN and Inf
                                goto _parse_num;
                        }
                }
                return "未预期的字符";
        }
_parse_str: {
        handler.start_str();
        // if allow single quotation string
        if constexpr (flag & flag_enable_single_quot) {
                if (temp_ch=='\'') {
                        auto err = parse_string<'\''>();
                        unlikely_if (err[0]!='\0')
                                return err;
                        handler.end_str();
                        goto _end_val;
                }
        }
        auto err = parse_string();
        unlikely_if (err[0]!='\0')
                return err;
        handler.end_str();
        goto _end_val;
}
_parse_num: {
        if constexpr (flag & flag_parse_number_as_str) {
                // if user want us parse number to string
                handler.start_num();
                auto err = parse_number_to_str(check_number_first_char);
                unlikely_if (err[0]!='\0')
                        return err;
                handler.end_num();
        } else {
                auto err = parse_number(check_number_first_char);
                unlikely_if (err[0]!='\0')
                        return err;
        }
        goto _end_val;
}
_arr_start:
        // set a new state stack
        ++top;
        *top = state::in_arr;
        handler.start_arr();
        istream.seek(); // skip '['
        // check empty array []
        unlikely_if (istream.peek()==']') {
                istream.seek();
                goto _arr_end;
        }
_arr_val:
        skip_space("遇到EOF, 缺值或']'");
        // check if allow following comma
        if constexpr (flag & flag_enable_following_comma) {
                if (istream.peek()==']') {
                        istream.seek(); // skip ']'
                        goto _arr_end;
                }
        }
        goto _start_val;
_arr_end:
        // pop the stack
        --top;
        handler.end_arr();
        goto _end_val;
_obj_start:
        handler.start_obj();
        istream.seek(); // skip '{'
        ++top; // part of object, at key
        // here is also object key, but object can be empty "{}", special judge
        *top = state::obj_k;
        skip_space("遇到EOF, 缺完整键值对或'}'");
        temp_ch = istream.peek();
        unlikely_if (temp_ch!='"') {
                if constexpr (flag & flag_enable_single_quot) {
                        if (temp_ch=='\'') {
                                goto _parse_str;
                        }
                }
                likely_if (temp_ch=='}') {
                        istream.seek(); // skip '}'
                        goto _obj_end;
                }
                return "未预期的字符, 应为键";
        }
        goto _parse_str;
// other key string (not the first one)
_obj_key:
        *top = state::obj_k;
        skip_space("遇到EOF, 缺完整键值对或'}'");
        temp_ch = istream.peek();
        // if it's not expected '"'
        unlikely_if (temp_ch!='"') {
                // if allow 'string'
                if constexpr (flag & flag_enable_single_quot) {
                        if (temp_ch=='\'') {
                                goto _parse_str;
                        }
                }
                // if allow following comma
                if constexpr (flag & flag_enable_following_comma) {
                        if (temp_ch=='}') {
                                istream.seek(); // skip '}'
                                goto _obj_end;
                        }
                }
                return "未预期的字符, 应为键";
        }
        goto _parse_str;
_obj_val:
        *top = state::obj_v; // also part of object, at value
        skip_space("遇到EOF, 键后缺值");
        goto _start_val;
_obj_end:
        --top; // pop the object stack, it's obj_v
        handler.end_obj();
        goto _end_val;
_parse_true:
        istream.seek();
        unlikely_if (istream.pos()+3 >= istream.length() ||
                        istream.get() != 'r' ||
                        istream.get() != 'u' ||
                        istream.get() != 'e' ){
                return "未预期的't'";
        }
        handler.push_bool(true);
        goto _end_val;
_parse_false:
        istream.seek();
        unlikely_if (istream.pos()+4 >= istream.length() ||
                        istream.get() != 'a' ||
                        istream.get() != 'l' ||
                        istream.get() != 's' ||
                        istream.get() != 'e' ){
                return "未预期的'f'";
        }
        handler.push_bool(false);
        goto _end_val;
_parse_null:
        istream.seek();
        unlikely_if (istream.pos()+3 > istream.length() ||
                        istream.get() != 'u' ||
                        istream.get() != 'l' ||
                        istream.get() != 'l' ){
                return "未预期的'n'";
        }
        handler.push_null();
        goto _end_val;
_end_val:
        bool at_end;
        // skip space
        if constexpr (flag & flag_enable_comment) {
                const char* err = "";
                at_end = eat_space_and_comment(err);
                unlikely_if (err[0]!='\0')
                        return err;
        } else {
                at_end = eat_space();
        }
        // empty stack, end of json, exit
        unlikely_if (top == stack) {
                // single value json or value/object end
                if(at_end) return "";
                return "完整json结构后多余的字符";
        }
        if (*top==state::in_arr) {
                // path: _arr_val -> _start_val -> _end_val(here)
                unlikely_if (at_end)
                        return "遇到EOF, 未闭合'[]'";
                temp_ch = istream.get();
                if(temp_ch==',') goto _arr_val;
                if(temp_ch==']') goto _arr_end;
                return "未预期字符, 应为']'";
        }
        if (*top==state::obj_k) {
                // path: _obj_key -> _parse_str -> _end_val(here)
                unlikely_if (at_end)
                        return "键后的EOF, 缺少值和'}'";
                temp_ch = istream.get();
                unlikely_if (temp_ch!=':')
                        return "键后未预期的字符, 应为':'";
                goto _obj_val;
        }
        if (*top==state::obj_v) {
                // path: _obj_val -> _start_val -> _end_val(here)
                unlikely_if (at_end)
                        return "值后的EOF, 缺少'}'";
                temp_ch = istream.get();
                if(temp_ch==',') goto _obj_key;
                if(temp_ch=='}') goto _obj_end;
                return "未预期的字符, 应为'}'";
        }
        // this doesn't run
        return "";
}

        // clang-format on

};  // class parser


}  // namespace hai


class Handler {
        std::string temp;
        std::string temp_num;
public:
        INLINE void start_str() {
                // print("start str\n");
        }
        INLINE void end_str() {
                // print("push str '{}'\nend str\n", temp);
                // temp.clear();
        }
        INLINE void start_arr() {
                // print("start arr\n");
        }
        INLINE void end_arr() {
                // print("end arr\n");
        }
        INLINE void start_obj() {
                // print("start obj\n");
        }
        INLINE void end_obj() {
                // print("end obj\n");
        }
        INLINE void push_char(char ) {
                // temp+=n;
        }
        INLINE void push_number(long long ) {
                // print("push \033[31m{}\033[0m\n", n);
        }
        INLINE void push_number(unsigned long long ) {
                // print("push \033[31m{}\033[0m\n", n);
        }
        INLINE void push_number(double ) {
                // print("push \033[31m{}\033[0m\n", n);
        }
        INLINE void push_bool(bool ) {
                // print("push \033[31m{}\033[0m\n", n);
        }
        INLINE void push_null() {
                // print("push \033[31mnull64\033[0m\n");
        }
        INLINE void start_num() {
                // print("start num\n");
        }
        INLINE void end_num() {
                // print("push num '{}'\nend num\n", temp_num);
                // temp_num.clear();
        }
        INLINE void push_num_ch(char ) {
                // temp_num+=n;
        }
};


using namespace hai;


int main() {
        Istream istream(std::ifstream("./test/1.json"));
        Handler handle;
        Parser<
                Istream,
                Handler,
                flag_enable_comment |
                flag_enable_following_comma |
                flag_enable_single_quot |
                flag_enable_escape_line_break
        > parser (istream, handle);
        std::string err = parser.parse();
        if (!err.empty()) {
                // print("{} at {}", err, istream.pos());
                return 1;
        }
        for (int i = 0; i < 10; ++i) {
                parser.parse();
                parser.istream.reset();
        }
}