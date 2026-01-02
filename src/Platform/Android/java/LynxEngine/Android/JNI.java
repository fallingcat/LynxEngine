package LynxEngine.Android;


import android.content.Context;
import android.view.MotionEvent;
import android.util.Log;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.File; 
import java.util.ArrayList;

/**
 * JNI Object 
 * 
 * @description
 * 1. Java call C Interface for Application , C call Java might need metadata 
 *    through Activity. 
 * 2. Regist method only native or Java function in JNIObject.   
 * 3. "javac", "javah" might be failed if associate with resource dependency (R.java )
 *    in this file.
 *    
 * @author Kasaki Tzeng
 */
public class JNI 
{
	public static final String tag = "LynxEngine.Android";
	
	// for log 
	public static final boolean enableLog = true;
	public static void LOGI(String str) { if(enableLog) { Log.i(tag, str);}}
	public static void LOGE(String str) { if(enableLog) { Log.e(tag, str);}}
			
	// for load .so
	static
	{ 
		System.loadLibrary("QubotWar");
	}
	
	// setup FindClass() function to find custom jave classs in C++
	public static native void OnLoad(NativeActivity act);
	// for native interface
	public static native boolean IsApplicationInForeground();
	public static native void OnBackPressed();
	public static native void OnTouchEvent(int action, MotionEvent e);
	public static native void OnTouch(int action, MotionEvent e);
	public static native void OnScroll(int action, MotionEvent e1, MotionEvent e2, float metaData1, float metaData2);
	public static native void OnAlertDialogClick(int index);
	public static native void OnQueryInventoryFinished(String[] arraySkuJSONString);
	public static native boolean OnVerifyDeveloperPayload(String type, String JSONString, String metaData1, String metaData2);
	public static native boolean OnVerifyPurchasePromoCode(String type, String JSONString, String metaData1, String metaData2);
	public static native void OnConsumeFinished(int response, String message, String type, String JSONString);
	public static native void OnIapPurchaseFinished(int response, String message);
	public static native void OnIapPurchaseFailed(int errorCode, String message);
	public static native void UpdateText(long ptr, String str);
	public static native void SetDownloadTotalSize(long size);
	public static native void SetDownloadProgressSize(long size);
	public static native void OnFinishFacebookLogin(String JSONResult);
	public static native void OnFinishQueryFacebookAccountData(String JSONResult);
	public static native void OnFinishQueryFacebookFriendsData(String JSONResult);
	public static native void OnFinishFacebookShareLink(String JSONResult);
	public static native void OnFinishFacebookUploadPic(String JSONResult);
	public static native void HandleURL(String key, String value);
	public static native void SetAdvertisingIDString(String value);
	public static native void OnClickRatingDialog(int buttonIndex);
}
