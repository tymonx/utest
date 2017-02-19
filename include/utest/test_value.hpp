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

#include <type_traits>

namespace utest {

class TestAssert;

class TestValue {
public:
    using Pointer = const void*;

    template<typename T>
    using enable_union = typename std::enable_if<
            std::is_union<T>::value
        , unsigned char>::type;

    template<typename T>
    using enable_pointer = typename std::enable_if<
            std::is_pointer<T>::value &&
            !std::is_convertible<T, const char*>::value
        , signed char>::type;

    template<typename T>
    using enable_integral = typename std::enable_if<
            std::is_integral<T>::value
        , int>::type;

    template<typename T>
    using enable_floating_point = typename std::enable_if<
            std::is_floating_point<T>::value
        , long>::type;

    template<typename T>
    using enable_array = typename std::enable_if<
            std::is_array<T>::value &&
            !std::is_convertible<T, const char*>::value
        , short>::type;

    template<typename T>
    using enable_enum = typename std::enable_if<
            std::is_enum<T>::value
        , unsigned short>::type;

    template<typename T>
    using enable_object = typename std::enable_if<
            std::is_class<T>::value &&
            !std::is_enum<T>::value &&
            !std::is_array<T>::value &&
            !std::is_union<T>::value &&
            !std::is_pointer<T>::value &&
            !std::is_function<T>::value &&
            !std::is_integral<T>::value &&
            !std::is_floating_point<T>::value &&
            !std::is_convertible<T, const char*>::value
        , long>::type;

    enum Type {
        NIL,
        BOOLEAN,
        CHAR,
        SIGNED_CHAR,
        SIGNED_SHORT,
        SIGNED_INT,
        SIGNED_LONG,
        SIGNED_LONG_LONG,
        UNSIGNED_CHAR,
        UNSIGNED_SHORT,
        UNSIGNED_INT,
        UNSIGNED_LONG,
        UNSIGNED_LONG_LONG,
        FLOAT,
        DOUBLE,
        STRING,
        ARRAY,
        ENUM,
        UNION,
        OBJECT,
        POINTER
    };

    Type type() const noexcept;

    Type underlying_type() const noexcept;

    TestSize size() const noexcept;

    TestSize dimension() const noexcept;

    Pointer data() const noexcept;

    explicit operator std::intmax_t() const noexcept;

    explicit operator std::uintmax_t() const noexcept;

    explicit operator double() const noexcept;

    explicit operator char() const noexcept;

    explicit operator bool() const noexcept;

    explicit operator const char*() const noexcept;

    explicit operator const void*() const noexcept;
private:
    friend class TestAssert;
    friend TestString to_string(const TestValue&, const TestSpan<char>&) noexcept;

    template<typename T>
    static constexpr Type type() noexcept;

    TestValue(Type t, TestSize s, Pointer p, std::uintmax_t value) noexcept;

    TestValue(const std::nullptr_t& value) noexcept;

    TestValue(const TestString& value) noexcept;

    TestValue(const bool& value) noexcept;

    TestValue(const char& value) noexcept;

    TestValue(const char* str) noexcept;

    template<TestSize N>
    TestValue(const char (&str)[N]) noexcept;

    template<typename T, enable_pointer<T> = 0>
    TestValue(const T& str) noexcept;

    template<typename T, enable_integral<T> = 0>
    TestValue(const T& value) noexcept;

    template<typename T, enable_floating_point<T> = 0>
    TestValue(const T& value) noexcept;

    template<typename T, enable_enum<T> = 0>
    TestValue(const T& value) noexcept;

    template<typename T, enable_union<T> = 0>
    TestValue(const T& value) noexcept;

    template<typename T, enable_array<T> = 0>
    TestValue(const T& value) noexcept;

    template<typename T, enable_object<T> = 0>
    TestValue(const T& value) noexcept;

    Type m_type{NIL};
    TestSize m_size{0};
    Pointer m_data{nullptr};
    Type m_underlying_type{NIL};

    union {
        TestSize m_dimension;
        char m_char;
        bool m_bool;
        double m_double;
        std::uintmax_t m_uint;
    };
};

TestString to_string(TestValue::Type type) noexcept;

TestString to_string(const TestValue& value,
        const TestSpan<char>& buffer) noexcept;

inline auto
TestValue::type() const noexcept -> Type {
    return m_type;
}

inline auto
TestValue::underlying_type() const noexcept -> Type {
    return m_underlying_type;
}

inline auto
TestValue::dimension() const noexcept -> TestSize {
    return m_dimension;
}

inline auto
TestValue::size() const noexcept -> TestSize {
    return m_size;
}

inline auto
TestValue::data() const noexcept -> Pointer {
    return m_data;
}

inline
TestValue::operator std::intmax_t() const noexcept {
    return std::intmax_t(m_uint);
}

inline
TestValue::operator std::uintmax_t() const noexcept {
    return m_uint;
}

inline
TestValue::operator double() const noexcept {
    return m_double;
}

inline
TestValue::operator bool() const noexcept {
    return m_bool;
}

inline
TestValue::operator char() const noexcept {
    return m_char;
}

inline
TestValue::operator const void*() const noexcept {
    return data();
}

inline
TestValue::operator const char*() const noexcept {
    return static_cast<const char*>(data());
}

template<> inline constexpr auto
TestValue::type<bool>() noexcept -> Type {
    return BOOLEAN;
}

template<> inline constexpr auto
TestValue::type<char>() noexcept -> Type {
    return CHAR;
}

template<> inline constexpr auto
TestValue::type<signed char>() noexcept -> Type {
    return SIGNED_CHAR;
}

template<> inline constexpr auto
TestValue::type<unsigned char>() noexcept -> Type {
    return UNSIGNED_CHAR;
}

template<> inline constexpr auto
TestValue::type<signed short>() noexcept -> Type {
    return SIGNED_SHORT;
}

template<> inline constexpr auto
TestValue::type<unsigned short>() noexcept -> Type {
    return UNSIGNED_SHORT;
}

template<> inline constexpr auto
TestValue::type<signed int>() noexcept -> Type {
    return SIGNED_INT;
}

template<> inline constexpr auto
TestValue::type<unsigned int>() noexcept -> Type {
    return UNSIGNED_INT;
}

template<> inline constexpr auto
TestValue::type<signed long>() noexcept -> Type {
    return SIGNED_LONG;
}

template<> inline constexpr auto
TestValue::type<unsigned long>() noexcept -> Type {
    return UNSIGNED_LONG;
}

template<> inline constexpr auto
TestValue::type<signed long long>() noexcept -> Type {
    return SIGNED_LONG_LONG;
}

template<> inline constexpr auto
TestValue::type<unsigned long long>() noexcept -> Type {
    return UNSIGNED_LONG_LONG;
}

template<> inline constexpr auto
TestValue::type<float>() noexcept -> Type {
    return FLOAT;
}

template<> inline constexpr auto
TestValue::type<double>() noexcept -> Type {
    return DOUBLE;
}

inline
TestValue::TestValue(Type t, TestSize s, Pointer p,
        std::uintmax_t value) noexcept :
    m_type{t}, m_size{s}, m_data{p}, m_uint{value}
{ }

inline
TestValue::TestValue(const std::nullptr_t& value) noexcept :
    m_type{NIL}, m_size{sizeof(value)}, m_data{&value}
{ }

inline
TestValue::TestValue(const char& value) noexcept :
    m_type{CHAR}, m_size{sizeof(value)}, m_data{&value}, m_char{value}
{ }

inline
TestValue::TestValue(const bool& value) noexcept :
    m_type{BOOLEAN}, m_size{sizeof(value)}, m_data{&value}, m_bool{value}
{ }

inline
TestValue::TestValue(const TestString& value) noexcept :
    m_type{STRING}, m_size{value.size()}, m_data{value.data()}
{ }

template<TestSize N> inline
TestValue::TestValue(const char (&str)[N]) noexcept :
    TestValue{TestString{str}}
{ }

template<typename T, TestValue::enable_integral<T>>
TestValue::TestValue(const T& value) noexcept :
    m_type{type<T>()}, m_size{sizeof(value)}, m_data{&value},
    m_uint{static_cast<std::uintmax_t>(value)}
{ }

template<typename T, TestValue::enable_floating_point<T>>
TestValue::TestValue(const T& value) noexcept :
    m_type{type<T>()}, m_size{sizeof(value)}, m_data{&value},
    m_double{static_cast<double>(value)}
{ }

template<typename T, TestValue::enable_enum<T>>
TestValue::TestValue(const T& value) noexcept :
    m_type{ENUM}, m_size{sizeof(value)}, m_data{&value},
    m_underlying_type{type<typename std::underlying_type<T>::type>()},
    m_uint{std::uintmax_t(value)}
{ }

template<typename T, TestValue::enable_pointer<T>>
TestValue::TestValue(const T& value) noexcept :
    m_type{POINTER}, m_size{sizeof(value)}, m_data{value}
{ }

template<typename T, TestValue::enable_array<T>>
TestValue::TestValue(const T& value) noexcept :
    m_type{ARRAY}, m_size{sizeof(value)}, m_data{value},
    m_dimension{std::rank<T>::value}
{ }

template<typename T, TestValue::enable_union<T>>
TestValue::TestValue(const T& value) noexcept :
    m_type{UNION}, m_size{sizeof(value)}, m_data{&value}
{ }

template<typename T, TestValue::enable_object<T>>
TestValue::TestValue(const T& value) noexcept :
    m_type{OBJECT}, m_size{sizeof(value)}, m_data{&value}
{ }

}

#endif /* UTEST_TEST_VALUE_HPP */
