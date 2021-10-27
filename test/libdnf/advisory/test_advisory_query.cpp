/*
Copyright Contributors to the libdnf project.

This file is part of libdnf: https://github.com/rpm-software-management/libdnf/

Libdnf is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Libdnf is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with libdnf.  If not, see <https://www.gnu.org/licenses/>.
*/


#include "test_advisory_query.hpp"

#include "test/libdnf/utils.hpp"

#include "libdnf/rpm/package_query.hpp"
#include "libdnf/rpm/package_set.hpp"

#include <filesystem>
#include <set>
#include <vector>


CPPUNIT_TEST_SUITE_REGISTRATION(AdvisoryAdvisoryQueryTest);

void AdvisoryAdvisoryQueryTest::setUp() {
    LibdnfTestCase::setUp();
    LibdnfTestCase::add_repo_repomd("repomd-repo1");
}

void AdvisoryAdvisoryQueryTest::test_size() {
    auto adv_query = libdnf::advisory::AdvisoryQuery(base);
    std::vector<std::string> expected = {"DNF-2019-1", "DNF-2020-1", "PKG-NEWER", "PKG-OLDER"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));
}

void AdvisoryAdvisoryQueryTest::test_filter_name() {
    // Tests filter_name method
    libdnf::advisory::AdvisoryQuery adv_query =
        libdnf::advisory::AdvisoryQuery(base).filter_name("*2020-1", libdnf::sack::QueryCmp::GLOB);
    std::vector<std::string> expected = {"DNF-2020-1"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));

    adv_query = libdnf::advisory::AdvisoryQuery(base).filter_name("DNF-20*", libdnf::sack::QueryCmp::GLOB);
    expected = {"DNF-2019-1", "DNF-2020-1"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));

    adv_query = libdnf::advisory::AdvisoryQuery(base).filter_name(
        std::vector<std::string>{"DNF-2019-1", "DNF-2020-1"}, libdnf::sack::QueryCmp::EQ);
    expected = {"DNF-2019-1", "DNF-2020-1"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));
}

void AdvisoryAdvisoryQueryTest::test_filter_type() {
    // Tests filter_type method
    libdnf::advisory::AdvisoryQuery adv_query = libdnf::advisory::AdvisoryQuery(base).filter_type("bugfix");
    std::vector<std::string> expected = {"DNF-2020-1", "PKG-OLDER"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));

    adv_query = libdnf::advisory::AdvisoryQuery(base).filter_type("enhancement");
    expected = {"PKG-NEWER"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));

    adv_query = libdnf::advisory::AdvisoryQuery(base).filter_type(
        std::vector<std::string>{"bugfix", "security"}, libdnf::sack::QueryCmp::EQ);
    expected = {"DNF-2019-1", "DNF-2020-1", "PKG-OLDER"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));
}

void AdvisoryAdvisoryQueryTest::test_filter_packages() {
    // Tests filter_packages method
    libdnf::rpm::PackageQuery pkg_query(base);

    libdnf::advisory::AdvisoryQuery adv_query =
        libdnf::advisory::AdvisoryQuery(base).filter_packages(pkg_query, libdnf::sack::QueryCmp::GT);
    std::vector<std::string> expected = {"PKG-NEWER"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));

    adv_query = libdnf::advisory::AdvisoryQuery(base).filter_packages(pkg_query, libdnf::sack::QueryCmp::GTE);
    expected = {"DNF-2019-1", "PKG-NEWER"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));

    adv_query = libdnf::advisory::AdvisoryQuery(base).filter_packages(pkg_query, libdnf::sack::QueryCmp::EQ);
    expected = {"DNF-2019-1"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));

    adv_query = libdnf::advisory::AdvisoryQuery(base).filter_packages(pkg_query, libdnf::sack::QueryCmp::LTE);
    expected = {"DNF-2019-1", "PKG-OLDER"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));

    adv_query = libdnf::advisory::AdvisoryQuery(base).filter_packages(pkg_query, libdnf::sack::QueryCmp::LT);
    expected = {"PKG-OLDER"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));
}

void AdvisoryAdvisoryQueryTest::test_filter_cve() {
    // Tests filter_cve method
    libdnf::advisory::AdvisoryQuery adv_query =
        libdnf::advisory::AdvisoryQuery(base).filter_CVE("3333", libdnf::sack::QueryCmp::EQ);
    std::vector<std::string> expected = {"DNF-2020-1"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));

    adv_query =
        libdnf::advisory::AdvisoryQuery(base).filter_CVE(std::vector<std::string>{"1111", "3333"}, libdnf::sack::QueryCmp::EQ);
    expected = {"DNF-2019-1", "DNF-2020-1"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));

    adv_query =
        libdnf::advisory::AdvisoryQuery(base).filter_CVE(std::vector<std::string>{"1111", "4444"}, libdnf::sack::QueryCmp::EQ);
    expected = {"DNF-2019-1"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));

    adv_query = libdnf::advisory::AdvisoryQuery(base).filter_CVE("*", libdnf::sack::QueryCmp::GLOB);
    expected = {"DNF-2019-1", "DNF-2020-1"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));
}

void AdvisoryAdvisoryQueryTest::test_filter_bug() {
    // Tests filter_bug method
    libdnf::advisory::AdvisoryQuery adv_query =
        libdnf::advisory::AdvisoryQuery(base).filter_bug("2222", libdnf::sack::QueryCmp::EQ);
    std::vector<std::string> expected = {"DNF-2020-1"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));

    adv_query =
        libdnf::advisory::AdvisoryQuery(base).filter_bug(std::vector<std::string>{"1111", "3333"}, libdnf::sack::QueryCmp::EQ);
    expected = {};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));

    adv_query = libdnf::advisory::AdvisoryQuery(base).filter_bug("*", libdnf::sack::QueryCmp::GLOB);
    expected = {"DNF-2020-1"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));
}

void AdvisoryAdvisoryQueryTest::test_filter_severity() {
    // Tests filter_severity method
    libdnf::advisory::AdvisoryQuery adv_query =
        libdnf::advisory::AdvisoryQuery(base).filter_severity("moderate", libdnf::sack::QueryCmp::EQ);
    std::vector<std::string> expected = {"DNF-2019-1"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));

    adv_query = libdnf::advisory::AdvisoryQuery(base).filter_severity(
        std::vector<std::string>{"moderate", "critical"}, libdnf::sack::QueryCmp::EQ);
    expected = {"DNF-2019-1", "DNF-2020-1"};
    CPPUNIT_ASSERT_EQUAL(expected, to_vector_name_string(adv_query));
}

void AdvisoryAdvisoryQueryTest::test_get_advisory_packages() {
    // Tests get_advisory_packages method
    libdnf::rpm::PackageQuery pkg_query(base);
    // pkg_query contains: pkg-1.2-3.x86_64, pkg-libs-1:1.3-4.x86_64, unresolvable-1:2-3.noarch
    std::vector<libdnf::advisory::AdvisoryPackage> adv_pkgs =
        libdnf::advisory::AdvisoryQuery(base).get_advisory_packages(pkg_query, libdnf::sack::QueryCmp::GTE);
    CPPUNIT_ASSERT_EQUAL(2lu, adv_pkgs.size());
    CPPUNIT_ASSERT_EQUAL(std::string("pkg"), adv_pkgs[0].get_name());
    CPPUNIT_ASSERT_EQUAL(std::string("1.2-3"), adv_pkgs[0].get_evr());
    CPPUNIT_ASSERT_EQUAL(std::string("pkg"), adv_pkgs[1].get_name());
    CPPUNIT_ASSERT_EQUAL(std::string("4.0-1"), adv_pkgs[1].get_evr());

    adv_pkgs = libdnf::advisory::AdvisoryQuery(base).get_advisory_packages(pkg_query, libdnf::sack::QueryCmp::LTE);
    CPPUNIT_ASSERT_EQUAL(2lu, adv_pkgs.size());
    CPPUNIT_ASSERT_EQUAL(std::string("pkg"), adv_pkgs[0].get_name());
    CPPUNIT_ASSERT_EQUAL(std::string("1.2-3"), adv_pkgs[0].get_evr());
    CPPUNIT_ASSERT_EQUAL(std::string("pkg"), adv_pkgs[1].get_name());
    CPPUNIT_ASSERT_EQUAL(std::string("0.1-1"), adv_pkgs[1].get_evr());
}
