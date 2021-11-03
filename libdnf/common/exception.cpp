/*
Copyright Contributors to the libdnf project.

This file is part of libdnf: https://github.com/rpm-software-management/libdnf/

Libdnf is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 2.1 of the License, or
(at your option) any later version.

Libdnf is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with libdnf.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "libdnf/common/exception.hpp"

#include <algorithm>
#include <charconv>
#include <system_error>


namespace libdnf {


AssertionError::AssertionError(
    const char * assertion,
    const SourceLocation & location,
    const std::string & message)
    : logic_error(message),
      condition(assertion),
      location(location) {}

const char * AssertionError::what() const noexcept {
    try {
        str_what = location.file_name + std::string(":") + std::to_string(location.source_line) + ": " + location.function_name;
        if (condition) {
            str_what += std::string(": Assertion '") + condition + "' failed: ";
        } else {
            str_what += ": Assertion failed: ";
        }
        str_what += logic_error::what();
        return str_what.c_str();
    } catch (...) {
        return logic_error::what();
    }
}

const char * Exception::what() const noexcept {
    try {
        str_what = std::string(get_domain_name()) + "::" + get_name() + ": " + get_description();

        // related data, eg file name
        const char * what = runtime_error::what();
        if (what && what[0] != '\0') {
            str_what += ": ";
            str_what += what;
        }

        return str_what.c_str();
    } catch (...) {
        return runtime_error::what();
    }
}

SystemError::SystemError(int error_code, const std::string & what) : SystemError(error_code, what.c_str()) {}

SystemError::SystemError(int error_code, const char * what) : RuntimeError(what), error_code{error_code} {
    char * it = std::copy_n(NAME_PREFIX, NAME_PREFIX_LEN, name);
    auto res = std::to_chars(it, name + sizeof(name) - 1, get_error_code());
    *res.ptr = '\0';
}

const char * SystemError::get_description() const noexcept {
    try {
        description = std::system_category().default_error_condition(error_code).message();
    } catch (...) {
        return "Unknown error";
    }
    return description.c_str();
}

std::string format(const std::exception & e, std::size_t level) {
    std::string ret(std::string(level, ' ') + e.what() + '\n');
    try {
        std::rethrow_if_nested(e);
    } catch (const std::exception & e) {
        ret += format(e, level + 1);
    } catch (...) {
    }

    return ret;
}

}  // namespace libdnf
