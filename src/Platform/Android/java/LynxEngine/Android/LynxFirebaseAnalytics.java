package LynxEngine.Android;


import java.util.*;
import android.app.Activity;
import LynxEngine.Android.JNI;
import com.google.firebase.analytics.FirebaseAnalytics;
import android.os.Bundle;
import android.util.Log;

public class LynxFirebaseAnalytics
{
	private static FirebaseAnalytics mFirebaseAnalytics;

	public static void Config(String build)
	{
	}
	
	public static void Init(Activity activity)
	{
		mFirebaseAnalytics = FirebaseAnalytics.getInstance(activity);
	}

	public static void ConfigureAvailableEvent(int type, String[] stringArray)
	{
	}

	public static void AddProgressionEvent(int progressionStatus, String progression01, String progression02, String progression03)
	{
		Bundle params = new Bundle();
		params.putInt("progressionStatus", progressionStatus);
		params.putString("progression01", progression01);
		params.putString("progression02", progression02);
		params.putString("progression03", progression03);
		mFirebaseAnalytics.logEvent("ProgressionEvent", params);

	}

	public static void AddDesignEvent(String eventId)
	{
		Bundle params = new Bundle();
		params.putString("eventId", eventId);
		mFirebaseAnalytics.logEvent("DesignEvent", params);

	}

	public static void AddResourceEvent(int flowtype, String currency, float amount, String itemType, String itemId)
	{
		Bundle params = new Bundle();
		params.putInt("flowtype", flowtype);
		params.putString("currency", currency);
        params.putFloat("amount", amount);
		params.putString("itemType", itemType);
		params.putString("itemId", itemId);
		mFirebaseAnalytics.logEvent("ResourceEvent", params);

	}

	public static void AddBusinessEvent(String currency, double amount, String itemType, String itemId, String cartType, String receipt, String store, String signature)
	{
		/*
		JNI.LOGI("AddBusinessEvent: currency = " + currency +
									", amount = " + amount +
									", itemType = " + itemType +
									", itemId = " + itemId +
									", cartType = " + cartType +
									", receipt = " + receipt +
									", store = " + store +
									", signature = " + signature);
			*/
		Bundle params = new Bundle();
		params.putString("currency", currency);
		params.putFloat("amount", (float)amount);
		params.putString("itemType", itemType);
		params.putString("itemId", itemId);
		params.putString("cartType", cartType);
		params.putString("receipt", receipt);
		params.putString("store", store);
		params.putString("signature", signature);
		mFirebaseAnalytics.logEvent("BusinessEvent", params);
	}

	
    public static void UpdateUserProperty(String key, String value)
    {
        Log.v("firebase", "updateUserProperties ");
		mFirebaseAnalytics.setUserProperty(key, value);
    }
}