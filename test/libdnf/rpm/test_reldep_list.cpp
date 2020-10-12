
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

#include "test_reldep_list.hpp"

#include "libdnf/rpm/reldep_list.hpp"
#include "libdnf/utils/temp.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION(ReldepListTest);


void ReldepListTest::test_get() {
    libdnf::rpm::ReldepList list1(sack);
    libdnf::rpm::Reldep a(sack, "python3-labirinto = 4.2.0");
    libdnf::rpm::Reldep b(sack, "(lab-list if labirinto.txt)");
    libdnf::rpm::Reldep c(sack, "(labirinto unless labirinto_c)");

    list1.add(a);
    list1.add(b);
    list1.add(c);

    CPPUNIT_ASSERT(list1.get(0) == a);
    CPPUNIT_ASSERT(list1.get(1).to_string() == "(lab-list if labirinto.txt)");
    CPPUNIT_ASSERT(list1.get_id(2).id == c.get_id().id);
}


void ReldepListTest::test_add() {
    libdnf::rpm::ReldepList list1(sack);
    libdnf::rpm::Reldep a(sack, "python3-labirinto = 4.2.0");
    libdnf::rpm::Reldep b(sack, "(lab-list if labirinto.txt)");

    list1.add(a);
    list1.add(b.get_id());
    list1.add_reldep("delgado > 1.2");
    CPPUNIT_ASSERT(list1.size() == 3);
}


void ReldepListTest::test_size() {
    libdnf::rpm::ReldepList list1(sack);
    CPPUNIT_ASSERT(list1.size() == 0);

    libdnf::rpm::Reldep a(sack, "python3-labirinto = 4.2.0");
    libdnf::rpm::Reldep b(sack, "(lab-list if labirinto.txt)");
    list1.add(a);
    list1.add(b);
    CPPUNIT_ASSERT(list1.size() == 2);
}


void ReldepListTest::test_compare() {
    libdnf::rpm::ReldepList list1(sack);
    libdnf::rpm::ReldepList list2(sack);
    libdnf::rpm::Reldep a(sack, "python3-labirinto = 4.2.0");
    libdnf::rpm::Reldep b(sack, "(lab-list if labirinto.txt)");

    CPPUNIT_ASSERT(list1 == list2);

    list1.add(a);
    CPPUNIT_ASSERT(list1 != list2);

    list2.add(a);
    list1.add(b);
    list2.add(b);
    CPPUNIT_ASSERT(list1 == list2);
}


void ReldepListTest::test_append() {
    libdnf::rpm::Reldep a(sack, "python3-labirinto = 4.2.0");
    libdnf::rpm::Reldep b(sack, "(lab-list if labirinto.txt)");
    libdnf::rpm::Reldep c(sack, "(labirinto unless labirinto_c)");
    libdnf::rpm::Reldep d(sack, "labirinto.txt");

    libdnf::rpm::ReldepList list1(sack);
    list1.add(a);
    list1.add(b);
    list1.add_reldep("delgado > 1.2");


    libdnf::rpm::ReldepList list2(sack);
    list2.add(c);
    list2.add(d);

    list1.append(list2);
    CPPUNIT_ASSERT(list1.size() == 5);
    CPPUNIT_ASSERT(list1.get(0) == a);
    CPPUNIT_ASSERT(list1.get(1) == b);
    CPPUNIT_ASSERT(list1.get(2).to_string() == "delgado > 1.2");
    CPPUNIT_ASSERT(list1.get(3) == c);
    CPPUNIT_ASSERT(list1.get(4) == d);
}

void ReldepListTest::test_iterator() {
    libdnf::rpm::Reldep a(sack, "python3-labirinto = 4.2.0");
    libdnf::rpm::Reldep b(sack, "(lab-list if labirinto.txt)");
    libdnf::rpm::Reldep c(sack, "(labirinto unless labirinto_c)");
    libdnf::rpm::Reldep d(sack, "labirinto.txt");
    std::vector<libdnf::rpm::Reldep> expected;
    std::vector<libdnf::rpm::Reldep> result;
    libdnf::rpm::ReldepList list(sack);

    expected.push_back(a);
    list.add(a);
    expected.push_back(b);
    list.add(b);
    expected.push_back(c);
    list.add(c);
    expected.push_back(d);
    list.add(d);

    // check if begin() points to the first Reldep
    auto beg = list.begin();
    CPPUNIT_ASSERT((*beg) == a);

    // test if increasing an iterator moves to the second Reldep
    ++beg;
    CPPUNIT_ASSERT(*beg == b);

    // test if end() points to Reldep with id == -2
    auto end = list.end();
    CPPUNIT_ASSERT((*end).get_id().id == -2);

    // test iterating the whole Reldep list
    for (auto it = list.begin(); it != list.end(); it++) {
        result.push_back((*it));
    }
    CPPUNIT_ASSERT(result == expected);
}

// add_reldep_with_glob uses libsolvs Dataiterator which needs the actual packages
void ReldepListTest::test_add_reldep_with_glob() {
    add_repo("dnf-ci-fedora");

    libdnf::rpm::ReldepList list(sack);
    list.add_reldep_with_glob("dwm*");
    CPPUNIT_ASSERT(list.size() == 9);
}
