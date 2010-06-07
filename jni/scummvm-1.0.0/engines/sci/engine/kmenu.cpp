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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/sci/engine/kmenu.cpp $
 * $Id: kmenu.cpp 41340 2009-06-07 16:50:34Z thebluegr $
 *
 */

#include "sci/sci.h"
#include "sci/resource.h"
#include "sci/engine/state.h"
#include "sci/engine/kernel.h"
#include "sci/gfx/gfx_gui.h"
#include "sci/gfx/menubar.h"
#include "sci/gfx/gfx_state_internal.h"	// required for GfxPort, GfxVisual

namespace Sci {

reg_t kAddMenu(EngineState *s, int funct_nr, int argc, reg_t *argv) {
	char *name = kernel_dereference_char_pointer(s, argv[0], 0);
	char *contents = kernel_dereference_char_pointer(s, argv[1], 0);

	s->_menubar->addMenu(s->gfx_state, name,
	                 contents, s->titlebar_port->_font, argv[1]);

	return s->r_acc;

}


reg_t kSetMenu(EngineState *s, int funct_nr, int argc, reg_t *argv) {
	int index = argv[0].toUint16();
	int i = 2;

	while (i < argc) {
		s->_menubar->setAttribute(s, (index >> 8) - 1, (index & 0xff) - 1, argv[i - 1].toUint16(), argv[i]);
		i += 2;
	}

	return s->r_acc;
}

reg_t kGetMenu(EngineState *s, int funct_nr, int argc, reg_t *argv) {
	int index = argv[0].toUint16();

	return s->_menubar->getAttribute((index >> 8) - 1, (index & 0xff) - 1, argv[1].toUint16());
}


reg_t kDrawStatus(EngineState *s, int funct_nr, int argc, reg_t *argv) {
	reg_t text = argv[0];
	int fgcolor = (argc > 1) ? argv[1].toSint16() : s->status_bar_foreground;
	int bgcolor = (argc > 2) ? argv[2].toSint16() : s->status_bar_background;

	s->titlebar_port->_color.visual = get_pic_color(s, fgcolor);
	s->titlebar_port->_color.mask = GFX_MASK_VISUAL;
	s->titlebar_port->_bgcolor.visual = get_pic_color(s, bgcolor);
	s->titlebar_port->_bgcolor.mask = GFX_MASK_VISUAL;

	s->status_bar_foreground = fgcolor;
	s->status_bar_background = bgcolor;

	if (text.segment) {
		const char *tmp = strdup(kernel_dereference_char_pointer(s, text, 0));
		s->_statusBarText = tmp ? tmp : "";
	}

	sciw_set_status_bar(s, s->titlebar_port, s->_statusBarText, fgcolor, bgcolor);

	gfxop_update(s->gfx_state);

	return s->r_acc;
}


reg_t kDrawMenuBar(EngineState *s, int funct_nr, int argc, reg_t *argv) {

	if (argv[0].toSint16())
		sciw_set_menubar(s, s->titlebar_port, s->_menubar, -1);
	else
		sciw_set_status_bar(s, s->titlebar_port, "", 0, 0);

	s->titlebar_port->draw(Common::Point(0, 0));
	gfxop_update(s->gfx_state);

	return s->r_acc;
}


static int _menu_go_down(Menubar *menubar, int menu_nr, int item_nr) {
	int seeker;
	const int max = menubar->_menus[menu_nr]._items.size();
	seeker = item_nr + 1;

	while ((seeker < max) && !menubar->itemValid(menu_nr, seeker))
		++seeker;

	if (seeker != max)
		return seeker;
	else
		return item_nr;
}

#define FULL_REDRAW \
	s->visual->draw(Common::Point(0, 0)); \
	gfxop_update(s->gfx_state);


reg_t kMenuSelect(EngineState *s, int funct_nr, int argc, reg_t *argv) {
	reg_t event = argv[0];
	/*int pause_sound = (argc > 1) ? argv[1].toUint16() : 1;*/ /* FIXME: Do this eventually */
	bool claimed = false;
	int type = GET_SEL32V(event, type);
	int message = GET_SEL32V(event, message);
	int modifiers = GET_SEL32V(event, modifiers);
	int menu_nr = -1, item_nr = 0;
	MenuItem *item;
	int menu_mode = 0; /* Menu is active */
	int mouse_down = 0;

	gfxop_set_clip_zone(s->gfx_state, gfx_rect_fullscreen);

	/* Check whether we can claim the event directly as a keyboard or said event */
	if (type & (SCI_EVT_KEYBOARD | SCI_EVT_SAID)) {
		int menuc, itemc;

		if ((type == SCI_EVT_KEYBOARD)
		        && (message == SCI_K_ESC))
			menu_mode = 1;

		else if ((type == SCI_EVT_SAID) || message) { /* Don't claim 0 keyboard event */
			debugC(2, kDebugLevelMenu, "Menu: Got %s event: %04x/%04x\n",
			          ((type == SCI_EVT_SAID) ? "SAID" : "KBD"), message, modifiers);

			for (menuc = 0; menuc < (int)s->_menubar->_menus.size(); menuc++)
				for (itemc = 0; itemc < (int)s->_menubar->_menus[menuc]._items.size(); itemc++) {
					item = &s->_menubar->_menus[menuc]._items[itemc];

					debugC(2, kDebugLevelMenu, "Menu: Checking against %s: %04x/%04x (type %d, %s)\n",
					          !item->_text.empty() ? item->_text.c_str() : "--bar--", item->_key, item->_modifiers,
					          item->_type, item->_enabled ? "enabled" : "disabled");

					if (((item->_type == MENU_TYPE_NORMAL)
					        && (item->_enabled))
					        && (((type == SCI_EVT_KEYBOARD) /* keyboard event */
					             && item->matchKey(message, modifiers))
					            || ((type == SCI_EVT_SAID) /* Said event */
					                && (item->_flags & MENU_ATTRIBUTE_FLAGS_SAID)
					                && (said(s, item->_said, 
#ifdef DEBUG_PARSER
					                1
#else
									0
#endif
									) != SAID_NO_MATCH)
					               )
					           )
					   ) {
						/* Claim the event */
						debugC(2, kDebugLevelMenu, "Menu: Event CLAIMED for %d/%d\n", menuc, itemc);
						claimed = true;
						menu_nr = menuc;
						item_nr = itemc;
					}
				}
		}
	}

	if ((type == SCI_EVT_MOUSE_PRESS) && (s->gfx_state->pointer_pos.y < 10)) {
		menu_mode = 1;
		mouse_down = 1;
	}

	if (menu_mode) {
		int old_item;
		int old_menu;
		GfxPort *port = NULL;

		item_nr = -1;

		/* Default to menu 0, unless the mouse was used to generate this effect */
		if (mouse_down)
			s->_menubar->mapPointer(s->gfx_state->pointer_pos, menu_nr, item_nr, port);
		else
			menu_nr = 0;

		sciw_set_menubar(s, s->titlebar_port, s->_menubar, menu_nr);
		FULL_REDRAW;

		old_item = -1;
		old_menu = -1;

		while (menu_mode) {
			sci_event_t ev = gfxop_get_event(s->gfx_state, SCI_EVT_ANY);

			claimed = false;

			switch (ev.type) {
			case SCI_EVT_QUIT:
				quit_vm();
				return NULL_REG;

			case SCI_EVT_KEYBOARD:
				switch (ev.data) {

				case '`':
					if (ev.buckybits & SCI_EVM_CTRL)
						s->visual->print(0);
					break;

				case SCI_K_ESC:
					menu_mode = 0;
					break;

				case SCI_K_ENTER:
					menu_mode = 0;
					if ((item_nr >= 0) && (menu_nr >= 0))
						claimed = true;
					break;

				case SCI_K_LEFT:
					if (menu_nr > 0)
						--menu_nr;
					else
						menu_nr = s->_menubar->_menus.size() - 1;

					item_nr = _menu_go_down(s->_menubar, menu_nr, -1);
					break;

				case SCI_K_RIGHT:
					if (menu_nr < ((int)s->_menubar->_menus.size() - 1))
						++menu_nr;
					else
						menu_nr = 0;

					item_nr = _menu_go_down(s->_menubar, menu_nr, -1);
					break;

				case SCI_K_UP:
					if (item_nr > -1) {

						do { --item_nr; }
						while ((item_nr > -1) && !s->_menubar->itemValid(menu_nr, item_nr));
					}
					break;

				case SCI_K_DOWN: {
					item_nr = _menu_go_down(s->_menubar, menu_nr, item_nr);
				}
				break;

				}
				break;

			case SCI_EVT_MOUSE_RELEASE:
				menu_mode = (s->gfx_state->pointer_pos.y < 10);
				claimed = !menu_mode && !s->_menubar->mapPointer(s->gfx_state->pointer_pos, menu_nr, item_nr, port);
				mouse_down = 0;
				break;

			case SCI_EVT_MOUSE_PRESS:
				mouse_down = 1;
				break;

			case SCI_EVT_NONE:
				gfxop_sleep(s->gfx_state, 2500 / 1000);
				break;
			}

			if (mouse_down)
				s->_menubar->mapPointer(s->gfx_state->pointer_pos, menu_nr, item_nr, port);

			if ((item_nr > -1 && old_item == -1) || (menu_nr != old_menu)) { /* Update menu */

				sciw_set_menubar(s, s->titlebar_port, s->_menubar, menu_nr);

				delete port;

				port = sciw_new_menu(s, s->titlebar_port, s->_menubar, menu_nr);
				s->wm_port->add((GfxContainer *)s->wm_port, port);

				if (item_nr > -1)
					old_item = -42; /* Enforce redraw in next step */
				else {
					FULL_REDRAW;
				}

			} /* ...if the menu changed. */

			/* Remove the active menu item, if neccessary */
			if (item_nr != old_item) {
				port = sciw_toggle_item(port, &(s->_menubar->_menus[menu_nr]), old_item, false);
				port = sciw_toggle_item(port, &(s->_menubar->_menus[menu_nr]), item_nr, true);
				FULL_REDRAW;
			}

			old_item = item_nr;
			old_menu = menu_nr;

		} /* while (menu_mode) */

		if (port) {
			delete port;
			port = NULL;

			sciw_set_status_bar(s, s->titlebar_port, s->_statusBarText, s->status_bar_foreground, s->status_bar_background);
			gfxop_update(s->gfx_state);
		}
		FULL_REDRAW;
	}

	if (claimed) {
		PUT_SEL32(event, claimed, make_reg(0, 1));

		if (menu_nr > -1) {
			s->r_acc = make_reg(0, ((menu_nr + 1) << 8) | (item_nr + 1));
		} else
			s->r_acc = NULL_REG;

		debugC(2, kDebugLevelMenu, "Menu: Claim -> %04x\n", s->r_acc.offset);
	} else
		s->r_acc = NULL_REG; /* Not claimed */

	return s->r_acc;
}

} // End of namespace Sci
