package edu.umich.urMus;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;
import android.util.Log;
import android.content.res.AssetManager;

import java.io.InputStream;
import java.io.IOException;
import java.io.File;
import java.io.FileOutputStream;

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
		// get assets manager
		AssetManager assets = getAssets();
		
		// storage path : /data/data/edu.umich.urMus/files
		String storagePath=getFilesDir().getAbsolutePath();
		Log.i(TAG,"Storage Path : " + storagePath);
		
		// source directories in assets directory
		String sources[]={"html","html/js","html/js/jplayer","html/css","Sounds","Textures","urMus-Lua"};
		for(String source : sources) {
			Log.i(TAG,"traversing "+source);
			
			String[] files = null;
			try {
				files = assets.list(source);
			} catch (IOException e) {
				Log.e(TAG, "error while traversing " + source + " : " +e.getMessage());
				continue;
			}
			File destDir=new File(storagePath+"/"+source);
			destDir.mkdirs();
			byte buf[]=new byte[512];
			if(files!=null) {
				for(String file : files) {
					String srcPath=source+"/"+file;
					String destPath=storagePath+"/"+srcPath;
					Log.i(TAG,"opening : "+srcPath);
					try {
						File dest=new File(destPath);
						if(dest.isFile()) {
							Log.i(TAG,srcPath+" already exists, skipping");
							continue;
						}
						InputStream input=assets.open(srcPath,AssetManager.ACCESS_STREAMING);
						FileOutputStream output=new FileOutputStream(dest);
						int total=0;
						while(true) {
							int len=input.read(buf);
							if(len==-1) break;
							output.write(buf,0,len);
							total+=len;
						}
						Log.i(TAG, total + "bytes written");
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
