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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/backends/platform/sdl/main.cpp $
 * $Id: main.cpp 43638 2009-08-22 12:56:08Z sev $
 *
 */
#if defined(WIN32) && !defined(__SYMBIAN32__)
#include <windows.h>
// winnt.h defines ARRAYSIZE, but we want our own one...
#undef ARRAYSIZE
#endif

#include "backends/platform/sdl/sdl.h"
#include "backends/plugins/sdl/sdl-provider.h"
#include "base/main.h"

#if defined(ANDROID)

static const char *CLASS_PATH = "ScumVM";

#include <android/log.h>
#include <jni.h>

#endif

#if defined(__SYMBIAN32__)
#include "SymbianOs.h"
#endif

#if !defined(__MAEMO__) && !defined(_WIN32_WCE) && !defined(GP2XWIZ)&& !defined(LINUXMOTO)

#if defined (WIN32)
int __stdcall WinMain(HINSTANCE /*hInst*/, HINSTANCE /*hPrevInst*/,  LPSTR /*lpCmdLine*/, int /*iShowCmd*/) {
	SDL_SetModuleHandle(GetModuleHandle(NULL));
	return main(__argc, __argv);
}
#endif

int main(int argc, char *argv[]) {

#if defined(__SYMBIAN32__)
	//
	// Set up redirects for stdout/stderr under Windows and Symbian.
	// Code copied from SDL_main.
	//

	// Symbian does not like any output to the console through any *print* function
	char STDOUT_FILE[256], STDERR_FILE[256]; // shhh, don't tell anybody :)
	strcpy(STDOUT_FILE, Symbian::GetExecutablePath());
	strcpy(STDERR_FILE, Symbian::GetExecutablePath());
	strcat(STDOUT_FILE, "scummvm.stdout.txt");
	strcat(STDERR_FILE, "scummvm.stderr.txt");

	/* Flush the output in case anything is queued */
	fclose(stdout);
	fclose(stderr);

	/* Redirect standard input and standard output */
	FILE *newfp = freopen(STDOUT_FILE, "w", stdout);
	if (newfp == NULL) {	/* This happens on NT */
#if !defined(stdout)
		stdout = fopen(STDOUT_FILE, "w");
#else
		newfp = fopen(STDOUT_FILE, "w");
		if (newfp) {
			*stdout = *newfp;
		}
#endif
	}
	newfp = freopen(STDERR_FILE, "w", stderr);
	if (newfp == NULL) {	/* This happens on NT */
#if !defined(stderr)
		stderr = fopen(STDERR_FILE, "w");
#else
		newfp = fopen(STDERR_FILE, "w");
		if (newfp) {
			*stderr = *newfp;
		}
#endif
	}
	setbuf(stderr, NULL);			/* No buffering */

#endif // defined(__SYMBIAN32__)

	// Create our OSystem instance
#if defined(__SYMBIAN32__)
	g_system = new OSystem_SDL_Symbian();
#else
	g_system = new OSystem_SDL();
#endif
	assert(g_system);

#ifdef DYNAMIC_MODULES
	PluginManager::instance().addPluginProvider(new SDLPluginProvider());
#endif

	// Invoke the actual ScummVM main entry point:
	int res = scummvm_main(argc, argv);
	g_system->quit();	// TODO: Consider removing / replacing this!
	return res;
}

#endif


#if defined(ANDROID)

int _main(JNIEnv *env, jobject thiz)
{
	/*int argc = 2;
	char **argv = (char **) malloc(sizeof(char *) * 2);
	argv[0] = "-g";
	argv[1] = "1x";*/
	return main(0, NULL);
}

/*
 * JNI registration.
 */
static JNINativeMethod methods[] = {
	{ "main", "()I", (void*) _main }
};

// ----------------------------------------------------------------------------

/*
 * Register several native methods for one class.
 */
static int registerNativeMethods(JNIEnv* env, jclass clazz,
								 JNINativeMethod* gMethods, int numMethods)
{
    if (clazz == NULL) {
        //LOGE("Native registration unable to find class '%s'", className);
        return JNI_FALSE;
    }
	
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        //LOGE("RegisterNatives failed for '%s'", className);
        return JNI_FALSE;
    }
	
    return JNI_TRUE;
}

// ----------------------------------------------------------------------------

/*
 * This is called by the VM when the shared library is first loaded.
 */
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM* vm, void* reserved)
{
    jint result = JNI_ERR;
    JNIEnv* env = NULL;
	
	jclass cls;
	
    __android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "loading library");
	
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
    	__android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "Bad version");
        goto bail;
    }
	
    cls = env->FindClass("cz/havlena/scumvm/ScumVMActivity");
	if(cls == NULL)
	{
		__android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "can't load ScumVM activity class");
		goto bail;
	}
	
    if (env->RegisterNatives(cls, methods, sizeof(methods) / sizeof(methods[0])) < 0)
    {
       	__android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "can't register methods");
        goto bail;
    }
	
	__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "library loaded");
    result = JNI_VERSION_1_4;
	
bail:
    return result;
}

#endif //end of ANDROID
