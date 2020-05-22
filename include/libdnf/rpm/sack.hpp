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

#include "libdnf/utils/exception.hpp"

#include <memory>

namespace libdnf {

class Base;

}  // namespace libdnf

namespace libdnf::rpm {


class PackageSet;


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
class Repo;

class Sack {
public:
    class Exception : public RuntimeError {
    public:
        using RuntimeError::RuntimeError;
        const char * get_domain_name() const noexcept override { return "libdnf::rpm::Sack"; }
        const char * get_name() const noexcept override { return "Exception"; }
        const char * get_description() const noexcept override { return "rpm::Sack exception"; }
    };

    class NoCapability : public Exception {
    public:
        using Exception::Exception;
        const char * get_domain_name() const noexcept override { return "libdnf::rpm::Sack"; }
        const char * get_name() const noexcept override { return "NoCapability"; }
        const char * get_description() const noexcept override {
            return "repository does not provide required metadata type";
        }
    };

    // LoadRepoFlags:
    // @NONE:                    Load only main solvables
    // @USE_FILELISTS:           Add the filelists metadata
    // @USE_PRESTO:              Add presto deltas metadata
    // @USE_UPDATEINFO:          Add updateinfo metadata
    // @USE_OTHER:               Add other metadata
    enum class LoadRepoFlags {
        NONE = 0,
        USE_FILELISTS = 1 << 1,
        USE_PRESTO = 1 << 2,
        USE_UPDATEINFO = 1 << 3,
        USE_OTHER = 1 << 4,
    };

    Sack(Base & base);
    ~Sack();

    /// Loads rpm::Repo into Sack.
    void load_repo(Repo & repo, bool build_cache, LoadRepoFlags flags);

private:
    friend Package;
    friend PackageSet;
    friend Reldep;
    friend ReldepList;
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

inline constexpr Sack::LoadRepoFlags operator|(Sack::LoadRepoFlags lhs, Sack::LoadRepoFlags rhs) {
    return static_cast<Sack::LoadRepoFlags>(
        static_cast<std::underlying_type_t<Sack::LoadRepoFlags>>(lhs) |
        static_cast<std::underlying_type_t<Sack::LoadRepoFlags>>(rhs));
}

inline constexpr Sack::LoadRepoFlags operator&(Sack::LoadRepoFlags lhs, Sack::LoadRepoFlags rhs) {
    return static_cast<Sack::LoadRepoFlags>(
        static_cast<std::underlying_type_t<Sack::LoadRepoFlags>>(lhs) &
        static_cast<std::underlying_type_t<Sack::LoadRepoFlags>>(rhs));
}

inline constexpr bool any(Sack::LoadRepoFlags flags) {
    return static_cast<typename std::underlying_type<Sack::LoadRepoFlags>::type>(flags) != 0;
}

}  // namespace libdnf::rpm

#endif  // LIBDNF_RPM_SACK_HPP
