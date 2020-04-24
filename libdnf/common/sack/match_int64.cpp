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

#include "libdnf/common/sack/match_int64.hpp"

#include "libdnf/utils/exception.hpp"

#include <stdexcept>


namespace libdnf::sack {


bool match_int64(int64_t value, QueryCmp cmp, int64_t pattern) {
    bool result = false;
    switch (cmp) {
        case QueryCmp::EQ:
            result = value == pattern;
            break;
        case QueryCmp::NEQ:
            result = value != pattern;
            break;
        case QueryCmp::LT:
            result = value < pattern;
            break;
        case QueryCmp::LTE:
            result = value <= pattern;
            break;
        case QueryCmp::GT:
            result = value > pattern;
            break;
        case QueryCmp::GTE:
            result = value >= pattern;
            break;
        case QueryCmp::IEXACT:
        case QueryCmp::GLOB:
        case QueryCmp::IGLOB:
        case QueryCmp::REGEX:
        case QueryCmp::IREGEX:
        case QueryCmp::CONTAINS:
        case QueryCmp::ICONTAINS:
        case QueryCmp::STARTSWITH:
        case QueryCmp::ISTARTSWITH:
        case QueryCmp::ENDSWITH:
        case QueryCmp::IENDSWITH:
        case QueryCmp::ISNULL:
            throw std::runtime_error("Unsupported operator");
            break;
        case QueryCmp::NOT:
        case QueryCmp::ICASE:
            throw std::runtime_error("Operator flag cannot be used standalone");
            break;
    }
    return result;
}


bool match_int64(int64_t value, QueryCmp cmp, const std::vector<int64_t> & patterns) {
    bool result = false;
    for (auto & pattern : patterns) {
        if (match_int64(value, cmp, pattern)) {
            result = true;
            break;
        }
    }
    return result;
}


bool match_int64(const std::vector<int64_t> & values, QueryCmp cmp, int64_t pattern) {
    bool result = false;
    for (auto & value : values) {
        if (match_int64(value, cmp, pattern)) {
            result = true;
            break;
        }
    }
    return result;
}


bool match_int64(const std::vector<int64_t> & values, QueryCmp cmp, const std::vector<int64_t> & patterns) {
    bool result = false;
    bool found = false;
    for (auto & value : values) {
        for (auto & pattern : patterns) {
            if (match_int64(value, cmp, pattern)) {
                result = true;
                found = true;
                break;
            }
        }
        if (found) {
            break;
        }
    }
    return result;
}


}  // namespace libdnf::sack
