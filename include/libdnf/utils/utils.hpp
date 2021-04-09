/*
Copyright (C) 2020-2021 Red Hat, Inc.

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

#ifndef LIBDNF_UTILS_UTILS_HPP
#define LIBDNF_UTILS_UTILS_HPP

#include "libdnf/conf/config_main.hpp"
#include "libdnf/rpm/nevra.hpp"

#include <cstdint>
#include <type_traits>

namespace libdnf {

// forward declarations
class Goal;

/// Compares content of two files.
/// Returns "true" if files have the same content.
/// If content differs or error occurred (file doesn't exist, not readable, ...) returns "false".
bool have_files_same_content_noexcept(const char * file_path1, const char * file_path2) noexcept;

enum class ProblemRules {
    RULE_DISTUPGRADE = 1,
    RULE_INFARCH,
    RULE_UPDATE,
    RULE_JOB,
    RULE_JOB_UNSUPPORTED,
    RULE_JOB_NOTHING_PROVIDES_DEP,
    RULE_JOB_UNKNOWN_PACKAGE,
    RULE_JOB_PROVIDED_BY_SYSTEM,
    RULE_PKG,
    RULE_BEST_1,
    RULE_BEST_2,
    RULE_PKG_NOT_INSTALLABLE_1,
    RULE_PKG_NOT_INSTALLABLE_2,
    RULE_PKG_NOT_INSTALLABLE_3,
    RULE_PKG_NOT_INSTALLABLE_4,
    RULE_PKG_NOTHING_PROVIDES_DEP,
    RULE_PKG_SAME_NAME,
    RULE_PKG_CONFLICTS,
    RULE_PKG_OBSOLETES,
    RULE_PKG_INSTALLED_OBSOLETES,
    RULE_PKG_IMPLICIT_OBSOLETES,
    RULE_PKG_REQUIRES,
    RULE_PKG_SELF_CONFLICT,
    RULE_YUMOBS,
    RULE_UNKNOWN,
    RULE_PKG_REMOVAL_OF_PROTECTED,
    RULE_PKG_REMOVAL_OF_RUNNING_KERNEL
};

enum class GoalProblem : uint32_t {
    NO_PROBLEM = 0,
    SOLVER_ERROR = (1 << 0),
    REMOVAL_OF_PROTECTED = (1 << 1),
    NOT_FOUND = (1 << 2),
    EXCLUDED = (1 << 3),
    ONLY_SRC = (1 << 4),
    NOT_FOUND_IN_REPOSITORIES = (1 << 5),
    NOT_INSTALLED = (1 << 6),
    NOT_INSTALLED_FOR_ARCHITECTURE = (1 << 7),
    HINT_ICASE = (1 << 8),
    HINT_ALTERNATIVES = (1 << 9)
};

enum class GoalSetting { AUTO, SET_TRUE, SET_FALSE };

struct GoalJobSettings {
public:
    GoalJobSettings() = default;

    bool with_nevra{true};
    bool with_provides{true};
    bool with_filenames{true};
    bool icase{false};
    bool report_hint{true};
    GoalSetting strict{GoalSetting::AUTO};
    GoalSetting best{GoalSetting::AUTO};
    GoalSetting clean_requirements_on_remove{GoalSetting::AUTO};
    std::vector<std::string> from_repo_ids;
    std::vector<std::string> to_repo_ids;
    std::vector<libdnf::rpm::Nevra::Form> forms;

private:
    friend class Goal;
    bool get_strict(const libdnf::ConfigMain & cfg_main) const;
    bool get_best(const libdnf::ConfigMain & cfg_main) const;
    bool get_clean_requirements_on_remove(const libdnf::ConfigMain & cfg_main) const;
};


inline GoalProblem operator|(GoalProblem lhs, GoalProblem rhs) {
    return static_cast<GoalProblem>(
        static_cast<std::underlying_type<GoalProblem>::type>(lhs) |
        static_cast<std::underlying_type<GoalProblem>::type>(rhs));
}

inline GoalProblem operator|=(GoalProblem & lhs, GoalProblem rhs) {
    lhs = static_cast<GoalProblem>(
        static_cast<std::underlying_type<GoalProblem>::type>(lhs) |
        static_cast<std::underlying_type<GoalProblem>::type>(rhs));
    return lhs;
}

inline GoalProblem operator&(GoalProblem lhs, GoalProblem rhs) {
    return static_cast<GoalProblem>(
        static_cast<std::underlying_type<GoalProblem>::type>(lhs) &
        static_cast<std::underlying_type<GoalProblem>::type>(rhs));
}

inline bool GoalJobSettings::get_strict(const libdnf::ConfigMain & cfg_main) const {
    switch (strict) {
        case GoalSetting::AUTO:
            return cfg_main.strict().get_value();
        case GoalSetting::SET_TRUE:
            return true;
        case GoalSetting::SET_FALSE:
            return false;
    }
    throw LogicError("Invalid 'best' value in GoalJobSettings");
}

inline bool GoalJobSettings::get_best(const libdnf::ConfigMain & cfg_main) const {
    switch (best) {
        case GoalSetting::AUTO:
            return cfg_main.best().get_value();
        case GoalSetting::SET_TRUE:
            return true;
        case GoalSetting::SET_FALSE:
            return false;
    }
    throw LogicError("Invalid 'strict' value in GoalJobSettings");
}

inline bool GoalJobSettings::get_clean_requirements_on_remove(const libdnf::ConfigMain & cfg_main) const {
    switch (clean_requirements_on_remove) {
        case GoalSetting::AUTO:
            return cfg_main.clean_requirements_on_remove().get_value();
        case GoalSetting::SET_TRUE:
            return true;
        case GoalSetting::SET_FALSE:
            return false;
    }
    throw LogicError("Invalid 'clean_requirements_on_remove' value in GoalJobSettings");
}


}  // namespace libdnf

#endif
