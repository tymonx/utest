/*!
 * @copright
 * Copyright (c) 2017, Tymoteusz Blazejczyk
 * All rights reserved.
 *
 * @copright
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * @copright
 * * Redistributions of source code must retain the above copyright notice, this
 *   span of conditions and the following disclaimer.
 *
 * @copright
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this span of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * @copright
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * @copright
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @file utest/test_span.hpp
 *
 * @brief Test span interface
 */

#ifndef UTEST_TEST_SPAN_HPP
#define UTEST_TEST_SPAN_HPP

#include <utest/test_span_iterator.hpp>
#include <utest/test_size.hpp>

#include <cstddef>
#include <limits>
#include <algorithm>
#include <type_traits>

namespace utest {

template<typename T>
class TestSpan {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = TestSize;
    using difference_type = std::ptrdiff_t;
    using iterator = TestSpanIterator<T>;
    using const_iterator = TestSpanIterator<const T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static constexpr size_type npos{std::numeric_limits<size_type>::max()};

    constexpr TestSpan() noexcept = default;

    constexpr TestSpan(std::nullptr_t) noexcept;

    template<typename U = T, typename std::enable_if<
        std::is_const<U>::value>::type> constexpr
    TestSpan(const TestSpan<
            typename std::remove_const<U>::type>& other) noexcept;

    template<typename U, typename = typename std::enable_if<std::is_convertible<
        typename U::pointer, pointer>::value>::type> constexpr
    TestSpan(const U& other) noexcept;

    constexpr TestSpan(pointer ptr, size_type n) noexcept;

    template<typename InputIt>
    TestSpan(InputIt input_first, InputIt input_last) noexcept;

    template<TestSize N> constexpr
    TestSpan(value_type (&arr)[N]) noexcept;

    constexpr TestSpan(const TestSpan& other) noexcept = default;

    constexpr TestSpan(TestSpan&& other) noexcept = default;

    TestSpan& operator=(const TestSpan& other) noexcept = default;

    TestSpan& operator=(TestSpan&& other) noexcept = default;

    constexpr size_type size() const noexcept;

    constexpr size_type length() const noexcept;

    constexpr bool empty() const noexcept;

    constexpr bool operator!() const noexcept;

    constexpr explicit operator bool() const noexcept;

    pointer data() noexcept;

    constexpr const_pointer data() const noexcept;

    reference front() noexcept;

    constexpr const_reference front() const noexcept;

    reference back() noexcept;

    constexpr const_reference back() const noexcept;

    reference operator[](size_type pos) noexcept;

    constexpr const_reference operator[](size_type pos) const noexcept;

    TestSpan first(size_type count) const noexcept;

    TestSpan last(size_type count) const noexcept;

    TestSpan subspan(size_type offset, size_type count = npos) const noexcept;

    iterator begin() noexcept;

    const_iterator begin() const noexcept;

    const_iterator cbegin() const noexcept;

    iterator end() noexcept;

    const_iterator end() const noexcept;

    const_iterator cend() const noexcept;

    reverse_iterator rbegin() noexcept;

    const_reverse_iterator rbegin() const noexcept;

    const_reverse_iterator crbegin() const noexcept;

    reverse_iterator rend() noexcept;

    const_reverse_iterator rend() const noexcept;

    const_reverse_iterator crend() const noexcept;
private:
    pointer m_data{nullptr};
    size_type m_size{0};
};

template<typename T> inline constexpr
TestSpan<T>::TestSpan(std::nullptr_t) noexcept { }

template<typename T> template<typename U, typename std::enable_if<
    std::is_const<U>::value>::type> inline constexpr
TestSpan<T>::TestSpan(const TestSpan<
        typename std::remove_const<U>::type>& other) noexcept :
    m_data{other.data()}, m_size{other.size()}
{ }

template<typename T> template<typename U, typename> inline constexpr
TestSpan<T>::TestSpan(const U& other) noexcept :
    m_data{other.data()}, m_size{other.size()}
{ }

template<typename T> inline constexpr
TestSpan<T>::TestSpan(pointer ptr, size_type n) noexcept :
    m_data{ptr}, m_size{n}
{ }

template<typename T> template<typename InputIt> inline
TestSpan<T>::TestSpan(InputIt input_first, InputIt input_last) noexcept :
    m_data{&(*input_first)}, m_size{std::distance(input_first, input_last)}
{ }

template<typename T> template<TestSize N> inline constexpr
TestSpan<T>::TestSpan(value_type (&arr)[N]) noexcept :
    m_data{arr}, m_size{N}
{ }

template<typename T> inline constexpr auto
TestSpan<T>::size() const noexcept -> size_type {
    return m_size;
}

template<typename T> inline constexpr auto
TestSpan<T>::length() const noexcept -> size_type {
    return m_size;
}

template<typename T> inline constexpr auto
TestSpan<T>::empty() const noexcept -> bool {
    return (0 == m_size);
}

template<typename T> inline constexpr auto
TestSpan<T>::operator!() const noexcept -> bool {
    return (0 == m_size);
}

template<typename T> inline constexpr
TestSpan<T>::operator bool() const noexcept {
    return (0 != m_size);
}

template<typename T> inline auto
TestSpan<T>::data() noexcept -> pointer {
    return m_data;
}

template<typename T> inline constexpr auto
TestSpan<T>::data() const noexcept -> const_pointer {
    return m_data;
}

template<typename T> inline auto
TestSpan<T>::front() noexcept -> reference {
    return *m_data;
}

template<typename T> inline constexpr auto
TestSpan<T>::front() const noexcept -> const_reference {
    return *m_data;
}

template<typename T> inline auto
TestSpan<T>::back() noexcept -> reference {
    return *(m_data + m_size - 1);
}

template<typename T> inline constexpr auto
TestSpan<T>::back() const noexcept -> const_reference {
    return *(m_data + m_size - 1);
}

template<typename T> inline auto
TestSpan<T>::operator[](size_type pos) noexcept -> reference {
    return *(m_data + pos);
}

template<typename T> inline constexpr auto
TestSpan<T>::operator[](size_type pos) const noexcept -> const_reference {
    return *(m_data + pos);
}

template<typename T> inline auto
TestSpan<T>::first(size_type count) const noexcept -> TestSpan {
    if (count > size()) {
        count = size();
    }
    return {data(), count};
}

template<typename T> inline auto
TestSpan<T>::last(size_type count) const noexcept -> TestSpan {
    if (count > size()) {
        count = size();
    }
    return {data() + (size() - count), count};
}

template<typename T> inline auto
TestSpan<T>::subspan(size_type offset,
        size_type count) const noexcept -> TestSpan {
    return last(size() - offset).first(count);
}

template<typename T> inline auto
TestSpan<T>::begin() noexcept -> iterator {
    return iterator{m_data};
}

template<typename T> inline auto
TestSpan<T>::begin() const noexcept -> const_iterator {
    return const_iterator{m_data};
}

template<typename T> inline auto
TestSpan<T>::cbegin() const noexcept -> const_iterator {
    return const_iterator{m_data};
}

template<typename T> inline auto
TestSpan<T>::end() noexcept -> iterator {
    return iterator{m_data + m_size};
}

template<typename T> inline auto
TestSpan<T>::end() const noexcept -> const_iterator {
    return const_iterator{m_data + m_size};
}

template<typename T> inline auto
TestSpan<T>::cend() const noexcept -> const_iterator {
    return const_iterator{m_data + m_size};
}

template<typename T> inline auto
TestSpan<T>::rbegin() noexcept -> reverse_iterator {
    return iterator{m_data + m_size - 1};
}

template<typename T> inline auto
TestSpan<T>::rbegin() const noexcept -> const_reverse_iterator {
    return const_iterator{m_data + m_size - 1};
}

template<typename T> inline auto
TestSpan<T>::crbegin() const noexcept -> const_reverse_iterator {
    return const_iterator{m_data + m_size - 1};
}

template<typename T> inline auto
TestSpan<T>::rend() noexcept -> reverse_iterator {
    return iterator{m_data - 1};
}

template<typename T> inline auto
TestSpan<T>::rend() const noexcept -> const_reverse_iterator {
    return const_iterator{m_data - 1};
}

template<typename T> inline auto
TestSpan<T>::crend() const noexcept -> const_reverse_iterator {
    return const_iterator{m_data - 1};
}

template<typename T1, typename T2> static inline bool
operator==(const TestSpan<T1>& lhs, const TestSpan<T2>& rhs) noexcept {
    return (lhs.size() == rhs.size())
        ? std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin()) : false;
}

template<typename T1, typename T2> static inline bool
operator!=(const TestSpan<T1>& lhs, const TestSpan<T2>& rhs) noexcept {
    return !(lhs == rhs);
}

}

#endif /* UTEST_TEST_SPAN_HPP */
