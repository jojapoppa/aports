/* upgrade.c - Alpine Package Keeper (APK)
 *
 * Copyright (C) 2005-2008 Natanael Copa <n@tanael.org>
 * Copyright (C) 2008 Timo Teräs <timo.teras@iki.fi>
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation. See http://www.gnu.org/ for details.
 */

#include <errno.h>
#include <stdio.h>
#include <zlib.h>
#include "apk_applet.h"
#include "apk_database.h"
#include "apk_state.h"

static int upgrade_parse(void *ctx, struct apk_db_options *dbopts,
			 int optch, int optindex, const char *optarg)
{
	switch (optch) {
	case 'a':
		apk_flags |= APK_PREFER_AVAILABLE;
		break;
	default:
		return -1;
	}
	return 0;
}

static int upgrade_main(void *ctx, struct apk_database *db, int argc, char **argv)
{
	struct apk_state *state = NULL;
	int i, r = 0;

	apk_flags |= APK_UPGRADE;

	state = apk_state_new(db);
	if (state == NULL)
		goto err;

	for (i = 0; i < db->world->num; i++) {
		r = apk_state_lock_dependency(state, &db->world->item[i]);
		if (r != 0) {
			apk_error("Unable to upgrade '%s'",
				  db->world->item[i].name->name);
			goto err;
		}
	}
	r = apk_state_commit(state, db);
err:
	if (state != NULL)
		apk_state_unref(state);
	return r;
}

static struct apk_option upgrade_options[] = {
	{ 'a', "available",
	  "Re-install or downgrade if currently installed package is not "
	  "currently available from any repository" },
};

static struct apk_applet apk_upgrade = {
	.name = "upgrade",
	.help = "Upgrade (or downgrade with -a) the currently installed "
		"packages to versions available in repositories.",
	.open_flags = APK_OPENF_WRITE,
	.num_options = ARRAY_SIZE(upgrade_options),
	.options = upgrade_options,
	.parse = upgrade_parse,
	.main = upgrade_main,
};

APK_DEFINE_APPLET(apk_upgrade);

