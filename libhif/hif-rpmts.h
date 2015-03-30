/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2013 Richard Hughes <richard@hughsie.com>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 */

#if !defined (__LIBHIF_H) && !defined (HIF_COMPILATION)
#error "Only <libhif.h> can be included directly."
#endif

#ifndef __HIF_RPMTS_H
#define __HIF_RPMTS_H

#include <glib.h>
#include <rpm/rpmts.h>
#include <hawkey/package.h>

typedef enum {
	HIF_RPMTS_FLAG_ALLOW_UNTRUSTED = (1 << 0),
	HIF_RPMTS_FLAG_IS_UPDATE = (1 << 1),
	HIF_RPMTS_FLAG_OSTREE_MODE = (1 << 2)
} HifRpmTsFlags;

gboolean	 hif_rpmts_add_install_filename	(rpmts		 ts,
						 const gchar	*filename,
						 HifRpmTsFlags   flags,
						 GError		**error);
gboolean	 hif_rpmts_add_remove_pkg	(rpmts		 ts,
						 HyPackage	 pkg,
						 GError		**error);
gboolean	 hif_rpmts_look_for_problems	(rpmts		 ts,
						 GError		**error);

#endif /* __HIF_RPMTS_H */
