/****************************************************************************************
                                                             _____________________
  https://github.com/haiyyyyh                               |     by haiyyyyh     |
      only-json forward                                     |   version v0.7.0    |
                                                            ``````````````````````
                             _      _
 //|||\\  //\\    \\  ||     \\    //                \\  //||\\   //|||\\  //\\    \\
||    ||  || \\   ||  ||      \\  //                 ||  ||      ||    ||  || \\   ||
||    ||  ||  \\  ||  ||       \\//   ::::::::  _    ||  \\||\\  ||    ||  ||  \\  ||
||    ||  ||   \\ ||  ||        ||              \\   ||  _   ||  ||    ||  ||   \\ ||
\\\||//   \\    \\//  \\\||//   ||               \\\//   \\||//  \\|||//   \\    \\//

                                                                    ---- Bloated IS Sin
     =================
    [|      {↗}      |]
    [|   Only-Json   |]
    [| License : MIT |]
    [|  © haiyyyyh   |]
     ^^^^^^^^^^^^^^^^

    Nothing, but only json

*****************************************************************************************/


#include <map>              // object_t
#include <memory_resource>  // poll
#include <string>           // string_t
#include <vector>           // array_t

#include "base.hpp"



namespace hai {

namespace {

// string, require one argument, the allocator
template <typename Alloc>
using std_string_wrapping = std::basic_string<char, std::char_traits<char>, Alloc>;

// object, require three arguments: keytype, valuetype, allocater
template <typename k, typename v, typename Alloc>
using std_map_wrapping = std::map<k, v, std::less<k>, Alloc>;

}  // namespace

// for only-json basic_json template arguments contract
// wrapped some STL container

using json = basic_json<std_string_wrapping, std::vector, std_map_wrapping, std::allocator>;

template <template <typename> typename Alc>
using json_with_pool = basic_json<std_string_wrapping, std::vector, std_map_wrapping, Alc>;


// some builtin tools

// syntactic sugar
inline auto operator""_json(const char* constr, size_t) -> json {
        auto [temp, err, _] = json::parse(constr);
        unlikely_if(!err.empty()) { return nullptr; }
        return std::move(temp);
}


template <class T>
class pool {
private:
        // 全局静态内存池,也可按需替换为自己喜欢的池
        inline static std::pmr::monotonic_buffer_resource s_pool;

public:
        using value_type = T;

        pool() = default;
        pool(const pool&) noexcept = default;

        template <class U>
        pool(const pool<U>&) noexcept {}

        T* allocate(std::size_t n) { return static_cast<T*>(s_pool.allocate(n * sizeof(T))); }

        void deallocate(T* p, std::size_t n) noexcept { s_pool.deallocate(p, n * sizeof(T)); }

        template <class U>
        bool operator==(const pool<U>&) const noexcept {
                return true;
        }

        template <class U>
        bool operator!=(const pool<U>&) const noexcept {
                return false;
        }
};


// try to improve speed
template <typename K, typename V, typename Alloc>
class linear_map {
private:
        using dataT = std::pair<K, V>;
        std::vector<dataT, Alloc> _data;

public:
        linear_map() = default;
        linear_map(const linear_map&) = default;
        linear_map(linear_map&& other) noexcept : _data(std::move(other._data)) {}
        linear_map(std::initializer_list<dataT> init) : _data(init) {}

        auto operator[](K key) noexcept -> V& {
                for (auto& [_k, _v] : _data) {
                        if (_k == key) {
                                return _v;
                        }
                }
                _data.push_back({std::move(key), {}});
                return _data.back().second();
        }

        auto check_insert(const dataT& new_data) noexcept -> linear_map& {
                for (auto& [_k, _v] : _data) {
                        if (_k == new_data.first) {
                                _v = new_data.second;
                                return *this;
                        }
                }
                _data.push_back(new_data);
                return *this;
        }

        auto check_insert(dataT&& new_data) noexcept -> linear_map& {
                for (auto& [_k, _v] : _data) {
                        if (_k == new_data.first) {
                                _v = std::move(new_data.second);
                                return *this;
                        }
                }
                _data.push_back(std::move(new_data));
                return *this;
        }

        auto insert(const dataT& new_data) noexcept -> linear_map& {
                _data.push_back(new_data);
                return *this;
        }

        auto insert(dataT&& new_data) noexcept -> linear_map& {
                _data.push_back(std::move(new_data));
                return *this;
        }

        auto size() const noexcept -> size_t { return _data.size(); }

        auto data() const noexcept -> std::vector<dataT, Alloc>& { return _data; }

        auto begin() const noexcept { return _data.begin(); }

        auto end() const noexcept { return _data.end(); }

        auto at(size_t idx) const noexcept -> V& { return _data[idx].second(); }
};


}  // namespace hai