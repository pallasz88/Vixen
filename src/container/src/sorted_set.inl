#ifndef CONTAINER_SORTED_SET_INL_INCLUDED
#define CONTAINER_SORTED_SET_INL_INCLUDED

#include <algorithm>

namespace container
{

template <class Key, class Comparator, class Allocator>
constexpr SortedSet<Key, Comparator, Allocator>::SortedSet(std::initializer_list<Key> value)
{
    set.resize(value.size());
    std::ranges::copy(value, std::begin(set));
    std::ranges::sort(set);
}

template <class Key, class Comparator, class Allocator>
constexpr SortedSet<Key, Comparator, Allocator>::const_iterator SortedSet<Key, Comparator, Allocator>::find(
    const Key &element) const noexcept
{
    return std::lower_bound(std::begin(set), std::end(set), element);
}

template <class Key, class Comparator, class Allocator>
constexpr void SortedSet<Key, Comparator, Allocator>::insert(const Key &element)
{
    set.push_back(element);
    const auto it = std::lower_bound(std::begin(set), std::end(set), element);
    if (it != std::end(set))
        std::rotate(it, std::end(set) - 1, std::end(set));
}

template <class Key, class Comparator, class Allocator>
constexpr SortedSet<Key, Comparator, Allocator>::iterator SortedSet<Key, Comparator, Allocator>::insert(iterator hint, const Key &element)
{
    set.push_back(element);
    const auto it = std::lower_bound(hint, std::end(set), element);
    if (it != std::end(set))
        std::rotate(it, std::end(set) - 1, std::end(set));
    return it;
}

template <class Key, class Comparator, class Allocator>
constexpr SortedSet<Key, Comparator, Allocator>::iterator SortedSet<Key, Comparator, Allocator>::begin() noexcept
{
    return set.begin();
}

template <class Key, class Comparator, class Allocator>
constexpr SortedSet<Key, Comparator, Allocator>::iterator SortedSet<Key, Comparator, Allocator>::end() noexcept
{
    return set.end();
}

template <class Key, class Comparator, class Allocator>
constexpr SortedSet<Key, Comparator, Allocator>::const_iterator SortedSet<Key, Comparator, Allocator>::cbegin() noexcept
{
    return set.cbegin();
}

template <class Key, class Comparator, class Allocator>
constexpr SortedSet<Key, Comparator, Allocator>::const_iterator SortedSet<Key, Comparator, Allocator>::cend() noexcept
{
    return set.cend();
}

template <class Key, class Comparator, class Allocator>
constexpr SortedSet<Key, Comparator, Allocator>::size_type SortedSet<Key, Comparator, Allocator>::size() const noexcept
{
    return set.size();
}

} // namespace container

#endif // CONTAINER_SORTED_SET_INL_INCLUDED