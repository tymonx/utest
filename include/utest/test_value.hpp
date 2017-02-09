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
 * @file utest/test_value.hpp
 *
 * @brief Test value interface
 */

#ifndef UTEST_TEST_VALUE_HPP
#define UTEST_TEST_VALUE_HPP

#include <utest/test_string.hpp>
#include <utest/test_number.hpp>

#include <type_traits>

namespace utest {

class TestAssert;

class TestValue {
public:
    template<typename T>
    using enable_string_object = typename std::enable_if<
            std::is_same<typename std::remove_cv<typename T::pointer>::type,
                char*>::value
        , int>::type;

    template<typename T>
    using enable_string_char = typename std::enable_if<
            std::is_same<typename std::remove_cv<T>::type, char*>::value
        , unsigned>::type;

    template<typename T>
    using enable_pointer = typename std::enable_if<
            std::is_pointer<T>::value &&
            !std::is_same<typename std::remove_cv<T>::type, char*>::value
        , long>::type;

    template<typename T>
    using enable_object = typename std::enable_if<
            !std::is_pointer<T>::value &&
            !std::is_integral<T>::value &&
            !std::is_floating_point<T>::value
        , unsigned long>::type;

    enum Type {
        NUMBER,
        STRING,
        OBJECT,
        POINTER
    };

    Type type() const noexcept {
        return m_type;
    }

    template<typename T>
    const T& get() const noexcept;
private:
    friend class TestAssert;

    TestValue(const TestNumber& value) noexcept;

    TestValue(const TestString& value) noexcept;

    template<TestSize N>
    TestValue(const char (&arr)[N]) noexcept;

    template<typename T, enable_string_object<T>>
    TestValue(const T& value) noexcept;

    template<typename T, enable_string_char<T>>
    TestValue(const T& value) noexcept;

    template<typename T, enable_pointer<T>>
    TestValue(const T& value) noexcept;

    template<typename T, enable_object<T>>
    TestValue(const T& value) noexcept;

    Type m_type;

    union {
        const TestNumber m_number;
        const TestString m_string;
        const void* m_object;
    };
};

inline
TestValue::TestValue(const TestNumber& value) noexcept :
    m_type{NUMBER}, m_number{value}
{ }

template<TestSize N> inline
TestValue::TestValue(const char (&arr)[N]) noexcept :
    m_type{STRING}, m_string{arr}
{ }

template<typename T, TestValue::enable_string_object<T>> inline
TestValue::TestValue(const T& value) noexcept :
    m_type{STRING}, m_string{value}
{ }

template<typename T, TestValue::enable_string_char<T>> inline
TestValue::TestValue(const T& value) noexcept :
    m_type{STRING}, m_string{value}
{ }

template<typename T, TestValue::enable_pointer<T>> inline
TestValue::TestValue(const T& value) noexcept :
    m_type{POINTER}, m_object{value}
{ }

template<typename T, TestValue::enable_object<T>> inline
TestValue::TestValue(const T& value) noexcept :
    m_type{OBJECT}, m_object{value}
{ }

template<> inline auto
TestValue::get<TestString>() const noexcept -> const TestString& {
    return m_string;
}

template<> inline auto
TestValue::get<TestNumber>() const noexcept -> const TestNumber& {
    return m_number;
}

template<> inline auto
TestValue::get<const void*>() const noexcept ->
        const void* const& {
    return m_object;
}

}

#endif /* UTEST_TEST_VALUE_HPP */
