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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/backends/platform/ps2/iop/CoDyVDfs/iop/fiofs.h $
 * $Id: fiofs.h 27024 2007-05-30 21:56:52Z fingolfin $
 *
 */

#ifndef __FIOFS_H__
#define __FIOFS_H__

int initFio(void);

int cd_open(iop_file_t *handle, const char *name, int mode);
int cd_lseek(iop_file_t *handle, int ofs, int whence);
int cd_read(iop_file_t *handle, void *dest, int length);
int cd_close(iop_file_t *handle);
int cd_dopen(iop_file_t *handle, const char *path);
int cd_dread(iop_file_t *handle, fio_dirent_t *buf);
int cd_dclose(iop_file_t *handle);

#endif // __FIOFS_H__

