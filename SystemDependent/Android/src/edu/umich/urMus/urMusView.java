package edu.umich.urMus;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import java.util.List;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

import android.media.AudioTrack;
import android.media.AudioRecord;	
import android.media.AudioFormat;
import android.media.MediaRecorder;

class urMusView extends GLSurfaceView {
	public static final String TAG="urMusView";
	
	private boolean urMusReady=false;
	
	private int SCREEN_WIDTH;
	private int SCREEN_HEIGHT;
	private int HALF_SCREEN_WIDTH;
	private int HALF_SCREEN_HEIGHT;
	
	private final int PCM_FREQUENCY=48000;
	
    urMusView(Context context) {
        super(context);
        init();
    }
	
    public urMusView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }
	
    private void init() {
        setRenderer(new Renderer());
		initAccelerometer();
		initMicrophone();
    }
	
    private class Renderer implements GLSurfaceView.Renderer {
        private static final String TAG = "Renderer";
        public void onDrawFrame(GL10 gl) {
            urMus.step();
        }
		
        public void onSurfaceChanged(GL10 gl, int width, int height) {
			SCREEN_WIDTH=width;
			SCREEN_HEIGHT=height;
			HALF_SCREEN_WIDTH=SCREEN_WIDTH/2;
			HALF_SCREEN_HEIGHT=SCREEN_HEIGHT/2;
			if(urMusReady==false) {				
				urMus.init(width, height);
				urMusReady=true;
				urMus.setupAPI();
			}
        }
		
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            // Do nothing.
        }
    }
    
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
		Log.i(TAG,"key code : "+keyCode);
        urMus.changeBackground();
        return true;
    }
	
	////////////////////
	// Touch Handling //
	////////////////////
	
	private static final int MAX_FINGERS=10;
	private long lastDown[]=new long[MAX_FINGERS];
	private float oldx[]=new float[MAX_FINGERS];
	private float oldy[]=new float[MAX_FINGERS];
	
	@Override
	public boolean onTouchEvent(final MotionEvent event) {
		if(!urMusReady) return true;
		
		// get which type this event is
		final int action=event.getAction();
		final int type=action & MotionEvent.ACTION_MASK;
		
		// get index of related pointer
		final int index=action >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
		
		final int historySize=event.getHistorySize();
		
		// retrieve touch data
		final int pointers=event.getPointerCount();
		final float x[]=new float[pointers];
		final float y[]=new float[pointers];
		final int id[]=new int[pointers];
		final float oldx[]=new float[pointers];
		final float oldy[]=new float[pointers];
		for(int i=0;i<pointers;i++) {
			id[i]=event.getPointerId(i);
			x[i]=event.getX(i);
			y[i]=event.getY(i);
			oldx[i]=this.oldx[i];
			oldy[i]=this.oldy[i];
			this.oldx[i]=x[i];
			this.oldy[i]=y[i];
		}
		
		queueEvent(new Runnable() {
			public void run() {
				// single tap by default
				int numTaps=1;
				
				switch(type) {
					case MotionEvent.ACTION_DOWN:
						// detect double tap
						long time=System.currentTimeMillis();
						if(time-lastDown[id[index]] < 250) {
							numTaps=2;
						}
						lastDown[id[index]]=time;
						
						for(int i=0; i<pointers; i++) {
							callAllTouchSources(x[i]/(float)HALF_SCREEN_WIDTH-1.0, 1.0-y[i]/(float)HALF_SCREEN_HEIGHT,id[i]);
						}
						
						for(int i=0; i<pointers; i++) {
							onTouchDownParse(id[i], numTaps, x[i], y[i]);
						}
						
						for(int i=0; i<pointers-1; i++) {
							for(int j=i+1; j<pointers; j++) {
								if(testDoubleDragStart(id[i],id[j])) {
									doTouchDoubleDragStart(i,j,id[i],id[j]);
								}
							}
						}
						
						for(int i=0; i<pointers; i++) {
							if(testSingleDragStart(id[i])) {
								// TODO : not sure what position2 is
								doTouchSingleDragStart(i,id[i],x[i],y[i],x[i],y[i]);
							}
						}
						
						break;
					case MotionEvent.ACTION_MOVE:
						for(int i=0; i<pointers; i++) {
							callAllTouchSources(x[i]/(float)HALF_SCREEN_WIDTH-1.0, 1.0-y[i]/(float)HALF_SCREEN_HEIGHT,id[i]);
						}
						
						onTouchArgInit();
						
						for(int i=0; i<pointers; i++) {
							onTouchMoveUpdate(i,id[i],oldx[i],oldy[i],x[i],y[i]);
						}
						
						int args=getArg();
						for(int i=0; i<args; i++) {
							int t=getArgMoved(i);
							int dragidx = FindSingleDragTouch(t);
							if(dragidx != -1) {
								//TODO : double touch
								onTouchSingleDragUpdate(t, dragidx);
							} else {
								onTouchScrollUpdate(t);
							}
						}
						
						callAllOnEnterLeaveRegions();
						
						break;
					case MotionEvent.ACTION_UP:
						for(int i=0; i<pointers; i++) {
							callAllTouchSources(x[i]/(float)HALF_SCREEN_WIDTH-1.0, 1.0-y[i]/(float)HALF_SCREEN_HEIGHT,id[i]);
						}
						
						onTouchArgInit();
						
						for(int i=0; i<pointers; i++) {
							onTouchDragEnd(i,id[i],x[i],y[i]);
							onTouchEnds(numTaps, oldx[i], oldy[i], x[i], y[i]);
						}
						
						callAllOnEnterLeaveRegions();
						break;
					default:
						break;
				}
			}
		});
		
		return true;
	}
	
	private native int FindDoubleDragTouch(int t1, int t2);
	private native int FindSingleDragTouch(int t);
	private native void onTouchDownParse(int t, int numTaps, float posx, float posy);
	private native void onTouchArgInit();
	private native void onTouchMoveUpdate(int t, int t2, float oposx, float oposy, float posx, float posy);
	private native void onTouchEnds(int numTaps, float oposx, float oposy, float posx, float posy);
	private native void onTouchDoubleDragUpdate(int t, int dragidx, float pos1x, float pos1y, float pos2x, float pos2y);
	private native boolean testDoubleDragStart(int t1, int t2);
	private native void doTouchDoubleDragStart(int t1,int t2,int touch1, int touch2);
	private native boolean testSingleDragStart(int t);
	private native int getSingleDoubleTouchConversionID(int t);
	private native void doTouchSingleDragStart(int t, int touch1, float pos1x, float pos1y, float pos2x, float pos2y);
	private native void onTouchSingleDragUpdate(int t, int dragidx);
	private native void onTouchScrollUpdate(int t);
	private native void onTouchDragEnd(int t,int touch, float posx, float posy);
	private native void callAllOnEnterLeaveRegions();
	private native void callAllTouchSources(double x, double y, int index);
	private native int getArg();
	private native int getArgMoved(int i);
	
	
	/////////////////////
	/// Accelerometer ///
	/////////////////////
	float accelX, accelY, accelZ;
	
	private SensorEventListener sensorEventListener; 
	
	private void initAccelerometer() {
		sensorEventListener = new SensorEventListener() {
			public void onAccuracyChanged(Sensor sensor, int accuracy) {};
			public void onSensorChanged(SensorEvent event) {
				accelX=-event.values[0]/10;
				accelY=-event.values[1]/10;
				accelZ=-event.values[2]/10;
				if(urMusReady) {
					didAccelerate(accelX,accelY,accelZ);
				}
			}
		};
		
		SensorManager sensorManager = (SensorManager)getContext().getSystemService(Context.SENSOR_SERVICE);
        List<Sensor> sensors = sensorManager.getSensorList(Sensor.TYPE_ACCELEROMETER);
        if (sensors.size() > 0) {
            sensorManager.registerListener(sensorEventListener, sensors.get(0), SensorManager.SENSOR_DELAY_GAME);
		} else {
			Log.i(TAG,"Could not initialize accelerometer");
		}
		
	}

	private native void didAccelerate(float x, float y, float z);

	
	//////////////////
	/// Microphone ///
	//////////////////
	
	private void initMicrophone() {
		int frequency = PCM_FREQUENCY;
		int channelConfiguration = AudioFormat.CHANNEL_CONFIGURATION_MONO;
		int audioEncoding = AudioFormat.ENCODING_PCM_16BIT;
		final int bufferSize = AudioRecord.getMinBufferSize(frequency, channelConfiguration,  audioEncoding);
		
		final AudioRecord audioRecord = new AudioRecord(MediaRecorder.AudioSource.MIC, 
												  frequency, channelConfiguration, 
												  audioEncoding, bufferSize);
		
		final short[] buffer = new short[bufferSize];   
		audioRecord.startRecording();
		
		Log.i(TAG,"Mic initialized : "+bufferSize);
		
		new Thread() {
			public void run() {
				while(true) {
					int bufferReadResult = audioRecord.read(buffer, 0, bufferSize);
//					Log.i(TAG,"AudioRecord read " + bufferReadResult + " samples ("+bufferSize+" samples requested");
					if(urMusReady) {
						callOnMicrophone(buffer,bufferReadResult);
					}
				}
			}
		}.start();
	}
	
	private native void callOnMicrophone(short [] buffer, int length);
}

