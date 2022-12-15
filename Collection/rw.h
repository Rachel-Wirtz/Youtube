#pragma once

#include <bit>
#include <array>
#include <iterator>
#include <vector>

#define RW_NAMESPACE        rw
#define RW_NAMESPACE_BEGIN  namespace RW_NAMESPACE {
#define RW_NAMESPACE_END    }

RW_NAMESPACE_BEGIN

template<typename T>
using byte_array = std::array<char, sizeof(T)>;

template<typename T>
constexpr auto to_byte_array(const T& from) noexcept -> byte_array<T> {
    return std::bit_cast<byte_array<T>>(from);
}

template<typename T>
constexpr auto from_byte_array(const byte_array<T>& from) noexcept -> T {
    return std::bit_cast<T>(from);
}

template<typename InputIterator, typename OutputIterator, typename T = typename std::iterator_traits<InputIterator>::value_type>
constexpr auto to_byte_array(const InputIterator begin, const InputIterator end, OutputIterator output) -> OutputIterator {
    for (auto it = begin; it != end; ++it, ++output) {
        *output = std::bit_cast<byte_array<T>>(*it);
    }
    return output;
}

template<typename InputIterator, typename OutputIterator, typename T = typename std::iterator_traits<OutputIterator>::value_type>
constexpr auto from_byte_array(const InputIterator begin, const InputIterator end, OutputIterator output) -> OutputIterator {
    for (auto it = begin; it != end; ++it, ++output) {
        *output = std::bit_cast<T>(*it);
    }
    return output;
}

template<typename InputIterator, typename T = typename std::iterator_traits<InputIterator>::value_type>
constexpr auto to_byte_array(const InputIterator begin, const InputIterator end) -> std::vector<byte_array<T>> {
    std::vector<byte_array<T>> result{};
    result.reserve(std::distance(begin, end));
    to_byte_array(begin, end, std::back_inserter(result));
    return result;
}

template<typename OutputIterator, typename T = typename std::iterator_traits<OutputIterator>::value_type>
constexpr auto from_byte_array(const std::vector<byte_array<T>>& from, OutputIterator output) -> OutputIterator {
    // Testii
    return from_byte_array(from.begin(), from.end(), output);
}

RW_NAMESPACE_END