..
  Copyright (C) 2014-2018 Red Hat, Inc.

  This copyrighted material is made available to anyone wishing to use,
  modify, copy, or redistribute it subject to the terms and conditions of
  the GNU General Public License v.2, or (at your option) any later version.
  This program is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY expressed or implied, including the implied warranties of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
  Public License for more details.  You should have received a copy of the
  GNU General Public License along with this program; if not, write to the
  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.  Any Red Hat trademarks that are incorporated in the
  source code or documentation are not subject to the GNU General Public
  License and may only be used or replicated with the express permission of
  Red Hat, Inc.

######################
 LIBDNF Release Notes
######################

====================
0.74.0 Release Notes
====================

- Fix a segfault in iterator of a ConfigParser section
- Update ko.po
- Split $releasever to $releasever_major and $releasever_minor in c api
- copr: Add Copr build files
- Add `persistence` config option
- ConfigParser: make splitReleasever public
- C API: Detect releasever_major, releasever_minor from provides
- C API: Use releasever_{major,minor} from context instead of always splitting
- C API: support shell-style variable substitution
- C API: test shell-style variable expressions
- conf: Improve granularity of ConfigParser exceptions
- module: Warn if module config file is inaccessible
- Enable automatic PR reviews
- build: Increase CMake minimal version to 3.5.0
- spec: Increase cmake minimal version to 3.5.0

====================
0.73.4 Release Notes
====================

- Set `POOL_FLAG_ADDFILEPROVIDESFILTERED` only when not loading filelists

====================
0.73.3 Release Notes
====================

- Support colon in username, use LRO_USERNAME and LRO_PASSWORD
- Set pool flag to fix pool_addfileprovides_queue() without filelists.xml
- Fix a memory leak in glob_for_cachedir()

====================
0.73.2 Release Notes
====================

- Bug fixes:
  - MergedTransaction: Fix invalid memory access when dropping items
  - ConfigParser: fix use-out-of-scope leaks
  - Fix "microdnf cannot reinstall curl-minimal"
  - Fix up some comments in addCountmeFlag()
  - Fix countme bucket calculation

- Others:
  - Add tests for shell-style variable expansion

====================
0.73.1 Release Notes
====================

- Bug fixes:
  - Fix https://issues.redhat.com/browse/RHEL-27657
  - subject-py: Fix memory leak

- Others:
  - MergedTransaction: Calculate RPM difference between two same versions as no-op
  - Onboard packit tests
  - Add virtual destructor to TransactionItem

====================
0.73.0 Release Notes
====================

- Major changes:
  - filelists metadata not loaded by default
  - deltarpm disabled by default

- New features:
  - conf: Introduce new optional_metadata_types option to load filelists on demand
  - goal: Method for detecting file dependency problems

Bugs fixed in 0.73.0:

* :rhbug:`2252128`
* :rhbug:`2254789`

====================
0.72.0 Release Notes
====================

- New features:
  - conf: Add limited shell-style variable expansion (RhBug:1789346)
  - conf: Add support for $releasever_major, $releasever_minor (RhBug:1789346)
  - repo: Don't download the repository if the local cache is up to date

- Bug fixes:
  - Avoid reinstalling installonly packages marked for ERASE (RhBug:2163474)
  - transaction: Save the reason for installing (RhBug:1733274)
  - hawkey.subject: get_best_selectors only obsoleters of latest (RhBug:2183279,2176263)

- Others:
  - Allow DNF to be removed by DNF 5 (RhBug:2221907)
  - Include dist-info for python3-libdnf
  - bindings: Load all modules with RTLD_GLOBAL
  - Update translations

Bugs fixed in 0.72.0:

* :rhbug:`2163474`
* :rhbug:`1733274`
* :rhbug:`2183279`
* :rhbug:`2176263`
* :rhbug:`1789346`
* :rhbug:`2221907`

====================
0.71.0 Release Notes
====================

- New features:
  - PGP: Use new librepo PGP API, remove gpgme dependency
  - API: Basic support for OpenPGP public keys
  - Make code C++20 compatible

- Bug fixes:
  - Avoid using GNU extensions in the dependency splitter regex
  - filterAdvisory: match installed_solvables sort with lower_bound (RhBug:2212838)

Bugs fixed in 0.71.0:

* :rhbug:`2212838`

====================
0.70.2 Release Notes
====================

- Fix #1558: Don't assume inclusion of cstdint
- Disconnect monitors in `dnf_repo_loader_finalize()` (RhBug:2070153)

- New features:
  - Support "proxy=_none_" in main config (RhBug:2155713)

Bugs fixed in 0.70.2:

* :rhbug:`2070153`
* :rhbug:`2155713`

====================
0.70.1 Release Notes
====================

- New features:
  - Add repoid to solver errors for RPMs (RhBug:2179413)

- Others:
  - Avoid using obsolete RPM API and drop redundant calls
  - Remove DNF from list of protected packages

Bugs fixed in 0.70.1:

* :rhbug:`2179413`

====================
0.70.0 Release Notes
====================

- Security fixes:
  - Allow change of architecture for packages during security updates with noarch involved (RhBug:2124483)

- Bug fixes:
  - "dnf_keyring_add_public_keys": reset localError to NULL after free (RhBug:2121222)
  - context: Get RPM db path from RPM
  - Fix memory leak of SolvUserdata

Bugs fixed in 0.70.0:

* :rhbug:`2124483`
* :rhbug:`2121222`

====================
0.69.0 Release Notes
====================

- New features:
  - Expose librepo max_downloads_per_mirror configuration

====================
0.68.0 Release Notes
====================

- New features:
  - context: Support <package-spec> (NEVRA forms, provides, file provides) including globs in the dnf_context_remove func (RhBug:2084602)

- Bug fixes:
  - dnf-context: Disconnect signal handler before dropping file monitor ref
  - Filter out advisory pkgs with different arch during advisory upgrade, fixes possible problems in dependency resulution (RhBug:2088149)
  - Gracefully handle failure to open repo primary file
  - Fix listing a repository without cpeid (RhBug:2066334)

Bugs fixed in 0.68.0:

* :rhbug:`2084602`
* :rhbug:`2088149`
* :rhbug:`2066334`

====================
0.67.0 Release Notes
====================

- New features:
  - Add 'loongarch' support

- Bug fixes:
  - Use dnf solv userdata to check versions and checksum (RhBug:2027445)
  - context: Substitute all repository config options (RhBug:2076853)

Bugs fixed in 0.67.0:

* :rhbug:`2027445`
* :rhbug:`2076853`

====================
0.66.0 Release Notes
====================

- Bug fixes:
  - Use `rpmdbCookie` from librpm, remove `hawkey.Sack._rpmdb_version`
  - Fix handling transaction id in resolveTransactionItemReason (RhBug:2010259,2053014)
  - Remove deprecated assertions (RhBug:2027383)
  - Skip rich deps for autodetection of unmet dependencies (RhBug:2033130, 2048394)
  - Increase required rpm version since we use `rpmdbCookie()`

Bugs fixed in 0.66.0:

* :rhbug:`2033130`
* :rhbug:`2027383`
* :rhbug:`2010259`
* :rhbug:`2053014`

====================
0.65.0 Release Notes
====================

- New features:
  - Add support for excluding packages to be installed as weak dependencies
  - Add support for autodetecting packages to be excluded from being installed as weak dependencies

- Bug fixes:
  - Turn off strict validation of modulemd documents (RhBug:2004853,2007166,2007167)

Bugs fixed in 0.65.0:

* :rhbug:`2004853`
* :rhbug:`2007166`
* :rhbug:`2007167`

====================
0.64.0 Release Notes
====================

- Implement logic for demodularization of modular rpms (RhBug:1805260)
- DnfContext: fix handling of default module profiles
- ModuleMetadata: gracefully handle modules with no defaults
- Remove failovermethod config option (RhBug:1961083)

Bugs fixed in 0.64.0:

* :rhbug:`1961083`
* :rhbug:`1805260`

====================
0.63.1 Release Notes
====================

- Add hy_query_get_advisory_pkgs to C API (RhBug:1960561)
- Add dnf_advisorypkg_get_advisory()

- Bug fixes:
  - DNF does not fail on non UTF-8 file names in a package (RhBug:1893176)
  - Improve error-reporting for modular functions

Bugs fixed in 0.63.1:

* :rhbug:`1893176`
* :rhbug:`1960561`

====================
0.63.0 Release Notes
====================

- ModuleProfile: add isDefault()
- ModulePackage: add getDefaultProfile()

- New features:
  - Add new dnf_context_module_install() C API

- Bug fixes:
  - Fix a crash when [media] section in .treeinfo is missing for bootable media (RhBug:1946024)

Bugs fixed in 0.63.0:

* :rhbug:`1946024`

====================
0.62.0 Release Notes
====================

- Bump version to fix tag in upstream release

Bugs fixed in 0.62.0:


====================
0.61.1 Release Notes
====================

- Fix: Fully set ssl in newHandle function
- [conf] Add options for working with certificates used with proxy
- lock: Switch return-if-fail to assert to quiet gcc -fanalyzer
- Modify module NSVCA parsing - context definition (RhBug:1926771)
- libdnf.h: Remove overall extern "C"
- [context] Fix: dnf_package_is_installonly (RhBug:1928056)
- Fix problematic language
- Add getApplicablePackages to advisory and isApplicable to advisorymodule
- Keep isAdvisoryApplicable to preserve API
- Run ModulePackageContainerTest tests in tmpdir, merge interdependent
- [context] Support config file option "proxy_auth_method", defaults "any"

- Security fixes:
  - Hardening: add signature check with rpmcliVerifySignatures (RhBug:1932079)

- New features:
  - do not allow 1 as installonly_limit value (RhBug:1926261)
  - Add a config option to check TLS certificate revocation status (using OCSP stapling), defaults to false (RhBug:1814383)

- Bug fixes:
  - Bugs fixed (RhBug:1916786)

Bugs fixed in 0.61.1:

* :rhbug:`1921063`
* :rhbug:`1814383`
* :rhbug:`1932079`
* :rhbug:`1926261`
* :rhbug:`1847035`

====================
0.60.0 Release Notes
====================

- Fix repo.fresh() implementation
- build-sys: Add ENABLE_STATIC option
- Fix: Fully set ssl in newHandle function
- [conf] Add options for working with certificates used with proxy
- Apply proxy certificate options
- lock: Switch return-if-fail to assert to quiet gcc -fanalyzer
- build-sys: Clean up message about Python bindings
- Modify module NSVCA parsing - context definition (RhBug:1926771)
- [context] Fix: dnf_package_is_installonly (RhBug:1928056)
- Fix problematic language
- Add getApplicablePackages to advisory and isApplicable to advisorymodule
- Keep isAdvisoryApplicable to preserve API
- Run ModulePackageContainerTest tests in tmpdir, merge interdependent
- [context] Support config file option "proxy_auth_method", defaults "any"

- Bug fixes:
  - Support main config file option "installonlypkgs".      Changes behaviour of microdnf and PackageKit.
  - Support main config file option "protected_packages".      Changes behaviour of microdnf and PackageKit.
  - Properly handle multiple collections in updateinfo.xml (RhBug:1804234)

Bugs fixed in 0.60.0:

* :rhbug:`1928056`
* :rhbug:`1804234`
* :rhbug:`1926771`

====================
0.58.0 Release Notes
====================

- Option: Add reset() method
- Add OptionBinds::getOption() method
- [context] Add dnf_repo_conf_from_gkeyfile() and dnf_repo_conf_reset()
- [context] Add support for options: minrate, throttle, bandwidth, timeout
- [context] Remove g_key_file_get_string() from dnf_repo_set_keyfile_data()
- Allow loading ext metadata even if only cache (solv) is present
- Add ASAN_OPTIONS for test_libdnf_main
- [context,API] Functions for accessing main/global configuration options
- [context,API] Function for adding setopt
- Add getter for modular obsoletes from ModuleMetadata
- Add ModulePackage.getStaticContext() and getRequires()
- Add compatible layer for MdDocuments v2
- Fix modular queries with the new solver
- Improve formatting of error string for modules
- Change mechanism of module conflicts
- Fix load/update FailSafe

- New features:
  - Extend repo loadCache method with ignoreMissing parameter to allow loading incomplete xml cache (repomd.xml is required). (RhBug:1865803)
  - Add a new option module_obsoletes
  - Add new API applyObsoletes() function to apply modular obsoletes
  - Extend filter_modules with an optional parameter to enable applying modular obsoletes

Bugs fixed in 0.58.0:

* :rhbug:`1918818`
* :rhbug:`1865803`

====================
0.55.2 Release Notes
====================

- Add a dnf sanitizers wrapper to /etc/profile.d
- Improve performance of query installed() and available()
- Swdb: Add a method to get the current transaction
- [modules] Add special handling for src artifacts (RhBug:1809314)
- Better msgs if "basecachedir" or "proxy_password" isn't set (RhBug:1888946)

- New features:
  - Add new options module_stream_switch
  - Support allow_vendor_change setting in dnf context API

Bugs fixed in 0.55.2:

* :rhbug:`1888946`
* :rhbug:`1809314`

====================
0.55.0 Release Notes
====================

- Add vendor to dnf API (RhBug:1876561)
- Add formatting function for solver error
- Add error types in ModulePackageContainer
- Implement module enable for context part
- Improve string formatting for translation
- Remove redundant printf and change logging info to notice (RhBug:1827424)
- Add allow_vendor_change option (RhBug:1788371) (RhBug:1788371)


====================
0.54.2 Release Notes
====================

- history: Fix dnf history rollback when a package was removed (RhBug:1683134)
- Add support for HY_GT, HY_LT in query nevra_strict
- Fix parsing empty lines in config files
- Accept '==' as an operator in reldeps (RhBug:1847946)
- Add log file level main config option (RhBug:1802074)
- Add protect_running_kernel configuration option (RhBug:1698145)
- Context part of libdnf cannot assume zchunk is on (RhBug:1851841,1779104)
- Fix memory leak of resultingModuleIndex and handle g_object refs
- Redirect librepo logs to libdnf logs with different source
- Introduce changelog metadata in commit messages
- Add hy_goal_lock
- Update Copr targets for packit and use alias
- Enum/String conversions for Transaction Store/Replay
- utils: Add a method to decode URLs
- Unify hawkey.log line format with the rest of the logs

====================
0.48.0 Release Notes
====================

- swdb: Catch only SQLite3 exceptions and simplify the messages
- MergedTransaction list multiple comments (RhBug:1773679)
- Modify CMake to pull *.po files from weblate
- Optimize DependencyContainer creation from an existing queue
- fix a memory leak in dnf_package_get_requires()
- Fix memory leaks on g_build_filename()
- Fix memory leak in dnf_context_setup()
- Add `hy_goal_favor` and `hy_goal_disfavor`
- Define a cleanup function for `DnfPackageSet`
- dnf-repo: fix dnf_repo_get_public_keys double-free
- Do not cache RPMDB
- Use single-quotes around string literals used in SQL statements
- SQLite3: Do not close the database if it wasn't opened (RhBug:1761976)
- Don't create a new history DB connection for in-memory DB
- transaction/Swdb: Use a single logger variable in constructor
- utils: Add a safe version of pathExists()
- swdb: Handle the case when pathExists() fails on e.g. permission
- Repo: prepend "file://" if a local path is used as baseurl
- Move urlEncode() to utils
- utils: Add 'exclude' argument to urlEncode()
- Encode package URL for downloading through librepo (RhBug:1817130)
- Replace std::runtime_error with libdnf::RepoError
- Fixes and error handling improvements of the File class
- [context] Use ConfigRepo for gpgkey and baseurl (RhBug:1807864)
- [context] support "priority" option in .repo config file (RhBug:1797265)

====================
0.47.0 Release Notes
====================

- Fix release version

====================
0.46.2 Release Notes
====================

- Add prereq_ignoreinst & regular_requires properties for pkg (RhBug:1543449)
- Reset active modules when no module enabled or default (RhBug:1767351)
- Add comment option to transaction (RhBug:1773679)
- Failing to get module defauls is a recoverable error
- Baseurl is not exclusive with mirrorlist/metalink (RhBug: 1775184)
- Add new function to reset all modules in C API (dnf_context_reset_all_modules)
- [context] Fix to preserve additionalMetadata content (RhBug:1808677)
- Fix filtering of DepSolvables with source rpms (RhBug:1812596)
- Add setter for running kernel protection setting
- Handle situation when an unprivileged user cannot create history database (RhBug:1634385)
- Add query filter: latest by priority
- Add DNF_NO_PROTECTED flag to allow empty list of protected packages
- Remove 'dim' option from terminal colors to make them more readable (RhBug:1807774,1814563)
- [context] Error when main config file can't be opened (RhBug:1794864)
- [context] Add function function dnf_context_is_set_config_file_path

====================
0.45.0 Release Notes
====================

- Config options: only first empty value clears existing (RhBug:1788154)
- Make parsing of reldeps more strict (RhBug:1788107)
- [context] Support repositories defined in main configuration file
- Fix filtering packages by advisory when more versions and arches are available (RhBug:1770125)
- Add expanding solvable provides for dependency matching (RhBug:1534123)
- DnfRepo: fix module_hotfixes keyfile priority level
- Add custom exceptions to libdnf interface
- [conf] Set useful default colors when color is enabled
- Port to libmodulemd-2 API (RhBug:1693683)

====================
0.43.1 Release Notes
====================

- Allow excluding packages with "excludepkgs" and globs
- Add two new query filters: obsoletes_by_priority, upgrades_by_priority
- [context] Use installonly_limit from global config (RhBug:1256108)
- [context] Add API to get/set "install_weak_deps"
- [context] Add wildcard support for repo_id in dnf_context_repo_enable/disable (RhBug:1781420)
- [context] Adds support for includepkgs in repository configuration.
- [context] Adds support for excludepkgs, exclude, includepkgs, and disable_excludes in main configuration.
- [context] Added function dnf_transaction_set_dont_solve_goal
- [context] Added functions dnf_context_get/set_config_file_path
- [context] Respect "plugins" global conf value
- [context] Add API to disable/enable plugins

====================
0.39.1 Release Notes
====================

- Report reason how package was excluded (RhBug:1649754)
- Additional Arm detection improvements (RhBug:1691430)
- Set skip_if_unavailable for media repos to skip their update (RhBug:1716067)
- Add support of xml:base for remote and local url in context (RhBug:1734350, 1717865)

====================
0.38.1 Release Notes
====================

- Handle NoModuleException in dnf_context_reset_modules (RhBug:1767453)
- Add missing C function hy_nevra_free() for HyNevra deallocation
- Context part of libdnf now uses metadata_expire from global configuration 

====================
0.37.1 Release Notes
====================

- Change the best option default to false
- Use more descriptive message when failed to retrieve GPG key (RhBug:1605117)
- Add removeMetadataTypeFromDownload function to the API
- Context part of libdnf can now read vars (urlvars) from dirs and environment
- Throw exception immediately if file cannot be opened
- Add test when there is no primary metadata in compatible format (RhBug:1744960)
- Various improvements to countme features
- Don't abort on rpmdb checksum calculation failure
- Enable module dependency trees when using set_modules_enabled_by_pkgset() (RhBug:1762314)
- Resolve problem with --best and search in provides (RhBug:1737469)
- New method "Query::filterSubject()", replaces Solution::getBestSolution()
- The Solution class was removed
- Add query argument into get_best_query and get_best_solution
- Add module reset function into dnf_context
- Add method to get all repository metadata locations
- Catch NoModuleException in case of not existent value was used in persistor (RhBug:1761773)

====================
0.35.5 Release Notes
====================

- Fix crash in PackageKit (RhBug:1636803)
- Do not create @System.solv files (RhBug:1707995)
- Set LRO_CACHEDIR so zchunk works again (RhBug:1739867)
- Don't reinstall modified packages with the same NEVRA (RhBug:1644241)
- Fix bug when moving temporary repository metadata after download (RhBug:1700341)
- Improve detection of extras packages by comparing (name, arch) pair instead of full NEVRA (RhBuh:1684517)
- Improve handling multilib packages in the history command (RhBug:1728637)
- Repo download: use full error description into the exception text (RhBug:1741442)
- Properly close hawkey.log (RhBug:1594016)
- Fix dnf updateinfo --update to not list advisories for packages updatable only from non-enabled modules
- Apply modular filtering by package name (RhBug:1702729)

====================
0.35.3 Release Notes
====================

- Detect armv7 with crypto extension only on arm version >= 8
- A new standardized User-Agent field consisting of the libdnf and OS version
  (including the variant) (RhBug:1156007)
- Add basic countme support (RhBug:1647454)

====================
0.35.2 Release Notes
====================

- Make libdnf own its plugin directory (RhBug:1714265)
- Don't disable nonexistent but required repositories (RhBug:1689331)
- Set priority of dnf.conf.d drop-ins
- Fix toString() to not insert [] (RhBug:1584442)
- Ignore trailing blank lines in config (RhBug:1722493)
- Fix handling large number of filenames on input (RhBug:1690915)
- Fix attaching/detaching of libsolvRepo (RhBug:1727343,1727424)

====================
0.35.1 Release Notes
====================
- Skip invalid key files in "/etc/pki/rpm-gpg" with warning (RhBug:1644040)
- Enable timestamp preserving for downloaded data (RhBug:1688537)
- Set default to skip_if_unavailable=false (RhBug:1679509)
- Add configuration option skip_if_unavailable (RhBug:1689931)
- Fix 'database is locked' error (RhBug:1631533)
- Replace the 'Failed to synchronize cache' message (RhBug:1712055)
- Fix 'no such table: main.trans_cmdline' error (RhBug:1596540)
- Add support of modular FailSafe (RhBug:1623128)
- Add support of DNF main config file in context; used by PackageKit and microdnf (RhBug:1689331)
- Exit gpg-agent after repokey import (RhBug:1650266)

====================
0.33.0 Release Notes
====================
- Enhance logging handling
- Do not log DEBUG messages by default
- Also add subkeys when adding GPG keys
- Reintroduce hawkey.Repo (deprecated, for compatibility)
- [module] Fix swig binding for getModuleDependencies()

====================
0.31.0 Release Notes
====================
- Installroot now requires absolute path
- Support "_none_" value for repo option "proxy" (RhBug:1680272)
- Add support for Module advisories
- Add support for xml:base attribute from primary.xml (RhBug:1691315)
- Improve detection of Platform ID (RhBug:1688462)

====================
0.28.1 Release Notes
====================
- Return empty query if incorrect reldep (RhBug:1687135)
- ConfigParser: Improve compatibility with Python ConfigParser and dnf-plugin-spacewalk (RhBug:1692044)
- ConfigParser: Unify default set of string represenation of boolean values
- Fix segfault when interrupting dnf process (RhBug:1610456)

====================
0.28.0 Release Notes
====================
- Exclude module pkgs that have conflict (RhBug:1670496)
- Fix zchunk configuration flags
- Enhance config parser to preserve order of data, and keep comments and format
- [history] Allow using :memory: db to avoid disk writes
- Improve ARM detection
- Add support for SHA-384

====================
0.26.0 Release Notes
====================
- Enhance modular solver to handle enabled and default module streams differently (RhBug:1648839)
- Add support of wild cards for modules (RhBug:1644588)
- Add best as default behavior (RhBug:1671683,1670776)

====================
0.24.1 Release Notes
====================
- Add support for zchunk
- Enhance LIBDNF plugins support
- Enhance sorting for module list (RhBug:1590358)
- [repo] Check whether metadata cache is expired (RhBug:1539620,1648274)
- [DnfRepo] Add methods for alternative repository metadata type and download (RhBug:1656314)
- Remove installed profile on module  enable or disable (RhBug:1653623)
- [sack] Implement dnf_sack_get_rpmdb_version()

====================
0.22.3 Release Notes
====================
- Modify solver_describe_decision to report cleaned (RhBug:1486749)
- [swdb] create persistent WAL files (RhBug:1640235)
- Relocate ModuleContainer save hook (RhBug:1632518)
- [transaction] Fix transaction item lookup for obsoleted packages (RhBug: 1642796)
- Fix memory leaks and memory allocations
- [repo] Possibility to extend downloaded repository metadata

====================
0.22.0 Release Notes
====================
- Fix segfault in repo_internalize_trigger (RhBug:1375895)
- Change sorting of installonly packages (RhBug:1627685)
- [swdb] Fixed pattern searching in history db (RhBug:1635542)
- Check correctly gpg for repomd when refresh is used (RhBug:1636743)
- [conf] Provide additional VectorString methods for compatibility with Python list.
- [plugins] add plugin loading and hooks into libdnf

====================
0.20.0 Release Notes
====================
- [module] Report module solver errors
- [module] Enhance module commands and errors
- [transaction] Fixed several problems with SWDB
- Remove unneeded regex URL tests (RhBug:1598336)
- Allow quoted values in ini files (RhBug:1624056)
- Filter out not unique set of solver problems (RhBug:1564369)
- Disable python2 build for Fedora 30+

====================
0.19.1 Release Notes
====================
- Fix compilation errors on gcc-4.8.5
- [module] Allow module queries on disabled modules

====================
0.19.0 Release Notes
====================
- [query] Reldeps can contain a space char (RhBug:1612462)
- [transaction] Avoid adding duplicates via Transaction::addItem()
- Fix compilation errors on gcc-4.8.5
- [module] Make available ModuleProfile using SWIG
- [module] Redesign module disable and reset

====================
0.18.0 Release Notes
====================
- [repo] Implement GPG key import
- [repo] Introduce Repo class replacing dnf.repo.Repo
- [context] Fix memory corruption in dnf_context
- [rhsm] Fix: RHSM don't write .repo file with same content (RhBug:1600452)
- [module] Create /etc/dnf/modules.d if it doesn't exist.
- [module] Forward C++ exceptions to bindings.

====================
0.17.2 Release Notes
====================
- [sqlite3] Change db locking mode to DEFAULT.
- [doc] Add libsmartcols-devel to devel deps.

====================
0.17.1 Release Notes
====================
- [module] Solve a problem in python constructor of NSVCAP if no version.
- [translations] Update translations from zanata.
- [transaction] Fix crash after using dnf.comps.CompsQuery and forking the process in Anaconda.
- [module] Support for resetting module state.
- [output] Introduce wrapper for smartcols.

====================
0.17.0 Release Notes
====================
- [conf] Add module_platform_id option.
- [module] Add ModulePackageContainer class.
- [module] Add ModulePersistor class.
- [sack] Module filtering made available in python API
- [sack] Module auto-enabling according to installed packages

====================
0.16.1 Release Notes
====================
* Implement 'module_hotfixes' conf option to skip filtering RPMs from hotfix repos.
* Fix distupgrade filter, allow downgrades.
* Module dependency resolution
* Platform pseudo-module based on /etc/os-release
* Add Goal::listSuggested()

====================
0.16.0 Release Notes
====================
* Fix RHSM plugin
* Add support for logging

====================
0.15.2 Release Notes
====================

Bugs fixed in 0.15.2:

* :rhbug:`1595487`

====================
0.15.0 Release Notes
====================

* Filtering rpms by module metadata
* New SWIG bindings
* New history database
* New config classes
* Query performance improvements
* New query filter nevra_strict

Bugs fixed in 0.15.0:

* :rhbug:`1498207`
* :rhbug:`1500361`
* :rhbug:`1486749`
* :rhbug:`1525542`
* :rhbug:`1550030`
* :rhbug:`1576749`
* :rhbug:`1537981`
* :rhbug:`1588443`
* :rhbug:`1565647`

====================
0.11.1 Release Notes
====================

* Improvement query performance
* Run file query in hy_subject_get_best_solution only for files (arguments that start with ``/`` or ``*/``)

Bugs fixed in 0.11.1:

* :rhbug:`1498207`

====================
0.10.1 Release Notes
====================

It improves query performance with ``name`` and ``arch`` filters. Also ``nevra`` filter will now
handle string with or without ``epoch``.
Additionally for python bindings it renames ``NEVRA._has_just_name()`` to ``NEVRA.has_just_name()``
due to movement of code into c part of library.

Bugs fixed in 0.10.1:

* :rhbug:`1260242`
* :rhbug:`1485881`
* :rhbug:`1361187`

===================
0.9.3 Release Notes
===================

It moves query glob optimization from python code to C part.

Bugs fixed in 0.9.3:

* :rhbug:`1381506`
* :rhbug:`1464249`

===================
0.1.7 Release Notes
===================
Released: 2014-12-19

Notes:
 - librepo >= 1.7.11 is now required

New Features:
 - Add HIF_SOURCE_UPDATE_FLAG_SIMULATE (Richard Hughes)
 - Add a large number of GPG tests (Richard Hughes)
 - Add hif_source_get_filename_md() (Richard Hughes)
 - Add the concept of metadata-only software sources (Richard Hughes)
 - Support appstream and appstream-icons metadata types (Richard Hughes)

Bugfixes:
 - Automatically import public keys into the librepo keyring (Richard Hughes)
 - Call hif_state_set_allow_cancel() when the state is uncancellable (Richard Hughes)
 - Correctly update sources with baseurls ending with a slash (Richard Hughes)
 - Don't unref the HifSource when invalidating as this is not threadsafe (Richard Hughes)
 - Fix crash when parsing the bumblebee.repo file (Richard Hughes)
 - Improve handling of local metadata (Richard Hughes)
 - Only set LRO_GPGCHECK when repo_gpgcheck=1 (Richard Hughes)

===================
0.1.6 Release Notes
===================
Released: 2014-11-10

New Features:
 - Add support for package reinstallation and downgrade (Michal Minar)
 - Copy the vendor cache if present (Richard Hughes)

Bugfixes:
 - Allow to get repo loader out of context (Michal Minar)
 - Ensure created directories are world-readable (Richard Hughes)
 - Support local repositories (Michal Minar)

===================
0.1.5 Release Notes
===================
Released: 2014-09-22

Bugfixes:
 - Add all native architectures for ARM and i386 (Richard Hughes)
 - Check for libQtGui rather than libkde* to detect GUI apps (Kevin Kofler)

===================
0.1.4 Release Notes
===================
Released: 2014-09-12

New Features:
 - Add hif_source_commit() so we don't rewrite the file for each change (Richard Hughes)
 - Allow setting the default lock directory (Richard Hughes)

Bugfixes:
 - Ensure all the required directories exist when setting up the context (Richard Hughes)
 - Use a real path for hy_sack_create() (Richard Hughes)

===================
0.1.3 Release Notes
===================
Released: 2014-09-01

Bugfixes:
 - Add an error path for when the sources are not valid (Richard Hughes)
 - Do not call hif_context_setup_sack() automatically (Richard Hughes)
 - Don't error out for missing treeinfo files (Kalev Lember)
 - Fix a logic error to fix refreshing with HIF_SOURCE_UPDATE_FLAG_FORCE (Richard Hughes)

===================
0.1.2 Release Notes
===================
Released: 2014-07-17

Notes:

New Features:
 - Add HifContext accessor in -private for HifState (Colin Walters)
 - Improve rpm callback handling for packages in the cleanup state (Kalev Lember)

Bugfixes:
 - Add name of failing repository (Colin Walters)
 - Create an initial sack in HifContext (Colin Walters)
 - Error if we can't find any package matching provided name (Colin Walters)
 - Fix a mixup of HifStateAction and HifPackageInfo (Kalev Lember)
 - Only set librepo option if value is set (Colin Walters)
 - Respect install root for rpmdb Packages monitor (Colin Walters)
 - Update Makefile.am (Elan Ruusamäe)

===================
0.1.1 Release Notes
===================
Released: 2014-06-23

New Features:
 - Only add system repository if it exists (Colin Walters)

Bugfixes:
 - Add private accessors for goal/sack (Colin Walters)
 - Fix a potential crash when removing software (Richard Hughes)
 - Pass install root to hawkey (Colin Walters)

===================
0.1.0 Release Notes
===================
Released: 2014-06-10

Notes:
 - This is the first release of a simple library that uses librepo and hawkey
   to do some high level package management tasks.
 - libhif is not 100% API or ABI stable yet.

New Features:
 - Add HifContext as a high level operation (Richard Hughes)

Bugfixes:
 - Add several g-i annotations (Colin Walters)
 - Correctly set the cleanup status (Kalev Lember)
 - Fix a crash when using hif_source_set_keyfile_data() (Richard Hughes)
 - Use GLib version macros to pin to 2.36 by default (Colin Walters)
