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


#ifndef TEST_LIBDNF_CONF_CONF_HPP
#define TEST_LIBDNF_CONF_CONF_HPP


#include "libdnf/conf/config_main.hpp"
#include "libdnf/conf/vars.hpp"
#include "libdnf/logger/log_router.hpp"

#include <cppunit/extensions/HelperMacros.h>


class ConfTest : public CppUnit::TestCase {
    CPPUNIT_TEST_SUITE(ConfTest);
    CPPUNIT_TEST(test_config_main);
    CPPUNIT_TEST(test_config_repo);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() override;

    void test_config_main();
    void test_config_repo();

    libdnf::Vars vars;
    libdnf::LogRouter logger;
    libdnf::ConfigMain config;
};


#endif
