// #include "./header/json.hpp"
#include <yyjson.h>
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
        yyjson_doc* a = yyjson_read(str.c_str(), str.length(), YYJSON_READ_INSITU);
        yyjson_doc_free(a);
    }
}

// using namespace hai;

int main() {
    // benchmark("./test/big2.json");
    benchmark("./test/big2.json");
}