# $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/tools/create_kyradat/module.mk $
# $Id: module.mk 30962 2008-02-25 14:10:17Z fingolfin $

MODULE := tools/create_kyradat

MODULE_OBJS := \
	create_kyradat.o \
	pak.o \
	md5.o \
	util.o

# Set the name of the executable
TOOL_EXECUTABLE := create_kyradat

# Include common rules
include $(srcdir)/rules.mk
