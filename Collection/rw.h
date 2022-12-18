#pragma once

#include <bit>
#include <array>
#include <iterator>
#include <functional>
#include <iostream>
#include <fstream>
#include <vector>

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

    constexpr byte_array()                  noexcept = default;
    constexpr byte_array(const byte_array&) noexcept = default;
    constexpr byte_array(byte_array&&)      noexcept = default;

    constexpr byte_array& operator=(const byte_array&) noexcept = default;
    constexpr byte_array& operator=(byte_array&&)      noexcept = default;

    constexpr byte_array(const T& value) noexcept
        : m_Data(std::bit_cast<array_type>(value))
    {
    }

    template<typename ... EmplaceArgs>
    constexpr byte_array(EmplaceArgs&& ... args) noexcept(noexcept(T(EmplaceArgs()...)))
        : m_Data(std::bit_cast<array_type>(T(std::forward<EmplaceArgs>(args)...)))
    {
    }

    constexpr auto data(void) const noexcept -> const char* {
        return m_Data.data();
    }

    constexpr auto data(void) noexcept -> char* {
        return m_Data.data();
    }

    constexpr auto size(void) noexcept -> std::size_t {
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


template<typename T, std::endian Endian> requires std::is_scalar_v<T>
class endian {
    template<typename OtherT, std::endian OtherEndian> requires std::is_scalar_v<OtherT>
    friend class endian;

public:
    constexpr endian() noexcept = default;

    template<std::endian OtherEndian>
    constexpr endian(const endian<T, OtherEndian>& other) noexcept
        : m_Data(other.m_Data)
    {
        this->swap<OtherEndian>();
    }

    template<std::endian OtherEndian>
    constexpr endian(endian<T, OtherEndian>&& other) noexcept
        : m_Data(std::move(other.m_Data))
    {
        this->swap<OtherEndian>();
    }

    constexpr endian(const T& value) noexcept
        : m_Data(value)
    {
        this->swap<std::endian::native>();
    }

    template<std::endian OtherEndian>
    constexpr endian& operator=(const endian<T, OtherEndian>& other) noexcept {
        if (this != &other) {
            m_Data = other.m_Data;
            this->swap<OtherEndian>();
        }
        return *this;
    }

    template<std::endian OtherEndian>
    constexpr endian& operator=(endian<T, OtherEndian>&& other) noexcept {
        if (this != &other) {
            m_Data = std::move(other.m_Data);
            this->swap<OtherEndian>();
        }
        return *this;
    }

    constexpr auto data(void) const noexcept -> const char* {
        return m_Data.data();
    }

    constexpr auto data(void) noexcept -> char* {
        return m_Data.data();
    }

    constexpr auto size(void) const noexcept -> std::size_t {
        return m_Data.size();
    }

    constexpr operator T () const noexcept {
        if constexpr (Endian == std::endian::native) {
            return m_Data;
        }
        else {
            return endian<T, std::endian::native>(*this);
        }
    }

private:
    template<std::endian FromEndian>
    constexpr auto swap(void) noexcept -> void {
        static_assert(
            std::endian::native == std::endian::little || std::endian::native == std::endian::big,
            "Currently only little or big endian platforms are supported by the endian API"
            );

        if constexpr (FromEndian != Endian && sizeof(T) > 1) {
            std::reverse(m_Data.begin(), m_Data.end());
        }
    }

    byte_array<T> m_Data{};
};

template<typename T>
class little_endian : public endian<T, std::endian::little> {
public:
    using endian<T, std::endian::little>::endian;
};

template<typename T>
little_endian(const T&) -> little_endian<T>;

template<typename T>
class big_endian : public endian<T, std::endian::big> {
public:
    using endian<T, std::endian::big>::endian;
};

template<typename T>
big_endian(const T&) -> big_endian<T>;

RW_NAMESPACE_END