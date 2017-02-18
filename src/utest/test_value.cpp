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
 * @file utest/test_value.cpp
 *
 * @brief Test value implementation
 */

#include <utest/test_value.hpp>

using utest::TestSpan;
using utest::TestString;
using utest::TestValue;

TestValue::TestValue(const char* str) noexcept :
    m_type{STRING}, m_size{TestString::length(str)}, m_data{str}
{ }

TestString utest::to_string(TestValue::Type type) noexcept {
    TestString str;

    switch(type) {
    case TestValue::NIL:
        str = "nullptr";
        break;
    case TestValue::BOOLEAN:
        str = "boolean";
        break;
    case TestValue::CHAR:
        str = "char";
        break;
    case TestValue::UNSIGNED_CHAR:
        str = "unsigned char";
        break;
    case TestValue::SIGNED_CHAR:
        str = "signed char";
        break;
    case TestValue::UNSIGNED_SHORT:
        str = "unsigned short";
        break;
    case TestValue::SIGNED_SHORT:
        str = "signed short";
        break;
    case TestValue::UNSIGNED_INT:
        str = "unsigned int";
        break;
    case TestValue::SIGNED_INT:
        str = "signed int";
        break;
    case TestValue::UNSIGNED_LONG:
        str = "unsigned long";
        break;
    case TestValue::SIGNED_LONG:
        str = "signed long";
        break;
    case TestValue::UNSIGNED_LONG_LONG:
        str = "unsigned long long";
        break;
    case TestValue::SIGNED_LONG_LONG:
        str = "signed long long";
        break;
    case TestValue::FLOAT:
        str = "float";
        break;
    case TestValue::DOUBLE:
        str = "double";
        break;
    case TestValue::POINTER:
        str = "pointer";
        break;
    case TestValue::ARRAY:
        str = "pointer";
        break;
    case TestValue::OBJECT:
        str = "object";
        break;
    case TestValue::ENUM:
        str = "enum";
        break;
    case TestValue::FUNCTION:
        str = "function";
        break;
    case TestValue::UNION:
        str = "union";
        break;
    case TestValue::STRING:
        str = "string";
        break;
    default:
        break;
    }

    return str;
}

TestString utest::to_string(const TestValue& value,
        const TestSpan<char>& buffer) noexcept {
    TestString str;

    switch (value.type()) {
    case TestValue::NIL:
        str = "null";
        break;
    case TestValue::BOOLEAN:
        str = bool(value) ? "true" : "false";
        break;
    case TestValue::CHAR:
        if (!buffer.empty()) {
            const_cast<char*>(buffer.data())[0] = char(value);
            str = {buffer.data(), 1};
        }
        break;
    case TestValue::UNSIGNED_CHAR:
    case TestValue::UNSIGNED_SHORT:
    case TestValue::UNSIGNED_INT:
    case TestValue::UNSIGNED_LONG:
    case TestValue::UNSIGNED_LONG_LONG:
        str = to_string(std::uintmax_t(value), buffer);
        break;
    case TestValue::SIGNED_SHORT:
    case TestValue::SIGNED_CHAR:
    case TestValue::SIGNED_INT:
    case TestValue::SIGNED_LONG:
    case TestValue::SIGNED_LONG_LONG:
        str = to_string(std::intmax_t(value), buffer);
        break;
    case TestValue::FLOAT:
    case TestValue::DOUBLE:
        str = to_string(double(value), buffer);
        break;
    case TestValue::ENUM:
        str = to_string({value.underlying_type(), value.size(), value.data(),
                std::uintmax_t(value)}, buffer);
        break;
    case TestValue::ARRAY:
    case TestValue::UNION:
    case TestValue::OBJECT:
    case TestValue::POINTER:
    case TestValue::FUNCTION:
        str = to_string(static_cast<const void*>(value), buffer);
        break;
    case TestValue::STRING:
        str = {static_cast<const char*>(value), value.size()};
        break;
    default:
        break;
    }

    return str;
}
