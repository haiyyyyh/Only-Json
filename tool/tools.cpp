// import depend;

namespace hai {

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
auto mut_memcast(void *mem) -> T & {
        return *reinterpret_cast<T *>(mem);
}

template <typename T>
auto memcast(const void *mem) -> const T & {
        return *reinterpret_cast<const T *>(mem);
}

void panic(const char *msg) {
        fprintf(stderr, "\e[31;1m[panic] \e[34;3monly-json\e[0m : %s\n", msg);
        abort();
}

void bad_using_panic(const char *func_name, const char *on_type) {
        fprintf(stderr,
                "\e[31;1m[panic] \e[34;3monly-json\e[0m : "
                "\e[33;1;4mtype check failed\e[0m"
                " (not '\e[36;1m%s\e[0m') when using '\e[32;3;1m%s\e[0m'\n",
                on_type, func_name);
        abort();
}

void bad_getting_panic(const char *msg) {
        fprintf(stderr,
                "\e[31;1m[panic] \e[34;3monly-json\e[0m : "
                "\e[33;1;4mtype check failed\e[0m"
                " when getting '\e[36;1m%s\e[0m'\n",
                msg);
        abort();
}

void bad_conversion_panic(const char *to_type) {
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

auto to_str(long long num) -> const char * {
        if (num <= 100 && num >= -100) {
                return table[num + 100];
        }
        auto [p, err] = std::to_chars(buf, buf + 20, num);
        *p = '\0';
        return buf;
}

auto to_str(unsigned long long num) -> const char * {
        if (num <= 100) {
                return table[num + 100];
        }
        auto [p, err] = std::to_chars(buf, buf + 20, num);
        *p = '\0';
        return buf;
}

auto to_str(double num) -> const char * {
        auto [p, err] = std::to_chars(buf, buf + 20, num);
        *p = '\0';
        return buf;
}

// clang-format off

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
        auto &get_item(Tag<idx>) {
                return _other.template get<idx - 1>();
        }

        auto &get_item(Tag<0>) { return _0; }

public:
        tuple() {}
        tuple(T0 _t0, Args... _other_t) : _0(_t0), _other(_other_t...) {}
        template <int idx>
        auto &get() {
                return this->get_item(Tag<idx>{});
        }
};


// template <typename T1>
// struct tuple<T1> {
//     T1 _1;
// }


}  // namespace tools

}  // namespace hai


// using namespace hai::tools;


// int main(){
//     tuple<int, int, int> a;
//     a.get<1>();
// }