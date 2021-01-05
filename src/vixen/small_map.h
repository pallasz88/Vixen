#ifndef _SMALL_MAP_H
#define _SMALL_MAP_H

#include <cstdint>
#include <array>
#include <utility>
#include <algorithm>
#include <stdexcept>

template <class Key, class Value, std::size_t Size>
class SmallMap
{
public:

    using size_type = std::size_t;

    explicit constexpr SmallMap(std::array<std::pair<Key, Value>, Size> a) : map(a){}

    [[nodiscard]] constexpr Value at(const Key &key) const
    {
        const auto pos = std::find_if(begin(map), end(map), 
            [&key](const auto &value){return key == value.first;});
        if (pos != end(map)) [[likely]]
            return pos->second;
        else
            return {};
    }
private:
    std::array<std::pair<Key, Value>, Size> map;
};

#endif
