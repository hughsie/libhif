/*
Copyright (C) 2020 Red Hat, Inc.

This file is part of dnfdaemon-server: https://github.com/rpm-software-management/libdnf/

Dnfdaemon-server is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Dnfdaemon-server is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with dnfdaemon-server.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "dnfdaemon-server/types.hpp"
#include "configuration.hpp"
#include "RepoConf.hpp"

#include <sdbus-c++/sdbus-c++.h>

#include <string>

void RepoConf::dbus_register(const std::string object_path)
{
    const std::string interfaceName = "org.rpm.dnf.v0.rpm.RepoConf";
    dbus_object = sdbus::createObject(session.connection, object_path);
    dbus_object->registerMethod(interfaceName, "list", "as", "aa{sv}", [this](sdbus::MethodCall call) -> void {this->list(call);});
    dbus_object->registerMethod(interfaceName, "get", "s", "a{sv}", [this](sdbus::MethodCall call) -> void {this->get(call);});
    dbus_object->registerMethod(interfaceName, "enable", "as", "as", [this](sdbus::MethodCall call) -> void {this->enable_disable(call, true);});
    dbus_object->registerMethod(interfaceName, "disable", "as", "as", [this](sdbus::MethodCall call) -> void {this->enable_disable(call, false);});
    dbus_object->finishRegistration();
}

void RepoConf::dbus_deregister() {
    dbus_object->unregister();
}

bool RepoConf::check_authorization(const std::string &actionid, const std::string &sender)
{
    // create proxy for PolicyKit1 object
    const std::string destinationName = "org.freedesktop.PolicyKit1";
    const std::string objectPath = "/org/freedesktop/PolicyKit1/Authority";
    const std::string interfaceName = "org.freedesktop.PolicyKit1.Authority";
    auto polkit_proxy = sdbus::createProxy(session.connection, destinationName, objectPath);
    polkit_proxy->finishRegistration();

    // call CheckAuthorization method
    sdbus::Struct<bool, bool, std::map<std::string, std::string>> auth_result;
    sdbus::Struct<std::string, KeyValueMap> subject {"system-bus-name", {{"name", sender}}};
    std::map<std::string, std::string> details {};
    uint flags = 0;
    std::string cancelation_id = "";
    polkit_proxy->callMethod("CheckAuthorization").onInterface(interfaceName).withArguments(subject, actionid, details, flags, cancelation_id).storeResultsTo(auth_result);

    // get results
    bool res_is_authorized = std::get<0>(auth_result);
    /*
    bool res_is_challenge = std::get<1>(auth_result);
    std::map<std::string, std::string> res_details = std::get<2>(auth_result);
    */

    return res_is_authorized;
}

KeyValueMapList RepoConf::repo_list(const std::vector<std::string> &ids)
{
    auto install_root = session.session_configuration_value<std::string>("installroot", "/");
    Configuration cfg(install_root);
    cfg.read_configuration();

    bool empty_ids = ids.empty();
    KeyValueMapList out;
    for (auto &repo: cfg.get_repos()) {
        if (empty_ids || std::find(ids.begin(), ids.end(), repo.first) != ids.end()) {
            auto parser = cfg.find_parser(repo.second->file_path);
            if (parser) {
                KeyValueMap dbus_repo;
                dbus_repo.emplace(std::make_pair("repoid", repo.first));
                for (const auto &section: parser->get_data()) {
                    if (section.first == repo.first) {
                        for (const auto &line: section.second) {
                            if (line.first[0] != '#') {
                                dbus_repo.emplace(std::make_pair(line.first, line.second));
                            }
                        }
                    }
                }
                out.push_back(dbus_repo);
            }
        }
    }
    return out;
}

void RepoConf::list(sdbus::MethodCall call)
{
    std::vector<std::string> ids;
    call >> ids;

    auto out = repo_list(std::move(ids));

    auto reply = call.createReply();
    reply << out;
    reply.send();
}

void RepoConf::get(sdbus::MethodCall call)
{
    std::string id;
    call >> id;

    auto ids = std::vector<std::string>{std::move(id)};
    auto lst = repo_list(ids);
    if (lst.empty()) {
        throw sdbus::Error(REPO_CONF_ERROR, "Repository not found");
    } else if (lst.size() > 1) {
        throw sdbus::Error(REPO_CONF_ERROR, "Multiple repositories found");
    } else {
        auto reply = call.createReply();
        reply << lst[0];
        reply.send();
    }
}

std::vector<std::string> RepoConf::enable_disable_repos(const std::vector<std::string> &ids, const bool enable) {
    auto install_root = session.session_configuration_value<std::string>("installroot", "/");
    Configuration cfg(install_root);
    cfg.read_configuration();

    std::vector<std::string> out;
    std::vector<std::string> changed_config_files;
    for (auto &repoid: ids) {
        auto repoinfo = cfg.find_repo(repoid);
        if (repoinfo && repoinfo->repoconfig->enabled().get_value() != enable) {
            auto parser = cfg.find_parser(repoinfo->file_path);
            if (parser) {
                parser->set_value(repoid, "enabled", enable ? "1" : "0");
                changed_config_files.push_back(repoinfo->file_path);
                out.push_back(repoid);
            }
        }
    }
    for (auto &config_file: changed_config_files) {
        try {
            cfg.find_parser(config_file)->write(config_file, false);
        } catch (std::exception &e) {
            throw sdbus::Error(REPO_CONF_ERROR, std::string("Unable to write configuration file: ") + e.what());
        }
    }

    return out;
}

void RepoConf::enable_disable(sdbus::MethodCall call, const bool enable)
{
    auto sender = call.getSender();
    auto is_authorized = check_authorization("org.rpm.dnf.v0.rpm.RepoConf.write", sender);
    if (!is_authorized) {
        throw sdbus::Error(REPO_CONF_ERROR, "Not authorised.");
    }
    std::vector<std::string> ids;
    call >> ids;
    auto reply = call.createReply();
    reply << enable_disable_repos(ids, enable);
    reply.send();
}
