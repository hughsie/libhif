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

#ifndef LIBDNF_ADVISORY_ADVISORY_HPP
#define LIBDNF_ADVISORY_ADVISORY_HPP

#include <libdnf/rpm/package_sack.hpp>

#include <vector>


namespace libdnf::advisory {

class AdvisoryReference;

struct AdvisoryId {
public:
    AdvisoryId() = default;
    explicit AdvisoryId(int id) : id(id) {}

    bool operator==(const AdvisoryId & other) const noexcept { return id == other.id; };
    bool operator!=(const AdvisoryId & other) const noexcept { return id != other.id; };

    int id{0};
};

/// An advisory, represents advisory used to track security updates
class Advisory {
public:
    /// Construct the Advisory object
    ///
    /// @param sack   WeakPtr to libdnf::rpm::PackageSack instance which holds the data.
    /// @param id     AdvisoryId into libsolv pool.
    /// @return New Advisory instance.
    Advisory(const libdnf::BaseWeakPtr & base, AdvisoryId id);
    Advisory(libdnf::Base & base, AdvisoryId id);

    bool operator==(const Advisory & other) const noexcept;
    bool operator!=(const Advisory & other) const noexcept;

    /// Destroy the Advisory object
    ~Advisory();

    /// Get name of this advisory.
    ///
    /// @return Name of this advisory as std::string.
    std::string get_name() const;

    /// Get severity of this advisory.
    ///
    /// @return Severity of this advisory as std::string.
    std::string get_severity() const;

    /// Get type of this advisory.
    ///
    /// @return type of this advisory as std::string.
    std::string get_type() const;

    /// Get buildtime of this advisory. Libsolv combines issued and updated dates
    /// into buildtime by always using the newer one.
    ///
    /// @return buildtime of this advisory.
    unsigned long long get_buildtime() const;

    /// Get vendor of this advisory.
    ///
    /// @return Vendor of this advisory as std::string.
    std::string get_vendor() const;

    /// Get description of this advisory.
    ///
    /// @return Description of this advisory as std::string.
    std::string get_description() const;

    /// Get title of this advisory.
    ///
    /// @return Title of this advisory as std::string.
    std::string get_title() const;

    /// Get status of this advisory.
    ///
    /// @return Status of this advisory as std::string.
    std::string get_status() const;

    /// Get rights of this advisory.
    ///
    /// @return Rights of this advisory as std::string.
    std::string get_rights() const;

    /// Get message of this advisory.
    ///
    /// @return Message of this advisory as std::string.
    std::string get_message() const;

    /// Get AdvisoryId.
    ///
    /// @return AdvisoryId of this advisory.
    AdvisoryId get_id() const;

    /// Get all references of specified type from this advisory.
    ///
    /// @param types     What types of references to get. If not specified gets all types.
    /// @return Vector of AdvisoryReference objects.
    std::vector<AdvisoryReference> get_references(std::vector<std::string> types = {}) const;

    /// Get all collections from this advisory.
    ///
    /// @return Vector of AdvisoryCollection objects.
    std::vector<AdvisoryCollection> get_collections() const;

    /// Check whether at least one collection from this advisory is applicable.
    ///
    /// @return True if applicable, False otherwise.
    bool is_applicable() const;

private:
    libdnf::BaseWeakPtr base;

    AdvisoryId id;
};

inline bool Advisory::operator==(const Advisory & other) const noexcept {
    return id == other.id && base == other.base;
}

inline bool Advisory::operator!=(const Advisory & other) const noexcept {
    return id != other.id && base != other.base;
}

}  // namespace libdnf::advisory

#endif
