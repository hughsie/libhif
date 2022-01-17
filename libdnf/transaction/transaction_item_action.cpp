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


#include "libdnf/transaction/transaction_item_action.hpp"


namespace libdnf::transaction {


std::string transaction_item_action_to_string(TransactionItemAction action) {
    switch (action) {
        case TransactionItemAction::INSTALL:
            return "Install";
        case TransactionItemAction::DOWNGRADE:
            return "Downgrade";
        case TransactionItemAction::DOWNGRADED:
            return "Downgraded";
        case TransactionItemAction::OBSOLETE:
            return "Obsolete";
        case TransactionItemAction::OBSOLETED:
            return "Obsoleted";
        case TransactionItemAction::UPGRADE:
            return "Upgrade";
        case TransactionItemAction::UPGRADED:
            return "Upgraded";
        case TransactionItemAction::REMOVE:
            return "Removed";
        case TransactionItemAction::REINSTALL:
            return "Reinstall";
        case TransactionItemAction::REINSTALLED:
            return "Reinstalled";
        case TransactionItemAction::REASON_CHANGE:
            return "Reason Change";
        case TransactionItemAction::REPLACED:
            return "Replaced";
    }
    return "";
}


std::string transaction_item_action_to_letter(TransactionItemAction action) {
    // TODO(dmach): consider adding the direction, e.g. ">U" == "Upgrade", "<U" == "Upgraded"
    switch (action) {
        case TransactionItemAction::INSTALL:
            return "I";
        case TransactionItemAction::DOWNGRADE:
            return "D";
        case TransactionItemAction::DOWNGRADED:
            return "D";
        case TransactionItemAction::OBSOLETE:
            return "O";
        case TransactionItemAction::OBSOLETED:
            return "O";
        case TransactionItemAction::UPGRADE:
            return "U";
        case TransactionItemAction::UPGRADED:
            return "U";
        case TransactionItemAction::REMOVE:
            // "R" is for Reinstall, therefore use "E" for rEmove (or Erase)
            return "E";
        case TransactionItemAction::REINSTALL:
            return "R";
        case TransactionItemAction::REINSTALLED:
            return "R";
        case TransactionItemAction::REASON_CHANGE:
            // TODO(dmach): replace "?" with something better
            return "?";
        case TransactionItemAction::REPLACED:
            return "O";
    }
    return "";
}


bool transaction_item_action_is_inbound(TransactionItemAction action) {
    switch (action) {
        case TransactionItemAction::INSTALL:
        case TransactionItemAction::DOWNGRADE:
        case TransactionItemAction::OBSOLETE:
        case TransactionItemAction::UPGRADE:
        case TransactionItemAction::REINSTALL:
            return true;
        default:
            return false;
    }
    return false;
}


bool transaction_item_action_is_outbound(TransactionItemAction action) {
    switch (action) {
        case TransactionItemAction::REMOVE:
        case TransactionItemAction::DOWNGRADED:
        case TransactionItemAction::OBSOLETED:
        case TransactionItemAction::UPGRADED:
        case TransactionItemAction::REINSTALLED:
        case TransactionItemAction::REPLACED:
            return true;
        default:
            return false;
    }
    return false;
}


}  // namespace libdnf::transaction
