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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/common/events.h $
 * $Id: events.h 42403 2009-07-12 05:35:56Z sunmax $
 *
 */

#ifndef COMMON_EVENTS_H
#define COMMON_EVENTS_H

#include "common/keyboard.h"
#include "common/queue.h"
#include "common/rect.h"
#include "common/noncopyable.h"

namespace Common {

/**
 * The types of events backends may generate.
 * @see Event
 *
 * @todo Merge EVENT_LBUTTONDOWN, EVENT_RBUTTONDOWN and EVENT_WHEELDOWN;
 *       likewise EVENT_LBUTTONUP, EVENT_RBUTTONUP, EVENT_WHEELUP.
 *       To do that, we just have to add a field to the Event which
 *       indicates which button was pressed.
 */
enum EventType {
	EVENT_INVALID = 0,
	/** A key was pressed, details in Event::kbd. */
	EVENT_KEYDOWN = 1,
	/** A key was released, details in Event::kbd. */
	EVENT_KEYUP = 2,
	/** The mouse moved, details in Event::mouse. */
	EVENT_MOUSEMOVE = 3,
	EVENT_LBUTTONDOWN = 4,
	EVENT_LBUTTONUP = 5,
	EVENT_RBUTTONDOWN = 6,
	EVENT_RBUTTONUP = 7,
	EVENT_WHEELUP = 8,
	EVENT_WHEELDOWN = 9,
	EVENT_MBUTTONDOWN = 13,
	EVENT_MBUTTONUP = 14,

	EVENT_MAINMENU = 15,
	EVENT_RTL = 16,
	EVENT_MUTE = 17,

	EVENT_QUIT = 10,
	EVENT_SCREEN_CHANGED = 11,
	/**
	 * The backend requests the agi engine's predictive dialog to be shown.
	 * TODO: Fingolfin suggests that it would be of better value to expand
	 * on this notion by generalizing its use. For example the backend could
	 * use events to ask for the save game dialog or to pause the engine.
	 * An associated enumerated type can accomplish this.
	 **/
	EVENT_PREDICTIVE_DIALOG = 12
};

/**
 * Data structure for an event. A pointer to an instance of Event
 * can be passed to pollEvent.
 * @todo Rework/document this structure. It should be made 100% clear which
 *       field is valid for which event type.
 *       Implementation wise, we might want to use the classic
 *       union-of-structs trick. It goes roughly like this:
 *       struct BasicEvent {
 *          EventType type;
 *       };
 *       struct MouseMovedEvent : BasicEvent {
 *          Common::Point pos;
 *       };
 *       struct MouseButtonEvent : MouseMovedEvent {
 *          int button;
 *       };
 *       struct KeyEvent : BasicEvent {
 *          ...
 *       };
 *       ...
 *       union Event {
 *          EventType type;
 *          MouseMovedEvent mouse;
 *          MouseButtonEvent button;
 *          KeyEvent key;
 *          ...
 *       };
 */
struct Event {
	/** The type of the event. */
	EventType type;
	/** Flag to indicate if the event is real or synthetic. E.g. keyboard
	  * repeat events are synthetic.
	  */
	bool synthetic;
	/**
	  * Keyboard data; only valid for keyboard events (EVENT_KEYDOWN and
	  * EVENT_KEYUP). For all other event types, content is undefined.
	  */
	KeyState kbd;
	/**
	 * The mouse coordinates, in virtual screen coordinates. Only valid
	 * for mouse events.
	 * Virtual screen coordinates means: the coordinate system of the
	 * screen area as defined by the most recent call to initSize().
	 */
	Common::Point mouse;

	Event() : type(EVENT_INVALID), synthetic(false) {}
};

class Keymapper;

/**
 * The EventManager provides user input events to the client code.
 * In addition, it keeps track of the state of various input devices,
 * like keys, mouse position and buttons.
 */
class EventManager : NonCopyable {
public:
	EventManager() {}
	virtual ~EventManager() {}

	enum {
		LBUTTON = 1 << 0,
		RBUTTON = 1 << 1
	};


	/**
	 * Initialise the event manager.
	 * @note	called after graphics system has been set up
	 */
	virtual void init() {}
	/**
	 * Get the next event in the event queue.
	 * @param event	point to an Event struct, which will be filled with the event data.
	 * @return true if an event was retrieved.
	 */
	virtual bool pollEvent(Common::Event &event) = 0;

	/**
	 * Pushes a "fake" event into the event queue
	 */
	virtual void pushEvent(const Common::Event &event) = 0;

	/** Register random source so it can be serialized in game test purposes **/
	virtual void registerRandomSource(Common::RandomSource &rnd, const char *name) = 0;

	virtual void processMillis(uint32 &millis) = 0;

	/** Return the current mouse position */
	virtual Common::Point getMousePos() const = 0;

	/**
	 * Return a bitmask with the button states:
	 * - bit 0: left button up=1, down=0
	 * - bit 1: right button up=1, down=0
	 */
	virtual int getButtonState() const = 0;

	/** Get a bitmask with the current modifier state */
	virtual int getModifierState() const = 0;

	/**
	 * Should the application terminate? Set to true if we
	 * received an EVENT_QUIT.
	 */
	virtual int shouldQuit() const = 0;

	/**
	 * Should we return to the launcher?
	 */
	virtual int shouldRTL() const = 0;

	/**
	 * Reset the "return to launcher" flag (as returned shouldRTL()) to false.
	 * Used when we have returned to the launcher.
	 */
	virtual void resetRTL() = 0;
#ifdef FORCE_RTL
	virtual void resetQuit() = 0;
#endif
	// Optional: check whether a given key is currently pressed ????
	//virtual bool isKeyPressed(int keycode) = 0;

	// TODO: Keyboard repeat support?

	// TODO: Consider removing OSystem::getScreenChangeID and
	// replacing it by a generic getScreenChangeID method here
#ifdef ENABLE_KEYMAPPER
	virtual Common::Keymapper *getKeymapper() = 0;
#endif

protected:

	Common::Queue<Common::Event> artificialEventQueue;
};

} // End of namespace Common

#endif
