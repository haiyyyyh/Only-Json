#include "./header/json.hpp"
#include <fstream>

// auto benchmark(const char* path) {
//     std::ifstream file(path);
//     file.seekg(0, std::ios::end);
//     size_t json_len = (size_t)file.tellg();
//     file.seekg(0, std::ios::beg);
//     std::string str;
//     str.resize(json_len);
//     file.read(str.data(), json_len);
//     for(int i=0; i<10; ++i){
//         yyjson_doc* a = yyjson_read(str.c_str(), str.length(), 0);
//         yyjson_doc_free(a);
//     }
// }

using namespace hai;

int main() {
    // benchmark("./test/big2.json");
    json a(std::ifstream("./test/big2.json"));
    a.dump();
}