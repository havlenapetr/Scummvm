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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/sci/gfx/gfx_options.h $
 * $Id: gfx_options.h 41785 2009-06-22 21:48:47Z fingolfin $
 *
 */


#ifndef SCI_GFX_GFX_OPTIONS_H
#define SCI_GFX_GFX_OPTIONS_H

#include "sci/gfx/gfx_resource.h"
#include "sci/gfx/gfx_tools.h"

// Define this to enable user-defined custom graphics options
// TODO: Most of these options don't work in 256-color mode, plus there
// is currently no way to actually set/change them somehow (other than
// modifying the code)
//#define CUSTOM_GRAPHICS_OPTIONS

#ifdef CUSTOM_GRAPHICS_OPTIONS
#include "sci/gfx/gfx_res_options.h"
#endif

namespace Sci {

/** Dirty rectangle heuristics. */
enum {
	GFXOP_DIRTY_FRAMES_ONE = 1, /**< One: Redraw one rectangle surrounding the dirty area (insert is O(1)) */
	GFXOP_DIRTY_FRAMES_CLUSTERS = 2 /**< Clusters: Accumulate dirty rects, merging those that overlap (insert is O(n))  */
};

/**
 * All user options to the rendering pipeline
 *
 * See note in sci_conf.h for config_entry_t before changing types of
 * variables
 */
struct gfx_options_t {
#ifdef CUSTOM_GRAPHICS_OPTIONS
	int buffer_pics_nr; /* Number of unused pics to buffer */

	/* SCI0 pic resource options */
	int pic0_unscaled; /* Don't draw scaled SCI0 pics */

	int pic0_dither_mode; /* Defined in gfx_resource.h */
	int pic0_dither_pattern; /* Defined in gfx_resource.h */

	gfx_brush_mode_t pic0_brush_mode;
	gfx_line_mode_t pic0_line_mode;

	gfx_xlate_filter_t cursor_xlate_filter;
	gfx_xlate_filter_t view_xlate_filter;
	gfx_xlate_filter_t pic_xlate_filter; /* Only relevant if (pic0_unscaled) */
	gfx_xlate_filter_t text_xlate_filter;
	gfx_res_fullconf_t res_conf; /* Resource customisation: Per-resource palettes etc. */

	int dirty_frames;

	int workarounds;	// Workaround flags - see below
#endif
};

#ifdef CUSTOM_GRAPHICS_OPTIONS
/* SQ3 counts whitespaces towards the total text size, as does gfxop_get_text_params() if this is set: */
#define GFX_WORKAROUND_WHITESPACE_COUNT (1 << 0)
#endif

} // End of namespace Sci

#endif // SCI_GFX_GFX_OPTIONS_H
