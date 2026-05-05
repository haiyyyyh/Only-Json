import depend;

#include <string>
#include <vector>
#include "tools.cpp"

namespace hai {

class json;

using string_t = std::string;
using array_t = std::vector<json>;
using object_t = std::map<string_t, json>;

// clang-format off
enum class json_t {
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

class json {
private:
    // 利用char固定1字节的特性, 申请一块足够大的栈内存来手动管理
    // 这属于高危操作, 我们应知道自己在做什么
    alignas(std::max_align_t) char mem_block[tools::max_sizeof<string_t, array_t, object_t>()];
    json_t Type;

    void free() noexcept {
        // literal type, quickly skip
        if (Type < json_t::String) {
            return;
        }
        // RAII类型需要析构
        switch (Type) {
        case json_t::String:
            tools::mut_memcast<string_t>(mem_block).~string_t();
            break;
        case json_t::Array:
            tools::mut_memcast<array_t>(mem_block).~array_t();
            break;
        case json_t::Object:
            tools::mut_memcast<object_t>(mem_block).~object_t();
            break;
        default:
            break;
        }
    }

    // 构造函数和析构函数
    // constructor and destructor
public:
    json() { Type = json_t::Null; }
    // Integer match this function
    template <typename T>
        requires(tools::IntegerOnly<T>)
    json(T i_v) noexcept {
        if (i_v > 9223372036854775807LL) {
            new (mem_block) unsigned long long(i_v);
            Type = json_t::Unsign;
        } else {
            new (mem_block) long long(i_v);
            Type = json_t::Integer;
        }
    }

    // Floating match this function
    template <typename T>
        requires(tools::FloatOnly<T>)
    json(T f_v) noexcept {
        new (mem_block) double(f_v);
        Type = json_t::Float;
    }

    // bool only
    json(bool b_v) noexcept {
        new (mem_block) bool(b_v);
        Type = json_t::Boolean;
    }

    // null
    json(decltype(nullptr)) noexcept { Type = json_t::Null; }

    // string
    json(const string_t& str_v) noexcept {
        new (mem_block) string_t(str_v);
        Type = json_t::String;
    }
    json(string_t&& str_v) noexcept {
        new (mem_block) string_t((string_t&&)str_v);
        Type = json_t::String;
    }
    json(const char* constr) noexcept {
        new (mem_block) string_t(constr);
        Type = json_t::String;
    }

    // list
    json(const array_t& arr_v) noexcept {
        new (mem_block) array_t(arr_v);
        Type = json_t::Array;
    }
    json(array_t&& arr_v) noexcept {
        new (mem_block) array_t((array_t&&)arr_v);
        Type = json_t::Array;
    }

    template <typename = std::enable_if<true>>
    json(std::initializer_list<json> initial_list) noexcept {
        new (mem_block) array_t(initial_list);
        Type = json_t::Array;
    }

    // 嵌套对象
    json(const object_t& obj_v) noexcept {
        new (mem_block) object_t(obj_v);
        Type = json_t::Object;
    }
    json(object_t&& obj_v) noexcept {
        new (mem_block) object_t((object_t&&)obj_v);
        Type = json_t::Object;
    }
    // another initializer_list constructor, constructor of 'json' is ambiguous, property using this, else use array initializer
    json(std::initializer_list<std::pair<const string_t, json>> initial_list) noexcept {
        new (mem_block) object_t(initial_list);  // aka std::map<string, json>
        Type = json_t::Object;
    }

    // copy constructor
    json(const json& other) noexcept {
        if (other.Type < json_t::Boolean) {
            memcpy(mem_block, other.mem_block, 8);
        } else {
            switch (other.Type) {
            case json_t::Boolean:
                new (mem_block) bool(tools::memcast<bool>(other.mem_block));
                break;
            case json_t::String:
                new (mem_block) string_t(tools::memcast<string_t>(other.mem_block));
                break;
            case json_t::Array:
                new (mem_block) array_t(tools::memcast<array_t>(other.mem_block));
                break;
            case json_t::Object:
                new (mem_block) object_t(tools::memcast<object_t>(other.mem_block));
                break;
            default:
                break;
            }
        }
        Type = other.Type;
    }
    // move constructor
    json(json&& other) noexcept {
        switch (other.Type) {
        case json_t::String:
            new (mem_block) string_t((string_t&&)tools::mut_memcast<string_t>(other.mem_block));
            break;
        case json_t::Array:
            new (mem_block) array_t((array_t&&)tools::mut_memcast<array_t>(other.mem_block));
            break;
        case json_t::Object:
            new (mem_block) object_t((object_t&&)tools::mut_memcast<object_t>(other.mem_block));
            break;
        default:
            memcpy(mem_block, other.mem_block, sizeof(mem_block));
            break;
        }
        Type = other.Type;
        other.Type = json_t::Null;
    }
    ~json() noexcept { free(); }

    // 等于号重载
    // assignment
public:
    // Integer match this function
    template <typename T>
        requires(tools::IntegerOnly<T>)
    void operator=(T i_v) noexcept {
        free();
        if (i_v >= 9223372036854775807LL) {
            new (mem_block) unsigned long long(i_v);
            Type = json_t::Unsign;
        } else {
            new (mem_block) long long(i_v);
            Type = json_t::Integer;
        }
    }

    // Floating match this function
    template <typename T>
        requires(tools::FloatOnly<T>)
    void operator=(T f_v) noexcept {
        free();
        new (mem_block) double(f_v);
        Type = json_t::Float;
    }

    // bool only
    void operator=(bool b_v) noexcept {
        free();
        new (mem_block) bool(b_v);
        Type = json_t::Boolean;
    }

    // null
    void operator=(decltype(nullptr)) noexcept {
        free();
        Type = json_t::Null;
    }

    // string
    void operator=(const string_t& str_v) noexcept {
        free();
        new (mem_block) string_t(str_v);
        Type = json_t::String;
    }
    void operator=(string_t&& str_v) noexcept {
        free();
        new (mem_block) string_t((string_t&&)str_v);
        Type = json_t::String;
    }
    void operator=(const char* constr) noexcept {
        free();
        new (mem_block) string_t{constr};
        Type = json_t::String;
    }

    // list
    void operator=(const array_t& arr_v) noexcept {
        free();
        new (mem_block) array_t(arr_v);
        Type = json_t::Array;
    }
    void operator=(array_t&& arr_v) noexcept {
        free();
        new (mem_block) array_t((array_t&&)arr_v);
        Type = json_t::Array;
    }

    template <typename = std::enable_if<true>>
    void operator=(std::initializer_list<json> initial_list) noexcept {
        free();
        new (mem_block) array_t(initial_list);
        Type = json_t::Array;
    }

    // dict
    void operator=(const object_t& obj_v) noexcept {
        free();
        new (mem_block) object_t(obj_v);
        Type = json_t::Object;
    }
    void operator=(object_t&& obj_v) noexcept {
        free();
        new (mem_block) object_t((object_t&&)obj_v);
        Type = json_t::Object;
    }
    void operator=(std::initializer_list<std::pair<const string_t, json>> initial_list) noexcept {
        free();
        new (mem_block) object_t(initial_list);
        Type = json_t::Object;
    }

    // copy
    void operator=(const json& other) noexcept {
        free();
        if (other.Type < json_t::Boolean) {
            memcpy(mem_block, other.mem_block, 8);
        } else {
            switch (other.Type) {
            case json_t::Boolean:
                memcpy(mem_block, other.mem_block, sizeof(bool));
                break;
            case json_t::String:
                new (mem_block) string_t(tools::memcast<string_t>(other.mem_block));
                break;
            case json_t::Array:
                new (mem_block) array_t(tools::memcast<array_t>(other.mem_block));
                break;
            case json_t::Object:
                new (mem_block) object_t(tools::memcast<object_t>(other.mem_block));
                break;
            default:
                break;
            }
        }
        Type = other.Type;
    }
    // move
    void operator=(json&& other) noexcept {
        free();
        switch (other.Type) {
        case json_t::String:
            new (mem_block) string_t((string_t&&)tools::mut_memcast<string_t>(other.mem_block));
            break;
        case json_t::Array:
            new (mem_block) array_t((array_t&&)tools::mut_memcast<array_t>(other.mem_block));
            break;
        case json_t::Object:
            new (mem_block) object_t((object_t&&)tools::mut_memcast<object_t>(other.mem_block));
            break;
        default:
            memcpy(mem_block, other.mem_block, sizeof(mem_block));
            break;
        }
        Type = other.Type;
        other.Type = json_t::Null;
    }

    // 隐式\显式转换
    // convert
public:
    template <typename T>
        requires(tools::IntegerOnly<T> || tools::FloatOnly<T>)
    operator T() {
        if (Type > json_t::Float) {
            tools::bad_conversion_panic("number");
        }
        switch (Type){
        case json_t::Integer:
            return (T)tools::memcast<long long>(mem_block);
        case json_t::Float:
            return (T)tools::memcast<double>(mem_block);
        default:
            return (T)tools::memcast<unsigned long long>(mem_block);
        }
    }

    operator bool() {
        if (Type != json_t::Boolean) {
            tools::bad_conversion_panic("boolean");
        }
        return tools::memcast<bool>(mem_block);
    }

    template <typename T>
    operator T*() const {
        if (Type != json_t::Null) {
            tools::bad_conversion_panic("null type");
        }
        return nullptr;
    }

    operator string_t() {
        if (Type != json_t::String) {
            tools::bad_conversion_panic("string");
        }
        return tools::memcast<string_t>(mem_block);
    }

    operator array_t() {
        if (Type != json_t::Array) {
            tools::bad_conversion_panic("array");
        }
        return tools::mut_memcast<array_t>(mem_block);
    }

    operator object_t() {
        if (Type != json_t::Object) {
            tools::bad_conversion_panic("object");
        }
        return tools::mut_memcast<object_t>(mem_block);
    }

    template <typename T>
        requires(tools::IntegerOnly<T> || tools::FloatOnly<T>)
    operator const T() const {
        if (Type > json_t::Float) {
            tools::bad_conversion_panic("number");
        }
        switch (Type){
        case json_t::Integer:
            return (T)tools::memcast<long long>(mem_block);
        case json_t::Float:
            return (T)tools::memcast<double>(mem_block);
        default:
            return (T)tools::memcast<unsigned long long>(mem_block);
        }
    }

    operator const bool() const {
        if (Type != json_t::Boolean) {
            tools::bad_conversion_panic("boolean");
        }
        return tools::memcast<bool>(mem_block);
    }

    operator const string_t() const {
        if (Type != json_t::String) {
            tools::bad_conversion_panic("string");
        }
        return tools::memcast<string_t>(mem_block);
    }

    operator const array_t() const {
        if (Type != json_t::Array) {
            tools::bad_conversion_panic("array");
        }
        return tools::memcast<array_t>(mem_block);
    }

    operator const object_t() const {
        if (Type != json_t::Object) {
            tools::bad_conversion_panic("object");
        }
        return tools::memcast<object_t>(mem_block);
    }

    // 获取内部对象的函数
    // get
public:
    auto type() -> json_t { return Type; }

    auto integer() const -> long long& {
        if (Type != json_t::Integer) {
            tools::bad_getting_panic("integer number");
        }
        // tools::memcast & memcast or use const_cast<> at here all cause errors
        // so use C-style unsafe conversion
        return *(long long*)mem_block;
    }

    auto unsign() const -> unsigned long long& {
        if (Type != json_t::Integer) {
            tools::bad_getting_panic("unsigned number");
        }
        return *(unsigned long long*)mem_block;
    }

    auto floating() const -> double& {
        if (Type != json_t::Float) {
            tools::bad_getting_panic("floating-point number");
        }
        return *(double*)mem_block;
    }

    auto boolean() const -> bool& {
        if (Type != json_t::Boolean) {
            tools::bad_getting_panic("boolean");
        }
        return *(bool*)mem_block;
    }

    auto string() const -> string_t& {
        if (Type != json_t::String) {
            tools::bad_getting_panic("string");
        }
        return *(string_t*)mem_block;
    }

    auto array() const -> array_t& {
        if (Type != json_t::Array) {
            tools::bad_getting_panic("array");
        }
        return *(array_t*)mem_block;
    }

    auto object() const -> object_t& {
        if (Type != json_t::Object) {
            tools::bad_getting_panic("object");
        }
        return *(object_t*)mem_block;
    }

    // 容器功能函数
    // other operator overload
public:
    auto operator[](unsigned long idx) -> json& {
        if (Type == json_t::Null) {
            new (mem_block) array_t(idx + 1);
            Type = json_t::Array;
        } else if (Type != json_t::Array) {
            tools::bad_using_panic("operator[](size_t)", "array");
        }
        // undefined behavior (depend on the array implement)
        return (tools::mut_memcast<array_t>(mem_block))[idx];
    }

    auto operator[](const string_t& key) -> json& {
        if (Type == json_t::Null) {
            new (mem_block) object_t();
            Type = json_t::Object;
        } else if (Type != json_t::Object) {
            tools::bad_using_panic("operator[](string key)", "object");
        }
        // undefined behavior (depend on the dict implement)
        return (tools::mut_memcast<object_t>(mem_block))[key];
    }

    auto operator[](const char (&key)[]) -> json& {
        if (Type == json_t::Null) {
            new (mem_block) object_t();
            Type = json_t::Object;
        } else if (Type != json_t::Object) {
            tools::bad_using_panic("operator[](string literal)", "object");
        }
        // undefined behavior (depend on the dict implement)
        return (tools::mut_memcast<object_t>(mem_block))[key];
    }

    // for dumping function ↓↓↓
private:
    static void string_decode_dump(const string_t& in_str, string_t& to_str) {
        to_str += '"';
        for (const auto& ch : in_str) {
            if (ch >= 0 && ch < 32) {
                to_str += tools::escape_table[(unsigned)ch];
                continue;
            }
            switch (ch) {
            case '"':
                to_str += R"(\")";
            case '\\':
                to_str += R"(\\)";
            case '/':
                to_str += R"(\/)";
            case 0x7F:
                to_str += R"(\u007F)";
            default:
                to_str += ch;
            }
        }
        to_str += '"';
    }

    // dumping
private:
    // dump to string
    void dump(string_t& str, int indent) const noexcept {
        switch (Type) {
        case json_t::Integer:
            str += tools::to_str(tools::memcast<long long>(mem_block));
            break;
        case json_t::Unsign:
            str += tools::to_str(tools::memcast<unsigned long long>(mem_block));
            break;
        case json_t::Float:
            str += tools::to_str(tools::memcast<double>(mem_block));
            break;
        case json_t::Boolean:
            str += tools::memcast<bool>(mem_block) ? "true" : "false";
            break;
        case json_t::Null:
            str += "null";
            break;
        case json_t::String:
            string_decode_dump(tools::memcast<string_t>(mem_block), str);
        default:
            break;
        }
        if (Type == json_t::Array) {
            std::vector<string_t> fields;
            bool line_break = false;
            auto& this_array = tools::memcast<array_t>(mem_block);
            for (auto& item : this_array) {
                if (!line_break && item.Type > json_t::String) {
                    line_break = true;
                }
                fields.push_back({});
                item.dump(fields.back(), indent + 1);
                if (fields.size() != this_array.size()) {
                    fields.back() += ", ";
                }
            }
            str += '[';
            if (!fields.empty()) {
                if (!line_break) {
                    for (auto& item_str : fields) {
                        str += item_str;
                    }
                } else {
                    for (auto& item_str : fields) {
                        str += '\n';
                        str += tools::indent_table[indent+1];
                        str += item_str;
                    }
                    str += '\n';
                    str += tools::indent_table[indent];
                }
            }
            str+=']';
        } else if (Type == json_t::Object) {
            str += "{\n";
            for (auto& [key, val] : tools::memcast<object_t>(mem_block)) {
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
        return;
    }  // function `dump`

    // no pretty dumping (no indent and line break)
    void fast_dump(string_t& str) const noexcept {
        switch (Type) {
        case json_t::Integer:
            str += tools::to_str(tools::memcast<long long>(mem_block));
            break;
        case json_t::Unsign:
            str += tools::to_str(tools::memcast<unsigned long long>(mem_block));
            break;
        case json_t::Float:
            str += tools::to_str(tools::memcast<double>(mem_block));
            break;
        case json_t::Boolean:
            str += tools::memcast<bool>(mem_block) ? "true" : "false";
            break;
        case json_t::Null:
            str += "null";
            break;
        case json_t::String:
            string_decode_dump(tools::memcast<string_t>(mem_block), str);
        default:
            if (Type == json_t::Array) {
                std::vector<string_t> fields;
                auto& this_array = tools::memcast<array_t>(mem_block);
                str += '[';
                for (auto& item : this_array) {
                    item.fast_dump(str);
                    str += ", ";
                }
                str.resize(str.length() - 2);
                str += ']';
            } else if (Type == json_t::Object) {
                str += '{';
                for (auto& [key, val] : tools::memcast<object_t>(mem_block)) {
                    string_decode_dump(key, str);
                    str += ": ";
                    val.fast_dump(str);
                    str += ", ";
                }
                str.resize(str.size() - 2);
                str += '}';
            }
            break;
        }
        return;
    }  // function `fast_dump`

public:
    auto dump() -> string_t {
        string_t ret;
        dump(ret, 0);
        return ret;
    }

    // json parse (most important and complex)
private:
    friend class json_parser;

};  // class json

class json_parser {
private:
    // private parsing function, switch and call some parse_xxx function, be call by public parse
    // and parse_xxx return <json object : error massage>
    static auto parsing_func(const string_t& str, size_t& idx) -> std::pair<json, string_t> {
        json result;
        char ch = str[idx];
        // number
        if ((ch >= '0' && ch <= '9') || ch == '-') {
            auto ret = parse_number(str, idx);
            switch (ret.get<1>()) {
            case json_t::Null:
                return {nullptr, ret.get<2>()};
                break;
            case json_t::Integer:
                return {ret.get<0>(), ""};
                break;
            case json_t::Unsign:
                result = *reinterpret_cast<unsigned long long*>(&ret.get<0>());
                return {std::move(result), ""};
                break;
            case json_t::Float:
                result = *reinterpret_cast<double*>(&ret.get<0>());
                return {std::move(result), ""};
                break;
            default:
                tools::panic("development bug, something bad");  // [remove in release]
                break;
            }
        }
        // clang-format off
        switch (ch) {
        case '"': {
            auto [string, err] = parse_string(str, idx);
            if (!err.empty()) return {"", err};
            return {std::move(string), ""};
        }
        case '[': {
            auto [array, err] = parse_array(str, idx);
            if (!err.empty()) return {{}, err};
            return {std::move(array), ""};
        }
        case '{': {
            auto [object, err] = parse_object(str, idx);
            if(!err.empty()) return {{}, err};
            return {std::move(object), ""};
        }
        case 'n': {
            // is it 'null' ?
            if(idx+3 >= str.length() || str[idx+1]!='u' || str[idx+2]!='l' || str[idx+3]!='l'){
                return {{}, "未预期的字符'n', 未能匹配到关键字'null', 错误"};
            }
            idx+=3;
            return {nullptr, ""};
        }
        case 't': {
            // is it 'true' ?
            if(idx+3 >= str.length() || str[idx+1]!='r' || str[idx+2]!='u' || str[idx+3]!='e'){
                return {{}, "未预期的字符't', 未能匹配到关键字'true', 错误"};
            }
            idx+=3;
            return {true, ""};
        }
        case 'f': {
            // is it 'false' ?
            if(idx+4 >= str.length() || str[idx+1]!='a' || str[idx+2]!='l' || str[idx+3]!='s' || str[idx+4]!='e'){
                return {{}, "未预期的字符'f', 未能匹配到关键字'false', 错误"};
            }
            idx+=4;
            return {false, ""};
        }
        default:
            return {{}, "未预期的字符, 语法错误, 非法的json格式"};
        }
        // clang-format on
        return {};
    }
    static auto parse_string(const string_t& str, size_t& idx) -> std::pair<string_t, string_t> {
        string_t ret;
        ++idx;  // we make sure that the first character is '"'
        for (; idx < str.length(); ++idx) {
            char ch = str[idx];
            if (ch == '"' && str[idx - 1] != '\\') {
                return {ret, ""};
            }
            if (ch == '\n') {
                return {"", "'\"'未闭合, 找到行尾'\n', 非法的`string`"};
            }
            // [todo] ==> 处理复杂的转义和UTF-16
            ret += ch;
        }
        return {"", "'\"'未闭合, 找到EOF, 非法的`string`"};
    }

    static auto parse_number(const string_t& str, size_t& idx) -> tools::tuple<long long, json_t, string_t> {
        bool is_negative = false, find_dot = false, find_e = false;
        string_t temp_num_literal_str;
        char ch = str[idx];
        if (ch == '0') {
            if (idx + 1 < str.length()) {
                if (str[idx + 1] == '.') {
                    temp_num_literal_str += '.';
                    find_dot = true;
                } else if (str[idx + 1] == 'e') {
                    temp_num_literal_str += 'e';
                    find_e = true;
                } else {
                    return {0, json_t::Integer, ""};
                }
            } else {
                return {0, json_t::Integer, ""};
            }
        } else if (ch == '-') {
            if (idx + 1 < str.length()) {
                ch = str[idx + 1];
                if (ch < '0' || ch > '9') {
                    return {0, json_t::Null, "孤立的负号'-', 非法的`number`"};
                }
            } else {
                return {0, json_t::Null, "孤立的负号'-', 发现EOF, 非法的`number`"};
            }
            is_negative = true;
            ++idx;
        }
        for (; idx < str.length(); ++idx) {
            ch = str[idx];
            if (ch >= '0' && ch <= '9') {
                temp_num_literal_str += ch;
                continue;
            }
            switch (ch) {
            case '\n':
                if (temp_num_literal_str.back() == '.' || temp_num_literal_str.back() == 'e') {
                    return {0, json_t::Null, "数字以'.'或'e'结尾, 发现行尾'\n', 非法的`number`"};
                }
                // [todo] ==> 结束, 进行数字字面量解析
                goto parse_string_to_number;
                break;
            case '.':
                if (find_dot || find_e) {
                    return {0, json_t::Null, "尾随多余的'.', 非法的`number`"};
                }
                find_dot = true;
                break;
            case 'e':
                if (find_e) {
                    return {0, json_t::Null, "尾随多余的'e', 非法的`number`"};
                }
                find_e = true;
                break;
            default:
                // 这里可以直接做格式检查的, 但会让状态机的流程变乱, 多解析一次数字也没什么开销, 算了
                // [todo] ==> 结束, 解析
                goto parse_string_to_number;
            }
        }
        if (temp_num_literal_str.back() == '.' || temp_num_literal_str.back() == 'e') {
            return {0, json_t::Null, "数字以'.'或'e'结尾, 发现EOF, 非法的`number`"};
        }
    parse_string_to_number:
        // [todo] ==> 结束, 进行数字字面量解析
        if (is_negative) {
        }
        --idx;  // idx need to stop at the last number_part character, 1.45e100
                //                                                            ^
        return {123, json_t::Integer, ""};
    }

    static auto parse_array(const string_t& str, size_t& idx) -> std::pair<array_t, string_t> {  // [completed]
        ++idx;  // make sure that the first character is '['
        array_t result;
        bool need_comma = false;
        for (; idx < str.length(); ++idx) {
            char ch = str[idx];
            switch (ch) {
            case ' ':
                continue;
            case '\t':
                continue;
            case '\n':
                continue;
            case ',':
                if (need_comma)
                    need_comma = false;
                else
                    return {{}, "预期为'值', 发现冗余的',', 非法的`array`"};
                break;
            case ']':
                if (!need_comma && result.size() != 0) return {{}, "尾随',', 非法的`array`"};
                return {std::move(result), {}};
            default:  // 任何其他字符的情况
                if (need_comma) {
                    return {{}, "发现未预期的字符, 预期为',', 非法的`array`"};
                }
                auto [val, err] = parsing_func(str, idx);
                if (!err.empty()) return {{}, err};
                result.push_back(std::move(val));
                need_comma = true;
            }
        }
        return {{}, "未闭合的']', 发现EOF, 非法的`array`"};
    }

    static auto parse_object(const string_t& str, size_t& idx) -> std::pair<object_t, string_t> {
        ++idx;  // first character must be '{'
        object_t result;
        constexpr int part_key = 0, part_colon = 1, part_val = 2, comma_or_end = 3;
        int at_part = part_key;  // [0]key, [1]colon, [2]value, [3]comma or '}' ending
        std::pair<string_t, json> temp;
        for (; idx < str.length(); ++idx) {
            char ch = str[idx];
            switch (ch) {
            case ' ':
                continue;
            case '\t':
                continue;
            case '\n':
                continue;
            case '}':
                if (at_part != comma_or_end) {
                    if (at_part == part_key && result.size() == 0) {
                        return {result, ""};  // empty json object, like "{}"
                    } else
                        return {{}, "结构不完整, 未预期的'}', 非法的`object`"};
                }
                return {result, ""};
            }
            // clang-format off
            switch (at_part) {
            case part_key: {
                if (ch != '"')
                    return {{}, "预期为'键', 发现异常字符, 非法的`object`"};
                auto [key, err] = parse_string(str, idx);
                if (!err.empty()) return {{}, err};
                temp.first = std::move(key);
                at_part = part_colon;
                break;
            }
            case part_colon:
                if (ch != ':') return {{}, "预期为':', 发现异常字符, 非法的`object`"};
                at_part = part_val;
                break;
            case part_val: {
                auto [val, err] = parsing_func(str, idx);
                if (!err.empty()) return {{}, err};
                temp.second = std::move(val);
                result.insert(std::move(temp));
                at_part = comma_or_end;
                break;
            }
            case comma_or_end:
                if (ch != ',') return {{}, "预期为',', 发现异常字符, 非法的`object`"};
                at_part = part_key;
                break;
            }
            // clang-format on
        }  // end loop for
        return {{}, "结构不完整, 发现EOF, 非法的`object`"};
    }

public:
    // parse json
    static auto parse(const string_t& json_text) -> std::tuple<json, string_t, size_t> {
        size_t idx = 0;
        // skip the space at the start
        for(; idx<json_text.length(); ++idx){
            switch (json_text[idx]) {
            case ' ':
                continue;
            case '\t':
                continue;
            case '\n':
                continue;
            }
            break;
        }
        if(idx==json_text.length())
            return {nullptr, "", -1};
        auto [js, err] = parsing_func(json_text, idx);
        if (!err.empty()) {
            return {nullptr, err, idx};
        } else {
            ++idx; // at a new character we don't parsed
            // skip the space at the end
            for(; idx<json_text.length(); ++idx){
                switch (json_text[idx]) {
                case ' ':
                    continue;
                case '\t':
                    continue;
                case '\n':
                    continue;
                }
                break;
            }
            // not at the end, some redundant characters
            if (idx < json_text.length()) {
                return {nullptr, "完整结构后的多余字符, 非法的json", idx};
            }
            return {std::move(js), "", -1};
        }
    }
        // parse json
    static auto parse(std::fstream json_file) -> std::tuple<json, string_t, size_t> {
        json_file.seekg(0, std::ios::end);
        size_t json_len = (size_t)json_file.tellg();
        json_file.seekg(0, std::ios::beg);
        string_t json_text(json_len, '\0');
        json_file.read(json_text.data(), json_len);
        return parse(json_text);
    }
    static auto read_all(std::fstream json_file) -> string_t {
        json_file.seekg(0, std::ios::end);
        size_t json_len = (size_t)json_file.tellg()+1;
        json_file.seekg(0, std::ios::beg);
        string_t json_text(json_len, '\0');
        json_file.read(json_text.data(), json_len);
        return json_text;
    }
    static auto benchmark(std::fstream json_file) -> void {
        auto json_text = read_all(std::move(json_file));
        for(int i=0; i<10; i++){
            parse(json_text);
        }
    }
};

}  // namespace hai

// clang-format off




using namespace std;
using namespace hai;

int main() {
    // auto [js, err, idx] = json_parser::parse(fstream("./full.json", std::ios::binary));
    json_parser::benchmark(fstream("./full.json", (ios::openmode)(ios::binary | ios::in)));
    // json_parser::read_all()
    // if(!err.empty()){
    //     std::print("{} \033[31mat {}\033[0m", err, idx);
    //     return 1;
    // }
    // std::print("{}\n", js.dump());
    return 0;
}