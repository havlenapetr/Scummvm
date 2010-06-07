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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/sci/engine/state.h $
 * $Id: state.h 42398 2009-07-11 23:45:54Z thebluegr $
 *
 */

#ifndef SCI_INCLUDE_ENGINE_H
#define SCI_INCLUDE_ENGINE_H

#include "common/scummsys.h"
#include "common/array.h"
#include "common/serializer.h"

namespace Common {
	class SeekableReadStream;
	class WriteStream;
}

#include "sci/sci.h"
#include "sci/vocabulary.h"
#include "sci/resource.h"
#include "sci/engine/kernel.h"	// for kfunct_sig_pair_t
#include "sci/engine/message.h"		// for MessageState
#include "sci/engine/script.h"
#include "sci/engine/seg_manager.h"
#include "sci/gfx/gfx_system.h"
#include "sci/sfx/core.h"

namespace Sci {

class Menubar;

struct GfxState;
struct GfxPort;
struct GfxVisual;
struct GfxContainer;
struct GfxList;


class DirSeeker {
protected:
	EngineState *_vm;
	reg_t _outbuffer;
	Common::StringList _savefiles;
	Common::StringList::const_iterator _iter;

public:
	DirSeeker(EngineState *s) : _vm(s) {
		_outbuffer = NULL_REG;
		_iter = _savefiles.begin();
	}

	void firstFile(const char *mask, reg_t buffer);
	void nextFile();
};

enum {
	CURRENT_SAVEGAME_VERSION = 9,
	MINIMUM_SAVEGAME_VERSION = 9,

	MAX_SAVE_DIR_SIZE = MAXPATHLEN
};

/** values for EngineState.restarting_flag */
enum {
	SCI_GAME_IS_NOT_RESTARTING = 0,
	SCI_GAME_WAS_RESTARTED = 1,
	SCI_GAME_IS_RESTARTING_NOW = 2,
	SCI_GAME_WAS_RESTARTED_AT_LEAST_ONCE = 4
};

/** Supported languages */
enum kLanguage {
	K_LANG_NONE = 0,
	K_LANG_ENGLISH = 1,
	K_LANG_FRENCH = 33,
	K_LANG_SPANISH = 34,
	K_LANG_ITALIAN = 39,
	K_LANG_GERMAN = 49,
	K_LANG_JAPANESE = 81,
	K_LANG_PORTUGUESE = 351
};

struct drawn_pic_t {
	int nr;
	int palette;
};

class FileHandle {
public:
	Common::String _name;
	Common::SeekableReadStream *_in;
	Common::WriteStream *_out;

public:
	FileHandle();
	~FileHandle();

	void close();
	bool isOpen() const;
};

struct EngineState : public Common::Serializable {
public:
	EngineState(ResourceManager *res, sci_version_t version, uint32 flags);
	virtual ~EngineState();
	virtual void saveLoadWithSerializer(Common::Serializer &ser);

public:
	int widget_serial_counter; /**< Used for savegames */

	ResourceManager *resmgr; /**< The resource manager */

	const sci_version_t _version; /**< The approximated patchlevel of the version to emulate */
	const uint32 _flags;			/**< Specific game flags */

	Common::String _gameName; /**< Designation of the primary object (which inherits from Game) */
	char *game_version;

	/* Non-VM information */

	GfxState *gfx_state; /**< Graphics state and driver */
	gfx_pixmap_t *old_screen; /**< Old screen content: Stored during kDrawPic() for kAnimate() */

	SfxState _sound; /**< sound subsystem */
	int sfx_init_flags; /**< flags the sfx subsystem was initialised with */
	unsigned int sound_volume; /**< 0x0 -> 0xf Current volume of sound system */
	unsigned int sound_mute; /**< 0 = not, else == saved value */

	byte restarting_flags; /**< Flags used for restarting */

	byte pic_not_valid; /**< Is 0 if the background picture is "valid" */
	byte pic_is_new; /**< New pic was loaded or port was opened */

	int *pic_priority_table; /**< 16 entries with priorities or NULL if not present */

	/** Text on the status bar, or NULL if the title bar is blank */
	Common::String _statusBarText;

	int status_bar_foreground, status_bar_background;

	long game_time; /**< Counted at 60 ticks per second, reset during start time */

	GfxPort *port; /**< The currently active port */

	gfx_color_t ega_colors[16]; /**< The 16 EGA colors- for SCI0(1) */

	GfxVisual *visual; /**< A visual widget, containing all ports */

	GfxPort *titlebar_port; /**< Title bar viewport (0,0,9,319) */
	GfxPort *wm_port; /**< window manager viewport and designated &heap[0] view (10,0,199,319) */
	GfxPort *picture_port; /**< The background picture viewport (10,0,199,319) */
	GfxPort *iconbar_port; /**< Full-screen port used for non-clipped icon bar draw in SCI1 */

	gfx_map_mask_t pic_visible_map; /**< The number of the map to display in update commands */
	int pic_animate; /**< The animation used by Animate() to display the picture */

	GfxList *dyn_views; /**< Pointers to pic and dynamic view lists */
	GfxList *drop_views; /**< A list Animate() can dump dropped dynviews into */

	int animation_granularity; /**< Number of animation steps to perform betwen updates for transition animations */

	Menubar *_menubar; /**< The menu bar */

	int priority_first; /**< The line where priority zone 0 ends */
	int priority_last; /**< The line where the highest priority zone starts */

	Common::Array<drawn_pic_t> _pics;

	uint32 game_start_time; /**< The time at which the interpreter was started */
	uint32 last_wait_time; /**< The last time the game invoked Wait() */

	unsigned int kernel_opt_flags; /**< Kernel optimization flags- used for performance tweaking */

	/* Kernel File IO stuff */

	Common::Array<FileHandle> _fileHandles; /**< Array of file handles. Dynamically increased if required. */

	DirSeeker _dirseeker;

	/* VM Information */

	Common::List<ExecStack> _executionStack; /**< The execution stack */
	/**
	 * When called from kernel functions, the vm is re-started recursively on
	 * the same stack. This variable contains the stack base for the current vm.
	 */
	int execution_stack_base;
	bool _executionStackPosChanged;   /**< Set to true if the execution stack position should be re-evaluated by the vm */

	reg_t r_acc; /**< Accumulator */
	int16 restAdjust; /**< &rest register (only used for save games) */
	reg_t r_prev; /**< previous comparison result */

	SegmentId stack_segment; /**< Heap area for the stack to use */
	StackPtr stack_base; /**< Pointer to the least stack element */
	StackPtr stack_top; /**< First invalid stack element */

	reg_t parser_base; /**< Base address for the parser error reporting mechanism */
	reg_t parser_event; /**< The event passed to Parse() and later used by Said() */
	Script *script_000;  /**< script 000, e.g. for globals */

	uint16 currentRoomNumber() const;

	/**
	 * Processes a multilanguage string based on the current language settings and
	 * returns a string that is ready to be displayed.
	 * @param str		the multilanguage string
	 * @param sep		optional seperator between main language and subtitle language,
	 *					if NULL is passed no subtitle will be added to the returned string
	 * @return processed string
	 */
	Common::String strSplit(const char *str, const char *sep = "\r----------\r");

	/* Debugger data: */
	Breakpoint *bp_list;   /**< List of breakpoints */
	int have_bp;  /**< Bit mask specifying which types of breakpoints are used in bp_list */

	/* System strings */
	SegmentId sys_strings_segment;
	SystemStrings *sys_strings;

	SegmentId string_frag_segment;

	/* Parser data: */
	parse_tree_node_t parser_nodes[VOCAB_TREE_NODES]; /**< The parse tree */

	int parser_valid; /**< If something has been correctly parsed */

	SynonymList _synonyms; /**< The list of synonyms */

	reg_t game_obj; /**< Pointer to the game object */

	Common::Array<Class> _classtable; /**< Table of all classes */

	SegManager *seg_manager;
	int gc_countdown; /**< Number of kernel calls until next gc */

	MessageState _msgState;

	EngineState *successor; /**< Successor of this state: Used for restoring */

private:
	kLanguage charToLanguage(const char c) const;
	Common::String getLanguageString(const char *str, kLanguage lang) const;
};

/**
 * Retrieves the gfx_pixmap_color_t associated with a game color index.
 * @param s			game state
 * @param color		color to look up
 * @return the requested color
 */
PaletteEntry get_pic_color(EngineState *s, int color);

// FIXME: Document this strange function.
// It seems to negate the given register but only if the "cantBeHere" exists.
// My guess: Since some SCI versions have cantBeHere and some have canBeHere,
// this function allows unifying the code, making it look identical for both
// kinds of SCI games. That's fine, but the name not_register is rather
// misleading. A different name (and a different place for declaring this)
// would be highly welcome.
static inline reg_t not_register(EngineState *s, reg_t r) {
	if (((SciEngine*)g_engine)->getKernel()->_selectorMap.cantBeHere != -1)
		return make_reg(0, !r.offset);
	else
		return r;
}

} // End of namespace Sci

#endif // SCI_INCLUDE_ENGINE_H
