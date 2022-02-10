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

#include "libdnf/comps/group/sack.hpp"

#include "libdnf/comps/comps.hpp"
#include "libdnf/comps/group/group.hpp"
#include "libdnf/comps/group/query.hpp"


namespace libdnf::comps {


GroupSackWeakPtr GroupSack::get_weak_ptr() {
    return GroupSackWeakPtr(this, &sack_guard);
}


GroupSack::GroupSack(Comps & comps) : comps{comps} {}


GroupSack::~GroupSack() = default;


}  // namespace libdnf::comps
