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


#include "test_goal.hpp"

#include "libdnf/base/goal.hpp"
#include <libdnf/rpm/solv_query.hpp>


CPPUNIT_TEST_SUITE_REGISTRATION(BaseGoalTest);

void BaseGoalTest::setUp() {
    RepoFixture::setUp();
    add_repo("dnf-ci-fedora");
}

void BaseGoalTest::test_install() {
    libdnf::Goal goal(base.get());
    goal.add_rpm_install("wget", {}, true, {});
    goal.resolve();
    auto install_set = goal.list_rpm_installs();
    auto reinstall_set = goal.list_rpm_reinstalls();
    auto upgrade_set = goal.list_rpm_upgrades();
    auto downgrade_set = goal.list_rpm_downgrades();
    auto remove_set = goal.list_rpm_removes();
    auto obsoleted_set = goal.list_rpm_obsoleted();
    CPPUNIT_ASSERT_EQUAL(1lu, install_set.size());
    CPPUNIT_ASSERT_EQUAL(install_set[0].get_full_nevra(), std::string("wget-0:1.19.5-5.fc29.x86_64"));
    CPPUNIT_ASSERT_EQUAL(0lu, reinstall_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, upgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, downgrade_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, remove_set.size());
    CPPUNIT_ASSERT_EQUAL(0lu, obsoleted_set.size());
}

