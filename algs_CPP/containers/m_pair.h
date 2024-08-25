#pragma once

#include <utility>

namespace m_std
{
template <typename Key_t, typename Value_t>
struct pair
{
    using first_type  = Key_t;
    using second_type = Value_t;

    pair() = default;

    // forwarding constructor
    template <typename Key_t, typename Value_t>
    pair(Key_t&& k, Value_t&& v) :
        first(std::forward<Key_t>(k)), second(std::forward<Value_t>(v))
    {
    }

    friend void swap(pair& _this, pair& _other) noexcept
    {
        using std::swap;
        swap(_this.first, _other.first);
        swap(_this.second, _other.second);
    }

    Key_t   first;
    Value_t second;
};
} // namespace m_std