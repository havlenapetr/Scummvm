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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/common/archive.cpp $
 * $Id: archive.cpp 41090 2009-06-01 00:01:32Z wjpalenstijn $
 *
 */

#include "common/archive.h"
#include "common/fs.h"
#include "common/util.h"
#include "common/system.h"

namespace Common {

GenericArchiveMember::GenericArchiveMember(String name, Archive *parent)
	: _parent(parent), _name(name) {
}

String GenericArchiveMember::getName() const {
	return _name;
}

SeekableReadStream *GenericArchiveMember::createReadStream() const {
	return _parent->createReadStreamForMember(_name);
}


int Archive::listMatchingMembers(ArchiveMemberList &list, const String &pattern) {
	// Get all "names" (TODO: "files" ?)
	ArchiveMemberList allNames;
	listMembers(allNames);

	int matches = 0;

	// need to match lowercase key
	String lowercasePattern = pattern;
	lowercasePattern.toLowercase();

	ArchiveMemberList::iterator it = allNames.begin();
	for ( ; it != allNames.end(); ++it) {
		if ((*it)->getName().matchString(lowercasePattern, true)) {
			list.push_back(*it);
			matches++;
		}
	}

	return matches;
}



SearchSet::ArchiveNodeList::iterator SearchSet::find(const String &name) {
	ArchiveNodeList::iterator it = _list.begin();
	for ( ; it != _list.end(); ++it) {
		if (it->_name == name)
			break;
	}
	return it;
}

SearchSet::ArchiveNodeList::const_iterator SearchSet::find(const String &name) const {
	ArchiveNodeList::const_iterator it = _list.begin();
	for ( ; it != _list.end(); ++it) {
		if (it->_name == name)
			break;
	}
	return it;
}

/*
	Keep the nodes sorted according to descending priorities.
	In case two or node nodes have the same priority, insertion
	order prevails.
*/
void SearchSet::insert(const Node &node) {
	ArchiveNodeList::iterator it = _list.begin();
	for ( ; it != _list.end(); ++it) {
		if (it->_priority < node._priority)
			break;
	}
	_list.insert(it, node);
}

void SearchSet::add(const String &name, Archive *archive, int priority, bool autoFree) {
	if (find(name) == _list.end()) {
		Node node(priority, name, archive, autoFree);
		insert(node);
	} else {
		if (autoFree)
			delete archive;
		warning("SearchSet::add: archive '%s' already present", name.c_str());
	}

}

void SearchSet::addDirectory(const String &name, const String &directory, int priority, int depth, bool flat) {
	FSNode dir(directory);
	addDirectory(name, dir, priority, depth, flat);
}

void SearchSet::addDirectory(const String &name, const FSNode &dir, int priority, int depth, bool flat) {
	if (!dir.exists() || !dir.isDirectory())
		return;

	add(name, new FSDirectory(dir, depth, flat), priority);
}


void SearchSet::remove(const String &name) {
	ArchiveNodeList::iterator it = find(name);
	if (it != _list.end()) {
		if (it->_autoFree)
			delete it->_arc;
		_list.erase(it);
	}
}

bool SearchSet::hasArchive(const String &name) const {
	return (find(name) != _list.end());
}

void SearchSet::clear() {
	for (ArchiveNodeList::iterator i = _list.begin(); i != _list.end(); ++i) {
		if (i->_autoFree)
			delete i->_arc;
	}

	_list.clear();
}

void SearchSet::setPriority(const String &name, int priority) {
	ArchiveNodeList::iterator it = find(name);
	if (it == _list.end()) {
		warning("SearchSet::setPriority: archive '%s' is not present", name.c_str());
		return;
	}

	if (priority == it->_priority)
		return;

	Node node(*it);
	_list.erase(it);
	node._priority = priority;
	insert(node);
}

bool SearchSet::hasFile(const String &name) {
	if (name.empty())
		return false;

	ArchiveNodeList::iterator it = _list.begin();
	for ( ; it != _list.end(); ++it) {
		if (it->_arc->hasFile(name))
			return true;
	}

	return false;
}

int SearchSet::listMatchingMembers(ArchiveMemberList &list, const String &pattern) {
	int matches = 0;

	ArchiveNodeList::iterator it = _list.begin();
	for ( ; it != _list.end(); ++it)
		matches += it->_arc->listMatchingMembers(list, pattern);

	return matches;
}

int SearchSet::listMembers(ArchiveMemberList &list) {
	int matches = 0;

	ArchiveNodeList::iterator it = _list.begin();
	for ( ; it != _list.end(); ++it)
		matches += it->_arc->listMembers(list);

	return matches;
}

ArchiveMemberPtr SearchSet::getMember(const String &name) {
	if (name.empty())
		return ArchiveMemberPtr();

	ArchiveNodeList::iterator it = _list.begin();
	for ( ; it != _list.end(); ++it) {
		if (it->_arc->hasFile(name))
			return it->_arc->getMember(name);
	}

	return ArchiveMemberPtr();
}

SeekableReadStream *SearchSet::createReadStreamForMember(const String &name) const {
	if (name.empty())
		return 0;

	ArchiveNodeList::const_iterator it = _list.begin();
	for ( ; it != _list.end(); ++it) {
		SeekableReadStream *stream = it->_arc->createReadStreamForMember(name);
		if (stream)
			return stream;
	}

	return 0;
}


DECLARE_SINGLETON(SearchManager);

SearchManager::SearchManager() {
	clear();	// Force a reset
}

void SearchManager::clear() {
	SearchSet::clear();

	// Always keep system specific archives in the SearchManager.
	// But we give them a lower priority than the default priority (which is 0),
	// so that archives added by client code are searched first.
	if (g_system)
		g_system->addSysArchivesToSearchSet(*this, -1);

	// Add the current dir as a very last resort.
	// See also bug #2137680.
	addDirectory(".", ".", -2);
}

} // namespace Common
