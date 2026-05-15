module;

#include "json.hpp"

export module json;

export namespace hai{
    using hai::basic_json;
    using hai::json;
    using hai::check_failed_part;
    using hai::operator""_json;
}
