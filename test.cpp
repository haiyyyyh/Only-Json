#include <fstream>

// using json2 = hai::basic_json<boost::fast_pool_allocator<void>>;

auto benchmark(std::string path) {
    std::ifstream file(path);
    file.seekg(0, std::ios::end);
    size_t json_len = (size_t)file.tellg();
    file.seekg(0, std::ios::beg);
    std::string str;
    str.resize(json_len);
    file.read(str.data(), json_len);
    Json::Reader test;
    Json::Value js;
    for(int i=0; i<10; ++i){
        test.parse(str, js, false);
    }
}


int main() {
    benchmark("./test/big2.json");
    return 0;
}