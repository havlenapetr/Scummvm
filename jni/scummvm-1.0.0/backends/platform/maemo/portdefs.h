/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/backends/platform/maemo/portdefs.h $
 * $Id: portdefs.h 30664 2008-01-27 19:47:41Z jvprat $
 *
 */

#ifndef PORTDEFS_H
#define PORTDEFS_H

#define SCUMM_LITTLE_ENDIAN
#define SCUMM_NEED_ALIGNMENT

#undef HAVE_X86

#undef LINUPY

/* Data types */
typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef signed char int8;
typedef signed short int16;
typedef signed int int32;

/* Libs */
#undef USE_VORBIS
#undef USE_TREMOR
#undef USE_FLAC
#undef USE_MAD
#define USE_ZLIB
#undef USE_MPEG2
#undef USE_MT32EMU

/* Whether we should use i386 assembly routines */
#undef USE_NASM

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

void set_doubling(unsigned char enable);

#endif /* PORTDEFS_H */


