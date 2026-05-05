module;

#include <charconv>
#include <map>
#include <print>
#include <cstring>
#include <cstdio>
#include <fstream>

export module depend;

export namespace std{
    using std::map;
    using std::max_align_t;
    using std::initializer_list;
    using std::enable_if;
    using std::pair;
    using std::print;
    using std::to_chars;
    using std::from_chars;
    using std::fstream;
    using std::ios;
}

export using ::memcpy;
export using ::fprintf;
export using ::stderr;
export using ::abort;