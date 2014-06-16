/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2014 Richard Hughes <richard@hughsie.com>
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

#ifndef __HIF_CONTEXT_PRIVATE_H
#define __HIF_CONTEXT_PRIVATE_H

#include "hif-context.h"
#include "hif-state.h"
#include "hif-transaction.h"

gboolean	 hif_context_commit			(HifContext	*context,
							 HifState	*state,
							 GError		**error);
void		 hif_context_invalidate			(HifContext	*context,
							 const gchar	*message);
GPtrArray	*hif_context_get_sources		(HifContext	*context);

HifTransaction	*hif_context_get_transaction		(HifContext	*context);

HySack   	 hif_context_get_sack			(HifContext	*context);

HyGoal  	 hif_context_get_goal			(HifContext	*context);

G_END_DECLS

#endif /* __HIF_CONTEXT_PRIVATE_H */
