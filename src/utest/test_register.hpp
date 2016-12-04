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
 * @file utest/test_register.hpp
 *
 * @brief Test register interface
 */

#ifndef UTEST_TEST_SUITE_REGISTER_HPP
#define UTEST_TEST_SUITE_REGISTER_HPP

#include <utest/test_size.hpp>
#include <utest/test_list.hpp>

namespace utest {

class TestRunner;

class TestRegister {
public:
    using iterator = TestListIterator<TestRunner>;

    using const_iterator = TestListIterator<const TestRunner>;

    static TestRegister& get_instance() noexcept;

    void add(TestRunner* test_runner) noexcept;

    iterator begin() noexcept;

    const_iterator begin() const noexcept;

    const_iterator cbegin() const noexcept;

    iterator end() noexcept;

    const_iterator end() const noexcept;

    const_iterator cend() const noexcept;

    TestSize size() const noexcept;
private:
    TestList m_test_runners{};
};

inline auto
TestRegister::begin() noexcept -> iterator {
    return m_test_runners.begin<TestRunner>();
}

inline auto
TestRegister::begin() const noexcept -> const_iterator {
    return m_test_runners.cbegin<TestRunner>();
}

inline auto
TestRegister::cbegin() const noexcept -> const_iterator {
    return m_test_runners.cbegin<TestRunner>();
}

inline auto
TestRegister::end() noexcept -> iterator {
    return m_test_runners.end<TestRunner>();
}

inline auto
TestRegister::end() const noexcept -> const_iterator {
    return m_test_runners.cend<TestRunner>();
}

inline auto
TestRegister::cend() const noexcept -> const_iterator {
    return m_test_runners.cend<TestRunner>();
}

inline TestSize
TestRegister::size() const noexcept {
    return m_test_runners.size();
}

}

#endif /* UTEST_TEST_REGISTER_HPP */
