#pragma once

#include <bit>
#include <array>
#include <iterator>

#define RW_NAMESPACE        rw
#define RW_NAMESPACE_BEGIN  namespace RW_NAMESPACE {
#define RW_NAMESPACE_END    }

RW_NAMESPACE_BEGIN

template<typename T>
class byte_array {
public:
    using array_type             = std::array<char, sizeof(T)>;
    using iterator               = typename array_type::iterator;
    using const_iterator         = typename array_type::const_iterator;
    using reverse_iterator       = typename array_type::reverse_iterator;
    using const_reverse_iterator = typename array_type::const_reverse_iterator;

    constexpr byte_array(const T& value) noexcept
        : m_Data(std::bit_cast<array_type>(value))
    {
    }

    constexpr auto data(void) const noexcept -> const char* {
        return m_Data.data();
    }

    constexpr auto data(void) noexcept -> char* {
        return m_Data.data();
    }

    constexpr auto size(void) noexcept -> size_t {
        return m_Data.size();
    }

    constexpr operator T () const noexcept {
        return std::bit_cast<T>(m_Data);
    }

    constexpr auto begin(void) noexcept -> iterator {
        return m_Data.begin();
    }

    constexpr auto end(void) noexcept -> iterator {
        return m_Data.end();
    }

    constexpr auto begin(void) const noexcept -> const_iterator {
        return m_Data.begin();
    }

    constexpr auto end(void) const noexcept -> const_iterator {
        return m_Data.end();
    }

    constexpr auto cbegin(void) const noexcept -> const_iterator {
        return m_Data.cbegin();
    }

    constexpr auto cend(void) const noexcept -> const_iterator {
        return m_Data.cend();
    }

    constexpr auto rbegin(void) noexcept -> reverse_iterator {
        return m_Data.rbegin();
    }

    constexpr auto rend(void) noexcept -> reverse_iterator {
        return m_Data.rend();
    }

    constexpr auto rbegin(void) const noexcept -> const_reverse_iterator {
        return m_Data.rbegin();
    }

    constexpr auto rend(void) const noexcept -> const_reverse_iterator {
        return m_Data.rend();
    }

    constexpr auto crbegin(void) const noexcept -> const_reverse_iterator {
        return m_Data.crbegin();
    }

    constexpr auto crend(void) const noexcept -> const_reverse_iterator {
        return m_Data.crend();
    }

private:
    array_type m_Data{};
};


RW_NAMESPACE_END