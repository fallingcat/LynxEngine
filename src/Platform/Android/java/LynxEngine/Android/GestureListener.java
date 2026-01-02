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

package LynxEngine.Android;

import android.app.Activity;
import android.os.Bundle;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.View;
import LynxEngine.Android.JNI;

class GestureListener extends GestureDetector.SimpleOnGestureListener {	
	////////////////////////////////////////////////////////////////
	/**
	 * Java side:
	 * ref-https://github.com/android/platform_frameworks_base/blob/master/core/java/android/view/MotionEvent.java
	 * native side:
	 * ref-https://github.com/android/platform_development/blob/master/ndk/platforms/android-14/include/android/input.h	
	 */
	public static final int AMOTION_EVENT_GESTURE_START       = MotionEvent.ACTION_HOVER_EXIT; // 10
	public static final int	AMOTION_EVENT_GESTURE_DOWN		  = MotionEvent.ACTION_HOVER_EXIT + 1;
	public static final int	AMOTION_EVENT_GESTURE_SCROLL	  = MotionEvent.ACTION_HOVER_EXIT + 2;
	public static final int	AMOTION_EVENT_GESTURE_SINGLETAPUP = MotionEvent.ACTION_HOVER_EXIT + 3;
	public static final int	AMOTION_EVENT_GESTURE_SHOWPRESS   = MotionEvent.ACTION_HOVER_EXIT + 4;
	public static final int	AMOTION_EVENT_GESTURE_LONGPRESS   = MotionEvent.ACTION_HOVER_EXIT + 5;
	public static final int	AMOTION_EVENT_GESTURE_FLING		  = MotionEvent.ACTION_HOVER_EXIT + 6;
	public static final int	AMOTION_EVENT_GESTURE_DOUBLETAP   = MotionEvent.ACTION_HOVER_EXIT + 7;

	// for OnFling event
	private static final int SWIPE_MIN_DISTANCE = 120;
    private static final int SWIPE_MAX_OFF_PATH = 250;
    private static final int SWIPE_THRESHOLD_VELOCITY = 200;

	@Override
	public boolean onDown(MotionEvent ev) {
		final int pointerCount = ev.getPointerCount();
		// final long downTime = ev.getDownTime();
		// final long eventTime = ev.getEventTime();
		// final float x = ev.getX();
		// final float y = ev.getY();
		// final int action = ev.getAction();
		//JNI.LOGI("onDown[" + pointerCount + "]");
		JNI.OnTouch(AMOTION_EVENT_GESTURE_DOWN, ev);
		return true;
	}

	@Override  
	/*e1:initial down  e2:current move*/
	public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
		//JNI.LOGI("onScroll");
		JNI.OnScroll(AMOTION_EVENT_GESTURE_SCROLL, e1, e2, distanceX, distanceY);
		return true;
	}

	/**
	See different between onSingleTapUp and onSingleTapConfirmed
	ref. http://developer.android.com/reference/android/view/GestureDetector.SimpleOnGestureListener.html#onSingleTapConfirmed%28android.view.MotionEvent%29
	*/
	@Override
	public boolean onSingleTapUp(MotionEvent ev) {
		//JNI.LOGI("onSingleTapUp");
		JNI.OnTouch(AMOTION_EVENT_GESTURE_SINGLETAPUP, ev);
		return true;
	}
	
	/*
	@Override
	public boolean onSingleTapConfirmed (MotionEvent ev) {
		//JNI.LOGI("onSingleTapConfirmed");
		JNI.OnTouch(AMOTION_EVENT_GESTURE_SINGLETAPUP, ev);
		return true;
	}*/

	@Override
	public void onShowPress(MotionEvent ev) {
		//JNI.LOGI("onShowPress");
		JNI.OnTouch(AMOTION_EVENT_GESTURE_SHOWPRESS, ev);
	}

	@Override
	public void onLongPress(MotionEvent ev) {
		//JNI.LOGI("onLongPress");
		JNI.OnTouch(AMOTION_EVENT_GESTURE_LONGPRESS, ev);
	}

	@Override
	/*e1:initial down  e2:match up*/
	public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
		//JNI.LOGI("onFling");
		
		if (Math.abs(e1.getY() - e2.getY()) <= SWIPE_MAX_OFF_PATH)
        {
			final float distance = e1.getX() - e2.getX();
			final boolean velocity = Math.abs(velocityX) > SWIPE_THRESHOLD_VELOCITY;
			
			if (distance > SWIPE_MIN_DISTANCE && velocity) {  // Left
				JNI.OnScroll(AMOTION_EVENT_GESTURE_FLING, e1, e2, -1.0f, 0.0f);
			}
			else if (distance < -SWIPE_MIN_DISTANCE && velocity) { // Right
				JNI.OnScroll(AMOTION_EVENT_GESTURE_FLING, e1, e2, 1.0f, 0.0f);
			}
		}
		
		if (Math.abs(e1.getX() - e2.getX()) <= SWIPE_MAX_OFF_PATH)
        {
			final float distance = e1.getY() - e2.getY();
			final boolean velocity = Math.abs(velocityY) > SWIPE_THRESHOLD_VELOCITY;
			
			if (distance > SWIPE_MIN_DISTANCE && velocity) {  // Down
				JNI.OnScroll(AMOTION_EVENT_GESTURE_FLING, e1, e2, 0.0f, -1.0f);
			}
			else if (distance < -SWIPE_MIN_DISTANCE && velocity) { // Up
				JNI.OnScroll(AMOTION_EVENT_GESTURE_FLING, e1, e2, 0.0f, 1.0f);
			}
		}

		return true;
	}

	public boolean onDoubleTap(MotionEvent ev){
		//JNI.LOGI("onDoubleTap");
		JNI.OnTouch(AMOTION_EVENT_GESTURE_DOUBLETAP, ev);
		return true;
	}
}
