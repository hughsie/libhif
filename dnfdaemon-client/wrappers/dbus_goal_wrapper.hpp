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

#ifndef DNFDAEMON_CLIENT_WRAPPERS_DBUS_GOAL_WRAPPER_HPP
#define DNFDAEMON_CLIENT_WRAPPERS_DBUS_GOAL_WRAPPER_HPP

#include "dbus_transaction_package_wrapper.hpp"

#include <libdnf/transaction/transaction_item_action.hpp>

#include <vector>

using namespace libdnf::transaction;

namespace dnfdaemon::client {

class DbusGoalWrapper {
public:
    DbusGoalWrapper(std::vector<dnfdaemon::DbusTransactionItem>);

    std::vector<DbusTransactionPackageWrapper> get_transaction_packages() const { return transaction_packages; };

private:
    std::vector<DbusTransactionPackageWrapper> transaction_packages;
};

}  // namespace dnfdaemon::client

#endif  // DNFDAEMON_CLIENT_WRAPEPRS_DBUS_GOAL_WRAPPER_HPP
