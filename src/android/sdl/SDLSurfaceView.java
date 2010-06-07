package android.sdl;

import android.content.Context;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.util.Log;

public class SDLSurfaceView extends SurfaceView implements SurfaceHolder.Callback {

	private static final String TAG = "SDLSurfaceView";

	private SurfaceHolder mSurfaceHolder;
	private LibraryLoader mLibraryLoader;

	public SDLSurfaceView(Context context, String libPath) {
		super(context);
		init(libPath);
	}

	private void init(String libPath)
	{
		mLibraryLoader = new LibraryLoader(libPath);
		mSurfaceHolder = getHolder();
		mSurfaceHolder.addCallback(this);
	}

	public void surfaceChanged(SurfaceHolder holder, final int format, final int width, final int height) {}

	public void surfaceCreated(SurfaceHolder holder) {
		nativeInit(mSurfaceHolder.getSurface());
		mLibraryLoader.loadLibrary();
		mLibraryLoader.callMainAsync();
	}

	public void surfaceDestroyed(SurfaceHolder holder) {
		nativeRelease();
		mLibraryLoader.closeLibrary();
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		processEvent(event);
		return true;
	}

	public void processEvent(KeyEvent event) {
		nativeOnKeyEvent(event.getKeyCode(), event.getAction());
	}

	public void processEvent(MotionEvent event) {
		nativeOnMouseEvent((int)event.getX(), (int)event.getY(), event.getAction());
	}
	
	private native void nativeInit(Surface surface);
	private native void nativeRelease();
	private native void nativeOnKeyEvent(int key, int action);
    private native void nativeOnMouseEvent(int x, int y, int action);
	
}
