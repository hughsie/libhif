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


#include "test_group.hpp"

#include "utils.hpp"

#include "libdnf/comps/comps.hpp"
#include "libdnf/comps/group/package.hpp"
#include "libdnf/comps/group/query.hpp"
#include "libdnf/utils/format.hpp"

#include <filesystem>
#include <fstream>


CPPUNIT_TEST_SUITE_REGISTRATION(CompsGroupTest);


namespace CPPUNIT_NS {

template <>
struct assertion_traits<libdnf::comps::Package> {
    inline static bool equal(const libdnf::comps::Package & left, const libdnf::comps::Package & right) {
        return left == right;
    }

    inline static std::string toString(const libdnf::comps::Package & package) {
        return libdnf::utils::sformat(
            "{} (type: {}, condition: {})",
            package.get_name(),
            static_cast<int>(package.get_type()),
            package.get_condition());
    }
};

}  // namespace CPPUNIT_NS


using namespace libdnf::comps;


void CompsGroupTest::test_load() {
    add_repo_repomd("repomd-comps-core");

    GroupQuery q_core(base.get_comps()->get_group_sack());
    q_core.filter_installed(false);
    q_core.filter_groupid("core");
    auto core = q_core.get();
    CPPUNIT_ASSERT_EQUAL(std::string("core"), core.get_groupid());
    CPPUNIT_ASSERT_EQUAL(std::string("Core"), core.get_name());
    CPPUNIT_ASSERT_EQUAL(std::string("Kern"), core.get_translated_name("de"));
    CPPUNIT_ASSERT_EQUAL(std::string("Smallest possible installation"), core.get_description());
    CPPUNIT_ASSERT_EQUAL(std::string("Kleinstmögliche Installation"), core.get_translated_description("de"));
    CPPUNIT_ASSERT_EQUAL(std::string("1"), core.get_order());
    CPPUNIT_ASSERT_EQUAL(std::string("it"), core.get_langonly());
    CPPUNIT_ASSERT_EQUAL(false, core.get_uservisible());
    CPPUNIT_ASSERT_EQUAL(false, core.get_default());
    CPPUNIT_ASSERT_EQUAL(false, core.get_installed());

    std::vector<Package> exp_pkgs_core = {
        Package("bash", PackageType::MANDATORY, ""),
        Package("glibc", PackageType::MANDATORY, ""),
        Package("dnf", PackageType::DEFAULT, ""),
        Package("conditional", PackageType::CONDITIONAL, "nonexistent"),
        Package("dnf-plugins-core", PackageType::OPTIONAL, "")};
    CPPUNIT_ASSERT_EQUAL(exp_pkgs_core, core.get_packages());

    add_repo_repomd("repomd-comps-standard");

    GroupQuery q_standard(base.get_comps()->get_group_sack());
    q_standard.filter_installed(false);
    q_standard.filter_groupid("standard");
    auto standard = q_standard.get();
    CPPUNIT_ASSERT_EQUAL(std::string("standard"), standard.get_groupid());
    CPPUNIT_ASSERT_EQUAL(std::string("Standard"), standard.get_name());
    CPPUNIT_ASSERT_EQUAL(std::string("標準"), standard.get_translated_name("ja"));
    CPPUNIT_ASSERT_EQUAL(
        std::string("Common set of utilities that extend the minimal installation."), standard.get_description());
    CPPUNIT_ASSERT_EQUAL(
        std::string("最小限のインストールを拡張するユーティリティの共通セット"),
        standard.get_translated_description("ja"));
    CPPUNIT_ASSERT_EQUAL(std::string("1"), standard.get_order());
    CPPUNIT_ASSERT_EQUAL(std::string(""), standard.get_langonly());
    CPPUNIT_ASSERT_EQUAL(false, standard.get_uservisible());
    CPPUNIT_ASSERT_EQUAL(false, standard.get_default());
    CPPUNIT_ASSERT_EQUAL(false, standard.get_installed());

    std::vector<Package> exp_pkgs_standard = {
        Package("cryptsetup", PackageType::MANDATORY, ""),
        Package("chrony", PackageType::CONDITIONAL, "gnome-control-center"),
        Package("conditional", PackageType::CONDITIONAL, "nonexistent")};
    CPPUNIT_ASSERT_EQUAL(exp_pkgs_standard, standard.get_packages());
}


void CompsGroupTest::test_load_defaults() {
    add_repo_repomd("repomd-comps-core-empty");

    GroupQuery q_core_empty(base.get_comps()->get_group_sack());
    q_core_empty.filter_groupid("core");
    auto core_empty = q_core_empty.get();
    CPPUNIT_ASSERT_EQUAL(std::string("core"), core_empty.get_groupid());
    CPPUNIT_ASSERT_EQUAL(std::string(""), core_empty.get_name());
    CPPUNIT_ASSERT_EQUAL(std::string(""), core_empty.get_translated_name("ja"));
    CPPUNIT_ASSERT_EQUAL(std::string(""), core_empty.get_description());
    CPPUNIT_ASSERT_EQUAL(std::string(""), core_empty.get_translated_description("ja"));
    CPPUNIT_ASSERT_EQUAL(std::string(""), core_empty.get_order());
    CPPUNIT_ASSERT_EQUAL(std::string(""), core_empty.get_langonly());
    CPPUNIT_ASSERT_EQUAL(true, core_empty.get_uservisible());
    CPPUNIT_ASSERT_EQUAL(false, core_empty.get_default());
    CPPUNIT_ASSERT_EQUAL(false, core_empty.get_installed());
    CPPUNIT_ASSERT_EQUAL(0lu, core_empty.get_packages().size());
}


void CompsGroupTest::test_merge() {
    add_repo_repomd("repomd-comps-core");
    add_repo_repomd("repomd-comps-standard");
    // load another definiton of the core group that changes all attributes
    add_repo_repomd("repomd-comps-core-v2");

    GroupQuery q_core2(base.get_comps()->get_group_sack());
    q_core2.filter_groupid("core");
    auto core2 = q_core2.get();
    CPPUNIT_ASSERT_EQUAL(std::string("core"), core2.get_groupid());
    CPPUNIT_ASSERT_EQUAL(std::string("Core v2"), core2.get_name());
    // When attributes are missing in core-v2.xml, original values are kept
    CPPUNIT_ASSERT_EQUAL(std::string("Kern v2"), core2.get_translated_name("de"));
    CPPUNIT_ASSERT_EQUAL(std::string("Smallest possible installation v2"), core2.get_description());
    CPPUNIT_ASSERT_EQUAL(std::string("Kleinstmögliche Installation v2"), core2.get_translated_description("de"));
    CPPUNIT_ASSERT_EQUAL(std::string("2"), core2.get_order());
    CPPUNIT_ASSERT_EQUAL(std::string("de"), core2.get_langonly());
    // When boolean attributes are missing in core-v2.xml, default values are taken
    CPPUNIT_ASSERT_EQUAL(true, core2.get_uservisible());
    CPPUNIT_ASSERT_EQUAL(true, core2.get_default());
    CPPUNIT_ASSERT_EQUAL(false, core2.get_installed());

    std::vector<Package> exp_pkgs_core2 = {
        Package("bash", PackageType::MANDATORY, ""),
        Package("glibc", PackageType::MANDATORY, ""),
        Package("dnf", PackageType::DEFAULT, ""),
        Package("dnf-plugins-core", PackageType::OPTIONAL, "")};
    CPPUNIT_ASSERT_EQUAL(exp_pkgs_core2, core2.get_packages());
}


void CompsGroupTest::test_merge_with_empty() {
    add_repo_repomd("repomd-comps-core");
    add_repo_repomd("repomd-comps-standard");
    // load another definiton of the core group that has all attributes empty
    add_repo_repomd("repomd-comps-core-empty");

    GroupQuery q_core_empty(base.get_comps()->get_group_sack());
    q_core_empty.filter_groupid("core");
    auto core_empty = q_core_empty.get();
    CPPUNIT_ASSERT_EQUAL(std::string("core"), core_empty.get_groupid());
    CPPUNIT_ASSERT_EQUAL(std::string("Core"), core_empty.get_name());
    // attributes are missing in core-empty.xml -> original values are kept
    CPPUNIT_ASSERT_EQUAL(std::string("Kern"), core_empty.get_translated_name("de"));
    CPPUNIT_ASSERT_EQUAL(std::string("Smallest possible installation"), core_empty.get_description());
    CPPUNIT_ASSERT_EQUAL(std::string("Kleinstmögliche Installation"), core_empty.get_translated_description("de"));
    CPPUNIT_ASSERT_EQUAL(std::string("1"), core_empty.get_order());
    CPPUNIT_ASSERT_EQUAL(std::string("it"), core_empty.get_langonly());
    // boolean attributes are missing in core-empty.xml -> default values are taken
    CPPUNIT_ASSERT_EQUAL(true, core_empty.get_uservisible());
    CPPUNIT_ASSERT_EQUAL(false, core_empty.get_default());
    CPPUNIT_ASSERT_EQUAL(false, core_empty.get_installed());
    CPPUNIT_ASSERT_EQUAL(0lu, core_empty.get_packages().size());
}


void CompsGroupTest::test_merge_empty_with_nonempty() {
    // load definiton of the core group that has all attributes empty
    add_repo_repomd("repomd-comps-core-empty");
    add_repo_repomd("repomd-comps-standard");
    // load another definiton of the core group that has all attributes filled
    add_repo_repomd("repomd-comps-core");

    GroupQuery q_core(base.get_comps()->get_group_sack());
    q_core.filter_groupid("core");
    auto core = q_core.get();
    CPPUNIT_ASSERT_EQUAL(std::string("core"), core.get_groupid());
    CPPUNIT_ASSERT_EQUAL(std::string("Core"), core.get_name());
    CPPUNIT_ASSERT_EQUAL(std::string("Kern"), core.get_translated_name("de"));
    CPPUNIT_ASSERT_EQUAL(std::string("Smallest possible installation"), core.get_description());
    CPPUNIT_ASSERT_EQUAL(std::string("Kleinstmögliche Installation"), core.get_translated_description("de"));
    CPPUNIT_ASSERT_EQUAL(std::string("1"), core.get_order());
    CPPUNIT_ASSERT_EQUAL(std::string("it"), core.get_langonly());
    CPPUNIT_ASSERT_EQUAL(false, core.get_uservisible());
    CPPUNIT_ASSERT_EQUAL(false, core.get_default());
    CPPUNIT_ASSERT_EQUAL(false, core.get_installed());

    std::vector<Package> exp_pkgs_core = {
        Package("bash", PackageType::MANDATORY, ""),
        Package("glibc", PackageType::MANDATORY, ""),
        Package("dnf", PackageType::DEFAULT, ""),
        Package("conditional", PackageType::CONDITIONAL, "nonexistent"),
        Package("dnf-plugins-core", PackageType::OPTIONAL, "")};
    CPPUNIT_ASSERT_EQUAL(exp_pkgs_core, core.get_packages());
}


void CompsGroupTest::test_merge_different_translations() {
    add_repo_repomd("repomd-comps-core");
    // load another definiton of the core group with different set of translations
    add_repo_repomd("repomd-comps-core-different-translations");

    GroupQuery q_core(base.get_comps()->get_group_sack());
    q_core.filter_groupid("core");
    auto core = q_core.get();
    CPPUNIT_ASSERT_EQUAL(std::string("core"), core.get_groupid());
    CPPUNIT_ASSERT_EQUAL(std::string("Core"), core.get_name());
    CPPUNIT_ASSERT_EQUAL(std::string("Smallest possible installation"), core.get_description());
    // translations that are missing in repomd-comps-core-different-translations are taken from repomd-comps-core
    CPPUNIT_ASSERT_EQUAL(std::string("Kern"), core.get_translated_name("de"));
    CPPUNIT_ASSERT_EQUAL(std::string("Kleinstmögliche Installation"), core.get_translated_description("de"));
    // translations that are missing in repomd-comps-core are taken from repomd-comps-core-different-translations
    CPPUNIT_ASSERT_EQUAL(std::string("Tuum"), core.get_translated_name("et"));
    CPPUNIT_ASSERT_EQUAL(std::string("Väikseim võimalik paigaldus"), core.get_translated_description("et"));
}


void CompsGroupTest::test_dump() {
    add_repo_repomd("repomd-comps-standard");

    GroupQuery q_standard(base.get_comps()->get_group_sack());
    q_standard.filter_groupid("standard");
    auto standard = q_standard.get();

    auto dump_path = temp->get_path() / "dumped-standard.xml";
    standard.dump(dump_path);

    std::ifstream dumped_stream(dump_path);
    std::string actual;
    actual.assign(std::istreambuf_iterator<char>(dumped_stream), std::istreambuf_iterator<char>());

    std::filesystem::path expected_path =
        PROJECT_SOURCE_DIR "/test/data/repos-repomd/repomd-comps-standard/repodata/comps.xml";
    std::ifstream expected_stream(expected_path);
    std::string expected;
    expected.assign(std::istreambuf_iterator<char>(expected_stream), std::istreambuf_iterator<char>());

    CPPUNIT_ASSERT_EQUAL(expected, actual);
}
