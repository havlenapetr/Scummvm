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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/common/fs.cpp $
 * $Id: fs.cpp 41090 2009-06-01 00:01:32Z wjpalenstijn $
 */

#include "common/util.h"
#include "common/system.h"
#include "backends/fs/abstract-fs.h"
#include "backends/fs/fs-factory.h"

namespace Common {

FSNode::FSNode() {
}

FSNode::FSNode(AbstractFSNode *realNode)
	: _realNode(realNode) {
}

FSNode::FSNode(const Common::String &p) {
	assert(g_system);
	FilesystemFactory *factory = g_system->getFilesystemFactory();
	AbstractFSNode *tmp = 0;

	if (p.empty() || p == ".")
		tmp = factory->makeCurrentDirectoryFileNode();
	else
		tmp = factory->makeFileNodePath(p);
	_realNode = Common::SharedPtr<AbstractFSNode>(tmp);
}

bool FSNode::operator<(const FSNode& node) const {
	if (isDirectory() != node.isDirectory())
		return isDirectory();

	return getDisplayName().compareToIgnoreCase(node.getDisplayName()) < 0;
}

bool FSNode::exists() const {
	if (_realNode == 0)
		return false;

	return _realNode->exists();
}

FSNode FSNode::getChild(const Common::String &n) const {
	// If this node is invalid or not a directory, return an invalid node
	if (_realNode == 0 || !_realNode->isDirectory())
		return FSNode();

	AbstractFSNode *node = _realNode->getChild(n);
	return FSNode(node);
}

bool FSNode::getChildren(FSList &fslist, ListMode mode, bool hidden) const {
	if (!_realNode || !_realNode->isDirectory())
		return false;

	AbstractFSList tmp;

	if (!_realNode->getChildren(tmp, mode, hidden))
		return false;

	fslist.clear();
	for (AbstractFSList::iterator i = tmp.begin(); i != tmp.end(); ++i) {
		fslist.push_back(FSNode(*i));
	}

	return true;
}

Common::String FSNode::getDisplayName() const {
	assert(_realNode);
	return _realNode->getDisplayName();
}

Common::String FSNode::getName() const {
	assert(_realNode);
	return _realNode->getName();
}

FSNode FSNode::getParent() const {
	if (_realNode == 0)
		return *this;

	AbstractFSNode *node = _realNode->getParent();
	if (node == 0) {
		return *this;
	} else {
		return FSNode(node);
	}
}

Common::String FSNode::getPath() const {
	assert(_realNode);
	return _realNode->getPath();
}

bool FSNode::isDirectory() const {
	if (_realNode == 0)
		return false;

	return _realNode->isDirectory();
}

bool FSNode::isReadable() const {
	if (_realNode == 0)
		return false;

	return _realNode->isReadable();
}

bool FSNode::isWritable() const {
	if (_realNode == 0)
		return false;

	return _realNode->isWritable();
}

Common::SeekableReadStream *FSNode::createReadStream() const {
	if (_realNode == 0)
		return 0;

	if (!_realNode->exists()) {
		warning("FSNode::createReadStream: FSNode does not exist");
		return false;
	} else if (_realNode->isDirectory()) {
		warning("FSNode::createReadStream: FSNode is a directory");
		return false;
	}

	return _realNode->createReadStream();
}

Common::WriteStream *FSNode::createWriteStream() const {
	if (_realNode == 0)
		return 0;

	if (_realNode->isDirectory()) {
		warning("FSNode::createWriteStream: FSNode is a directory");
		return 0;
	}

	return _realNode->createWriteStream();
}

FSDirectory::FSDirectory(const FSNode &node, int depth, bool flat)
  : _node(node), _cached(false), _depth(depth), _flat(flat) {
}

FSDirectory::FSDirectory(const String &prefix, const FSNode &node, int depth, bool flat)
  : _node(node), _cached(false), _depth(depth), _flat(flat) {

	setPrefix(prefix);
}

FSDirectory::FSDirectory(const String &name, int depth, bool flat)
  : _node(name), _cached(false), _depth(depth), _flat(flat) {
}

FSDirectory::FSDirectory(const String &prefix, const String &name, int depth, bool flat)
  : _node(name), _cached(false), _depth(depth), _flat(flat) {

	setPrefix(prefix);
}

FSDirectory::~FSDirectory() {
}

void FSDirectory::setPrefix(const String &prefix) {
	_prefix = prefix;

	if (!_prefix.empty() && !_prefix.hasSuffix("/"))
		_prefix += "/";
}

FSNode FSDirectory::getFSNode() const {
	return _node;
}

FSNode *FSDirectory::lookupCache(NodeCache &cache, const String &name) const {
	// make caching as lazy as possible
	if (!name.empty()) {
		ensureCached();

		if (cache.contains(name))
			return &cache[name];
	}

	return 0;
}

bool FSDirectory::hasFile(const String &name) {
	if (name.empty() || !_node.isDirectory())
		return false;

	FSNode *node = lookupCache(_fileCache, name);
	return node && node->exists();
}

ArchiveMemberPtr FSDirectory::getMember(const String &name) {
	if (name.empty() || !_node.isDirectory())
		return ArchiveMemberPtr();

	FSNode *node = lookupCache(_fileCache, name);

	if (!node || !node->exists()) {
		warning("FSDirectory::getMember: FSNode does not exist");
		return ArchiveMemberPtr();
	} else if (node->isDirectory()) {
		warning("FSDirectory::getMember: FSNode is a directory");
		return ArchiveMemberPtr();
	}

	return ArchiveMemberPtr(new FSNode(*node));
}

SeekableReadStream *FSDirectory::createReadStreamForMember(const String &name) const {
	if (name.empty() || !_node.isDirectory())
		return 0;

	FSNode *node = lookupCache(_fileCache, name);
	if (!node)
		return 0;
	SeekableReadStream *stream = node->createReadStream();
	if (!stream)
		warning("FSDirectory::createReadStreamForMember: Can't create stream for file '%s'", name.c_str());

	return stream;
}

FSDirectory *FSDirectory::getSubDirectory(const String &name, int depth, bool flat) {
	return getSubDirectory(String::emptyString, name, depth, flat);
}

FSDirectory *FSDirectory::getSubDirectory(const String &prefix, const String &name, int depth, bool flat) {
	if (name.empty() || !_node.isDirectory())
		return 0;

	FSNode *node = lookupCache(_subDirCache, name);
	if (!node)
		return 0;

	return new FSDirectory(prefix, *node, depth, flat);
}

void FSDirectory::cacheDirectoryRecursive(FSNode node, int depth, const String& prefix) const {
	if (depth <= 0)
		return;

	FSList list;
	node.getChildren(list, FSNode::kListAll, false);

	FSList::iterator it = list.begin();
	for ( ; it != list.end(); ++it) {
		String name = prefix + it->getName();

		// don't touch name as it might be used for warning messages
		String lowercaseName = name;
		lowercaseName.toLowercase();

		// since the hashmap is case insensitive, we need to check for clashes when caching
		if (it->isDirectory()) {
			if (!_flat && _subDirCache.contains(lowercaseName)) {
				warning("FSDirectory::cacheDirectory: name clash when building cache, ignoring sub-directory '%s'", name.c_str());
			} else {
				if (_subDirCache.contains(lowercaseName)) {
					warning("FSDirectory::cacheDirectory: name clash when building subDirCache with subdirectory '%s'", name.c_str());
				}
				cacheDirectoryRecursive(*it, depth - 1, _flat ? prefix : lowercaseName + "/");
				_subDirCache[lowercaseName] = *it;
			}
		} else {
			if (_fileCache.contains(lowercaseName)) {
				warning("FSDirectory::cacheDirectory: name clash when building cache, ignoring file '%s'", name.c_str());
			} else {
				_fileCache[lowercaseName] = *it;
			}
		}
	}

}

void FSDirectory::ensureCached() const  {
	if (_cached)
		return;
	cacheDirectoryRecursive(_node, _depth, _prefix);
	_cached = true;
}

int FSDirectory::listMatchingMembers(ArchiveMemberList &list, const String &pattern) {
	if (!_node.isDirectory())
		return 0;

	// Cache dir data
	ensureCached();

	String lowercasePattern(pattern);
	lowercasePattern.toLowercase();

	int matches = 0;
	NodeCache::iterator it = _fileCache.begin();
	for ( ; it != _fileCache.end(); ++it) {
		if (it->_key.matchString(lowercasePattern, true)) {
			list.push_back(ArchiveMemberPtr(new FSNode(it->_value)));
			matches++;
		}
	}
	return matches;
}

int FSDirectory::listMembers(ArchiveMemberList &list) {
	if (!_node.isDirectory())
		return 0;

	// Cache dir data
	ensureCached();

	int files = 0;
	for (NodeCache::iterator it = _fileCache.begin(); it != _fileCache.end(); ++it) {
		list.push_back(ArchiveMemberPtr(new FSNode(it->_value)));
		++files;
	}

	return files;
}


}	// End of namespace Common
