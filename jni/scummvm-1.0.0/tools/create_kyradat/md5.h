/* Scumm Tools
 * Copyright (C) 2004-2006  The ScummVM Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/tools/create_kyradat/md5.h $
 * $Id: md5.h 29013 2007-09-22 07:49:51Z eriktorbjorn $
 *
 */

#ifndef COMMON_MD5_H
#define COMMON_MD5_H

#include "util.h"

typedef struct {
	uint32 total[2];
	uint32 state[4];
	uint8 buffer[64];
} md5_context;

void md5_starts(md5_context *ctx);
void md5_update(md5_context *ctx, const uint8 *input, uint32 length);
void md5_finish(md5_context *ctx, uint8 digest[16]);

bool md5_file(const char *name, uint8 digest[16], uint32 length = 0);

#endif
