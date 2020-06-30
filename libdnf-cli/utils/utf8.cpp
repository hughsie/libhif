/*
Copyright (C) 2020 Red Hat, Inc.

This file is part of libdnf: https://github.com/rpm-software-management/libdnf/

Libdnf is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Libdnf is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with libdnf.  If not, see <https://www.gnu.org/licenses/>.
*/


#include "utf8.hpp"

#include <clocale>
#include <cstring>
#include <cwchar>
#include <type_traits>


namespace libdnf::cli::utils::utf8 {


/// return length of an utf-8 encoded string
std::size_t length(const std::string & str) {
    std::size_t result = 0;

    if (str.empty()) {
        return result;
    }

    // pointers to the current position (defaults to begin) and the end of the input string
    auto ptr = &str.front();
    auto end = &str.back();

    // multi-byte string state; required by mbrtowc()
    std::mbstate_t state = std::mbstate_t();

    // maximum length of a multibyte character in the current locale
    // expand the macro to a constant once, avoid calling the underlying function in the cycle
    const auto mb_cur_max = MB_CUR_MAX;

    // the wide char read from the input string
    wchar_t wide_char = 0;

    while (ptr <= end) {
        auto bytes = static_cast<int64_t>(std::mbrtowc(&wide_char, ptr, mb_cur_max, &state));
        if (bytes <= 0) {
            break;
        }

        // increase character count
        result += 1;

        // move the input string pointer by number of bytes read into the wide_char
        ptr += bytes;
    }

    return result;
}


/// return printable width of an utf-8 encoded string (considers non-printable and wide characters)
std::size_t width(const std::string & str) {
    std::size_t result = 0;

    if (str.empty()) {
        return result;
    }

    // pointers to the current position (defaults to begin) and the end of the input string
    auto ptr = &str.front();
    auto end = &str.back();

    // multi-byte string state; required by mbrtowc()
    std::mbstate_t state = std::mbstate_t();

    // maximum length of a multibyte character in the current locale
    // expand the macro to a constant once, avoid calling the underlying function in the cycle
    const auto mb_cur_max = MB_CUR_MAX;

    // the wide char read from the input string
    wchar_t wide_char = 0;

    while (ptr <= end) {
        auto bytes = static_cast<int64_t>(std::mbrtowc(&wide_char, ptr, mb_cur_max, &state));
        if (bytes <= 0) {
            break;
        }

        // increase string width
        auto res_part = wcwidth(wide_char);
        if (res_part >= 0) {
            result += static_cast<std::make_unsigned<decltype(res_part)>::type>(res_part);
        }

        // move the input string pointer by number of bytes read into the wide_char
        ptr += bytes;
    }

    return result;
}


/// return an utf-8 sub-string that matches specified character count
std::string substr_length(const std::string & str, std::string::size_type pos, std::string::size_type len) {
    std::string result;

    if (str.empty()) {
        return result;
    }

    // pointers to the current position (defaults to begin) and the end of the input string
    auto ptr = &str.front();
    auto end = &str.back();

    // multi-byte string state; required by mbrtowc()
    std::mbstate_t state = std::mbstate_t();

    // maximum length of a multibyte character in the current locale
    // expand the macro to a constant once, avoid calling the underlying function in the cycle
    const auto mb_cur_max = MB_CUR_MAX;

    // the wide char read from the input string
    wchar_t wide_char = 0;

    while (ptr <= end) {
        auto bytes = static_cast<int64_t>(std::mbrtowc(&wide_char, ptr, mb_cur_max, &state));
        if (bytes <= 0) {
            break;
        }

        // skip first `pos` characters
        if (pos > 0) {
            ptr += bytes;
            pos--;
            continue;
        }

        result.append(ptr, bytes);

        // move the input string pointer by number of bytes read into the wide_char
        ptr += bytes;

        if (len != std::string::npos) {
            len--;
            if (len == 0) {
                break;
            }
        }
    }

    return result;
}


/// return an utf-8 sub-string that matches specified printable width
std::string substr_width(const std::string & str, std::string::size_type pos, std::string::size_type wid) {
    std::string result;

    if (str.empty()) {
        return result;
    }

    // pointers to the current position (defaults to begin) and the end of the input string
    auto ptr = &str.front();
    auto end = &str.back();

    // multi-byte string state; required by mbrtowc()
    std::mbstate_t state = std::mbstate_t();

    // maximum length of a multibyte character in the current locale
    // expand the macro to a constant once, avoid calling the underlying function in the cycle
    const auto mb_cur_max = MB_CUR_MAX;

    // the wide char read from the input string
    wchar_t wide_char = 0;

    while (ptr <= end) {
        auto bytes = static_cast<int64_t>(std::mbrtowc(&wide_char, ptr, mb_cur_max, &state));
        if (bytes <= 0) {
            break;
        }

        // skip first `pos` characters
        if (pos > 0) {
            ptr += bytes;
            pos--;
            continue;
        }

        // increase string width
        if (wid != std::string::npos) {
            auto char_width = wcwidth(wide_char);
            if (char_width >= 0) {
                auto new_wid = wid - static_cast<std::make_unsigned<decltype(char_width)>::type>(char_width);
                if (new_wid > 0) {
                    break;
                } else {
                    wid = static_cast<std::make_unsigned<decltype(new_wid)>::type>(new_wid);
                }
            }
        }
        result.append(ptr, bytes);

        // move the input string pointer by number of bytes read into the wide_char
        ptr += bytes;
    }

    return result;
}


}  // namespace libdnf::cli::utils::utf8
