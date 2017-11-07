/* dnf-swdb-sql.h
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
 * Lesser GeneDral Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef _DNF_SWDB_SQL
#define _DNF_SWDB_SQL

#define I_DESC_STATE "INSERT INTO state_type   VALUES (null, @desc)"
#define I_DESC_OUTPUT "INSERT INTO output_type  VALUES (null, @desc)"
#define I_TRANS_WITH "INSERT INTO TRANS_WITH   VALUES (null, @tid, @pid)"

#define INSERT_PKG \
    "insert into PACKAGE values(null,@name,@epoch,@version,@release,@arch,@cdata,@ctype,@type)"
#define INSERT_TRANS_BEG \
    "insert into TRANS values(null,@beg,null,@rpmdbv,null,@cmdline,@loginuid,@releasever,null)"
#define INSERT_TRANS_END \
    "UPDATE TRANS SET end_timestamp=@end,end_RPMDB_version=@rpmdbv, return_code=@rc WHERE " \
    "T_ID=@tid"
#define INSERT_REPO "insert into REPO values(null,@name)"

#define UPDATE_PKG_DATA \
    "UPDATE PACKAGE_DATA SET R_ID=@rid,from_repo_revision=@repo_r,from_repo_timestamp=@repo_t," \
    "installed_by=@installed_by,changed_by=@changed_by where P_ID=@pid"

#define INSERT_TRANS_DATA_BEG \
    "insert into TRANS_DATA values(null,@tid,@pdid,@tgid,0,@obsoleting,@reason,@state)"
#define UPDATE_TRANS_DATA_PID_END \
    "UPDATE TRANS_DATA SET done=@done WHERE T_ID=@tid and PD_ID=@pdid and state=@state"

#define FIND_REPO_BY_NAME "SELECT R_ID FROM REPO WHERE name=@name"
#define FIND_PDID_FROM_PID \
    "SELECT PD_ID FROM PACKAGE_DATA WHERE P_ID=@pid ORDER by PD_ID DESC LIMIT 1"
#define GET_TRANS_CMDLINE "SELECT cmdline FROM TRANS WHERE T_ID=@tid"

#define INSERT_PDID "insert into PACKAGE_DATA values(null,@pid,null,null,null,null,null)"
#define PKG_DATA_ATTR_BY_PID "FROM PACKAGE_DATA WHERE P_ID=@pid"
#define TRANS_DATA_ATTR_BY_PDID "FROM TRANS_DATA WHERE PD_ID=@pdid"
#define TRANS_ATTR_BY_TID "FROM TRANS WHERE T_ID=@tid"
#define PID_BY_TID "select P_ID from TRANS_DATA join PACKAGE_DATA using(PD_ID) where T_ID=@tid"

#define RESOLVE_GROUP_TRANS \
    "SELECT TG_ID FROM PACKAGE join GROUPS_PACKAGE using (name) join TRANS_GROUP_DATA using " \
    "(G_ID) WHERE P_ID=@pid and T_ID=@tid"

#define FIND_PIDS_BY_NAME "SELECT P_ID FROM PACKAGE WHERE NAME LIKE @pattern"

#define S_REASON_BY_PID \
    "SELECT reason FROM PACKAGE_DATA join TRANS_DATA using (PD_ID) WHERE P_ID=@pid ORDER by " \
    "TD_ID DESC LIMIT 1"

#define S_REASON_BY_NEVRA \
    "SELECT reason FROM PACKAGE join PACKAGE_DATA using (P_ID) join TRANS_DATA using (PD_ID) " \
    "WHERE name=@n and epoch=@e and version=@v and release=@r and arch=@a ORDER by TD_ID DESC " \
    "LIMIT 1"

#define S_PACKAGE_BY_PID "SELECT * FROM PACKAGE WHERE P_ID=@pid"
#define S_NAME_BY_PID "SELECT name FROM PACKAGE WHERE P_ID=@pid"
#define S_LAST_TDID_BY_NAME \
    "SELECT TD_ID FROM PACKAGE join PACKAGE_DATA using(P_ID) " \
    "join TRANS_DATA using(PD_ID) WHERE name=@name " \
    "and P_ID!=@pid and T_ID!=@tid ORDER BY TD_ID DESC LIMIT 1"

#define S_LAST_W_TDID_BY_NAME \
    "SELECT TD_ID FROM PACKAGE join PACKAGE_DATA using(P_ID) " \
    "join TRANS_DATA using(PD_ID) WHERE name=@name " \
    "and T_ID!=@tid ORDER BY TD_ID DESC LIMIT 1"

#define S_PACKAGE_DATA_BY_PID "SELECT * FROM PACKAGE_DATA WHERE P_ID=@pid ORDER BY PD_ID DESC"
#define S_REPO_BY_RID "select name from REPO where R_ID=@rid"
#define S_PREV_PKG_DATA \
    "SELECT PD_ID FROM PACKAGE_DATA join TRANS_DATA using(PD_ID) where P_ID=@pid and T_ID=@tid"

#define S_TRANS "SELECT * from TRANS ORDER BY T_ID DESC"
#define S_TRANS_W_LIMIT "SELECT * from TRANS ORDER BY T_ID DESC LIMIT @limit"
#define S_TRANS_COMP \
    "SELECT * from TRANS WHERE end_timestamp is not null or end_timestamp!=0 ORDER BY T_ID DESC"
#define S_TRANS_COMP_W_LIMIT \
    "SELECT * from TRANS WHERE end_timestamp is not null or end_timestamp!=0 ORDER BY T_ID DESC " \
    "LIMIT @limit"

#define S_PACKAGE_STATE \
    "select done, state, obsoleting from PACKAGE_DATA join TRANS_DATA using (PD_ID) " \
    "where P_ID=@pid and T_ID=@tid"

#define S_REPO_FROM_PID2 "SELECT name FROM PACKAGE_DATA join REPO using(R_ID) where P_ID=@pid"

#define S_CHECKSUM_BY_NEVRA \
    "SELECT checksum_data, checksum_type FROM PACKAGE WHERE name=@n and epoch=@e and version=@v " \
    "and release=@r and arch=@a"

#define S_PID_BY_NEVRA \
    "SELECT P_ID FROM PACKAGE WHERE name=@n and epoch=@e and version=@v and release=@r and " \
    "arch=@a"

#define S_STATE_TYPE_ID "SELECT state  FROM state_type   WHERE description=@desc"
#define S_OUTPUT_TYPE_ID "SELECT type   FROM output_type  WHERE description=@desc"

#define S_STATE_TYPE_BY_ID "SELECT description FROM state_type   WHERE state=@id"
#define S_OUTPUT_TYPE_BY_ID "SELECT description FROM output_type  WHERE type=@id"

#define S_LATEST_PACKAGE "SELECT p_id FROM package WHERE name=@name ORDER BY p_id DESC LIMIT 1"

#define S_ERASED_REASON \
    "SELECT reason FROM TRANS_DATA join PACKAGE_DATA using (PD_ID) WHERE T_ID=@tid and P_ID=@pid"

#define U_REASON_BY_PDID "UPDATE TRANS_DATA SET reason=@reason where PD_ID=@pdid"

#define U_REPO_BY_PID "UPDATE PACKAGE_DATA SET R_ID=@rid where P_ID=@pid"

#define T_OUTPUT "SELECT O_ID FROM OUTPUT WHERE T_ID=@tid and type=@type"

#endif
