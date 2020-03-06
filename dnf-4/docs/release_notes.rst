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
