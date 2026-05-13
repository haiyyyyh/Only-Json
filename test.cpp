#include <json.hpp>
#include <fstream>

using namespace nlohmann;
using namespace std;

auto benchmark(string path) {
        std::ifstream file(path);
        file.seekg(0, std::ios::end);
        size_t json_len = (size_t)file.tellg();
        file.seekg(0, std::ios::beg);
        string str;
        str.resize(json_len);
        file.read(str.data(), json_len);
        for(int i=0; i<10; ++i){
                json::parse(str);
        }
}


int main() {
        benchmark("./test/big2.json");
} 