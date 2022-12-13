#pragma once

#include <bit>
#include <array>

#define RW_NAMESPACE        rw
#define RW_NAMESPACE_BEGIN  namespace RW_NAMESPACE {
#define RW_NAMESPACE_END    }

RW_NAMESPACE_BEGIN

template<typename T>
constexpr auto to_byte_array(const T& from) noexcept -> std::array<char, sizeof(T)> {
    return std::bit_cast<std::array<char, sizeof(T)>>(from);
}

template<typename T>
constexpr auto from_byte_array(const std::array<char, sizeof(T)>& from) noexcept -> T {
    return std::bit_cast<T>(from);
}

RW_NAMESPACE_END