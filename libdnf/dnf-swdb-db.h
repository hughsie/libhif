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

#ifndef DNF_SWDB_DB_H
#define DNF_SWDB_DB_H

#include <glib-object.h>
#include <glib.h>
#include <sqlite3.h>
#include "dnf-swdb.h"

#ifdef __cplusplus
extern "C" {
#endif

gint            _create_db          (sqlite3        *db);
void            _db_step            (sqlite3_stmt   *res);
gint            _db_find_int        (sqlite3_stmt   *res);
gchar          *_db_find_str        (sqlite3_stmt   *res);
gchar          *_db_find_str_multi  (sqlite3_stmt   *res);
gint            _db_find_int_multi  (sqlite3_stmt   *res);
void            _db_prepare         (sqlite3        *db,
                                     const gchar    *sql,
                                     sqlite3_stmt  **res);
void            _db_bind_str        (sqlite3_stmt   *res,
                                     const gchar    *id,
                                     const gchar    *source);
void            _db_bind_int        (sqlite3_stmt   *res,
                                     const gchar    *id,
                                     gint            source);
gint            _db_exec            (sqlite3        *db,
                                     const gchar    *cmd,
                                     int           (*callback)(void *, int, char **, char**));
GArray         *_simple_search      (sqlite3        *db,
                                     const gchar    *pattern);
GArray         *_extended_search    (sqlite3        *db,
                                     const gchar    *pattern);

GPtrArray      *_load_output        (sqlite3        *db,
                                     gint            tid,
                                     gint            type);

void            _output_insert      (sqlite3        *db,
                                     gint            tid,
                                     const gchar    *msg,
                                     gint            type);
const gchar    *dnf_swdb_get_path   (DnfSwdb        *self);
void            dnf_swdb_set_path   (DnfSwdb        *self,
                                     const gchar    *path);
gboolean        dnf_swdb_exist      (DnfSwdb        *self);
gint            dnf_swdb_create_db  (DnfSwdb        *self);
gint            dnf_swdb_reset_db   (DnfSwdb        *self);
gint            dnf_swdb_open       (DnfSwdb        *self);
void            dnf_swdb_close      (DnfSwdb        *self);

#ifdef __cplusplus
}
#endif

#endif
