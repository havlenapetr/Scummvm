package cz.havlena.scumvm;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.os.PowerManager;
import android.sdl.SDLSurfaceView;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

public class ScumVMActivity extends Activity {
	private static final String TAG = "ScummVMActivity";
	
	private SDLSurfaceView mView;
	private PowerManager.WakeLock wakeLock = null;
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mView = new SDLSurfaceView(this, "/data/data/cz.havlena.scumvm/lib/libscummvm.so");
        setContentView(mView);
        
        PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        wakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, TAG);
    }
    
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
    	mView.processEvent(event);
    	return true;
    }
    
    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
    	mView.processEvent(event);
    	return true;
    }
    
    @Override
    protected void onPause() {
        //-- we will enable screen timeout, while scumm is paused
        if( wakeLock != null ) {
        	Log.d(TAG, "Pausing so releasing wakeLock");
            wakeLock.release();
        }
        super.onPause();
    }

    @Override
    protected void onResume() {
        //-- we will disable screen timeout, while scumm is running
        if( wakeLock != null ) {
        	Log.d(TAG, "Resuming so acquiring wakeLock");
            wakeLock.acquire();
        }
        super.onResume();
    }

    @Override
    protected void onStop() {
        //-- we will enable screen timeout, while scumm is closed
        if( wakeLock != null) {
        	Log.d(TAG, "Stoping so releasing wakeLock");
            wakeLock.release();
        }

        super.onStop();
        finish();
    }
}