/*
 * Copyright (C) 2020 Red Hat, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


#include "configuration.hpp"

#include <fmt/format.h>
#include <glob.h>
#include <rpm/rpmlib.h>

#include <filesystem>
#include <iostream>

Configuration::Configuration(Session & session) : cfg_main(new libdnf::ConfigMain), session(session) {
    install_root = session.session_configuration_value<std::string>("installroot", "/");
}

void Configuration::read_configuration() {
    read_main_config();
    read_repo_configs();
}

std::string Configuration::prepend_install_root(const std::string & path) {
    auto fs_path = std::filesystem::path(path);
    return (std::filesystem::path(install_root) / fs_path.relative_path()).string();
}

void Configuration::read_main_config() {
    auto & logger = session.get_base()->get_logger();
    auto main_config_path = prepend_install_root(cfg_main->config_file_path().get_value());
    auto base = session.get_base();

    try {
        // create new main config parser and read the config file
        std::unique_ptr<libdnf::ConfigParser> main_parser(new libdnf::ConfigParser);

        main_parser->read(main_config_path);
        cfg_main->load_from_parser(
            *main_parser,
            "main",
            base->get_vars(),
            base->get_logger()
        );

        base->get_vars().load(cfg_main->installroot().get_value(), cfg_main->varsdir().get_value());

        // read repos possibly configured in the main config file
        read_repos(main_parser.get(), main_config_path);
        // store the parser so it can be used for saving the config file later on
        config_parsers[std::move(main_config_path)] = std::move(main_parser);
    } catch (const std::exception & e) {
        logger.warning(fmt::format("Error parsing config file \"{}\": {}", main_config_path, e.what()));
    }
}

void Configuration::read_repos(const libdnf::ConfigParser * repo_parser, const std::string & file_path) {
    const auto & cfg_parser_data = repo_parser->get_data();
    for (const auto & cfg_parser_data_iter : cfg_parser_data) {
        if (cfg_parser_data_iter.first != "main") {
            // each section other than [main] is considered a repository
            auto section = cfg_parser_data_iter.first;
            std::unique_ptr<libdnf::rpm::ConfigRepo> cfg_repo(new libdnf::rpm::ConfigRepo(*cfg_main));

            cfg_repo->load_from_parser(
                *repo_parser,
                section,
                session.get_base()->get_vars(),
                session.get_base()->get_logger()
            );
            // save configured repo
            std::unique_ptr<RepoInfo> repoinfo(new RepoInfo());
            repoinfo->file_path = std::string(file_path);
            repoinfo->repoconfig = std::move(cfg_repo);
            repos[std::move(section)] = std::move(repoinfo);
        }
    }
}

void Configuration::read_repo_configs() {
    auto & logger = session.get_base()->get_logger();
    for (const auto & repos_dir : cfg_main->reposdir().get_value()) {
        // use canonical to resolve symlinks in repos_dir
        std::string pattern;
        try {
            pattern = std::filesystem::canonical(prepend_install_root(repos_dir)).string() + "/*.repo";
        } catch (std::filesystem::filesystem_error & e) {
            logger.debug(
                fmt::format("Error reading repository configuration directory \"{}\": {}", repos_dir, e.what()));
            continue;
        }
        glob_t glob_result;
        glob(pattern.c_str(), GLOB_MARK, nullptr, &glob_result);
        for (size_t i = 0; i < glob_result.gl_pathc; ++i) {
            std::unique_ptr<libdnf::ConfigParser> repo_parser(new libdnf::ConfigParser);
            std::string file_path = glob_result.gl_pathv[i];
            try {
                repo_parser->read(file_path);
            } catch (libdnf::ConfigParser::Exception & e) {
                logger.warning(fmt::format("Error parsing config file \"{}\": {}", file_path, e.what()));
                continue;
            }
            read_repos(repo_parser.get(), file_path);
            config_parsers[std::string(file_path)] = std::move(repo_parser);
        }
        globfree(&glob_result);
    }
}

Configuration::RepoInfo * Configuration::find_repo(const std::string & repoid) {
    auto repo_iter = repos.find(repoid);
    if (repo_iter == repos.end()) {
        return nullptr;
    }
    return repo_iter->second.get();
}

libdnf::ConfigParser * Configuration::find_parser(const std::string & file_path) {
    auto parser_iter = config_parsers.find(file_path);
    if (parser_iter == config_parsers.end()) {
        return nullptr;
    }
    return parser_iter->second.get();
}
