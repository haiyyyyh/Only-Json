#include "./header/json.hpp"
// #include <json.hpp>
#include <fstream>

auto benchmark(const char* path) {
    std::ifstream file(path);
    file.seekg(0, std::ios::end);
    size_t json_len = (size_t)file.tellg();
    file.seekg(0, std::ios::beg);
    std::string str;
    str.resize(json_len);
    file.read(str.data(), json_len);
    for(int i=0; i<10; ++i){
        hai::json_with_pool<hai::pool>::parse(str);
    }
}

int main() {
    benchmark("./test/big2.json");
}