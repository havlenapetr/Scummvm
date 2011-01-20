package cz.havlena.scumvm;

import android.os.Bundle;
import android.sdl.app.SDLActivity;

public class ScumVMActivity extends SDLActivity {
	
	private static final String TAG = "ScummVMActivity";

	@Override
	protected void onSDLCreate(Bundle bundle) {
		main();
	}
	
	static {
		System.loadLibrary("scummvm");
	}
	
	private native int main();
    
}