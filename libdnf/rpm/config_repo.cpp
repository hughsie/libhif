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

#include "libdnf/rpm/config_repo.hpp"

#include "../conf/config_utils.hpp"

#include "libdnf/conf/const.hpp"

namespace libdnf::rpm {

class ConfigRepo::Impl {
    friend class ConfigRepo;

    Impl(Config & owner, ConfigMain & master_config);

    Config & owner;
    ConfigMain & master_config;

    OptionString name{""};
    OptionChild<OptionBool> enabled{master_config.enabled()};
    OptionChild<OptionString> basecachedir{master_config.cachedir()};
    OptionStringList baseurl{std::vector<std::string>{}};
    OptionString mirrorlist{nullptr};
    OptionString metalink{nullptr};
    OptionString type{""};
    OptionString mediaid{""};
    OptionStringList gpgkey{std::vector<std::string>{}};
    OptionStringList excludepkgs{std::vector<std::string>{}};
    OptionStringList includepkgs{std::vector<std::string>{}};
    OptionChild<OptionBool> fastestmirror{master_config.fastestmirror()};
    OptionChild<OptionString> proxy{master_config.proxy()};
    OptionChild<OptionString> proxy_username{master_config.proxy_username()};
    OptionChild<OptionString> proxy_password{master_config.proxy_password()};
    OptionChild<OptionEnum<std::string>> proxy_auth_method{master_config.proxy_auth_method()};
    OptionChild<OptionString> username{master_config.username()};
    OptionChild<OptionString> password{master_config.password()};
    OptionChild<OptionStringList> protected_packages{master_config.protected_packages()};
    OptionChild<OptionBool> gpgcheck{master_config.gpgcheck()};
    OptionChild<OptionBool> repo_gpgcheck{master_config.repo_gpgcheck()};
    OptionChild<OptionBool> enablegroups{master_config.enablegroups()};
    OptionChild<OptionNumber<std::uint32_t>> retries{master_config.retries()};
    OptionChild<OptionNumber<std::uint32_t>> bandwidth{master_config.bandwidth()};
    OptionChild<OptionNumber<std::uint32_t>> minrate{master_config.minrate()};
    OptionChild<OptionEnum<std::string>> ip_resolve{master_config.ip_resolve()};
    OptionChild<OptionNumber<float>> throttle{master_config.throttle()};
    OptionChild<OptionSeconds> timeout{master_config.timeout()};
    OptionChild<OptionNumber<std::uint32_t>> max_parallel_downloads{master_config.max_parallel_downloads()};
    OptionChild<OptionSeconds> metadata_expire{master_config.metadata_expire()};
    OptionNumber<std::int32_t> cost{1000};
    OptionNumber<std::int32_t> priority{99};
    OptionBool module_hotfixes{false};
    OptionChild<OptionString> sslcacert{master_config.sslcacert()};
    OptionChild<OptionBool> sslverify{master_config.sslverify()};
    OptionChild<OptionString> sslclientcert{master_config.sslclientcert()};
    OptionChild<OptionString> sslclientkey{master_config.sslclientkey()};
    OptionChild<OptionBool> deltarpm{master_config.deltarpm()};
    OptionChild<OptionNumber<std::uint32_t>> deltarpm_percentage{master_config.deltarpm_percentage()};
    OptionChild<OptionBool> skip_if_unavailable{master_config.skip_if_unavailable()};
    OptionString enabled_metadata{""};
    OptionChild<OptionString> user_agent{master_config.user_agent()};
    OptionChild<OptionBool> countme{master_config.countme()};
    OptionEnum<std::string> failovermethod{"priority", {"priority", "roundrobin"}};
    OptionBool build_cache{true};
};

ConfigRepo::Impl::Impl(Config & owner, ConfigMain & master_config) : owner(owner), master_config(master_config) {
    owner.opt_binds().add("name", name);
    owner.opt_binds().add("enabled", enabled);
    owner.opt_binds().add("cachedir", basecachedir);
    owner.opt_binds().add("baseurl", baseurl);
    owner.opt_binds().add("mirrorlist", mirrorlist);
    owner.opt_binds().add("metalink", metalink);
    owner.opt_binds().add("type", type);
    owner.opt_binds().add("mediaid", mediaid);
    owner.opt_binds().add("gpgkey", gpgkey);

    owner.opt_binds().add(
        "excludepkgs",
        excludepkgs,
        [&](Option::Priority priority, const std::string & value) {
            option_T_list_append(excludepkgs, priority, value);
        },
        nullptr,
        true);
    //compatibility with yum
    owner.opt_binds().add(
        "exclude",
        excludepkgs,
        [&](Option::Priority priority, const std::string & value) {
            option_T_list_append(excludepkgs, priority, value);
        },
        nullptr,
        true);

    owner.opt_binds().add(
        "includepkgs",
        includepkgs,
        [&](Option::Priority priority, const std::string & value) {
            option_T_list_append(includepkgs, priority, value);
        },
        nullptr,
        true);

    owner.opt_binds().add("fastestmirror", fastestmirror);

    owner.opt_binds().add(
        "proxy",
        proxy,
        [&](Option::Priority priority, const std::string & value) {
            auto tmp_value(value);
            for (auto & ch : tmp_value) {
                ch = static_cast<char>(std::tolower(ch));
            }
            if (tmp_value == "_none_") {
                proxy.set(priority, "");
            } else {
                proxy.set(priority, value);
            }
        },
        nullptr,
        false);

    owner.opt_binds().add("proxy_username", proxy_username);
    owner.opt_binds().add("proxy_password", proxy_password);
    owner.opt_binds().add("proxy_auth_method", proxy_auth_method);
    owner.opt_binds().add("username", username);
    owner.opt_binds().add("password", password);
    owner.opt_binds().add("protected_packages", protected_packages);
    owner.opt_binds().add("gpgcheck", gpgcheck);
    owner.opt_binds().add("repo_gpgcheck", repo_gpgcheck);
    owner.opt_binds().add("enablegroups", enablegroups);
    owner.opt_binds().add("retries", retries);
    owner.opt_binds().add("bandwidth", bandwidth);
    owner.opt_binds().add("minrate", minrate);
    owner.opt_binds().add("ip_resolve", ip_resolve);
    owner.opt_binds().add("throttle", throttle);
    owner.opt_binds().add("timeout", timeout);
    owner.opt_binds().add("max_parallel_downloads", max_parallel_downloads);
    owner.opt_binds().add("metadata_expire", metadata_expire);
    owner.opt_binds().add("cost", cost);
    owner.opt_binds().add("priority", priority);
    owner.opt_binds().add("module_hotfixes", module_hotfixes);
    owner.opt_binds().add("sslcacert", sslcacert);
    owner.opt_binds().add("sslverify", sslverify);
    owner.opt_binds().add("sslclientcert", sslclientcert);
    owner.opt_binds().add("sslclientkey", sslclientkey);
    owner.opt_binds().add("deltarpm", deltarpm);
    owner.opt_binds().add("deltarpm_percentage", deltarpm_percentage);
    owner.opt_binds().add("skip_if_unavailable", skip_if_unavailable);
    owner.opt_binds().add("enabled_metadata", enabled_metadata);
    owner.opt_binds().add("user_agent", user_agent);
    owner.opt_binds().add("countme", countme);
    owner.opt_binds().add("build_cache", build_cache);
}

ConfigRepo::ConfigRepo(ConfigMain & master_config) : p_impl(new Impl(*this, master_config)) {}
ConfigRepo::~ConfigRepo() = default;
ConfigRepo::ConfigRepo(ConfigRepo && src) : p_impl(std::move(src.p_impl)) {}

ConfigMain & ConfigRepo::get_master_config() {
    return p_impl->master_config;
}

OptionString & ConfigRepo::name() {
    return p_impl->name;
}
OptionChild<OptionBool> & ConfigRepo::enabled() {
    return p_impl->enabled;
}
OptionChild<OptionString> & ConfigRepo::basecachedir() {
    return p_impl->basecachedir;
}
OptionStringList & ConfigRepo::baseurl() {
    return p_impl->baseurl;
}
OptionString & ConfigRepo::mirrorlist() {
    return p_impl->mirrorlist;
}
OptionString & ConfigRepo::metalink() {
    return p_impl->metalink;
}
OptionString & ConfigRepo::type() {
    return p_impl->type;
}
OptionString & ConfigRepo::mediaid() {
    return p_impl->mediaid;
}
OptionStringList & ConfigRepo::gpgkey() {
    return p_impl->gpgkey;
}
OptionStringList & ConfigRepo::excludepkgs() {
    return p_impl->excludepkgs;
}
OptionStringList & ConfigRepo::includepkgs() {
    return p_impl->includepkgs;
}
OptionChild<OptionBool> & ConfigRepo::fastestmirror() {
    return p_impl->fastestmirror;
}
OptionChild<OptionString> & ConfigRepo::proxy() {
    return p_impl->proxy;
}
OptionChild<OptionString> & ConfigRepo::proxy_username() {
    return p_impl->proxy_username;
}
OptionChild<OptionString> & ConfigRepo::proxy_password() {
    return p_impl->proxy_password;
}
OptionChild<OptionEnum<std::string>> & ConfigRepo::proxy_auth_method() {
    return p_impl->proxy_auth_method;
}
OptionChild<OptionString> & ConfigRepo::username() {
    return p_impl->username;
}
OptionChild<OptionString> & ConfigRepo::password() {
    return p_impl->password;
}
OptionChild<OptionStringList> & ConfigRepo::protected_packages() {
    return p_impl->protected_packages;
}
OptionChild<OptionBool> & ConfigRepo::gpgcheck() {
    return p_impl->gpgcheck;
}
OptionChild<OptionBool> & ConfigRepo::repo_gpgcheck() {
    return p_impl->repo_gpgcheck;
}
OptionChild<OptionBool> & ConfigRepo::enablegroups() {
    return p_impl->enablegroups;
}
OptionChild<OptionNumber<std::uint32_t>> & ConfigRepo::retries() {
    return p_impl->retries;
}
OptionChild<OptionNumber<std::uint32_t>> & ConfigRepo::bandwidth() {
    return p_impl->bandwidth;
}
OptionChild<OptionNumber<std::uint32_t>> & ConfigRepo::minrate() {
    return p_impl->minrate;
}
OptionChild<OptionEnum<std::string>> & ConfigRepo::ip_resolve() {
    return p_impl->ip_resolve;
}
OptionChild<OptionNumber<float>> & ConfigRepo::throttle() {
    return p_impl->throttle;
}
OptionChild<OptionSeconds> & ConfigRepo::timeout() {
    return p_impl->timeout;
}
OptionChild<OptionNumber<std::uint32_t>> & ConfigRepo::max_parallel_downloads() {
    return p_impl->max_parallel_downloads;
}
OptionChild<OptionSeconds> & ConfigRepo::metadata_expire() {
    return p_impl->metadata_expire;
}
OptionNumber<std::int32_t> & ConfigRepo::cost() {
    return p_impl->cost;
}
OptionNumber<std::int32_t> & ConfigRepo::priority() {
    return p_impl->priority;
}
OptionBool & ConfigRepo::module_hotfixes() {
    return p_impl->module_hotfixes;
}
OptionChild<OptionString> & ConfigRepo::sslcacert() {
    return p_impl->sslcacert;
}
OptionChild<OptionBool> & ConfigRepo::sslverify() {
    return p_impl->sslverify;
}
OptionChild<OptionString> & ConfigRepo::sslclientcert() {
    return p_impl->sslclientcert;
}
OptionChild<OptionString> & ConfigRepo::sslclientkey() {
    return p_impl->sslclientkey;
}
OptionChild<OptionBool> & ConfigRepo::deltarpm() {
    return p_impl->deltarpm;
}
OptionChild<OptionNumber<std::uint32_t>> & ConfigRepo::deltarpm_percentage() {
    return p_impl->deltarpm_percentage;
}
OptionChild<OptionBool> & ConfigRepo::skip_if_unavailable() {
    return p_impl->skip_if_unavailable;
}
OptionString & ConfigRepo::enabled_metadata() {
    return p_impl->enabled_metadata;
}
OptionChild<OptionString> & ConfigRepo::user_agent() {
    return p_impl->user_agent;
}
OptionChild<OptionBool> & ConfigRepo::countme() {
    return p_impl->countme;
}
OptionEnum<std::string> & ConfigRepo::failovermethod() {
    return p_impl->failovermethod;
}
OptionBool & ConfigRepo::build_cache() {
    return p_impl->build_cache;
}
}  // namespace libdnf::rpm
