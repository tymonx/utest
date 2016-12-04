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
 * @file utest/test_list_item.hpp
 *
 * @brief Test list item interface
 */

#ifndef UTEST_TEST_LIST_ITEM_HPP
#define UTEST_TEST_LIST_ITEM_HPP

namespace utest {

class TestListItem {
public:
    TestListItem* next() noexcept;

    const TestListItem* next() const noexcept;

    template<typename T>
    T* next() noexcept;

    template<typename T>
    const T* next() const noexcept;
protected:
    TestListItem() noexcept = default;

private:
    friend class TestList;

    TestListItem* m_next{nullptr};
};

inline auto
TestListItem::next() noexcept -> TestListItem* {
    return m_next;
}

inline auto
TestListItem::next() const noexcept -> const TestListItem* {
    return m_next;
}

template<typename T> inline auto
TestListItem::next() noexcept -> T* {
    return reinterpret_cast<T*>(m_next);
}

template<typename T> inline auto
TestListItem::next() const noexcept -> const T* {
    return reinterpret_cast<const T*>(m_next);
}

}

#endif /* UTEST_TEST_LIST_ITEM_HPP */
