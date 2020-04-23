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


#ifndef LIBDNF_RPM_SACK_HPP
#define LIBDNF_RPM_SACK_HPP

#include <memory>


namespace libdnf::rpm {

struct PackageId {
public:
    PackageId() = default;
    explicit PackageId(int id);

    bool operator==(const PackageId & other) const noexcept { return id == other.id; };
    bool operator!=(const PackageId & other) const noexcept { return id != other.id; };

    int id{0};
};

struct ReldepId {
public:
    ReldepId() = default;
    explicit ReldepId(int id);

    bool operator==(const ReldepId & other) const noexcept { return id == other.id; };
    bool operator!=(const ReldepId & other) const noexcept { return id != other.id; };

    int id{0};
};

inline PackageId::PackageId(int id) : id(id) {}

inline ReldepId::ReldepId(int id) : id(id) {}


// forward declarations
class Package;
class Reldep;
class ReldepList;

class Sack {
public:
private:
    friend Package;
    friend Reldep;
    friend ReldepList;
    class Impl;
    std::unique_ptr<Impl> pImpl;
};


}  // namespace libdnf::rpm

#endif  // LIBDNF_RPM_SACK_HPP
