package edu.umich.urMus;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;
import android.util.Log;
import android.content.res.AssetManager;
import android.content.res.AssetFileDescriptor;

import java.io.InputStream;
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
		AssetManager assets = getAssets();
		String storagePath=getFilesDir().getAbsolutePath();
		Log.i(TAG,"Storage Path : " + storagePath);
		// source directories in assets directory
		String sources[]={"html","html/js","html/css","Sounds","Textures","urMus-Lua"};
		for(String source : sources) {
			Log.i(TAG,"traversing "+source);
			String[] files = null;
			try {
				files = assets.list(source);
			} catch (IOException e) {
				Log.e(TAG, e.getMessage());
			}
			if(files!=null) {
				for(String file : files) {
					String filepath=source+"/"+file;
					Log.i(TAG,"opening : "+filepath);
					try {
						InputStream input=assets.open(filepath,AssetManager.ACCESS_STREAMING);
						int cnt=0, data;
						while((data=input.read())!=-1) cnt++;
						Log.i(TAG, "File size : "+cnt);
					} catch (IOException e) {
						Log.e(TAG, e.getMessage());
					}
				}
			}
		}
	}
	
    static {
        System.loadLibrary("urMus");
    }
}
