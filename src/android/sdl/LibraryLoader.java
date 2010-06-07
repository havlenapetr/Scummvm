package android.sdl;

public class LibraryLoader {

	private static final String TAG = "LibraryLoader";
	private String mLibPath;

	public LibraryLoader(String libPath) {
		mLibPath = libPath;
	}

	public int loadLibrary() {
		return nativeLoadLibrary(mLibPath);
	}

	public int callMain() {
		return nativeCallMain();
	}

	public void callMainAsync() {
		new Thread()
		{
			public void run() {
				nativeCallMain();
			}
		}.start();
	}

	public void closeLibrary() {
		nativeCloseLibrary();
	}

	
	static {
		System.loadLibrary("sdl_jni");
	}

	private native int nativeLoadLibrary(String libPath);
	private native int nativeCallMain();
	private native void nativeCloseLibrary();
}
