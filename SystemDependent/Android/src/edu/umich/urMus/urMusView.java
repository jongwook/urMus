/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package edu.umich.urMus;
/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
/**
 * An implementation of SurfaceView that uses the dedicated surface for
 * displaying an OpenGL animation.  This allows the animation to run in a
 * separate thread, without requiring that it be driven by the update mechanism
 * of the view hierarchy.
 *
 * The application-specific rendering code is delegated to a GLView.Renderer
 * instance.
 */
class urMusView extends GLSurfaceView {
	public static final String TAG="urMusView";
	
	private boolean urMusReady=false;
	
	private int SCREEN_WIDTH;
	private int SCREEN_HEIGHT;
	private int HALF_SCREEN_WIDTH;
	private int HALF_SCREEN_HEIGHT;
	
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
			
            urMus.init(width, height);
			if(urMusReady==false) {
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
        urMus.changeBackground();
        return true;
    }
	
	/** Show an event in the LogCat view, for debugging */
	private void dumpEvent(MotionEvent event) {
		String names[] = { "DOWN" , "UP" , "MOVE" , "CANCEL" , "OUTSIDE" ,
			"POINTER_DOWN" , "POINTER_UP" , "7?" , "8?" , "9?" };
		StringBuilder sb = new StringBuilder();
		int action = event.getAction();
		int actionCode = action & MotionEvent.ACTION_MASK;
		sb.append("event ACTION_" ).append(names[actionCode]);
		if (actionCode == MotionEvent.ACTION_POINTER_DOWN
			|| actionCode == MotionEvent.ACTION_POINTER_UP) {
			sb.append("(pid " ).append(
									   action >> MotionEvent.ACTION_POINTER_ID_SHIFT);
			sb.append(")" );
		}
		sb.append("[" );
		for (int i = 0; i < event.getPointerCount(); i++) {
			sb.append("#" ).append(i);
			sb.append("(pid " ).append(event.getPointerId(i));
			sb.append(")=" ).append((int) event.getX(i));
			sb.append("," ).append((int) event.getY(i));
			if (i + 1 < event.getPointerCount())
				sb.append(";" );
		}
		sb.append("]" );
		Log.d(TAG, sb.toString());
	}
	
	////////////////////
	// Touch Handling //
	////////////////////
	
	private final int MAX_FINGERS=10;
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
						if(lastDown[id[index]]-time < 250) {
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
		
		//dumpEvent(event);
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
}

