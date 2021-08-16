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

#include <libdnf/base/base.hpp>
#include <libdnf/comps/comps.hpp>
#include <libdnf/solv/pool.hpp>

extern "C" {
#include <solv/pool.h>
#include <solv/repo.h>
#include <solv/repo_comps.h>
}

#include <filesystem>
#include <iostream>

namespace libdnf::comps {


Comps::Comps(libdnf::Base & base) : base{base} {}


Comps::~Comps() {}


void Comps::load_installed() {
    // TODO(pkratoch): load real installed groups
    std::filesystem::path data_path = PROJECT_SOURCE_DIR "/test/libdnf/comps/data/";
    std::string mock_path = data_path / "core.xml";

    libdnf::solv::Pool & spool = get_pool(get_base());
    Pool * pool = *spool;
    Repo * repo = 0;
    Id repoid;
    // Search for repo named @System
    FOR_REPOS(repoid, repo) {
        if (!strcasecmp(repo->name, "@System")) {
            break;
        }
    }
    // If repo named @System doesn't exist, create new repo
    if (!repo) {
        repo = repo_create(pool, "@System");
    }

    FILE * xml_doc = fopen(mock_path.c_str(), "r");
    // Load comps from the file
    // TODO(pkratoch): libsolv doesn't support environments yet
    repo_add_comps(repo, xml_doc, 0);
    fclose(xml_doc);
}


void Comps::load_from_file(const std::string & path, const char * reponame) {
    libdnf::solv::Pool & spool = get_pool(get_base());
    Pool * pool = *spool;
    Repo * repo = 0;
    Repo * tmp_repo = 0;
    Id repoid;
    // Search for repo named reponame
    FOR_REPOS(repoid, tmp_repo) {
        if (!strcasecmp(tmp_repo->name, reponame)) {
            repo = tmp_repo;
            break;
        }
    }
    // If repo named reponame doesn't exist, create new repo
    if (!repo) {
        repo = repo_create(pool, reponame);
    }

    FILE * xml_doc = fopen(path.c_str(), "r");
    // Load comps from the file
    // TODO(pkratoch): libsolv doesn't support environments yet
    repo_add_comps(repo, xml_doc, 0);
    fclose(xml_doc);
}


CompsWeakPtr Comps::get_weak_ptr() { return CompsWeakPtr(this, &data_guard); }


BaseWeakPtr Comps::get_base() const {
    return base.get_weak_ptr();
}


}  // namespace libdnf::comps

