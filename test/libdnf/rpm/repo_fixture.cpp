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


#include "repo_fixture.hpp"

#include <filesystem>
#include <map>


// Static map (class_name -> cache_dir) that allows re-using cache dirs among test cases in a class.
// Prevents creating solv files over and over again.
static std::map<std::string, std::unique_ptr<libdnf::utils::TempDir>> cache_dirs;


void RepoFixture::add_repo(const std::string & name) {
    // Creates new repository in the repo_sack
    auto repo = repo_sack->new_repo(name);

    // Sets the repo baseurl
    std::filesystem::path repo_path = PROJECT_SOURCE_DIR "/test/libdnf/rpm/repos-data/";
    repo_path /= name;
    repo->get_config().baseurl().set(libdnf::Option::Priority::RUNTIME, "file://" + repo_path.native());

    // Loads repository into rpm::Repo.
    repo->load();

    // Loads rpm::Repo into rpm::SolvSack
    sack->load_repo(*repo.get(),
        libdnf::rpm::SolvSack::LoadRepoFlags::USE_FILELISTS |
        libdnf::rpm::SolvSack::LoadRepoFlags::USE_OTHER |
        libdnf::rpm::SolvSack::LoadRepoFlags::USE_PRESTO |
        libdnf::rpm::SolvSack::LoadRepoFlags::USE_UPDATEINFO
    );
}

void RepoFixture::setUp() {
    temp = std::make_unique<libdnf::utils::TempDir>(
        "libdnf_unittest_",
        std::vector<std::string>{"installroot"}
    );
    base = std::make_unique<libdnf::Base>();

    // set installroot to a temp directory
    base->get_config().installroot().set(libdnf::Option::Priority::RUNTIME, temp->get_path() / "installroot");

    // use the shared cache dir (see cache_dirs comment for more details)
    auto class_name = typeid(*this).name();
    auto it = cache_dirs.find(class_name);
    if (it == cache_dirs.end()) {
        cache_dirs.insert({class_name, std::make_unique<libdnf::utils::TempDir>("libdnf_unittest_")});
    }
    base->get_config().cachedir().set(libdnf::Option::Priority::RUNTIME, cache_dirs.at(class_name)->get_path());

    repo_sack = &(base->get_rpm_repo_sack());
    sack = &(base->get_rpm_solv_sack());
}

void RepoFixture::dump_debugdata() {
    sack->dump_debugdata("debugdata");
}
