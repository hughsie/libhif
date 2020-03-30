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

#ifndef LIBDNF_BASE_BASE_HPP
#define LIBDNF_BASE_BASE_HPP

#include "libdnf/conf/config_main.hpp"
#include "libdnf/logger/log_router.hpp"
#include "libdnf/rpm/repo/repo_sack.hpp"

#include <filesystem>
#include <map>

namespace libdnf {


/// Instances of :class:`libdnf::Base` are the central point of functionality supplied by libdnf.
/// An application will typically create a single instance of this class which it will keep for the run-time needed to accomplish its packaging tasks.
/// :class:`.Base` instances are stateful objects owning various data.
class Base {
public:
    void load_config_from_file(const std::filesystem::path & path);
    void load_config_from_file();
    void load_config_from_dir(const std::filesystem::path & path);
    void load_config_from_dir();

    ConfigMain * get_config() { return &config_main; }
    LogRouter * get_logger() { return &log_router; }
    rpm::RepoSack * get_rpm_repo_sack() { return &rpm_repo_sack; }
    std::map<std::string, std::string> * get_substitutions() { return &substitutions; }

private:
    LogRouter log_router;
    ConfigMain config_main;
    std::map<std::string, std::string> substitutions;
    rpm::RepoSack rpm_repo_sack{*this};
};


}  // namespace libdnf

#endif
