// Copyright (C) 2024 Shawn Rutledge <s@ecloud.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef COMMON_H
#define COMMON_H

#include "settings.h"

#define STR_ORGANIZATION "Electron Cloud"
#define STR_PRODUCT "ScanCrop"

#define SETTING_GROUP_MAIN "main"
#define SETTING_GROUP_OPEN_FILES "open-files"
#define SETTING_GROUP_RECENT_FILES "recent-files"
#define SETTING_RECENT_FILES_LIMIT "recentFilesLimit"
#define SETTING_GROUP_SCAN_GEOMETRY "scan-geometry"

#ifndef MIN
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#endif
#ifndef MAX
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#endif

#endif // COMMON_H
