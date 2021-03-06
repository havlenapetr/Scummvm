# -------- Generated by configure -----------

CXX := g++
CXXFLAGS :=   -g -ansi -W -Wno-unused-parameter -Wno-empty-body -pedantic
LIBS +=   -lasound  -lz -L/usr/local/lib -Wl,-rpath,/usr/local/lib -lSDL -lpthread
RANLIB := ranlib
STRIP := strip
AR := ar cru
AS := as
ASFLAGS := 
WINDRES := windres
WIN32PATH=C:/scummvm
AOS4PATH=Games:ScummVM
STATICLIBPATH=/sw

BACKEND := sdl
MODULES +=  backends/platform/sdl
MODULE_DIRS += 
EXEEXT := 
NASM := 
NASMFLAGS := 

PREFIX := /usr/local
BINDIR := /usr/local/bin
MANDIR := /usr/local/share/man
DATADIR := /usr/local/share
LIBDIR := /usr/local/lib


USE_MT32EMU = 1
# DISABLE_HQ_SCALERS = 1
# DISABLE_SCALERS = 1
USE_INDEO3 = 1
# USE_VORBIS = 1
# USE_TREMOR = 1
# USE_FLAC = 1
# USE_MAD = 1
USE_ZLIB = 1
# USE_MPEG2 = 1
# HAVE_NASM = 1
HAVE_GCC3 = 1
ENABLE_SCUMM = STATIC_PLUGIN
ENABLE_SCUMM_7_8 = 1
ENABLE_HE = 1
ENABLE_AGI = STATIC_PLUGIN
ENABLE_AGOS = STATIC_PLUGIN
# ENABLE_PN
ENABLE_AGOS2 = 1
ENABLE_CINE = STATIC_PLUGIN
ENABLE_CRUISE = STATIC_PLUGIN
ENABLE_DRASCULA = STATIC_PLUGIN
ENABLE_GOB = STATIC_PLUGIN
ENABLE_GROOVIE = STATIC_PLUGIN
# ENABLE_IGOR
ENABLE_KYRA = STATIC_PLUGIN
# ENABLE_LOL
ENABLE_LURE = STATIC_PLUGIN
# ENABLE_M4
ENABLE_MADE = STATIC_PLUGIN
ENABLE_PARALLACTION = STATIC_PLUGIN
ENABLE_QUEEN = STATIC_PLUGIN
ENABLE_SAGA = STATIC_PLUGIN
ENABLE_IHNM = 1
# ENABLE_SAGA2
# ENABLE_SCI
# ENABLE_SCI32
ENABLE_SKY = STATIC_PLUGIN
ENABLE_SWORD1 = STATIC_PLUGIN
ENABLE_SWORD2 = STATIC_PLUGIN
ENABLE_TINSEL = STATIC_PLUGIN
ENABLE_TOUCHE = STATIC_PLUGIN
ENABLE_TUCKER = STATIC_PLUGIN

INCLUDES +=    -I/usr/local/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT
OBJS += 
DEFINES +=  -DUNIX -DDATA_PATH=\"/usr/local/share/scummvm\" -DPLUGIN_DIRECTORY=\"/usr/local/lib/scummvm\" -DSDL_BACKEND
LDFLAGS +=  -lm



SAVED_CONFIGFLAGS := 
SAVED_LDFLAGS     := 
SAVED_CXX         := 
SAVED_CXXFLAGS    := 
SAVED_CPPFLAGS    := 
SAVED_ASFLAGS     := 
