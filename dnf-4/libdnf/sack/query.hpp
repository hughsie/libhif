/*
 * Copyright (C) 2017 Red Hat, Inc.
 *
 * Licensed under the GNU Lesser General Public License Version 2.1
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef __QUERY_HPP
#define __QUERY_HPP

#include <memory>
#include <vector>
#include "../hy-types.h"
#include "../hy-query.h"
#include "../hy-subject.h"
#include "../repo/solvable/Dependency.hpp"
#include "../repo/solvable/DependencyContainer.hpp"
#include "../transaction/Swdb.hpp"
#include "../dnf-types.h"
#include "advisorypkg.hpp"

#include <set>
#include <utility>

namespace libdnf {

union _Match {
    int num;
    DnfPackageSet *pset;
    Id reldep;
    char *str;
};

struct Filter {
public:
    Filter(int keyname, int cmp_type, int match);
    Filter(int keyname, int cmp_type, int nmatches, const int *matches);
    Filter(int keyname, int cmp_type, const DnfPackageSet * pset);
    Filter(int keyname, int cmp_type, const Dependency * reldep);
    Filter(int keyname, int cmp_type, const DependencyContainer * reldeplist);
    Filter(int keyname, int cmp_type, const char *match);
    Filter(int keyname, int cmp_type, const char **matches);
    Filter(_hy_key_name_e keyname, int comparisonType, const std::vector<const char *> &matches);
    ~Filter();
    int getKeyname() const noexcept;
    int getCmpType() const noexcept;
    int getMatchType() const noexcept;
    const std::vector<_Match> & getMatches() const noexcept;
private:
    class Impl;
    std::shared_ptr<Impl> pImpl;
};

/**
* @brief Provides package filtering
* addFilter() can return DNF_ERROR_BAD_QUERY in case if cmp_type or keyname is incompatible with provided data type
*
*/
struct Query {
public:
    enum class ExcludeFlags {
    APPLY_EXCLUDES = 0,
    IGNORE_MODULAR_EXCLUDES = 1 << 0,
    IGNORE_REGULAR_EXCLUDES = 1 << 1,
    IGNORE_EXCLUDES = IGNORE_MODULAR_EXCLUDES | IGNORE_REGULAR_EXCLUDES
    };

    Query(const Query & query_src);
    Query(Query && query_src) = delete;
    Query(DnfSack* sack, ExcludeFlags flags = ExcludeFlags::APPLY_EXCLUDES);
    ~Query();
    Query & operator=(const Query& query_src);
    Query & operator=(Query && src_query) = delete;
    Map * getResult() noexcept;
    const Map * getResult() const noexcept;
    /**
    * @brief Applies query and returns pointer of PackageSet
    *
    * @return PackageSet*
    */
    PackageSet * getResultPset();
    DnfSack * getSack();

    /**
    * @brief Return true if query was previously applied
    *
    * @return bool
    */
    bool getApplied() const noexcept;

    /**
    * @brief Remove all filters and reset the result
    *
    */
    void clear();

    /**
    * @brief Applies Query and retuns count of packages
    *
    * @return size_t
    */
    size_t size();
    int addFilter(int keyname, int cmp_type, int match);
    int addFilter(int keyname, int cmp_type, int nmatches, const int *matches);
    int addFilter(int keyname, int cmp_type, const DnfPackageSet *pset);
    int addFilter(int keyname, const Dependency * reldep);
    int addFilter(int keyname, const DependencyContainer * reldeplist);
    int addFilter(int keyname, int cmp_type, const char *match);
    int addFilter(int keyname, int cmp_type, const char **matches);
    int addFilter(_hy_key_name_e keyname, _hy_comparison_type_e comparisonType, const std::vector<const char *> &matches);
    int addFilter(HyNevra nevra, bool icase);
    void apply();

    /**
    * @brief Applies Query and returns DnfPackages in GPtrArray
    *
    * @return GPtrArray*
    */
    GPtrArray * run();

    /**
    * @brief Applies Query and returns result in DnfPackageSet
    *
    * @return DnfPackageSet*
    */
    const DnfPackageSet * runSet();
    Id getIndexItem(int index);

    /**
    * @brief Applies both queries and result of the other query is added to result of this query
    *
    * @param other p_other:...
    */
    void queryUnion(Query & other);

    /**
    * @brief Applies both queries and keep only common packages for both queries in this query
    *
    * @param other p_other:...
    */
    void queryIntersection(Query & other);

    /**
    * @brief Applies both queries and keep only packages in this query that are absent in other query
    *
    * @param other p_other:...
    */
    void queryDifference(Query & other);

    /**
    * @brief Applies Query and returns true if any package in the query
    *
    * @return bool
    */
    bool empty();
    /**
     * @brief Applies all filters and keep only installed packages that have no available package
     * with a same name and architecture.
     * Excluded available packages are handled like other available packages. Modular excludes are
     * applied.
     */
    void filterExtras();
    void filterRecent(const long unsigned int recent_limit);
    void filterDuplicated();
    int filterUnneeded(const Swdb &swdb, bool debug_solver);
    int filterSafeToRemove(const Swdb &swdb, bool debug_solver);
    void getAdvisoryPkgs(int cmpType,  std::vector<AdvisoryPkg> & advisoryPkgs);
    void filterUserInstalled(const Swdb &swdb);

    /**
     * @brief Apply query and return a set of strings representing information in provide that begin
     * by patternProvide. For pattern "base-platform" and presence of provide
     * "base-platform(platform:f26)", the function will return "platform:f26" in set.
     *
     * @param patternProvide p_patternProvide: No glob allowed!
     * @return std::set< std::__cxx11::string >
     */
    std::set<std::string> getStringsFromProvide(const char * patternProvide);

    /**
    * @brief Filter packages to match a given subject
    *
    * @param subject an subject to match
    * @param forms an array of pattern forms, nullptr means a default forms are used, used only for search with_nevra
    * @param icase true - matches the subject without sensitivity to case
    * @param with_nevra true - enable search by nevra
    * @param with_provides true - provides are searched for a match
    * @param with_filenames true - file provides are searched for a match
    *
    * @return std::pair<bool, std::unique_ptr<Nevra>> The bool is denoting whether there are matched packages.
    *         The Nevra is denoting used pattern form to match.  It is nullptr, if with_nevra search
    *         was not used or there is no package whose Nevra would match to the subject.
    */
    std::pair<bool, std::unique_ptr<Nevra>> filterSubject(const char * subject, HyForm * forms,
        bool icase, bool with_nevra, bool with_provides, bool with_filenames);
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

inline Query::ExcludeFlags operator|(Query::ExcludeFlags a, Query::ExcludeFlags b)
{
    return static_cast<Query::ExcludeFlags>(static_cast<int>(a) | static_cast<int>(b));
}

inline Query::ExcludeFlags operator&(Query::ExcludeFlags a, Query::ExcludeFlags b)
{
    return static_cast<Query::ExcludeFlags>(static_cast<int>(a) & static_cast<int>(b));
}

}

#endif /* __QUERY_HPP */
