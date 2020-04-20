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

#ifndef LIBDNF_RPM_PACKAGE_PRIVATE_HPP
#define LIBDNF_RPM_PACKAGE_PRIVATE_HPP


#include "libdnf/rpm/package.hpp"

extern "C" {
#include <solv/pool.h>
#include <solv/pooltypes.h>
#include <solv/repo.h>
#include <solv/solvable.h>
#include <solv/util.h>
}

#include <cassert>
#include <cstdlib>
#include <string>
#include <vector>

namespace libdnf::rpm::solv {

const char * BASE_EPOCH = "0";

inline static Solvable *
get_solvable(Pool * pool, libdnf::rpm::PackageId package_id)
{
    return pool_id2solvable(pool, package_id.id);
}

static char *
pool_tmpdup(Pool *pool, const char *s)
{
    char *dup = pool_alloctmpspace(pool, static_cast<int>(strlen(s)) + 1);
    return strcpy(dup, s);
}

/**
 * Split evr into its components.
 *
 * Believes blindly in 'evr' being well formed. This could be implemented
 * without 'pool' of course but either the caller would have to provide buffers
 * to store the split pieces, or this would call strdup (which is more expensive
 * than the pool temp space).
 */
void
pool_split_evr(Pool *pool, const char *evr_c, char **epoch, char **version, char **release)
{
    char *evr = pool_tmpdup(pool, evr_c);
    char *e, *v, *r;

    for (e = evr + 1; *e != ':' && *e != '-' && *e != '\0'; ++e)
        ;

    if (*e == '-') {
        *e = '\0';
        v = evr;
        r = e + 1;
        e = NULL;
    } else if (*e == '\0') {
        v = evr;
        e = NULL;
        r = NULL;
    } else { /* *e == ':' */
        *e = '\0';
        v = e + 1;
        e = evr;
        for (r = v + 1; *r != '-'; ++r)
            assert(*r);
        *r = '\0';
        r++;
    }
    *epoch = e;
    *version = v;
    *release = r;
}

void
repo_internalize_trigger(::Repo * repo)
{
    // TODO add lazy call of repo_internalize(repo)
    repo_internalize(repo);
}

static inline unsigned long long
lookup_num(Solvable * solvable, unsigned type)
{
    repo_internalize_trigger(solvable->repo);
    return solvable_lookup_num(solvable, type, 0);
}

static inline const char *
lookup_cstring(Solvable * solvable, unsigned type)
{
    repo_internalize_trigger(solvable->repo);
    return solvable_lookup_str(solvable, type);
}

inline const char *
get_name(Pool * pool, libdnf::rpm::PackageId package_id) noexcept
{
    return pool_id2str(pool, get_solvable(pool, package_id)->name);
}

inline const char *
get_evr(Pool * pool, libdnf::rpm::PackageId package_id) noexcept
{
    return pool_id2str(pool, get_solvable(pool, package_id)->evr);
}

/// @return const char* !! Return temporal value !!
inline const char *
get_epoch_cstring(Pool * pool, libdnf::rpm::PackageId package_id) noexcept
{
    char *e, *v, *r;
    pool_split_evr(pool, get_evr(pool, package_id), &e, &v, &r);
    if (e) {
        return e;
    }
    return BASE_EPOCH;
}

inline unsigned long
get_epoch(Pool * pool, libdnf::rpm::PackageId package_id) noexcept
{
    char *e, *v, *r, *endptr;
    pool_split_evr(pool, get_evr(pool, package_id), &e, &v, &r);
    if (e) {
        auto converted = std::strtol(e, &endptr, 10);
        assert(converted > 0);
        assert(*endptr == '\0');
        return static_cast<unsigned long>(converted);
    }
    return 0;
}

/// @return const char* !! Return temporal value !!
inline const char *
get_version(Pool * pool, libdnf::rpm::PackageId package_id) noexcept
{
    char *e, *v, *r;
    pool_split_evr(pool, get_evr(pool, package_id), &e, &v, &r);
    return v;
}

/// @return const char* !! Return temporal value !!
inline const char *
get_release(Pool * pool, libdnf::rpm::PackageId package_id) noexcept
{
    char *e, *v, *r;
    pool_split_evr(pool, get_evr(pool, package_id), &e, &v, &r);
    return r;
}

inline const char *
get_arch(Pool * pool, libdnf::rpm::PackageId package_id) noexcept
{
    return pool_id2str(pool, get_solvable(pool, package_id)->arch);
}

/// @return const char* !! Return temporal value !!
inline const char *
get_nevra(Pool * pool, libdnf::rpm::PackageId package_id) noexcept
{
    return pool_solvable2str(pool, get_solvable(pool, package_id));
}

/// @return const char* !! Return temporal value !!
inline const char *
get_group(Pool * pool, libdnf::rpm::PackageId package_id) noexcept
{
    return lookup_cstring(get_solvable(pool, package_id), SOLVABLE_GROUP);
}

inline bool
is_installed(Pool * pool, Solvable * solvable)
{
    return solvable->repo == pool->installed;
}

inline unsigned long long
get_download_size(Pool * pool, libdnf::rpm::PackageId package_id) noexcept
{
    return lookup_num(get_solvable(pool, package_id), SOLVABLE_DOWNLOADSIZE);
}

inline unsigned long long
get_install_size(Pool * pool, libdnf::rpm::PackageId package_id) noexcept
{
    return lookup_num(get_solvable(pool, package_id), SOLVABLE_INSTALLSIZE);
}

/// If package is installed, return get_install_size(). Return get_download_size() otherwise.
inline unsigned long long
get_size(Pool * pool, libdnf::rpm::PackageId package_id) noexcept
{
    Solvable * solvable = get_solvable(pool, package_id);
    if (is_installed(pool, solvable)) {
        return get_install_size(pool, package_id);
    }
    return get_download_size(pool, package_id);
}

/// @return const char* !! Return temporal value !!
inline const char *
get_license(Pool * pool, libdnf::rpm::PackageId package_id) noexcept
{
    return lookup_cstring(get_solvable(pool, package_id), SOLVABLE_LICENSE);
}

/// @return const char* !! Return temporal value !!
inline const char *
get_sourcerpm(Pool * pool, libdnf::rpm::PackageId package_id)
{
    Solvable * solvable = get_solvable(pool, package_id);
    repo_internalize_trigger(solvable->repo);
    return solvable_lookup_sourcepkg(solvable);
}

inline unsigned long long
get_build_time(Pool * pool, libdnf::rpm::PackageId package_id)
{
    return lookup_num(get_solvable(pool, package_id), SOLVABLE_BUILDTIME);
}

/// @return const char* !! Return temporal value !!
inline const char *
get_build_host(Pool * pool, libdnf::rpm::PackageId package_id) noexcept
{
    return lookup_cstring(get_solvable(pool, package_id), SOLVABLE_BUILDHOST);
}

/// @return const char* !! Return temporal value !!
inline const char *
get_packager(Pool * pool, libdnf::rpm::PackageId package_id) noexcept
{
    return lookup_cstring(get_solvable(pool, package_id), SOLVABLE_PACKAGER);
}

/// @return const char* !! Return temporal value !!
inline const char *
get_vendor(Pool * pool, libdnf::rpm::PackageId package_id) noexcept
{
    return lookup_cstring(get_solvable(pool, package_id), SOLVABLE_VENDOR);
}

/// @return const char* !! Return temporal value !!
inline const char *
get_url(Pool * pool, libdnf::rpm::PackageId package_id) noexcept
{
    return lookup_cstring(get_solvable(pool, package_id), SOLVABLE_URL);
}

/// @return const char* !! Return temporal value !!
inline const char *
get_summary(Pool * pool, libdnf::rpm::PackageId package_id) noexcept
{
    return lookup_cstring(get_solvable(pool, package_id), SOLVABLE_SUMMARY);
}

/// @return const char* !! Return temporal value !!
inline const char *
get_description(Pool * pool, libdnf::rpm::PackageId package_id) noexcept
{
    return lookup_cstring(get_solvable(pool, package_id), SOLVABLE_DESCRIPTION);
}

std::vector<std::string>
get_files(Pool * pool, libdnf::rpm::PackageId package_id)
{
    Solvable * solvable = get_solvable(pool, package_id);
    Dataiterator di;
    std::vector<std::string> ret;

    repo_internalize_trigger(solvable->repo);
    dataiterator_init(&di, pool, solvable->repo, package_id.id, SOLVABLE_FILELIST, NULL,
                      SEARCH_FILES | SEARCH_COMPLETE_FILELIST);
    while (dataiterator_step(&di)) {
       ret.push_back(di.kv.str);
    }
    dataiterator_free(&di);
    return ret;
}

}  // namespace libdnf::rpm::solv


#endif  // LIBDNF_RPM_PACKAGE_HPP
