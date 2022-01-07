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

#ifndef LIBDNF_COMMON_SACK_EXCLUDE_FLAGS_HPP
#define LIBDNF_COMMON_SACK_EXCLUDE_FLAGS_HPP

#include <type_traits>


namespace libdnf::sack {

enum class ExcludeFlags : unsigned {
    APPLY_EXCLUDES = 0,
    IGNORE_MODULAR_EXCLUDES = 1 << 0,
    IGNORE_REGULAR_EXCLUDES = 1 << 1,
    IGNORE_EXCLUDES = IGNORE_MODULAR_EXCLUDES | IGNORE_REGULAR_EXCLUDES
};

inline ExcludeFlags operator&(ExcludeFlags lhs, ExcludeFlags rhs) {
    return static_cast<ExcludeFlags>(
        static_cast<std::underlying_type<ExcludeFlags>::type>(lhs) &
        static_cast<std::underlying_type<ExcludeFlags>::type>(rhs));
}

}  // namespace libdnf::sack

#endif  // LIBDNF_COMMON_SACK_EXCLUDE_FLAGS_HPP
