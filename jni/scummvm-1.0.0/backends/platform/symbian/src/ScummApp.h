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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/backends/platform/symbian/src/ScummApp.h $
 * $Id: ScummApp.h 27024 2007-05-30 21:56:52Z fingolfin $
 */

#ifndef ScummAPPH
#define ScummAPPH

#include <eikapp.h>
#include <e32base.h>
#include <sdlapp.h>

#if defined (EPOC_AS_APP) && !defined (UIQ3) && !defined (S60V3)
#include "ECompXL.h"
#endif

class CScummApp : public CSDLApp {
public:
	CScummApp();
	~CScummApp();
#if defined (UIQ3)
	/**
	 * Returns the resource id to be used to declare the views supported by this UIQ3 app
	 * @return TInt, resource id
	 */
	TInt ViewResourceId();
#endif
	TUid AppDllUid() const;
#if defined (EPOC_AS_APP) && !defined (UIQ3) && !defined (S60V3)
	TECompXL    iECompXL;
#endif
};
#endif


