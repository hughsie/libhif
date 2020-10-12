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

#include "test_repo_query.hpp"

#include "libdnf/base/base.hpp"
#include "libdnf/rpm/repo_sack.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION(RepoQueryTest);


void RepoQueryTest::setUp() {}


void RepoQueryTest::tearDown() {}


void RepoQueryTest::test_query_basics() {
    libdnf::Base base;
    libdnf::rpm::RepoSack repo_sack(base);

    // Creates new repositories in the repo_sack
    auto repo1 = repo_sack.new_repo("repo1");
    auto repo2 = repo_sack.new_repo("repo2");
    auto repo1_updates = repo_sack.new_repo("repo1_updates");
    auto repo2_updates = repo_sack.new_repo("repo2_updates");

    // Tunes configuration of repositories
    repo1->enable();
    repo2->disable();
    repo1_updates->disable();
    repo2_updates->enable();
    repo1->get_config().baseurl().set(libdnf::Option::Priority::RUNTIME, "file:///path/to/repo1");
    repo2->get_config().baseurl().set(libdnf::Option::Priority::RUNTIME, "https://host/path/to/repo2");
    repo1_updates->get_config().baseurl().set(libdnf::Option::Priority::RUNTIME, "https://host/path/to/repo1_updates");
    repo2_updates->get_config().baseurl().set(libdnf::Option::Priority::RUNTIME, "https://host/path/to/repo2_updates");

    // Creates new query on the repo_sack
    auto repo_query = repo_sack.new_query();
    CPPUNIT_ASSERT_EQUAL(repo_query.size(), static_cast<size_t>(4));
    CPPUNIT_ASSERT((repo_query == libdnf::Set{repo1, repo2, repo1_updates, repo2_updates}));

    // Tests ifilter_enabled method
    repo_query.ifilter_enabled(true);
    CPPUNIT_ASSERT((repo_query == libdnf::Set{repo1, repo2_updates}));

    // Tests ifilter_id method
    auto repo_query1 = repo_sack.new_query().ifilter_id(libdnf::sack::QueryCmp::GLOB, "*updates");
    CPPUNIT_ASSERT((repo_query1 == libdnf::Set{repo1_updates, repo2_updates}));

    // Tests ifilter_local method
    repo_query1 = repo_sack.new_query().ifilter_local(false);
    CPPUNIT_ASSERT((repo_query1 == libdnf::Set{repo2, repo1_updates, repo2_updates}));
}
