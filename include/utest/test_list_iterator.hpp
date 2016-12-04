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
 *   list of conditions and the following disclaimer.
 *
 * @copright
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
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
 * @file utest/test_list_iterator.hpp
 *
 * @brief Test list iterator interface
 */

#ifndef UTEST_TEST_LIST_ITERATOR_HPP
#define UTEST_TEST_LIST_ITERATOR_HPP

#include <cstddef>
#include <iterator>
#include <type_traits>

namespace utest {

template<typename T>
class TestListIterator {
public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    template<typename U>
    using is_const_t = typename std::enable_if<std::is_const<U>::value>::type;

    template<typename U>
    using remove_const_t = typename std::remove_const<U>::type;

    TestListIterator() noexcept = default;

    TestListIterator(pointer ptr) noexcept;

    TestListIterator(const TestListIterator& other) noexcept = default;

    template<typename U = T, typename = is_const_t<U>>
    TestListIterator(const TestListIterator<remove_const_t<T>>& other) noexcept;

    TestListIterator(TestListIterator&& other) noexcept = default;

    TestListIterator& operator=(const TestListIterator& other) noexcept = default;

    TestListIterator& operator=(TestListIterator&& other) noexcept = default;

    TestListIterator& operator++() noexcept;

    TestListIterator operator++(int) noexcept;

    reference operator*() noexcept;

    reference operator*() const noexcept;

    pointer operator->() noexcept;

    pointer operator->() const noexcept;
private:
    pointer m_ptr{nullptr};
};

template<typename T>
template<typename U, typename> inline
TestListIterator<T>::TestListIterator(
        const TestListIterator<remove_const_t<T>>& other) noexcept :
    m_ptr{other.m_ptr}
{ }

template<typename T>
TestListIterator<T>::TestListIterator(pointer ptr) noexcept :
    m_ptr{ptr}
{ }

template<typename T> inline auto
TestListIterator<T>::operator++() noexcept -> TestListIterator& {
    if (m_ptr) {
        m_ptr = m_ptr->template next<T>();
    }
    return *this;
}

template<typename T> inline auto
TestListIterator<T>::operator++(int) noexcept -> TestListIterator {
    TestListIterator<T> it{m_ptr};
    if (m_ptr) {
        m_ptr = m_ptr->template next<T>();
    }
    return it;
}

template<typename T> inline auto
TestListIterator<T>::operator*() noexcept -> reference {
    return *m_ptr;
}

template<typename T> inline auto
TestListIterator<T>::operator*() const noexcept -> reference {
    return *m_ptr;
}

template<typename T> inline auto
TestListIterator<T>::operator->() noexcept -> pointer {
    return m_ptr;
}

template<typename T> inline auto
TestListIterator<T>::operator->() const noexcept -> pointer {
    return m_ptr;
}

template<typename T, typename U> static inline auto
operator==(const TestListIterator<T>& lhs,
        const TestListIterator<U>& rhs) noexcept -> bool {
    return &(*lhs) == &(*rhs);
}

template<typename T, typename U> static inline auto
operator!=(const TestListIterator<T>& lhs,
        const TestListIterator<U>& rhs) noexcept -> bool {
    return !(lhs == rhs);
}

}

#endif /* UTEST_TEST_LIST_ITERATOR_HPP */
