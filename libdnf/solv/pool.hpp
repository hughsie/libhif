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

#ifndef LIBDNF_SOLV_POOL_HPP
#define LIBDNF_SOLV_POOL_HPP

#include "id_queue.hpp"

#include "libdnf/repo/repo.hpp"

#include <cassert>
#include <climits>
#include <memory>

extern "C" {
#include <solv/evr.h>
#include <solv/dataiterator.h>
#include <solv/pool.h>
#include <solv/queue.h>
#include <solv/repodata.h>
#include <solv/repo.h>
#include <solv/util.h>
}


namespace libdnf::solv {

constexpr const char * ZERO_EPOCH = "0";

constexpr const char * SOLVABLE_NAME_ADVISORY_PREFIX = "patch:";
constexpr size_t SOLVABLE_NAME_ADVISORY_PREFIX_LENGTH = std::char_traits<char>::length(SOLVABLE_NAME_ADVISORY_PREFIX);


class Pool;

class TempEvr {
public:
    char * e = nullptr;
    char * v = nullptr;
    char * r = nullptr;

    TempEvr(const Pool & pool, const char * evr);
    ~TempEvr();

    TempEvr(const TempEvr & evr) = delete;
    TempEvr & operator=(const TempEvr & evr) = delete;

    const char * e_def() { return e == nullptr ? ZERO_EPOCH : e; }

private:
    //const Pool & pool;
    char * split_evr = nullptr;
};


class Pool {
public:
    Pool(::Pool * pool) : pool(pool) {}

    Pool(const Pool & pool) = delete;
    Pool & operator=(const Pool & pool) = delete;

    int get_nsolvables() const { return pool->nsolvables; }

    Solvable * id2solvable(Id id) const {
        return pool_id2solvable(pool, id);
    }

    const char * id2str(Id id) const {
        return pool_id2str(pool, id);
    }

    const char * id2rel(Id id) const {
        return pool_id2rel(pool, id);
    }

    const char * id2evr(Id id) const {
        return pool_id2evr(pool, id);
    }

    const char * dep2str(Id id) const {
        return pool_dep2str(pool, id);
    }

    const char * solvid2str(Id id) const {
        return pool_solvid2str(pool, id);
    }

    const char * solvable2str(Solvable * solvable) const {
        return pool_solvable2str(pool, solvable);
    }

    Id solvable2id(Solvable * solvable) const {
        return pool_solvable2id(pool, solvable);
    }

    Id str2id(const char * str, bool create) const {
        return pool_str2id(pool, str, create);
    }

    Id strn2id(const char * str, unsigned int len, bool create) const {
        return pool_strn2id(pool, str, len, create);
    }

    Id rel2id(Id name, Id evr, int flags, bool create) const {
        return pool_rel2id(pool, name, evr, flags, create);
    }

    Id lookup_id(Id id, Id keyname) const {
        return pool_lookup_id(pool, id, keyname);
    }

    const char * lookup_str(Id id, Id keyname) const {
        return pool_lookup_str(pool, id, keyname);
    }

    const char * get_str_from_pool(Id keyname, Id advisory, int index) const;

    Id queuetowhatprovides(IdQueue & queue) const {
        return pool_queuetowhatprovides(pool, &queue.get_queue());
    }

    int evrcmp(Id evr1, Id evr2, int mode) const {
        return pool_evrcmp(pool, evr1, evr2, mode);
    }

    int evrcmp_str(const char * evr1, const char * evr2, int mode) const {
        return pool_evrcmp_str(pool, evr1, evr2, mode);
    }

    /// Split evr into its components.
    ///
    /// Believes blindly in 'evr' being well formed. This could be implemented
    /// without 'pool' of course but either the caller would have to provide buffers
    /// to store the split pieces, or this would call strdup (which is more expensive
    /// than the pool temp space).
    TempEvr split_evr(const char * evr) const { return TempEvr(*this, evr); }


    const char * get_name(Id id) const noexcept {
        return id2str(id2solvable(id)->name);
    }

    const char * get_evr(Id id) const noexcept {
        return id2str(id2solvable(id)->evr);
    }

    const char * get_epoch(Id id) const noexcept {
        return split_evr(get_evr(id)).e_def();
    }

    unsigned long get_epoch_num(Id id) const noexcept {
        const auto evr = split_evr(get_evr(id));
        char * endptr;
        if (evr.e) {
            auto converted = std::strtol(evr.e, &endptr, 10);
            // TODO(lukash) don't do C asserts?
            assert(converted > 0);
            assert(converted < LONG_MAX);
            assert(*endptr == '\0');
            return static_cast<unsigned long>(converted);
        }
        return 0;
    }

    const char * get_version(Id id) const noexcept {
        return split_evr(get_evr(id)).v;
    }

    const char * get_release(Id id) const noexcept {
        return split_evr(get_evr(id)).r;
    }

    const char * get_arch(Id id) const noexcept {
        return id2str(id2solvable(id)->arch);
    }

    const char * get_nevra(Id id) const noexcept {
        return solvable2str(id2solvable(id));
    }

    std::string get_full_nevra(Id id) const;

    bool is_installed(Solvable * solvable) const {
        return solvable->repo == pool->installed;
    }

    bool is_installed(Id id) const {
        return is_installed(id2solvable(id));
    }

    repo::Repo * get_repo(Id id) const noexcept {
        auto solvable = id2solvable(id);
        return static_cast<repo::Repo *>(solvable->repo->appdata);
    }

    const char * get_sourcerpm(Id id) const;


    Id id_to_lowercase_id(const char * name_cstring, bool create) const {
        int name_length = static_cast<int>(strlen(name_cstring));
        auto tmp_name_cstring = pool_alloctmpspace(pool, name_length);
        for (int index = 0; index < name_length; ++index) {
            tmp_name_cstring[index] = static_cast<char>(tolower(name_cstring[index]));
        }
        return pool_strn2id(pool, tmp_name_cstring, static_cast<unsigned int>(name_length), create);
    }

    Id id_to_lowercase_id(Id id_input, bool create) const {
        auto name_cstring = id2str(id_input);
        return id_to_lowercase_id(name_cstring, create);
    }


    bool is_package(Id solvable_id) const {
        Solvable * solvable = id2solvable(solvable_id);
        const char * solvable_name = id2str(solvable->name);
        if (!solvable_name) {
            return true;
        }
        return strncmp(solvable_name, SOLVABLE_NAME_ADVISORY_PREFIX, SOLVABLE_NAME_ADVISORY_PREFIX_LENGTH) != 0;
    }


    ::Pool * operator*() { return &*pool; }
    ::Pool * operator*() const { return &*pool; }

    ::Pool * operator->() { return &*pool; }
    ::Pool * operator->() const { return &*pool; }

private:
    ::Pool * pool;
};

}  // namespace libdnf::solv

#endif  // LIBDNF_SOLV_POOL_HPP
