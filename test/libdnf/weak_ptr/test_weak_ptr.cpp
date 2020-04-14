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

#include "test_weak_ptr.hpp"

#include "libdnf/utils/weak_ptr.hpp"

#include <memory>
#include <string>
#include <vector>

CPPUNIT_TEST_SUITE_REGISTRATION(WeakPtrTest);


void WeakPtrTest::setUp() {}


void WeakPtrTest::tearDown() {}


// In this test the WeakPtr instances point to data owned by Sack instance.
void WeakPtrTest::test_weak_ptr() {
    class Sack {
    public:
        using DataItemWeakPtr = libdnf::WeakPtr<std::string, false>;

        DataItemWeakPtr add_item_with_return(std::unique_ptr<std::string> && item) {
            auto ret = DataItemWeakPtr(item.get(), &data_guard);
            data.push_back(std::move(item));
            return ret;
        }

    private:
        libdnf::WeakPtrGuard<std::string, false> data_guard;
        std::vector<std::unique_ptr<std::string>>
            data;  // Owns the data set. Objects get deleted when the Sack is deleted.
    };

    auto sack1 = std::make_unique<Sack>();
    auto item1_weak_ptr = sack1->add_item_with_return(std::make_unique<std::string>("sack1_item1"));
    auto item2_weak_ptr = sack1->add_item_with_return(std::make_unique<std::string>("sack1_item2"));

    auto sack2 = std::make_unique<Sack>();
    auto item3_weak_ptr = sack2->add_item_with_return(std::make_unique<std::string>("sack2_item1"));
    auto item4_weak_ptr = sack2->add_item_with_return(std::make_unique<std::string>("sack2_item2"));

    // test access to data managed by WeakPtr (by get() and operator ->)
    CPPUNIT_ASSERT(*item1_weak_ptr.get() == "sack1_item1");
    CPPUNIT_ASSERT(item2_weak_ptr->compare("sack1_item2") == 0);
    CPPUNIT_ASSERT(*item3_weak_ptr.get() == "sack2_item1");
    CPPUNIT_ASSERT(item4_weak_ptr->compare("sack2_item2") == 0);

    // test hase_same_guard() method
    CPPUNIT_ASSERT_EQUAL(item1_weak_ptr.has_same_guard(item1_weak_ptr), true);
    CPPUNIT_ASSERT_EQUAL(item1_weak_ptr.has_same_guard(item2_weak_ptr), true);
    CPPUNIT_ASSERT_EQUAL(item1_weak_ptr.has_same_guard(item3_weak_ptr), false);

    // delete sack2
    sack2.reset();

    // data from sack1 must be still accesible, but access to data from sack2 must throw exception
    CPPUNIT_ASSERT(*item1_weak_ptr.get() == "sack1_item1");
    CPPUNIT_ASSERT(item2_weak_ptr->compare("sack1_item2") == 0);
    CPPUNIT_ASSERT_THROW(*item3_weak_ptr.get() == "sack2_item1", Sack::DataItemWeakPtr::InvalidPtr);
    CPPUNIT_ASSERT_THROW((item4_weak_ptr->compare("sack2_item2") == 0), Sack::DataItemWeakPtr::InvalidPtr);

    // test is_valid() method
    CPPUNIT_ASSERT(item1_weak_ptr.is_valid());
    CPPUNIT_ASSERT(item2_weak_ptr.is_valid());
    CPPUNIT_ASSERT(!item3_weak_ptr.is_valid());
    CPPUNIT_ASSERT(!item4_weak_ptr.is_valid());

    // test copy constructor
    auto item5_weak_ptr(item1_weak_ptr);
    CPPUNIT_ASSERT(*item1_weak_ptr.get() == "sack1_item1");
    CPPUNIT_ASSERT(*item5_weak_ptr.get() == "sack1_item1");

    // there is no move constructor, copy constructor must be used
    auto item6_weak_ptr(std::move(item5_weak_ptr));
    CPPUNIT_ASSERT(*item5_weak_ptr.get() == "sack1_item1");
    CPPUNIT_ASSERT(*item6_weak_ptr.get() == "sack1_item1");

    // test copy assignment operator =
    item3_weak_ptr = item1_weak_ptr;
    CPPUNIT_ASSERT(*item1_weak_ptr.get() == "sack1_item1");
    CPPUNIT_ASSERT(*item3_weak_ptr.get() == "sack1_item1");

    // there is no move assignment operator =, copy assignment must be used
    item4_weak_ptr = std::move(item2_weak_ptr);
    CPPUNIT_ASSERT(*item2_weak_ptr.get() == "sack1_item2");
    CPPUNIT_ASSERT(*item4_weak_ptr.get() == "sack1_item2");

    // test operator ==
    CPPUNIT_ASSERT_EQUAL(item1_weak_ptr == item1_weak_ptr, true);
    CPPUNIT_ASSERT_EQUAL(item1_weak_ptr == item3_weak_ptr, true);
    CPPUNIT_ASSERT_EQUAL(item1_weak_ptr == item4_weak_ptr, false);

    // test operator !=
    CPPUNIT_ASSERT_EQUAL(item1_weak_ptr != item1_weak_ptr, false);
    CPPUNIT_ASSERT_EQUAL(item1_weak_ptr != item3_weak_ptr, false);
    CPPUNIT_ASSERT_EQUAL(item1_weak_ptr != item4_weak_ptr, true);
}


// In this test the WeakPtr instances own DependentItem instances that point to data owned by Sack instance.
void WeakPtrTest::test_weak_ptr_is_owner() {
    struct DependentItem {
        std::string * remote_data;
    };

    class Sack {
    public:
        using DataItemWeakPtr = libdnf::WeakPtr<DependentItem, true>;

        DataItemWeakPtr add_item_with_return(std::unique_ptr<std::string> && item) {
            auto ret = DataItemWeakPtr(new DependentItem{item.get()}, &data_guard);
            data.push_back(std::move(item));
            return ret;
        }

    private:
        libdnf::WeakPtrGuard<DependentItem, true> data_guard;
        std::vector<std::unique_ptr<std::string>>
            data;  // Owns the data set. Objects get deleted when the Sack is deleted.
    };

    auto sack1 = std::make_unique<Sack>();
    auto item1_weak_ptr = sack1->add_item_with_return(std::make_unique<std::string>("sack1_item1"));
    auto item2_weak_ptr = sack1->add_item_with_return(std::make_unique<std::string>("sack1_item2"));

    auto sack2 = std::make_unique<Sack>();
    auto item3_weak_ptr = sack2->add_item_with_return(std::make_unique<std::string>("sack2_item1"));
    auto item4_weak_ptr = sack2->add_item_with_return(std::make_unique<std::string>("sack2_item2"));

    // test access to data managed by WeakPtr (by get() and operator ->)
    CPPUNIT_ASSERT(*item1_weak_ptr.get()->remote_data == "sack1_item1");
    CPPUNIT_ASSERT(*item2_weak_ptr->remote_data == "sack1_item2");
    CPPUNIT_ASSERT(*item3_weak_ptr.get()->remote_data == "sack2_item1");
    CPPUNIT_ASSERT(*item4_weak_ptr->remote_data == "sack2_item2");

    // test hase_same_guard() method
    CPPUNIT_ASSERT_EQUAL(item1_weak_ptr.has_same_guard(item1_weak_ptr), true);
    CPPUNIT_ASSERT_EQUAL(item1_weak_ptr.has_same_guard(item2_weak_ptr), true);
    CPPUNIT_ASSERT_EQUAL(item1_weak_ptr.has_same_guard(item3_weak_ptr), false);

    // delete sack2
    sack2.reset();

    // data from sack1 must be still accesible, but access to data from sack2 must throw exception
    CPPUNIT_ASSERT(*item1_weak_ptr.get()->remote_data == "sack1_item1");
    CPPUNIT_ASSERT(*item2_weak_ptr->remote_data == "sack1_item2");
    CPPUNIT_ASSERT_THROW(*item3_weak_ptr.get()->remote_data == "sack2_item1", Sack::DataItemWeakPtr::InvalidPtr);
    CPPUNIT_ASSERT_THROW(*item4_weak_ptr->remote_data == "sack2_item2", Sack::DataItemWeakPtr::InvalidPtr);

    // test is_valid() method
    CPPUNIT_ASSERT(item1_weak_ptr.is_valid());
    CPPUNIT_ASSERT(item2_weak_ptr.is_valid());
    CPPUNIT_ASSERT(!item3_weak_ptr.is_valid());
    CPPUNIT_ASSERT(!item4_weak_ptr.is_valid());

    // test copy constructor
    auto item5_weak_ptr(item1_weak_ptr);
    CPPUNIT_ASSERT(*item1_weak_ptr->remote_data == "sack1_item1");
    CPPUNIT_ASSERT(*item5_weak_ptr->remote_data == "sack1_item1");

    // there move constructor
    auto item6_weak_ptr(std::move(item5_weak_ptr));
    CPPUNIT_ASSERT_THROW(*item5_weak_ptr->remote_data == "sack1_item1", Sack::DataItemWeakPtr::InvalidPtr);
    CPPUNIT_ASSERT(*item6_weak_ptr->remote_data == "sack1_item1");

    // test copy assignment operator =
    item3_weak_ptr = item1_weak_ptr;
    CPPUNIT_ASSERT(*item1_weak_ptr->remote_data == "sack1_item1");
    CPPUNIT_ASSERT(*item3_weak_ptr->remote_data == "sack1_item1");

    // test move assignment operator =
    item4_weak_ptr = std::move(item2_weak_ptr);
    CPPUNIT_ASSERT_THROW(*item2_weak_ptr->remote_data == "sack1_item2", Sack::DataItemWeakPtr::InvalidPtr);
    CPPUNIT_ASSERT(*item4_weak_ptr->remote_data == "sack1_item2");

    // test operator ==
    // Each WeakPtr instance has its own data. However, the data may depend on the same remote data.
    CPPUNIT_ASSERT_EQUAL(item1_weak_ptr == item1_weak_ptr, true);
    CPPUNIT_ASSERT_EQUAL(item1_weak_ptr->remote_data == item1_weak_ptr->remote_data, true);
    CPPUNIT_ASSERT_EQUAL(item1_weak_ptr == item3_weak_ptr, false);
    CPPUNIT_ASSERT_EQUAL(item1_weak_ptr->remote_data == item3_weak_ptr->remote_data, true);
    CPPUNIT_ASSERT_EQUAL(item1_weak_ptr == item4_weak_ptr, false);
    CPPUNIT_ASSERT_EQUAL(item1_weak_ptr->remote_data == item4_weak_ptr->remote_data, false);

    // test operator !=
    // Each WeakPtr instance has its own data. However, the data may depend on the same remote data.
    CPPUNIT_ASSERT_EQUAL(item1_weak_ptr != item1_weak_ptr, false);
    CPPUNIT_ASSERT_EQUAL(item1_weak_ptr->remote_data != item1_weak_ptr->remote_data, false);
    CPPUNIT_ASSERT_EQUAL(item1_weak_ptr != item3_weak_ptr, true);
    CPPUNIT_ASSERT_EQUAL(item1_weak_ptr->remote_data != item3_weak_ptr->remote_data, false);
    CPPUNIT_ASSERT_EQUAL(item1_weak_ptr != item4_weak_ptr, true);
    CPPUNIT_ASSERT_EQUAL(item1_weak_ptr->remote_data != item4_weak_ptr->remote_data, true);
}
