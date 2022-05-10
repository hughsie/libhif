/*
Copyright Contributors to the libdnf project.

This file is part of libdnf: https://github.com/rpm-software-management/libdnf/

Libdnf is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 2.1 of the License, or
(at your option) any later version.

Libdnf is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with libdnf.  If not, see <https://www.gnu.org/licenses/>.
*/


// IMPORTANT: Read before implementing new filters
//
// TransactionSack and TransactionQuery are different than any other similar objects.
// TransactionSack is loaded lazily any time a TransactionQuery retrieves a Transaction from the database.
// The filters should make a query to the database and return a list of matching transction IDs
// and the IDs should be passed  to filter_id() to load the Transaction objects.


#include "libdnf/transaction/query.hpp"

#include "utils/bgettext/bgettext-mark-domain.h"

#include "libdnf/base/base.hpp"
#include "libdnf/transaction/rpm_package.hpp"
#include "libdnf/transaction/sack.hpp"

#include <vector>


namespace libdnf::transaction {


TransactionQuery::TransactionQuery(const TransactionSackWeakPtr & sack) : sack{sack} {}

TransactionQuery::TransactionQuery(const BaseWeakPtr & base) : sack{base->get_transaction_sack()} {}

TransactionQuery::TransactionQuery(Base & base) : sack{base.get_transaction_sack()} {}

TransactionQuery & TransactionQuery::filter_id(const std::vector<int64_t> & patterns, sack::QueryCmp cmp) {
    switch (cmp) {
        case libdnf::sack::QueryCmp::EQ:
            // currently only EQ operator is supported
            // so we can ignore cmp later on
            break;
        case libdnf::sack::QueryCmp::NOT:
        case libdnf::sack::QueryCmp::ICASE:
        case libdnf::sack::QueryCmp::ISNULL:
        case libdnf::sack::QueryCmp::NEQ:
        case libdnf::sack::QueryCmp::GT:
        case libdnf::sack::QueryCmp::GTE:
        case libdnf::sack::QueryCmp::LT:
        case libdnf::sack::QueryCmp::LTE:
        case libdnf::sack::QueryCmp::IEXACT:
        case libdnf::sack::QueryCmp::NOT_IEXACT:
        case libdnf::sack::QueryCmp::CONTAINS:
        case libdnf::sack::QueryCmp::NOT_CONTAINS:
        case libdnf::sack::QueryCmp::ICONTAINS:
        case libdnf::sack::QueryCmp::NOT_ICONTAINS:
        case libdnf::sack::QueryCmp::STARTSWITH:
        case libdnf::sack::QueryCmp::ISTARTSWITH:
        case libdnf::sack::QueryCmp::ENDSWITH:
        case libdnf::sack::QueryCmp::IENDSWITH:
        case libdnf::sack::QueryCmp::REGEX:
        case libdnf::sack::QueryCmp::IREGEX:
        case libdnf::sack::QueryCmp::GLOB:
        case libdnf::sack::QueryCmp::NOT_GLOB:
        case libdnf::sack::QueryCmp::IGLOB:
        case libdnf::sack::QueryCmp::NOT_IGLOB:
            // TODO(dmach): Replace with a different exception class
            throw RuntimeError(M_("Unsupported comparison operator"));
            break;
    }

    if (!initialized) {
        // lock TransactionSack and load Transaction objects to this TransactionQuery
        // while adding missing ones to the Sack too

        std::lock_guard<std::mutex> lock(sack->mtx);

        if (sack->get_use_includes()) {
            // if includes are used, add only includes to the query
            *this |= sack->get_includes();
        } else {
            // else add all items
            for (auto & it : sack->get_data()) {
                add(TransactionSack::DataItemWeakPtr(it.get(), &sack->get_data_guard()));
            }
        }

        std::set<int64_t> seen_ids;
        for (auto & it : get_data()) {
            seen_ids.insert(it->get_id());
        }

        // add additional Transaction objects that were not found in TransactionSack
        std::set<int64_t> load_ids;
        std::set_difference(
            patterns.begin(),
            patterns.end(),
            seen_ids.begin(),
            seen_ids.end(),
            std::inserter(load_ids, load_ids.begin()));
        for (auto id : load_ids) {
            // add new Transaction to the sack
            auto weakptr = sack->add_item_with_return(std::make_unique<Transaction>(*sack, id));
            // add a weak pointer to the Transaction to the current query
            add(weakptr);
        }

        // apply excludes
        *this -= sack->get_excludes();

        initialized = true;
    }

    filter(F::id, patterns, cmp);
    return *this;
}


TransactionQuery & TransactionQuery::filter_id(int64_t pattern, sack::QueryCmp cmp) {
    std::vector<int64_t> patterns{pattern};
    return filter_id(patterns, cmp);
}


}  // namespace libdnf::transaction
