/*
Copyright (C) 2020 Red Hat, Inc.

This file is part of dnfdaemon-client: https://github.com/rpm-software-management/libdnf/

Dnfdaemon-client is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Dnfdaemon-client is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with dnfdaemon-client.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "repolist.hpp"

#include "../../context.hpp"
#include "libdnf-cli/output/repolist.hpp"

#include <dnfdaemon-server/dbus.hpp>
#include <libdnf/conf/option_string.hpp>
#include <libdnf-cli/output/repo_info.hpp>

#include <iostream>
#include <numeric>

namespace dnfdaemon::client {

void CmdRepolist::set_argument_parser(Context & ctx) {
    enable_disable_option = dynamic_cast<libdnf::OptionEnum<std::string> *>(
        ctx.arg_parser.add_init_value(std::unique_ptr<libdnf::OptionEnum<std::string>>(
            new libdnf::OptionEnum<std::string>("enabled", {"all", "enabled", "disabled"}))));

    auto all = ctx.arg_parser.add_new_named_arg("all");
    all->set_long_name("all");
    all->set_short_description("show all repos");
    all->set_const_value("all");
    all->link_value(enable_disable_option);

    auto enabled = ctx.arg_parser.add_new_named_arg("enabled");
    enabled->set_long_name("enabled");
    enabled->set_short_description("show enabled repos (default)");
    enabled->set_const_value("enabled");
    enabled->link_value(enable_disable_option);

    auto disabled = ctx.arg_parser.add_new_named_arg("disabled");
    disabled->set_long_name("disabled");
    disabled->set_short_description("show disabled repos");
    disabled->set_const_value("disabled");
    disabled->link_value(enable_disable_option);

    patterns_options = ctx.arg_parser.add_new_values();
    auto repos = ctx.arg_parser.add_new_positional_arg(
        "repos_to_show",
        libdnf::cli::ArgumentParser::PositionalArg::UNLIMITED,
        ctx.arg_parser.add_init_value(std::unique_ptr<libdnf::Option>(new libdnf::OptionString(nullptr))),
        patterns_options);
    repos->set_short_description("List of repos to show");

    auto conflict_args =
        ctx.arg_parser.add_conflict_args_group(std::unique_ptr<std::vector<libdnf::cli::ArgumentParser::Argument *>>(
            new std::vector<libdnf::cli::ArgumentParser::Argument *>{all, enabled, disabled}));

    all->set_conflict_arguments(conflict_args);
    enabled->set_conflict_arguments(conflict_args);
    disabled->set_conflict_arguments(conflict_args);

    auto repolist = ctx.arg_parser.add_new_command(command);
    repolist->set_short_description("display the configured software repositories");
    repolist->set_description("");
    repolist->set_named_args_help_header("Optional arguments:");
    repolist->set_positional_args_help_header("Positional arguments:");
    repolist->set_parse_hook_func([this, &ctx](
                                      [[maybe_unused]] libdnf::cli::ArgumentParser::Argument * arg,
                                      [[maybe_unused]] const char * option,
                                      [[maybe_unused]] int argc,
                                      [[maybe_unused]] const char * const argv[]) {
        ctx.select_command(this);
        return true;
    });

    repolist->register_named_arg(all);
    repolist->register_named_arg(enabled);
    repolist->register_named_arg(disabled);
    repolist->register_positional_arg(repos);

    ctx.arg_parser.get_root_command()->register_command(repolist);
}

/// Joins vector of strings to a single string using given separator
/// ["a", "b", "c"] -> "a b c"
std::string join(const std::vector<std::string> & str_list, const std::string & separator) {
    return std::accumulate(
        str_list.begin(), str_list.end(), std::string(),
        [&](const std::string & a, const std::string & b) -> std::string {
            return a + (a.length() > 0 ? separator : "") + b;
        });
}

/// Joins vector of string pairs to a single string. Pairs are joined using
/// field_separator, records using record_separator
/// [("a", "1"), ("b", "2")] -> "a: 1, b: 2"
std::string join_pairs(const std::vector<std::pair<std::string, std::string>> & pair_list, const std::string & field_separator, const std::string & record_separator) {
    std::vector<std::string> records {};
    for (auto & pair: pair_list) {
        records.emplace_back(pair.first + field_separator + pair.second);
    }
    return join(records, record_separator);
}

class RepoDbus {
public:
    RepoDbus(dnfdaemon::KeyValueMap & rawdata) : rawdata(rawdata){};
    std::string get_id() const { return rawdata.at("id"); }
    std::string get_name() const { return rawdata.at("name"); }
    bool is_enabled() const { return rawdata.at("enabled"); }
    uint64_t get_size() const { return rawdata.at("size"); }
    std::string get_revision() const { return rawdata.at("revision"); }
    std::vector<std::string> get_content_tags() const { return rawdata.at("content_tags"); }
    std::vector<std::pair<std::string, std::string>> get_distro_tags() const {
        // sdbus::Variant cannot handle vector<pair<string,string>> so values are
        // serialized to vector<string>.
        // convert [tag1, val1, tag2, val2,...] back to [(tag1, val1), (tag2, val2),...]
        std::vector<std::pair<std::string, std::string>> dt {};
        std::vector<std::string> tags_raw = rawdata.at("distro_tags");
        if (!tags_raw.empty()) {
            for(size_t i = 0; i < (tags_raw.size() - 1); i+=2){
                dt.emplace_back(tags_raw[i], tags_raw[i + 1]);
            }
        }
        return dt;
    }
    int get_max_timestamp() const { return rawdata.at("updated"); }
    uint64_t get_pkgs() const { return rawdata.at("pkgs"); }
    uint64_t get_available_pkgs() const { return rawdata.at("available_pkgs"); }
    std::string get_metalink() const { return rawdata.at("metalink"); }
    std::string get_mirrorlist() const { return rawdata.at("mirrorlist"); }
    std::vector<std::string> get_baseurl() const { return rawdata.at("baseurl"); }
    int get_metadata_expire() const { return rawdata.at("metadata_expire"); }
    std::vector<std::string> get_excludepkgs() const { return rawdata.at("excludepkgs"); }
    std::vector<std::string> get_includepkgs() const { return rawdata.at("includepkgs"); }
    std::string get_repofile() const { return rawdata.at("repofile"); }

private:
    dnfdaemon::KeyValueMap rawdata;
};

class QueryRepoDbus {
public:
    QueryRepoDbus(dnfdaemon::KeyValueMapList & repositories) {
        for (auto raw_repo : repositories) {
            queryrepo.push_back(std::make_unique<RepoDbus>(raw_repo));
        }
    }

    const std::vector< std::unique_ptr<RepoDbus> > & get_data() const { return queryrepo; }

private:
    std::vector< std::unique_ptr<RepoDbus> > queryrepo;
};

void CmdRepolist::run(Context & ctx) {
    // prepare options from command line arguments
    dnfdaemon::KeyValueMap options = {};
    options["enable_disable"] = enable_disable_option->get_value();
    std::vector<std::string> patterns;
    if (!patterns_options->empty()) {
        options["enable_disable"] = "all";
        patterns.reserve(patterns_options->size());
        for (auto & pattern : *patterns_options) {
            auto option = dynamic_cast<libdnf::OptionString *>(pattern.get());
            patterns.emplace_back(option->get_value());
        }
    }
    options["patterns"] = patterns;

    std::vector<std::string> attrs {"id", "name", "enabled"};
    if (command == "repoinfo") {
        std::vector<std::string> repoinfo_attrs {
            "size", "revision", "content_tags", "distro_tags", "updated",
            "pkgs", "available_pkgs", "metalink", "mirrorlist", "baseurl",
            "metadata_expire", "excludepkgs", "includepkgs", "repofile"};
        attrs.insert(attrs.end(), repoinfo_attrs.begin(), repoinfo_attrs.end());
    }
    options["repo_attrs"] = attrs;

    // call list() method on repo interface via dbus
    dnfdaemon::KeyValueMapList repositories;
    ctx.session_proxy->callMethod("list")
        .onInterface(dnfdaemon::INTERFACE_REPO)
        .withArguments(options)
        .storeResultsTo(repositories);

    if (command == "repolist") {
        // print the output table
        bool with_status = enable_disable_option->get_value() == "all";
        libdnf::cli::output::print_repolist_table(
            QueryRepoDbus(repositories), with_status,
            libdnf::cli::output::COL_REPO_ID);
    } else {
        // repoinfo command

        for (auto & raw_repo : repositories) {
            RepoDbus repo(raw_repo);
            auto repo_info = libdnf::cli::output::RepoInfo();
            repo_info.add_repo(repo, ctx.verbose.get_value(), ctx.verbose.get_value());
            repo_info.print();
            std::cout << std::endl;
        }
    }
}

}  // namespace dnfdaemon::client
