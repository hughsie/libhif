/* dnf-swdb-test.c
 *
 * Copyright (C) 2016 Red Hat, Inc.
 * Author: Eduard Cuba <ecuba@redhat.com>
 *
 * Licensed under the GNU Lesser General Public License Version 2.1
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <glib-object.h>
#include <glib.h>
#include "libdnf/libdnf.h"
#include "libdnf/dnf-swdb.h"


#define INIT_PACAKGES 10
#define TRANS_OUT "initial transaction"
#define TEST_PATH "tmp_swdb.sqlite"

/**
 * Generate some random string of given length
 */
static gchar *
generate_str (guint len)
{
    g_autoptr (GRand) r = g_rand_new ();
    gchar *str = g_malloc (sizeof (gchar) * (len + 1));
    for (guint i = 0; i < len; ++i) {
        str[i] = g_rand_int_range (r, 97, 122);
    }
    str[len] = 0;
    return str;
}

/**
 * Get current time in unix timestamp converted to gchar
 */
static gint64
generate_timestamp (void)
{
    g_autoptr (GDateTime) t_struct = g_date_time_new_now_utc ();
    return g_date_time_to_unix (t_struct);
}

/**
 * Generate ordinary package with random attributes
 */
static DnfSwdbPkg *
generate_package (void)
{
    g_autoptr (GRand) r = g_rand_new ();

    g_autofree gchar *name = generate_str (8);
    gint epoch = g_rand_boolean (r) ? 0 : 1;
    g_autofree gchar *version = generate_str (4);
    g_autofree gchar *release = generate_str (6);
    g_autofree gchar *arch = generate_str (4);
    g_autofree gchar *checksum_data = generate_str (32);
    const gchar *checksum_type = g_rand_boolean (r) ? "sha256" : "sha1";

    DnfSwdbPkg *pkg = dnf_swdb_pkg_new (
        name,
        epoch,
        version,
        release,
        arch,
        checksum_data,
        checksum_type,
        DNF_SWDB_ITEM_RPM);

    g_assert (pkg);
    return pkg;
}

/**
 * Generate some package data with random attributes
 */
static DnfSwdbItemData *
generate_package_data (void)
{
    const gchar *from_repo = "testaconda";
    gint64 from_repo_timestamp = generate_timestamp ();

    g_autoptr (GRand) r = g_rand_new ();

    const gchar *installed_by = "1000";
    const gchar *changed_by = g_rand_boolean (r) ? NULL : "4242";

    //FIX THIS
    DnfSwdbItemData *pkg_data = dnf_swdb_pkgdata_new (from_repo_revision,
                                                     from_repo_timestamp,
                                                     installed_by,
                                                     changed_by,
                                                     from_repo);

    return pkg_data;
}

static gint
begin_trans (DnfSwdb *self)
{
    gint64 time_str = generate_timestamp ();

    g_autofree gchar *rpmdb_version = generate_str (32);
    g_autofree gchar *cmdline = generate_str (16);

    // open initial transaction
    guint tid = dnf_swdb_trans_beg (self, time_str, rpmdb_version, cmdline, "1000", "42");

    return tid;
}

static gint
end_trans (DnfSwdb *self, gint tid)
{
    gint rc;
    gint64 time_str = generate_timestamp ();
    g_autofree gchar *rpmdb_version = generate_str (32);

    // close transaction as success
    rc = dnf_swdb_trans_end (self, tid, time_str, rpmdb_version, 0);

    return rc;
}

static void
check_package_persistor (DnfSwdb *self, DnfSwdbPkg *pkg)
{
    const gchar *new_repo = "testdora";

    g_assert_false (dnf_swdb_set_repo (self, pkg->nevra, new_repo));

    g_autofree gchar *repo = dnf_swdb_repo (self, pkg->nevra);

    g_assert_false (g_strcmp0 (new_repo, repo));

    gint iid = dnf_swdb_iid_by_nevra (self, pkg->nevra);
    g_assert (iid && iid == pkg->iid);

    // test another method how to obtain same package
    g_autoptr (DnfSwdbPkg) same_pkg = dnf_swdb_package (self, pkg->nevra);
    g_assert (same_pkg);
    g_assert (same_pkg->iid == iid);
    g_assert_false (g_strcmp0 (pkg->checksum_data, same_pkg->checksum_data));

    // obtained package should contain new repo settings
    g_autoptr (DnfSwdbPkgData) pkgdata = dnf_swdb_item_data (self, same_pkg->nevra);
    g_assert_false (g_strcmp0 (pkgdata->from_repo, new_repo));

    g_autofree gchar *ui_from_repo = dnf_swdb_pkg_ui_from_repo (same_pkg);
    g_autofree gchar *ui_repo = g_strdup_printf ("@%s", new_repo);
    g_assert_false (g_strcmp0 (ui_repo, ui_from_repo));

    // playing with reason
    g_assert (dnf_swdb_user_installed (self, pkg->nevra));

    g_assert_false (dnf_swdb_set_reason (self, pkg->nevra, DNF_SWDB_REASON_DEP));

    g_assert_false (dnf_swdb_user_installed (self, pkg->nevra));

    g_assert_false (dnf_swdb_set_reason (self, pkg->nevra, DNF_SWDB_REASON_USER));

    g_assert (dnf_swdb_user_installed (self, pkg->nevra));
}

static void
check_initial_transaction (DnfSwdb *self)
{
    g_autoptr (DnfSwdbTrans) trans = dnf_swdb_last (self, TRUE);
    g_assert (trans);
    g_assert (trans->tid == 1);
    g_assert (trans->return_code == 0);
    g_assert_false (trans->altered_lt_rpmdb);
    g_assert_false (trans->altered_gt_rpmdb);

    // get me all packages from that transaction and verify them
    GPtrArray *packages = dnf_swdb_trans_packages (trans);
    GPtrArray *trans_data = dnf_swdb_trans_data (trans);
    g_assert (trans_data && trans_data->len == INIT_PACAKGES);
    g_assert (packages && packages->len == INIT_PACAKGES);

    GPtrArray *nevras = g_ptr_array_new ();
    // get packages and check if they are user installed
    for (guint i = 0; i < packages->len; ++i) {
        DnfSwdbPkg *pkg = g_ptr_array_index (packages, i);
        g_ptr_array_add (nevras, pkg->nevra);
    }
    GArray *user_installed = dnf_swdb_select_user_installed (self, nevras);
    g_assert (user_installed);
    g_assert (user_installed->len == packages->len);
    g_assert (g_array_index (user_installed, gint, packages->len - 1) == ((int)packages->len - 1));

    g_array_free (user_installed, TRUE);
    g_ptr_array_free (nevras, TRUE);

    for (guint i = 0; i < packages->len; ++i) {
        // check object attributes
        g_autoptr (DnfSwdbPkg) pkg = g_ptr_array_index (packages, i);
        g_assert (pkg->name && *pkg->name);
        g_assert (pkg->version && *pkg->version);
        g_assert (pkg->release && *pkg->release);
        g_assert (pkg->arch && *pkg->arch);
        g_assert (pkg->checksum_data && *pkg->checksum_data);
        g_assert (pkg->checksum_type && *pkg->checksum_type);
        g_assert (pkg->type == DNF_SWDB_ITEM_RPM);
        g_assert (pkg->done);
        g_assert (pkg->state && *pkg->state);
        g_assert (pkg->iid);
        g_assert (pkg->nevra && *pkg->nevra);
        g_autofree gchar *ui_from_repo = dnf_swdb_pkg_ui_from_repo (pkg);
        g_assert (ui_from_repo);

        // check package trans and package data
        g_autoptr (DnfSwdbItemData) transdata = g_ptr_array_index (trans_data, i);
        g_autoptr (DnfSwdbPkgData) pkgdata = dnf_swdb_item_data (self, pkg->nevra);

        g_assert (pkgdata);
        g_assert (pkgdata->from_repo && *pkgdata->from_repo);
        g_assert (pkgdata->from_repo_revision && *pkgdata->from_repo_revision);
        g_assert (pkgdata->from_repo_timestamp);
        g_assert (pkgdata->installed_by && *pkgdata->installed_by);

        g_autofree gchar *ui_repo = g_strdup_printf("@%s", pkgdata->from_repo);
        g_assert_false (g_strcmp0 (ui_repo, ui_from_repo));

        g_assert (transdata->done);

        g_assert (transdata->reason == DNF_SWDB_REASON_USER);

        g_assert (transdata->state);
        g_assert_false (g_strcmp0 (transdata->state, "Install"));

        // check bindings
        g_assert (pkgdata->pdid);
        g_assert (transdata->pdid);
        g_assert (pkgdata->pdid == transdata->pdid);

        g_assert (pkgdata->iid);
        g_assert (pkgdata->iid == pkg->iid);

        g_assert (transdata->tid);
        g_assert (transdata->tid == trans->tid);

        // check methods operating with package
        check_package_persistor (self, pkg);
    }
    g_ptr_array_free (packages, TRUE);
    g_ptr_array_free (trans_data, TRUE);

    // verify trans output
    GPtrArray *output = dnf_swdb_load_output (self, trans->tid);
    g_assert_false (g_strcmp0 (g_ptr_array_index (output, 0), TRANS_OUT));

    g_free (g_ptr_array_index (output, 0));
    g_ptr_array_free (output, TRUE);
}

static void
run_initial_transaction (DnfSwdb *self)
{
    guint tid = begin_trans (self);
    g_assert (tid); // tid not 0

    // now add some packages
    for (guint i = 0; i < INIT_PACAKGES; ++i) {
        g_autoptr (DnfSwdbPkg) pkg = generate_package ();

        // add pkg to database
        g_assert (dnf_swdb_add_package (self, pkg));

        g_autoptr (DnfSwdbPkgData) pkg_data = generate_package_data ();

        // dont forget trans data
        g_assert_false (dnf_swdb_trans_data_beg (self,
                                                 tid,
                                                 pkg->iid,
                                                 DNF_SWDB_REASON_USER, // reason
                                                 "Install", // state
                                                 0)); // obsoleting

        // add package data
        g_assert_false (dnf_swdb_update_item_data (self, pkg->iid, tid, pkg_data));

        // package installed successfully
        g_assert_false (dnf_swdb_trans_data_iid_end (self, pkg->iid, tid, "Install"));
    }

    // check for complete/incomplete transaction
    g_autoptr (DnfSwdbTrans) last = dnf_swdb_last (self, FALSE);
    g_assert (last);
    g_assert_false (dnf_swdb_last (self, TRUE));

    // add some output
    g_assert_false (dnf_swdb_log_output (self, tid, TRANS_OUT));

    // close transaction
    g_assert_false (end_trans (self, tid));
}

static void
dnf_swdb_setup_func (void)
{
    g_autoptr (DnfSwdb) self = dnf_swdb_new (TEST_PATH, "42");

    // test path
    g_assert_false (g_strcmp0 (dnf_swdb_get_path (self), TEST_PATH));

    // reset database
    dnf_swdb_reset_db (self);

    // test db existence
    g_assert (dnf_swdb_exist (self));
}

static void
dnf_swdb_initial_func (void)
{
    // create DB object
    g_autoptr (DnfSwdb) self = dnf_swdb_new (TEST_PATH, "42");

    // run initial transaction
    run_initial_transaction (self);

    // verify initial transaction
    check_initial_transaction (self);
}

static void
dnf_swdb_search_func (void)
{
    g_autoptr (DnfSwdb) self = dnf_swdb_new (TEST_PATH, "42");

    // open database
    g_assert_false (dnf_swdb_open (self));

    // get a package
    g_autoptr (DnfSwdbPkg) pkg = _get_package_by_iid (self->db, INIT_PACAKGES / 2);

    // find transaction of this package by name
    GPtrArray *patts = g_ptr_array_new ();
    g_ptr_array_add (patts, (gpointer)pkg->name);

    GArray *tids = dnf_swdb_search (self, patts);

    g_assert (tids);
    g_assert (tids->len == 1);
    g_assert (g_array_index (tids, gint, 0) == 1); // initial trans

    g_array_free (tids, TRUE);
    g_ptr_array_free (patts, TRUE);
}

static void
dnf_swdb_get_func (void)
{
    g_autoptr (DnfSwdb) self = dnf_swdb_new (TEST_PATH, "42");

    // open database
    g_assert_false (dnf_swdb_open (self));

    // get a package
    g_autoptr (DnfSwdbPkg) rpkg = _get_package_by_iid (self->db, INIT_PACAKGES / 2);

    // find this package by nevra
    g_autoptr (DnfSwdbPkg) pkg = dnf_swdb_package (self, rpkg->nevra);
    g_assert_false (g_strcmp0 (pkg->nevra, rpkg->nevra));
}

static void
dnf_swdb_update_func (void)
{
    g_autoptr (DnfSwdb) self = dnf_swdb_new (TEST_PATH, "42");

    // open database
    g_assert_false (dnf_swdb_open (self));

    // get a package
    g_autoptr (DnfSwdbPkg) pkg = _get_package_by_iid (self->db, INIT_PACAKGES / 2);

    // change package version and checksum
    g_free ((gchar *)pkg->version);
    pkg->version = generate_str (4);
    g_free ((gchar *)pkg->checksum_data);
    pkg->checksum_data = generate_str (32);

    // update
    gint tid = begin_trans (self);

    g_assert (tid); // tid not 0

    // add package
    g_assert (dnf_swdb_add_package (self, pkg));

    // initialize transaction data
    g_assert_false (dnf_swdb_trans_data_beg (self,
                                             tid,
                                             pkg->iid,
                                             DNF_SWDB_REASON_USER, // reason
                                             "Update", // state
                                             0)); // obsoleting

     // add package data
     g_autoptr (DnfSwdbPkgData) pkg_data = generate_package_data ();
     g_assert_false (dnf_swdb_update_item_data (self, pkg->iid, tid, pkg_data));

    // finalize transaction data
    g_assert_false (dnf_swdb_trans_data_iid_end (self, pkg->iid, tid, "Update"));

    g_assert_false (end_trans (self, tid));

    // check last transaction
    g_autoptr (DnfSwdbTrans) last_trans = dnf_swdb_last (self, TRUE);
    g_assert (last_trans);

    // check last transaction data
    GPtrArray *trans_data_array = dnf_swdb_trans_data (last_trans);
    g_assert (trans_data_array || trans_data_array->len);

    // check trans data about package
    g_autoptr (DnfSwdbItemData) trans_data = g_ptr_array_index (trans_data_array, 0);
    g_assert (trans_data);
    g_assert (trans_data->idid);

    g_ptr_array_free (trans_data_array, TRUE);
}

static void
dnf_swdb_reinstall_func (void)
{
    g_autoptr (DnfSwdb) self = dnf_swdb_new (TEST_PATH, "42");

    // open database
    g_assert_false (dnf_swdb_open (self));

    // get a package
    g_autoptr (DnfSwdbPkg) pkg = _get_package_by_iid (self->db, INIT_PACAKGES / 2);

    // initialize transaction
    gint tid = begin_trans (self);
    g_assert (tid); // tid not 0

    // initialize transaction data
    g_assert_false (dnf_swdb_trans_data_beg (self,
                                             tid,
                                             pkg->iid,
                                             DNF_SWDB_REASON_USER, // reason
                                             "Reinstall", // state
                                             0)); // obsoleting

    // insert package data
    g_autoptr (DnfSwdbPkgData) pkg_data = generate_package_data ();
    g_assert_false (dnf_swdb_update_item_data (self, pkg->iid, tid, pkg_data));

    // finalize transaction data
    g_assert_false (dnf_swdb_trans_data_iid_end (self, pkg->iid, tid, "Reinstall"));

    // finalize transaction
    g_assert_false (end_trans (self, tid));
    g_autoptr (DnfSwdbTrans) last_trans = dnf_swdb_last (self, TRUE);
    g_assert (last_trans);

    // get trans data of the transaction
    GPtrArray *trans_data_array = dnf_swdb_trans_data (last_trans);
    g_assert (trans_data_array || trans_data_array->len);
    g_autoptr (DnfSwdbItemData) trans_data = g_ptr_array_index (trans_data_array, 0);

    g_assert (trans_data);
    g_assert (trans_data->idid);

    g_ptr_array_free (trans_data_array, TRUE);
}

static void
dnf_swdb_erase_func (void)
{
    g_autoptr (DnfSwdb) self = dnf_swdb_new (TEST_PATH, "42");

    // open database
    g_assert_false (dnf_swdb_open (self));

    // get a package
    g_autoptr (DnfSwdbPkg) pkg = _get_package_by_iid (self->db, INIT_PACAKGES / 2);

    // initialize transaction
    gint tid = begin_trans (self);
    g_assert (tid); // tid not 0

    // initialize transaction data
    g_assert_false (dnf_swdb_trans_data_beg (self,
                                             tid,
                                             pkg->iid,
                                             DNF_SWDB_REASON_USER, // reason
                                             "Erase", // state
                                             0)); // obsoleting

    // insert package data
    g_autoptr (DnfSwdbPkgData) pkg_data = generate_package_data ();
    g_assert_false (dnf_swdb_update_item_data (self, pkg->iid, tid, pkg_data));

    // finalize transaction data
    g_assert_false (dnf_swdb_trans_data_iid_end (self, pkg->iid, tid, "Erase"));

    // finalize transaction
    g_assert_false (end_trans (self, tid));

    // get last transaction
    g_autoptr (DnfSwdbTrans) last_trans = dnf_swdb_last (self, TRUE);
    g_assert (last_trans);

    // get last transaction data
    GPtrArray *trans_data_array = dnf_swdb_trans_data (last_trans);
    g_assert (trans_data_array || trans_data_array->len);
    g_autoptr (DnfSwdbItemData) trans_data = g_ptr_array_index (trans_data_array, 0);

    g_assert (trans_data);
    g_assert (trans_data->idid);

    g_ptr_array_free (trans_data_array, TRUE);
}

int
main (int argc, char **argv)
{
    g_setenv ("G_MESSAGES_DEBUG", "all", FALSE);
    g_setenv ("GIO_USE_VFS", "local", TRUE);

    g_test_init (&argc, &argv, NULL);

    // swdb tests
    g_test_add_func ("/libdnf/swdb[setup]", dnf_swdb_setup_func);
    g_test_add_func ("/libdnf/swdb[initial]", dnf_swdb_initial_func);
    g_test_add_func ("/libdnf/swdb[get_package]", dnf_swdb_get_func);
    g_test_add_func ("/libdnf/swdb[search]", dnf_swdb_search_func);
    g_test_add_func ("/libdnf/swdb[update_package]", dnf_swdb_update_func);
    g_test_add_func ("/libdnf/swdb[reinstall_package]", dnf_swdb_reinstall_func);
    g_test_add_func ("/libdnf/swdb[erase_package]", dnf_swdb_erase_func);

    return g_test_run ();
}
