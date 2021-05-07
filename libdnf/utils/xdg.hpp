/*
Copyright (C) 2020 Red Hat, Inc.

This file is part of microdnf: https://github.com/rpm-software-management/libdnf/

Microdnf is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Microdnf is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with microdnf.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef LIBDNF_UTILS_XDG_HPP
#define LIBDNF_UTILS_XDG_HPP

#include <filesystem>

namespace libdnf::utils::xdg {

/// Returns user home directory.
std::filesystem::path get_user_home_dir();

/// Returns user cache directory.
/// A base directory relative to which user specific non-essential data files should be stored.
std::filesystem::path get_user_cache_dir();

/// Returns user configuration directory.
/// A base directory relative to which user specific configuration files should be stored.
std::filesystem::path get_user_config_dir();

/// Returns user data directory.
/// A base directory relative to which user specific data files should be stored.
std::filesystem::path get_user_data_dir();

/// Returns user runtime directory.
/// A base directory relative to which user-specific non-essential runtime files and other file objects
/// (such as sockets, named pipes, ...) should be stored.
std::filesystem::path get_user_runtime_dir();

}  // namespace libdnf::utils::xdg

#endif
