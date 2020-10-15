/*
Copyright (C) 2017-2020 Red Hat, Inc.

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


#include "comps_group.hpp"

#include "transaction.hpp"
#include "transaction_item.hpp"

#include "libdnf/transaction/db/comps_group.hpp"
#include "libdnf/transaction/db/comps_group_package.hpp"

#include <algorithm>


namespace libdnf::transaction {


CompsGroup::CompsGroup(Transaction & trans) : TransactionItem::TransactionItem(trans, Type::GROUP) {}


/*
std::vector< TransactionItemPtr >
CompsGroup::getTransactionItemsByPattern(libdnf::utils::SQLite3Ptr conn, const std::string &pattern)
{
    const char *sql = R"**(
        SELECT DISTINCT
            groupid
        FROM
            comps_group
        WHERE
            groupid LIKE ?
            OR name LIKE ?
            OR translated_name LIKE ?
    )**";

    std::vector< TransactionItemPtr > result;

    // HACK: create a private connection to avoid undefined behavior
    // after forking process in Anaconda
    libdnf::utils::SQLite3 privateConn(conn->get_path());
    libdnf::utils::SQLite3::Query query(privateConn, sql);
    std::string pattern_sql = pattern;
    std::replace(pattern_sql.begin(), pattern_sql.end(), '*', '%');
    query.bindv(pattern, pattern, pattern);
    while (query.step() == libdnf::utils::SQLite3::Statement::StepResult::ROW) {
        auto groupid = query.get< std::string >("groupid");
        auto trans_item = getTransactionItem(conn, groupid);
        if (!trans_item) {
            continue;
        }
        result.push_back(trans_item);
    }
    return result;
}
*/

CompsGroupPackage & CompsGroup::new_package() {
    auto pkg = new CompsGroupPackage(*this);
    auto pkg_ptr = std::unique_ptr<CompsGroupPackage>(std::move(pkg));
    // TODO(dmach): following lines are not thread-safe
    packages.push_back(std::move(pkg_ptr));
    return *packages.back();
}


CompsGroupPackage::CompsGroupPackage(CompsGroup & group) : group(group) {}


}  // namespace libdnf::transaction
