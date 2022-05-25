/*
Copyright Contributors to the libdnf project.

This file is part of libdnf: https://github.com/rpm-software-management/libdnf/

Libdnf is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Libdnf is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with libdnf.  If not, see <https://www.gnu.org/licenses/>.
*/


#include "advisory_summary.hpp"

#include <libdnf-cli/output/advisorysummary.hpp>
#include <libdnf/rpm/package_query.hpp>

#include <filesystem>
#include <fstream>
#include <set>


namespace dnf5 {


using namespace libdnf::cli;


AdvisorySummaryCommand::AdvisorySummaryCommand(Command & parent)
    : AdvisorySubCommand(parent, "summary", _("Print summary of advisories")) {}

void AdvisorySummaryCommand::process_and_print_queries(
    Context & ctx, libdnf::advisory::AdvisoryQuery & advisories, libdnf::rpm::PackageQuery & packages) {
    std::string mode;

    if (all->get_value()) {
        packages.filter_installed();
        advisories.filter_packages(packages, libdnf::sack::QueryCmp::LTE);
        auto advisory_query_not_installed = libdnf::advisory::AdvisoryQuery(ctx.base);
        advisory_query_not_installed.filter_packages(packages, libdnf::sack::QueryCmp::GT);
        advisories |= advisory_query_not_installed;
        mode = _("All");
    } else if (installed->get_value()) {
        packages.filter_installed();
        advisories.filter_packages(packages, libdnf::sack::QueryCmp::LTE);
        mode = _("Installed");
    } else if (updates->get_value()) {
        packages.filter_upgradable();
        advisories.filter_packages(packages, libdnf::sack::QueryCmp::GT);
        mode = _("Updates");
    } else {  // available is the default
        packages.filter_installed();
        packages.filter_latest_evr();

        add_running_kernel_packages(ctx.base, packages);

        advisories.filter_packages(packages, libdnf::sack::QueryCmp::GT);
        mode = _("Available");
    }

    libdnf::cli::output::print_advisorysummary_table(advisories, mode);
}

}  // namespace dnf5
