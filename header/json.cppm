module;

#include "json.hpp"

export module json;

export namespace hai{
    using hai::json;
    using hai::json_t;
    using hai::parse;
    using hai::operator""_json;
    using hai::string_t;
    using hai::array_t;
    using hai::object_t;
}
