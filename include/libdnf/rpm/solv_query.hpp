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


#ifndef LIBDNF_RPM_SOLV_QUERY_HPP
#define LIBDNF_RPM_SOLV_QUERY_HPP

#include "sack.hpp"

#include "libdnf/common/sack/query_cmp.hpp"
#include "libdnf/utils/exception.hpp"

#include <memory>
#include <string>
#include <vector>


namespace libdnf::rpm {

class Sack;

/// @replaces libdnf/hy-query.h:struct:HyQuery
/// @replaces libdnf/sack/query.hpp:struct:Query
/// @replaces hawkey:hawkey/__init__.py:class:Query
class Query {
public:
    enum class InitFlags {
        APPLY_EXCLUDES = 0,
        IGNORE_MODULAR_EXCLUDES = 1 << 0,
        IGNORE_REGULAR_EXCLUDES = 1 << 1,
        IGNORE_EXCLUDES = IGNORE_MODULAR_EXCLUDES | IGNORE_REGULAR_EXCLUDES,
        EMPTY = 1 << 2
    };

    struct NotSupportedCmpType : public RuntimeError {
        using RuntimeError::RuntimeError;
        const char * get_domain_name() const noexcept override { return "libdnf::rpm::Query"; }
        const char * get_name() const noexcept override { return "NotSupportedCmpType"; }
        const char * get_description() const noexcept override { return "Query exception"; }
    };

    /// @replaces libdnf/hy-query.h:function:hy_query_create(DnfSack *sack);
    /// @replaces libdnf/hy-query.h:function:hy_query_create_flags(DnfSack *sack, int flags);
    /// @replaces libdnf/sack/query.hpp:method:Query(DnfSack* sack, ExcludeFlags flags = ExcludeFlags::APPLY_EXCLUDES)
    /// @replaces libdnf/dnf-reldep.h:function:dnf_reldep_free(DnfReldep *reldep)
    explicit Query(Sack * sack, InitFlags flags = InitFlags::APPLY_EXCLUDES);
    Query(const Query & src);
    Query(Query && src) noexcept;
    ~Query();

    Query & operator=(const Query & src);
    Query & operator=(Query && src) noexcept;

    /// cmp_type could be only libdnf::sack::QueryCmp::EQ, NEQ, GLOB, NOT_GLOB, IEXACT, NOT_IEXACT, ICONTAINS, NOT_ICONTAINS, IGLOB, NOT_IGLOB, CONTAINS, NOT_CONTAINS.
    ///
    /// @replaces libdnf/sack/query.hpp:method:addFilter(int keyname, int cmp_type, const char *match) - cmp_type = HY_PKG_NAME
    /// @replaces libdnf/sack/query.hpp:method:addFilter(int keyname, int cmp_type, const char **matches) - cmp_type = HY_PKG_NAME
    Query & ifilter_name(libdnf::sack::QueryCmp cmp_type, std::vector<std::string> & patterns);

    /// cmp_type could be only libdnf::sack::QueryCmp::GT, LT, GTE, LTE, EQ.
    ///
    /// @replaces libdnf/sack/query.hpp:method:addFilter(int keyname, int cmp_type, const char *match) - cmp_type = HY_PKG_EVR
    /// @replaces libdnf/sack/query.hpp:method:addFilter(int keyname, int cmp_type, const char **matches) - cmp_type = HY_PKG_EVR
    Query & ifilter_evr(libdnf::sack::QueryCmp cmp_type, std::vector<std::string> & patterns);

    /// cmp_type could be only libdnf::sack::QueryCmp::EQ, NEQ, GLOB, NOT_GLOB.
    Query & ifilter_arch(libdnf::sack::QueryCmp cmp_type, std::vector<std::string> & patterns);

    /// Requires full nevra including epoch. QueryCmp::EQ, NEG, GT, GTE, LT, and LTE are tolerant when epoch 0 is not present.
    /// cmp_type could be only libdnf::sack::QueryCmp::EQ, NEQ, GT, GTE, LT, LTE, GLOB, NOT_GLOB, IGLOB, NOT_IGLOB, IEXACT, NOT_IEXACT.
    ///
    /// @replaces libdnf/sack/query.hpp:method:addFilter(int keyname, int cmp_type, const char *match) - cmp_type = HY_PKG_NEVRA
    /// @replaces libdnf/sack/query.hpp:method:addFilter(int keyname, int cmp_type, const char **matches) - cmp_type = HY_PKG_NEVRA
    /// @replaces libdnf/sack/query.hpp:method:addFilter(int keyname, int cmp_type, const char *match) - cmp_type = HY_PKG_NEVRA_STRICT
    /// @replaces libdnf/sack/query.hpp:method:addFilter(int keyname, int cmp_type, const char **matches) - cmp_type = HY_PKG_NEVRA_STRICT
    Query & ifilter_nevra_strict(libdnf::sack::QueryCmp cmp_type, std::vector<std::string> & patterns);

private:
    class Impl;
    std::unique_ptr<Impl> p_impl;
};


}  // namespace libdnf::rpm

#endif  // LIBDNF_RPM_SOLV_QUERY_HPP
