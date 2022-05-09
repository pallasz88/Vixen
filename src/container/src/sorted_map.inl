#ifndef CONTAINER_SORTED_MAP_INL_INCLUDED
#define CONTAINER_SORTED_MAP_INL_INCLUDED

#include <algorithm>

namespace container
{

template <class Key, class Value>
constexpr SortedMap<Key, Value>::SortedMap(std::initializer_list<std::pair<key_type, value_type>> c)
{
    std::transform(std::begin(c), std::end(c), std::inserter(keys, std::begin(keys)), [](auto &pair) { return pair.first; });
    std::ranges::transform(c, std::back_inserter(values), [](auto &pair) { return pair.second; });
}

template <class Key, class Value> Value &SortedMap<Key, Value>::operator[](const Key &key) noexcept
{
    const auto iter = keys.find(key);
    return values[std::distance(keys.cbegin(), iter)];
}

template <class Key, class Value> constexpr SortedMap<Key, Value>::iterator SortedMap<Key, Value>::begin() noexcept
{
}

} // namespace container

#endif