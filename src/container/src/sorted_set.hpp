#ifndef CONTAINER_SORTED_SET_HPP_INCLUDED
#define CONTAINER_SORTED_SET_HPP_INCLUDED

#include <functional>
#include <memory>
#include <vector>

namespace container
{

template <class Key, class Compararator = std::less<Key>, class Allocator = std::allocator<Key>> class SortedSet
{
  public:
    using allocator_type = Allocator;

    using key_type = Key;

    using value_type = Key;

    using size_type = std::size_t;

    using difference_type = std::ptrdiff_t;

    using reference = value_type &;

    using const_reference = const value_type &;

    using pointer = std::allocator_traits<Allocator>::pointer;

    using const_pointer = std::allocator_traits<Allocator>::const_pointer;

    using iterator = std::vector<Key, Allocator>::iterator;

    using const_iterator = std::vector<Key, Allocator>::const_iterator;

    constexpr SortedSet() = default;

    constexpr SortedSet(std::initializer_list<Key> value);

    constexpr void insert(const Key &);

    constexpr iterator insert(iterator, const Key &);

    constexpr const_iterator find(const Key &) const noexcept;

    constexpr iterator begin() noexcept;

    constexpr iterator end() noexcept;

    constexpr const_iterator cbegin() noexcept;

    constexpr const_iterator cend() noexcept;

    constexpr size_type size() const noexcept;

  private:
    std::vector<key_type, allocator_type> set;
};

} // namespace container

#include "sorted_set.inl"

#endif // CONTAINER_SORTED_SET_HPP_INCLUDED