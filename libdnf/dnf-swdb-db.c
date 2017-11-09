/* dnf-swdb-db.c
 *
 * Copyright (C) 2017 Red Hat, Inc.
 * Author: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
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

#include "dnf-swdb-db.h"
#include "dnf-swdb-db-sql.h"
#include "dnf-swdb.h"
#include <glib/gstdio.h>

/**
 * _create_db:
 * @db: sqlite database handle
 *
 * Create empty database
 *
 * Returns: 0 if successful else count of tables failed to create
 **/
gint
_create_db (sqlite3 *db)
{
    return _db_exec (db, CREATE_TABLES, NULL);
}

/**
 * _db_step:
 * @res: sqlite statement
 *
 * Execute statement @res
 *
 * Returns: 1 if successful
 **/
void
_db_step (sqlite3_stmt *res)
{
    if (sqlite3_step (res) != SQLITE_DONE) {
        sqlite3 *db = sqlite3_db_handle (res);
        sqlite3_finalize (res);
        g_error ("SWDB statement execution: %s", sqlite3_errmsg (db));
    }
    sqlite3_finalize (res);
}

/**
 * _db_find_int:
 * @res: sqlite statement
 *
 * Returns: first integral result of statement @res or 0 if error occurred
 **/
gint
_db_find_int (sqlite3_stmt *res)
{
    if (sqlite3_step (res) == SQLITE_ROW) // id for description found
    {
        gint result = sqlite3_column_int (res, 0);
        sqlite3_finalize (res);
        return result;
    } else {
        sqlite3_finalize (res);
        return 0;
    }
}

/**
 * _db_find_str:
 * @res: sqlite statement
 *
 * Returns: first string result of statement @res or %NULL if error occurred
 **/
gchar *
_db_find_str (sqlite3_stmt *res)
{
    if (sqlite3_step (res) == SQLITE_ROW) // id for description found
    {
        gchar *result = g_strdup ((gchar *)sqlite3_column_text (res, 0));
        sqlite3_finalize (res);
        return result;
    } else {
        sqlite3_finalize (res);
        return NULL;
    }
}

/**
 * _db_find_str_multi:
 * @res: sqlite statement
 *
 * String result iterator
 *
 * Returns: next string result or %NULL if search hit the bottom
 **/
gchar *
_db_find_str_multi (sqlite3_stmt *res)
{
    if (sqlite3_step (res) == SQLITE_ROW) // id for description found
    {
        gchar *result = g_strdup ((gchar *)sqlite3_column_text (res, 0));
        return result;
    } else {
        sqlite3_finalize (res);
        return NULL;
    }
}

/**
 * _db_find_int_multi:
 * @res: sqlite statement
 *
 * Integral result iterator
 *
 * Returns: next integral or 0 if search hit the bottom
 **/
gint
_db_find_int_multi (sqlite3_stmt *res)
{
    if (sqlite3_step (res) == SQLITE_ROW) // id for description found
    {
        gint result = sqlite3_column_int (res, 0);
        return result;
    } else {
        sqlite3_finalize (res);
        return 0;
    }
}

/**
 * _db_prepare:
 * @db: sqlite database handle
 * @sql: sql command in string
 * @res: target sql statement
 **/
void
_db_prepare (sqlite3 *db, const gchar *sql, sqlite3_stmt **res)
{
    gint rc = sqlite3_prepare_v2 (db, sql, -1, res, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize (*res);
        g_error ("SWDB prepare error: '%s' - %s", sql, sqlite3_errmsg (db));
    }
}

/**
 * _db_bind_str:
 * @res: sqlite statement
 * @id: value placeholder
 * @source: new placeholder value
 *
 * Bind string to placeholder
 **/
void
_db_bind_str (sqlite3_stmt *res, const gchar *id, const gchar *source)
{
    gint idx = sqlite3_bind_parameter_index (res, id);
    gint rc = sqlite3_bind_text (res, idx, source, -1, SQLITE_STATIC);
    if (rc) {
        sqlite3 *db = sqlite3_db_handle (res);
        const gchar *err = sqlite3_errmsg (db);
        g_error ("SWDB string bind (args: %d|%s|%s): %s", idx, id, source, err);
    }
}

/**
 * _db_bind_int:
 * @res: sqlite statement
 * @id: value placeholder
 * @source: new placeholder value
 *
 * Bind integer to placeholder
 *
 * Returns: 1 if successful
 **/
void
_db_bind_int (sqlite3_stmt *res, const gchar *id, gint source)
{
    gint idx = sqlite3_bind_parameter_index (res, id);
    gint rc = sqlite3_bind_int (res, idx, source);
    if (rc) {
        sqlite3 *db = sqlite3_db_handle (res);
        const gchar *err = sqlite3_errmsg (db);
        g_error ("SWDB integer bind (args: %d|%s|%d): %s", idx, id, source, err);
    }
}

/**
 * _db_exec:
 * @db: sqlite database handle
 * @cmd: sql command
 * @callback: callback function
 *
 * Direct execution of sql command
 *
 * Returns: 0 if successful
 **/
gint
_db_exec (sqlite3 *db, const gchar *cmd, int (*callback) (void *, int, char **, char **))
{
    gchar *err_msg;
    gint result = sqlite3_exec (db, cmd, callback, 0, &err_msg);
    if (result != SQLITE_OK) {
        g_warning ("SWDB error: %s", err_msg);
        sqlite3_free (err_msg);
        return 1;
    }
    return 0;
}

/**
 * _tids_from_idid:
 * @db: sqlite database handle
 * @idid: item data ID
 *
 * Get transaction IDs for selected item data ID
 *
 * Returns: list of transaction IDs
 **/
static GArray *
_tids_from_idid (sqlite3 *db, gint idid)
{
    sqlite3_stmt *res;
    GArray *tids = g_array_new (0, 0, sizeof (gint));
    const gchar *sql = FIND_TIDS_FROM_IDID;
    _db_prepare (db, sql, &res);
    _db_bind_int (res, "@idid", idid);
    gint tid = 0;
    while (sqlite3_step (res) == SQLITE_ROW) {
        tid = sqlite3_column_int (res, 0);
        g_array_append_val (tids, tid);
    }
    sqlite3_finalize (res);
    return tids;
}

/**
 * _all_idid_for_iid:
 * @db: sqlite database handle
 * @iid: item ID
 *
 * Get all item data IDs connected with package with ID @iid.
 *
 * Returns: list of package data IDs
 **/
static GArray *
_all_idid_for_iid (sqlite3 *db, gint iid)
{
    GArray *idids = g_array_new (0, 0, sizeof (gint));
    sqlite3_stmt *res;
    const gchar *sql = FIND_ALL_IDID_FOR_IID;
    _db_prepare (db, sql, &res);
    _db_bind_int (res, "@iid", iid);
    gint idid;
    while ((idid = _db_find_int_multi (res))) {
        g_array_append_val (idids, idid);
    }
    return idids;
}

/**
 * _simple_search:
 * @db: sqlite database handle
 * @pattern: package name
 *
 * Find transactions which operated with package matched by name
 *
 * Returns: list of transaction IDs
 **/
GArray *
_simple_search (sqlite3 *db, const gchar *pattern)
{
    GArray *tids = g_array_new (0, 0, sizeof (gint));
    sqlite3_stmt *res_simple;
    const gchar *sql_simple = SIMPLE_SEARCH;
    _db_prepare (db, sql_simple, &res_simple);
    _db_bind_str (res_simple, "@pat", pattern);
    gint iid_simple;
    GArray *simple = g_array_new (0, 0, sizeof (gint));
    while ((iid_simple = _db_find_int_multi (res_simple))) {
        g_array_append_val (simple, iid_simple);
    }
    gint idid;
    for (guint i = 0; i < simple->len; i++) {
        iid_simple = g_array_index (simple, gint, i);
        GArray *idids = _all_idid_for_iid (db, iid_simple);
        for (guint j = 0; j < idids->len; j++) {
            idid = g_array_index (idids, gint, j);
            GArray *tids_for_idid = _tids_from_idid (db, idid);
            tids = g_array_append_vals (tids, tids_for_idid->data, tids_for_idid->len);
            g_array_free (tids_for_idid, TRUE);
        }
        g_array_free (idids, TRUE);
    }
    g_array_free (simple, TRUE);
    return tids;
}

/**
 * _extended_search:
 * @db: sqlite database handle
 * @pattern: package pattern
 *
 * Find transactions containing packages matched by @pattern
 * Accepted pattern formats:
 *   name.arch
 *   name-version-release.arch
 *   name-version
 *   epoch:name-version-release.arch
 *   name-epoch-version-release.arch*
 *
 * Returns: list of transaction IDs
 **/
GArray *
_extended_search (sqlite3 *db, const gchar *pattern)
{
    GArray *tids = g_array_new (0, 0, sizeof (gint));
    sqlite3_stmt *res;
    const gchar *sql = SEARCH_PATTERN;
    _db_prepare (db, sql, &res);
    _db_bind_str (res, "@pat", pattern);
    _db_bind_str (res, "@pat", pattern);
    _db_bind_str (res, "@pat", pattern);
    _db_bind_str (res, "@pat", pattern);
    _db_bind_str (res, "@pat", pattern);
    _db_bind_str (res, "@pat", pattern);
    GArray *iids = g_array_new (0, 0, sizeof (gint));
    gint iid;
    while (sqlite3_step (res) == SQLITE_ROW) {
        iid = sqlite3_column_int (res, 0);
        g_array_append_val (iids, iid);
    }
    sqlite3_finalize (res);
    gint idid;
    for (guint j = 0; j < iids->len; ++j) {
        iid = g_array_index (iids, gint, j);
        GArray *idids = _all_idid_for_iid (db, iid);
        for (guint i = 0; i < idids->len; ++i) {
            idid = g_array_index (idids, gint, i);
            GArray *tids_for_idid = _tids_from_idid (db, idid);
            tids = g_array_append_vals (tids, tids_for_idid->data, tids_for_idid->len);
            g_array_free (tids_for_idid, TRUE);
        }
        g_array_free (idids, TRUE);
    }
    g_array_free (iids, TRUE);
    return tids;
}

/**
 * _load_output:
 * @db: sqlite database handle
 * @tid: transaction ID
 * @type: output type
 *
 * Load output for transaction @tid
 *
 * Returns: list of transaction outputs in string
 **/
GPtrArray *
_load_output (sqlite3 *db, gint tid, gint type)
{
    sqlite3_stmt *res;
    const gchar *sql = S_OUTPUT;
    _db_prepare (db, sql, &res);
    _db_bind_int (res, "@tid", tid);
    _db_bind_int (res, "@type", type);
    GPtrArray *l = g_ptr_array_new ();
    gchar *row;
    while ((row = _db_find_str_multi (res))) {
        g_ptr_array_add (l, row);
    }
    return l;
}

/**
 * _output_insert:
 * @db: sqlite database handle
 * @tid: transaction ID
 * @msg: output text
 * @type: type of @msg (stdout or stderr)
 **/
void
_output_insert (sqlite3 *db, gint tid, const gchar *msg, gint type)
{
    sqlite3_stmt *res;
    const gchar *sql = I_OUTPUT;
    _db_prepare (db, sql, &res);
    _db_bind_int (res, "@tid", tid);
    _db_bind_str (res, "@msg", msg);
    _db_bind_int (res, "@type", type);
    _db_step (res);
}

/**
 * dnf_swdb_get_path:
 * @self: SWDB object
 *
 * Returns: database path
 **/
const gchar *
dnf_swdb_get_path (DnfSwdb *self)
{
    return self->path;
}

/**
 * dnf_swdb_set_path:
 * @self: SWDB object
 * @path: new path to sql database
 **/
void
dnf_swdb_set_path (DnfSwdb *self, const gchar *path)
{
    if (g_strcmp0 (path, self->path) != 0) {
        g_free (self->path);
        self->path = g_strdup (path);
    }
}

/**
 * dnf_swdb_exist:
 * @self: SWDB object
 *
 * Returns: %TRUE if sqlite database exists
 **/
gboolean
dnf_swdb_exist (DnfSwdb *self)
{
    return g_file_test (dnf_swdb_get_path (self), G_FILE_TEST_EXISTS);
}

/**
 * dnf_swdb_open:
 * @self: SWDB object
 *
 * Open SWDB database and set EXCLUSIVE and TRUNCATE pragmas
 *
 * Returns: 0 if successful
 **/
gint
dnf_swdb_open (DnfSwdb *self)
{
    if (!self) {
        return 1;
    }

    if (self->db) {
        return 0;
    }

    if (sqlite3_open (self->path, &self->db)) {
        g_warning ("ERROR: %s %s\n", sqlite3_errmsg (self->db), self->path);
        return 1;
    }
    return _db_exec (self->db, TRUNCATE_EXCLUSIVE, NULL);
}

/**
 * dnf_swdb_close:
 * @self: SWDB object
 *
 * Finalize all pending statements and close database
 **/
void
dnf_swdb_close (DnfSwdb *self)
{
    if (self->db) {
        if (sqlite3_close (self->db) == SQLITE_BUSY) {
            sqlite3_stmt *res;
            while ((res = sqlite3_next_stmt (self->db, NULL))) {
                sqlite3_finalize (res);
            }
            if (sqlite3_close (self->db)) {
                g_warning ("ERROR: %s\n", sqlite3_errmsg (self->db));
            }
        }
        self->db = NULL;
    }
}

/**
 * dnf_swdb_create_db:
 * @self: SWDB object
 *
 * Create new database at path set in SWDB object
 *
 * Returns: 0 if successful
 **/
gint
dnf_swdb_create_db (DnfSwdb *self)
{
    if (dnf_swdb_open (self))
        return 1;

    int failed = _create_db (self->db);
    if (failed) {
        g_warning ("SWDB error: Unable to create SWDB tables\n");
        sqlite3_close (self->db);
        self->db = NULL;
        return 2;
    }
    // close - allow transformer to work with DB
    dnf_swdb_close (self);
    return 0;
}

/**
 * dnf_swdb_reset_db:
 * @self: SWDB object
 *
 * Remove and create new database
 *
 * Returns: 0 if successful
 **/
gint
dnf_swdb_reset_db (DnfSwdb *self)
{
    if (dnf_swdb_exist (self)) {
        dnf_swdb_close (self);
        if (g_remove (self->path) != 0) {
            g_warning ("SWDB error: Database reset failed!\n");
            return 1;
        }
    }
    return dnf_swdb_create_db (self);
}
