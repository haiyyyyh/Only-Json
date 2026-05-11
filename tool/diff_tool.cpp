// compare difference between src.cpp and json.hpp (actual library) file
// 比较src.cpp和json.hpp(实际的库)的不同

#include <print>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char* argv[]){
    if(argc!=3) return 1;
    ifstream src("./json.cpp");
    ifstream lib("./header/json.hpp");
    if(!src.is_open() || !lib.is_open()) return -1;
    vector<string> src_buf;
    {
        string temp;
        while(getline(src, temp)){
            src_buf.push_back(temp);
        }
    }
    vector<string> lib_buf;
    {
        string temp;
        while(getline(lib, temp)){
            lib_buf.push_back(temp);
        }
    }
    int l1 = stoi(argv[1]);
    int l2 = stoi(argv[2]);
    for(int i1=l1, i2=l2; i1<src_buf.size() && i2<lib_buf.size(); ++i1, ++i2){
        if(src_buf[i1] != lib_buf[i2]){
            print("\033[31m{}\033[0m\n\033[34m{}\033[0m\n", src_buf[i1], lib_buf[i2]);
        }
    }
}