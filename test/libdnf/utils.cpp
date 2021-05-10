/*
Copyright (C) 2020-2021 Red Hat, Inc.

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


#include "utils.hpp"


std::vector<std::string> to_vector_string(const libdnf::rpm::ReldepList & rdl) {
    std::vector<std::string> res;
    for (int i = 0; i < rdl.size(); ++i) {
        res.emplace_back(rdl.get(i).to_string());
    }
    return res;
}


std::vector<std::string> to_vector_string(const libdnf::rpm::PackageSet & pset) {
    std::vector<std::string> res;
    for (auto pkg : pset) {
        res.emplace_back(pkg.get_full_nevra());
    }
    return res;
}


std::vector<std::string> to_vector_string(const std::vector<libdnf::rpm::Package> & pkg_list) {
    std::vector<std::string> result;
    for (auto & pkg : pkg_list) {
        result.emplace_back(pkg.get_full_nevra());
    }
    return result;
}


std::vector<std::string> to_vector_name_string(const libdnf::advisory::AdvisoryQuery & advisory_query) {
    std::vector<std::string> result;
    for (auto & adv : advisory_query.get_advisories()) {
        result.emplace_back(adv.get_name());
    }
    return result;
}
