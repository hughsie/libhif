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

#ifndef LIBDNF_UTILS_TEMP_HPP
#define LIBDNF_UTILS_TEMP_HPP

#include <filesystem>
#include <vector>
#include <string>


namespace libdnf::utils {

/// Object that creates and holds a temp directory.
/// The directory gets removed when the object is deleted.
class TempDir {
public:
    /// Creates a temporary directory in the system temporary directory path.
    explicit TempDir(const std::string & prefix);

    /// Creates a temporary directory in `destdir`.
    TempDir(const std::string & destdir, const std::string & prefix);

    TempDir(const TempDir &) = delete;
    TempDir & operator=(const TempDir &) = delete;

    ~TempDir();
    const std::filesystem::path & get_path() const noexcept { return path; }

private:
    std::filesystem::path path;
};

}  // namespace libdnf::utils

#endif  // LIBDNF_UTILS_TEMP_HPP