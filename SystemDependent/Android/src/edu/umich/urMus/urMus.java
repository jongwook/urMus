package edu.umich.urMus;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;
import android.util.Log;
import android.content.res.AssetManager;

import java.io.IOException;

public class urMus extends Activity
{
	public static final String TAG="urMus";
	urMusView mView;
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
		mView = new urMusView(getApplication());
		mView.setFocusableInTouchMode(true);
		setContentView(mView);
		install();
    }

    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
    }
	
    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
    }
	
	public static native void init(int width, int height);
	public static native void step();
	public static native void changeBackground();

	public void install() {
		AssetManager assetManager = getAssets();
        String[] files = null;
        try {
            files = assetManager.list("html");
        } catch (IOException e) {
            Log.e(TAG, e.getMessage());
        }
		if(files!=null) {
			for(String file : files) {
				Log.i("opening : %s",file);
				
			}
		}
	}
	
    static {
        System.loadLibrary("urMus");
    }
}
