LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_CFLAGS := -DHAVE_CONFIG_H -DUNIX -DANDROID -DDYNAMIC_MODULES \
                -DSDL_BACKEND -DENABLE_SCUMM=STATIC_PLUGIN -DENABLE_SCUMM_7_8 \
		-DENABLE_SWORD1=STATIC_PLUGIN -DENABLE_KYRA=DYNAMIC_PLUGIN

BASE := base/*.cpp 

COMMON := common/*.cpp

GUI := gui/*.cpp

GRAPHICS := \
	graphics/*.cpp \
	graphics/fonts/*.cpp \
	graphics/scaler/*.cpp \
	graphics/video/*.cpp \
	graphics/video/coktelvideo/*.cpp

BACKENDS := \
	backends/*.cpp \
	backends/events/default/*.cpp \
	backends/fs/*.cpp \
	backends/fs/posix/posix-fs-factory.cpp \
	backends/fs/windows/windows-fs-factory.cpp \
	backends/keymapper/*.cpp \
	backends/midi/*.cpp \
	backends/platform/sdl/*.cpp \
	backends/plugins/sdl/*.cpp \
	backends/plugins/posix/*.cpp \
	backends/plugins/dc/*.cpp \
	backends/plugins/win32/*.cpp \
	backends/saves/*.cpp \
	backends/saves/default/*.cpp \
	backends/saves/posix/*.cpp \
	backends/saves/psp/*.cpp \
	backends/timer/default/*.cpp \
	backends/vkeybd/*.cpp

ENGINES := \
	engines/*.cpp \
	engines/kyra/*.cpp \
	engines/sword1/*.cpp \
	engines/scumm/*.cpp \
	engines/scumm/he/*.cpp \
	engines/scumm/imuse/*.cpp \
	engines/scumm/imuse_digi/*.cpp \
	engines/scumm/insane/*.cpp \
	engines/scumm/smush/*.cpp

SOUND := \
	sound/vorbis.cpp \
	sound/aiff.cpp \
	sound/vag.cpp \
	sound/adpcm.cpp \
	sound/wave.cpp \
	sound/voc.cpp \
	sound/audiocd.cpp \
	sound/softsynth/*.cpp \
	sound/softsynth/opl/*.cpp \
	sound/softsynth/mt32/*.cpp \
	sound/fmopl.cpp \
	sound/audiostream.cpp \
	sound/mididrv.cpp \
	sound/midiparser.cpp \
	sound/midiparser_xmidi.cpp \
	sound/midiparser_smf.cpp \
	sound/rate.cpp \
	sound/mixer.cpp \
	sound/mpu401.cpp \
	sound/musicplugin.cpp \
	sound/null.cpp

SRCS := $(BASE) $(COMMON) $(GUI) $(GRAPHICS) $(BACKENDS) $(ENGINES) $(SOUND)

# Note this simple makefile var substitution, you can find even simpler examples in different Android projects
LOCAL_SRC_FILES = $(foreach F, $(SRCS), $(addprefix $(dir $(F)),$(notdir $(wildcard $(LOCAL_PATH)/$(F)))))

#include sdl includes
LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/../sdl/include

LOCAL_MODULE := libscummvm

LOCAL_C_INCLUDES  += \
	$(LOCAL_PATH)/engines

LOCAL_LDLIBS := -ldl -llog -lz

LOCAL_SHARED_LIBRARIES := libsdl

include $(BUILD_SHARED_LIBRARY)
