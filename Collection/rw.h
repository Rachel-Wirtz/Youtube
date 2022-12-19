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

    constexpr auto operator[](const std::size_t index) const noexcept -> const char& {
        return m_Data.at(index);
    }

    constexpr auto operator[](const std::size_t index) noexcept -> char& {
        return m_Data.at(index);
    }

    constexpr auto at(const std::size_t index) const noexcept -> const char& {
        return m_Data.at(index);
    }

    constexpr auto at(const std::size_t index) noexcept -> char& {
        return m_Data.at(index);
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

struct uint24_t {
    static constexpr uint32_t max    = (256 * 256 * 256) - 1;
    static constexpr uint8_t  hibyte = std::endian::native == std::endian::little ? 2 : 0;
    static constexpr uint8_t  lobyte = std::endian::native == std::endian::little ? 0 : 2;

    constexpr uint24_t()                noexcept = default;
    constexpr uint24_t(const uint24_t&) noexcept = default;
    constexpr uint24_t(uint24_t&&)      noexcept = default;

    constexpr uint24_t& operator=(const uint24_t&) noexcept = default;
    constexpr uint24_t& operator=(uint24_t&&)      noexcept = default;

    constexpr uint24_t(uint32_t u) noexcept
        : m_Data{}
    {
        uint32_t wrapped_u32 = u % (max + 1);
        m_Data[hibyte] = wrapped_u32 >> 16;
        m_Data[1]      = (wrapped_u32 >> 8) & 0xFF;
        m_Data[lobyte] = wrapped_u32 & 0xFF;
    }

    constexpr auto operator+=(const uint24_t& other) noexcept -> uint24_t& {
        return (*this) = (*this + other);
    }

    constexpr auto operator-=(const uint24_t& other) noexcept -> uint24_t& {
        return (*this) = (*this - other);
    }

    constexpr auto operator*=(const uint24_t& other) noexcept -> uint24_t& {
        return (*this) = (*this * other);
    }

    constexpr auto operator/=(const uint24_t& other) noexcept -> uint24_t& {
        return (*this) = (*this / other);
    }

    constexpr auto operator%=(const uint24_t& other) noexcept -> uint24_t& {
        return (*this) = (*this % other);
    }

    constexpr auto operator&=(const uint24_t& other) noexcept -> uint24_t& {
        return (*this) = (*this & other);
    }

    constexpr auto operator|=(const uint24_t& other) noexcept -> uint24_t& {
        return (*this) = (*this | other);
    }

    constexpr auto operator^=(const uint24_t& other) noexcept -> uint24_t& {
        return (*this) = (*this ^ other);
    }

    constexpr auto operator<<=(const uint24_t& other) noexcept -> uint24_t& {
        return (*this) = (*this << other);
    }

    constexpr auto operator>>=(const uint24_t& other) noexcept -> uint24_t& {
        return (*this) = (*this >> other);
    }

    constexpr operator uint32_t () const noexcept {
        return ((m_Data[hibyte] << 16) | m_Data[1] << 8 | m_Data[lobyte]);
    }

private:
    std::array<uint8_t, 3> m_Data{};
};

using little_uint8_t  = little_endian<uint8_t>;
using little_int8_t   = little_endian<int8_t>;
using little_uint16_t = little_endian<uint16_t>;
using little_int16_t  = little_endian<int16_t>;
using little_uint32_t = little_endian<uint32_t>;
using little_int32_t  = little_endian<int32_t>;
using little_uint64_t = little_endian<uint64_t>;
using little_int64_t  = little_endian<int64_t>;
using little_float_t  = little_endian<float_t>;
using little_double_t = little_endian<double_t>;
using little_uint24_t = little_endian<uint24_t>;

using big_uint8_t  = big_endian<uint8_t>;
using big_int8_t   = big_endian<int8_t>;
using big_uint16_t = big_endian<uint16_t>;
using big_int16_t  = big_endian<int16_t>;
using big_uint32_t = big_endian<uint32_t>;
using big_int32_t  = big_endian<int32_t>;
using big_uint64_t = big_endian<uint64_t>;
using big_int64_t  = big_endian<int64_t>;
using big_float_t  = big_endian<float_t>;
using big_double_t = big_endian<double_t>;
using big_uint24_t = big_endian<uint24_t>;

RW_NAMESPACE_END