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
 * @file utest/test_list.hpp
 *
 * @brief Test list interface
 */

#ifndef UTEST_TEST_LIST_HPP
#define UTEST_TEST_LIST_HPP

#include <utest/test_list_item.hpp>
#include <utest/test_list_iterator.hpp>
#include <utest/test_size.hpp>

namespace utest {

class TestList {
public:
    template<typename T>
    using iterator = TestListIterator<T>;

    template<typename T>
    using const_iterator = TestListIterator<const T>;

    void add(TestListItem* item) noexcept;

    template<typename T>
    iterator<T> begin() noexcept;

    template<typename T>
    const_iterator<T> begin() const noexcept;

    template<typename T>
    const_iterator<T> cbegin() const noexcept;

    template<typename T>
    iterator<T> end() noexcept;

    template<typename T>
    const_iterator<T> end() const noexcept;

    template<typename T>
    const_iterator<T> cend() const noexcept;

    TestSize size() const noexcept;
private:
    TestListItem* m_first{nullptr};
    TestListItem* m_last{nullptr};
    TestSize m_size{0};
};

template<typename T> inline auto
TestList::begin() noexcept -> iterator<T> {
    return iterator<T>(reinterpret_cast<T*>(m_first));
}

template<typename T> inline auto
TestList::begin() const noexcept -> const_iterator<T> {
    return const_iterator<T>(reinterpret_cast<T*>(m_first));
}

template<typename T> inline auto
TestList::cbegin() const noexcept -> const_iterator<T> {
    return const_iterator<T>(reinterpret_cast<T*>(m_first));
}

template<typename T> inline auto
TestList::end() noexcept -> iterator<T> {
    return iterator<T>(nullptr);
}

template<typename T> inline auto
TestList::end() const noexcept -> const_iterator<T> {
    return const_iterator<T>(nullptr);
}

template<typename T> inline auto
TestList::cend() const noexcept -> const_iterator<T> {
    return const_iterator<T>(nullptr);
}

inline auto
TestList::size() const noexcept -> TestSize {
    return m_size;
}

}

#endif /* UTEST_TEST_LIST_HPP */
