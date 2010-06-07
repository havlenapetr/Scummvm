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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/backends/platform/wince/CEgui/Panel.h $
 * $Id: Panel.h 28966 2007-09-19 08:40:12Z peres001 $
 *
 */

#ifndef CEGUI_PANEL
#define CEGUI_PANEL

#include "common/scummsys.h"
#include "common/system.h"
#include "common/hashmap.h"
#include "common/str.h"
#include "common/config-manager.h"

#include "PanelItem.h"
#include "Toolbar.h"

using Common::String;
using Common::HashMap;

namespace CEGUI {

	class Panel : public Toolbar {
	public:
		Panel(int interleave_first, int interleave);
		virtual bool draw(SDL_Surface *surface);
		virtual ~Panel();
		bool add(const String &name, const PanelItem *item);
		void clear();
		virtual void forceRedraw();
		virtual bool action(int x, int y, bool pushed);
	private:

		typedef HashMap<String, PanelItem*, Common::IgnoreCase_Hash , Common::IgnoreCase_EqualTo> ItemMap;

		ItemMap _itemsMap;
		int _interleave;
		int _currentItem;
	};
}

#endif
