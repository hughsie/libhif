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


#include "test_solv_map.hpp"

#include <cstdint>


CPPUNIT_TEST_SUITE_REGISTRATION(SolvMapTest);


void SolvMapTest::setUp() {
    map1 = new libdnf::rpm::solv::SolvMap(32);
    map1->add(libdnf::rpm::PackageId(0));
    map1->add(libdnf::rpm::PackageId(2));
    map1->add(libdnf::rpm::PackageId(28));
    map1->add(libdnf::rpm::PackageId(30));

    map2 = new libdnf::rpm::solv::SolvMap(32);
    map2->add(libdnf::rpm::PackageId(0));
    map2->add(libdnf::rpm::PackageId(1));
}


void SolvMapTest::tearDown() {
    delete map1;
    delete map2;
}


void SolvMapTest::test_add() {
    // test if the maps created in the constructor
    // have the correct bits set

    CPPUNIT_ASSERT(map1->contains(libdnf::rpm::PackageId(0)) == true);
    CPPUNIT_ASSERT(map1->contains(libdnf::rpm::PackageId(1)) == false);
    CPPUNIT_ASSERT(map1->contains(libdnf::rpm::PackageId(2)) == true);

    CPPUNIT_ASSERT(map2->contains(libdnf::rpm::PackageId(0)) == true);
    CPPUNIT_ASSERT(map2->contains(libdnf::rpm::PackageId(1)) == true);
    CPPUNIT_ASSERT(map2->contains(libdnf::rpm::PackageId(2)) == false);

    // add an item that is in the map already
    map1->add(libdnf::rpm::PackageId(0));
    CPPUNIT_ASSERT(map1->contains(libdnf::rpm::PackageId(0)) == true);

    // test invalid ranges
    CPPUNIT_ASSERT_THROW(map1->add(libdnf::rpm::PackageId(-1)), std::out_of_range);
    CPPUNIT_ASSERT_THROW(map1->add(libdnf::rpm::PackageId(33)), std::out_of_range);
}


void SolvMapTest::test_contains() {
    CPPUNIT_ASSERT(map1->contains(libdnf::rpm::PackageId(0)) == true);
    CPPUNIT_ASSERT(map1->contains(libdnf::rpm::PackageId(1)) == false);

    CPPUNIT_ASSERT(map2->contains(libdnf::rpm::PackageId(0)) == true);
    CPPUNIT_ASSERT(map2->contains(libdnf::rpm::PackageId(1)) == true);

    // test invalid ranges
    CPPUNIT_ASSERT(map2->contains(libdnf::rpm::PackageId(-1)) == false);
    CPPUNIT_ASSERT(map2->contains(libdnf::rpm::PackageId(33)) == false);
}


void SolvMapTest::test_remove() {
    // remove an item that is in the map
    CPPUNIT_ASSERT(map1->contains(libdnf::rpm::PackageId(0)) == true);
    map1->remove(libdnf::rpm::PackageId(0));
    CPPUNIT_ASSERT(map1->contains(libdnf::rpm::PackageId(0)) == false);

    // remove an item that is not in the map
    map1->remove(libdnf::rpm::PackageId(0));
    CPPUNIT_ASSERT(map1->contains(libdnf::rpm::PackageId(1)) == false);

    // test invalid ranges
    CPPUNIT_ASSERT_THROW(map1->remove(libdnf::rpm::PackageId(-1)), std::out_of_range);
    CPPUNIT_ASSERT_THROW(map1->remove(libdnf::rpm::PackageId(33)), std::out_of_range);
}


void SolvMapTest::test_map_allocation_range() {
    // allocate map for 25 bits, but in fact map for 4 bytes (32 bits) is allocated
    libdnf::rpm::solv::SolvMap map(25);

    // setting bit 31 works
    map.add(libdnf::rpm::PackageId(31));

    // setting bit 32 does not work (we're indexing from 0)
    CPPUNIT_ASSERT_THROW(map1->add(libdnf::rpm::PackageId(32)), std::out_of_range);
}


void SolvMapTest::test_union() {
    *map2 |= *map1;
    CPPUNIT_ASSERT(map2->contains(libdnf::rpm::PackageId(0)) == true);
    CPPUNIT_ASSERT(map2->contains(libdnf::rpm::PackageId(1)) == true);
    CPPUNIT_ASSERT(map2->contains(libdnf::rpm::PackageId(2)) == true);
    CPPUNIT_ASSERT(map2->contains(libdnf::rpm::PackageId(3)) == false);
}


void SolvMapTest::test_intersection() {
    *map2 &= *map1;
    CPPUNIT_ASSERT(map2->contains(libdnf::rpm::PackageId(0)) == true);
    CPPUNIT_ASSERT(map2->contains(libdnf::rpm::PackageId(1)) == false);
    CPPUNIT_ASSERT(map2->contains(libdnf::rpm::PackageId(2)) == false);
    CPPUNIT_ASSERT(map2->contains(libdnf::rpm::PackageId(3)) == false);
}


void SolvMapTest::test_difference() {
    *map2 -= *map1;
    CPPUNIT_ASSERT(map2->contains(libdnf::rpm::PackageId(0)) == false);
    CPPUNIT_ASSERT(map2->contains(libdnf::rpm::PackageId(1)) == true);
    CPPUNIT_ASSERT(map2->contains(libdnf::rpm::PackageId(2)) == false);
    CPPUNIT_ASSERT(map2->contains(libdnf::rpm::PackageId(3)) == false);
}



void SolvMapTest::test_iterator_empty() {
    std::vector<libdnf::rpm::PackageId> expected = {};
    std::vector<libdnf::rpm::PackageId> result;
    libdnf::rpm::solv::SolvMap map(16);
    for(auto it = map.begin(); it != map.end(); it++) {
        result.push_back(*it);
    }
    CPPUNIT_ASSERT(result == expected);
}


void SolvMapTest::test_iterator_full() {
    std::vector<libdnf::rpm::PackageId> expected = {
        libdnf::rpm::PackageId(0),
        libdnf::rpm::PackageId(1),
        libdnf::rpm::PackageId(2),
        libdnf::rpm::PackageId(3),
        libdnf::rpm::PackageId(4),
        libdnf::rpm::PackageId(5),
        libdnf::rpm::PackageId(6),
        libdnf::rpm::PackageId(7),
        libdnf::rpm::PackageId(8),
        libdnf::rpm::PackageId(9),
        libdnf::rpm::PackageId(10),
        libdnf::rpm::PackageId(11),
        libdnf::rpm::PackageId(12),
        libdnf::rpm::PackageId(13),
        libdnf::rpm::PackageId(14),
        libdnf::rpm::PackageId(15)
    };
    std::vector<libdnf::rpm::PackageId> result;
    
    libdnf::rpm::solv::SolvMap map(16);
    for (int i = 0; i < 16; i++) {
        map.add(libdnf::rpm::PackageId(i));
    }
    for(auto it = map.begin(); it != map.end(); it++) {
        result.push_back(*it);
    }
    CPPUNIT_ASSERT(result == expected);
}


void SolvMapTest::test_iterator_performance_empty() {
    // initialize a map filed with zeros
    constexpr int max = 1000000;
    libdnf::rpm::solv::SolvMap map(max);

    for (int i = 0; i < 500; i++) {
        std::vector<libdnf::rpm::PackageId> result;
        for(auto it = map.begin(); it != map.end(); it++) {
            result.push_back(*it);
        }
    }
}


void SolvMapTest::test_iterator_performance_full() {
    // initialize a map filed with ones
    constexpr int max = 1000000;
    libdnf::rpm::solv::SolvMap map(max);
    memset(map.get_map()->map, 255, static_cast<std::size_t>(map.get_map()->size));

    for (int i = 0; i < 500; i++) {
        std::vector<libdnf::rpm::PackageId> result;
        for(auto it = map.begin(); it != map.end(); it++) {
            result.push_back(*it);
        }
    }
}


void SolvMapTest::test_iterator_performance_4bits() {
    // initialize a map filed with 00001111 bytes
    constexpr int max = 1000000;
    libdnf::rpm::solv::SolvMap map(max);
    memset(map.get_map()->map, 15, static_cast<std::size_t>(map.get_map()->size));

    for (int i = 0; i < 500; i++) {
        std::vector<libdnf::rpm::PackageId> result;
        for(auto it = map.begin(); it != map.end(); it++) {
            result.push_back(*it);
        }
    }
}
