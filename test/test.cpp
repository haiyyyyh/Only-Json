#include <iostream>
#include "../header/json.hpp"

using namespace hai;

int main() {
    // 从任何类型初始化
    json j1 = "string";

    // 列表初始化
    json j2 = {
        {"pi", 3.141},
        {"happy", true},
        {"name", "Niels"},
        {"nothing", nullptr},
        {"answer", {
            {"everything", 42}
        }},
        {"list", {1, 0, 2}},
        {"object", {
            {"currency", "USD"},
            {"value", 42.99}
        }}
    };
    // 如果形如这种,如果想要array请显式声明,否则会初始化成object
    // json构造为object的初始化列表构造函数优先级更高
    j2 = json::array_t {
        {"don't be a object!!!", "test"},
        {"not a key", 123}
    };

    // 解析初始化
    // 此语法糖来自json.hpp,会返回一个标准json对象,但不报告错误
    json j3 = R"(
        {
            "pi": 3.141,
            "happy": true,
            "name": "Niels",
            "nothing": null,
            "answer": {
                "everything": 42
            },
            "list": [1, 0, 2],
            "object": {
                "currency": "USD",
                "value": 42.99
            }
        }
    )"_json;
    // 从文件解析初始化(赋值操作)
    json j4 = std::ifstream("./test/1.json");
    // 自然,支持赋值为文件
    j4 = std::ifstream("./test/2.json");
    // 判空检查失败,两种条件等价
    if (j4==nullptr) {
        return -1;
    }
    if (j4.type()==json::types::Null) {
        return -1;
    }
    std::string json_text = R"({"string":123})";
    // 保留错误信息的解析方式,更常用
    auto [err, idx] = j4.parse_from(json_text);
    // 检查错误
    if (!err.empty()) {
        std::cout<<hai::check_failed_part(json_text, idx);
        return -1;
    }
    // 或者
    auto [j5, err1, idx1] = json::parse(json_text);

    // 访问
    json j; // it's null
    // 自动创建不存在的键
    j["pi"] = 3.141;
    j["happy"] = true;
    j["name"] = "Niels";
    j["nothing"] = nullptr;
    j["answer"]["everything"] = 42;
    j["list"] = { 1, 0, 2 };
    j["object"] = { {"currency", "USD"}, {"value", 42.99} };

    std::cout<<j.dump()<<'\n'; // dump成格式化字符串
    std::cout<<j.fast_dump()<<'\n'; // 未格式化的字符串(一整行)

    j = {1, 2, 3, 4, 5};
    j += 6;
    // 通过这种方式得到实际对象,可以直接使用和扩展
    for (int i:j.array()) {
        std::cout<<i<<' ';
    }
}