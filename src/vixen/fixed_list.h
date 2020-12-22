#ifndef _fixed_list
#define _fixed_list

#include <array>
#include <iterator>

template <class T, std::size_t Capacity = 218> class FixedList
{

  public:
    using value_type = T;

    using size_type = std::size_t;

    using difference_type = std::ptrdiff_t;

    using reference = value_type &;

    using const_reference = const value_type &;

    using pointer = T *;

    using const_pointer = const T *;

    using iterator = typename std::array<T, Capacity>::iterator;

    using const_iterator = typename std::array<T, Capacity>::const_iterator;

    template <class... Args> constexpr void emplace_back(Args &&... args)
    {
        _list[_size++] = value_type(std::forward<Args>(args)...);
    }

    constexpr size_t size() const noexcept
    {
        return _size;
    }

    constexpr bool empty() const noexcept
    {
        return size() == 0;
    }

    constexpr iterator begin() noexcept
    {
        return _list.begin();
    }

    constexpr const_iterator begin() const noexcept
    {
        return _list.begin();
    }

    constexpr iterator end() noexcept
    {
        return _list.begin() + _size;
    }

    constexpr const_iterator end() const noexcept
    {
        return _list.begin() + _size;
    }

    constexpr value_type operator[](size_type position) const
    {
        return _list[position];
    }

  private:
    std::array<T, Capacity> _list;

    size_type _size{};
};

template <class T> constexpr auto begin(FixedList<T> &list) noexcept
{
    return list.begin();
}

template <class T> constexpr auto end(FixedList<T> &list)
{
    return list.begin() + list.size();
}

template <class T> constexpr auto begin(const FixedList<T> &list) noexcept
{
    return list.begin();
}

template <class T> constexpr auto end(const FixedList<T> &list)
{
    return list.begin() + list.size();
}

#endif
