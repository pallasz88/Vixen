#ifndef CONTAINER_SORTED_MAP_HPP_INCLUDED
#define CONTAINER_SORTED_MAP_HPP_INCLUDED

#include "sorted_map.hpp"
#include "sorted_set.hpp"

namespace container
{

template <class Key, class Value> class SortedMap
{
  public:
    using key_type = Key;

    using value_type = Value;

    using size_type = std::size_t;

    using difference_type = std::ptrdiff_t;

    using reference = value_type &;

    using const_reference = const value_type &;

    using pointer = Key *;

    using const_pointer = const Key *;

    using iterator = typename std::vector<Key>::iterator;

    using const_iterator = typename std::vector<Key>::const_iterator;

    constexpr SortedMap() = default;

    constexpr SortedMap(std::initializer_list<std::pair<key_type, value_type>>);

    constexpr iterator begin() noexcept;

    constexpr iterator end() noexcept;

    constexpr const_iterator cbegin() noexcept;

    constexpr const_iterator cend() noexcept;

    value_type &operator[](const key_type &) noexcept;

  private:
    SortedSet<key_type> keys;

    std::vector<value_type> values;
};

} // namespace container

#include "sorted_map.inl"

#endif