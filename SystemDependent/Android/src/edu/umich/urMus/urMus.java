package edu.umich.urMus;

import java.io.InputStream;
import java.io.IOException;
import java.io.File;
import java.io.FileOutputStream;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.os.AsyncTask;
import android.util.Log;
import android.widget.TextView;


public class urMus extends Activity
{
	public static final String TAG="urMus";
	protected urMusView mView;
	protected TextView textView;
	protected InitTask initTask;
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		textView = (TextView) findViewById( R.id.text_view );
		
		initTask=new InitTask();
		initTask.execute( this );

		mView = new urMusView(getApplication());
		mView.setFocusableInTouchMode(true);
    }

    @Override protected void onPause() {
        super.onPause();
		Log.i(TAG,"paused");
		if(mView!=null) {
			mView.onPause();
		}
		finish();
    }
	
    @Override protected void onResume() {
        super.onResume();
		Log.i(TAG,"resumed");
		if(mView!=null) {
			mView.onResume();
		}
    }
	
	@Override protected void onDestroy() {
		System.exit(0);
	}
	
	public static native void init(int width, int height);
	public static native void step();
	public static native void changeBackground();
	public static native void startServer(String path);
	public static native void setupAPI();
	
	public class InitTask extends AsyncTask<Context, String, String>
	{
		@Override
		protected String doInBackground( Context... params ) {
			publishProgress("Installing Files");
			install();
			
			publishProgress("Starting HTTP Server");
			startServer(getFilesDir().getAbsolutePath());
			
			publishProgress("Loading urMus.lua");
			
			return "Complete!";
		}
		
		@Override
		protected void onPreExecute() {
			super.onPreExecute();
		}
		
		@Override
		protected void onProgressUpdate(String... values) {
			if(values.length==0) return;
			if(values.length==1) {
				textView.setText(values[0]+"\n");
			} else {
				textView.setText(values[0]+"\n"+values[1]);
			}
		}
		
		@Override
		protected void onCancelled() {
		}
		
		@Override
		protected void onPostExecute(String result) {
			setContentView(mView);
		}
		
		public void install() {
			// get assets manager
			AssetManager assets = getAssets();
			
			// storage path : /data/data/edu.umich.urMus/files
			String storagePath=getFilesDir().getAbsolutePath();
			Log.i(TAG,"Storage Path : " + storagePath);
			
			// source directories in assets directory
			String sources[][]={
				{"html","html"},
				{"html/js","html/js"},
				{"html/js/jplayer","html/js/jplayer"},
				{"html/css","html/css"},
				{"Sounds","."},
				{"Textures","."},
				{"urMus-Lua","."} };
			for(String [] source : sources) {
				Log.i(TAG,"traversing "+source[0]);
				
				String[] files = null;
				try {
					files = assets.list(source[0]);
				} catch (IOException e) {
					Log.e(TAG, "error while traversing " + source + " : " +e.getMessage());
					continue;
				}
				File destDir=new File(storagePath+"/"+source[1]);
				destDir.mkdirs();
				byte buf[]=new byte[512];
				if(files!=null) {
					for(String file : files) {
						String srcPath=source[0]+"/"+file;
						String destPath=storagePath+"/"+source[1]+"/"+file;
						Log.i(TAG,"opening : "+srcPath);
						publishProgress("Installing Files",srcPath);
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
	}
	
    static {
        System.loadLibrary("urMus");
    }
}
