/*
Copyright (C) 2018-2020 Red Hat, Inc.

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

#ifndef LIBDNF_RELDEP_HPP
#define LIBDNF_RELDEP_HPP

#include "sack.hpp"

#include <memory>
#include <string>
#include <vector>

// forward declarations
class ReldepList;

namespace libdnf::rpm {
/// @brief Represent a relational dependency from libsolv
///
/// @replaces libdnf/dnf-reldep.h:struct:DnfReldep
/// @replaces libdnf/repo/solvable/Dependency.hpp:struct:Dependency
/// @replaces hawkey:hawkey/__init__.py:class:Reldep
class Reldep
{
public:
    enum class ComparisonType {
        NONE = 0,
        GT = 1,
        EQ = 2,
        GT_EQ = 3,
        LT = 4,
        LT_EQ = 6
    };

    /// @brief Creates a reldep from Char*. If parsing fails it raises std::runtime_error.
    ///
    /// @param sack p_sack:...
    /// @param dependency p_dependency:...
    /// @replaces libdnf/repo/solvable/Dependency.hpp:method:Dependency(Sack * sack, const std::string & dependency)
    Reldep(Sack * sack, const std::string & reldep_string);

    /// @replaces libdnf/repo/solvable/Dependency.hpp:method:Dependency(const Dependency & dependency);
    Reldep(const Reldep & reldep);

    /// @replaces libdnf/repo/solvable/Dependency.hpp:method:~Dependency();
    /// @replaces libdnf/dnf-reldep.h:function:dnf_reldep_free(DnfReldep *reldep)
    ~Reldep();

    /// @replaces libdnf/repo/solvable/Dependency.hpp:method:getName()
    const char * get_name() const;

    /// @replaces libdnf/repo/solvable/Dependency.hpp:method:getRelation()
    const char * get_relation() const;

    /// @replaces libdnf/repo/solvable/Dependency.hpp:method:getVersion()
    const char * get_version() const;

    /// @replaces libdnf/repo/solvable/Dependency.hpp:method:toString()
    /// @replaces libdnf/dnf-reldep.h:function:dnf_reldep_to_string(DnfReldep *reldep)
    const char * to_string() const;

    /// @replaces libdnf/repo/solvable/Dependency.hpp:method:getId()
    /// @replaces libdnf/dnf-reldep.h:function:dnf_reldep_to_string(DnfReldep *reldep)
    ReldepId get_id() const noexcept { return id; };

private:
    friend ReldepList;

    /// @brief Creates a reldep from Id
    /// @replaces libdnf/repo/solvable/Dependency.hpp:method:Dependency(Sack * sack, Id id)
    Reldep(Sack * sack, ReldepId dependency_id);

    /// @brief Creates a reldep from name, version, and comparison type.
    ///
    /// @param sack p_sack: DnfSack*
    /// @param name p_name: Required
    /// @param version p_version: Can be also NULL
    /// @param cmpType p_cmpType: ComparisonType, and their combinations
    /// @replaces libdnf/repo/solvable/Dependency.hpp:method:get_id()
    /// @replaces libdnf/dnf-reldep.h:function:dnf_reldep_new(DnfSack *sack, const char *name, int cmp_type, const char *evr)
    Reldep(Sack * sack, const char * name, const char * version, ComparisonType cmp_type);

    /// @brief Returns Id of reldep
    ///
    /// @param sack p_sack: DnfSack*
    /// @param name p_name: Required
    /// @param version p_version: Can be also NULL
    /// @param cmpType p_cmpType: ComparisonType, and their combinations
    /// @return DependencyId
    /// @replaces libdnf/repo/solvable/Dependency.hpp:method:getReldepId(DnfSack *sack, const char *name, const char *version, int cmpType)
    static ReldepId get_reldep_id(Sack * sack, const char * name, const char * version, ComparisonType cmp_type);

    /// @brief Returns Id of reldep or raises std::runtime_error if parsing fails
    ///
    /// @param sack p_sack:DnfSack
    /// @param reldepStr p_reldepStr: const Char* of reldep
    /// @return DependencyId
    /// @replaces libdnf/repo/solvable/Dependency.hpp:method:getReldepId(DnfSack *sack, const char * reldepStr)
    static ReldepId get_reldep_id(Sack * sack, const std::string & reldep_str);

    Sack * sack;
    ReldepId id;
};


}  // namespace libdnf::rpm

#endif //LIBDNF_RELDEP_HPP
