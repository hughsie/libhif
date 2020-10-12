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


#ifndef TEST_LIBDNF_CONF_VARS_HPP
#define TEST_LIBDNF_CONF_VARS_HPP


#include "libdnf/conf/vars.hpp"

#include <cppunit/extensions/HelperMacros.h>


class VarsTest : public CppUnit::TestCase {
    CPPUNIT_TEST_SUITE(VarsTest);
    CPPUNIT_TEST(test_vars);
    CPPUNIT_TEST(test_vars_multiple_dirs);
    CPPUNIT_TEST(test_vars_env);
    CPPUNIT_TEST_SUITE_END();

public:
    void test_vars();
    void test_vars_multiple_dirs();
    void test_vars_env();

    libdnf::Vars vars;
};


#endif
