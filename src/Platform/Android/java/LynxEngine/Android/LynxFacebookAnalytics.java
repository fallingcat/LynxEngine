package LynxEngine.Android;


import java.util.*;
import android.app.Activity;
import LynxEngine.Android.JNI;
import com.facebook.appevents.AppEventsLogger;
import com.facebook.FacebookSdk;
import com.facebook.GraphRequest;
import com.facebook.GraphResponse;
import java.math.BigDecimal;
import android.os.Bundle;
import android.util.Log;

import java.lang.*;

public class LynxFacebookAnalytics
{
    static AppEventsLogger m_Logger;
    public static void Config(String build)
    {
    }

    public static void Init(Activity activity)
    {
        if (!FacebookSdk.isInitialized())
        {
            FacebookSdk.sdkInitialize(activity);
        }
        AppEventsLogger.activateApp(activity.getApplication());
        m_Logger = AppEventsLogger.newLogger(activity);
    }
	
    public static void SetUserID(String userid)
    {
		AppEventsLogger.setUserID(userid);
        Log.v("FB", "AppEventsLogger.setUserID " + userid);
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
        m_Logger.logEvent("ProgressionEvent", params);


    }

    public static void AddDesignEvent(String eventId)
    {
        Bundle params = new Bundle();
        params.putString("eventId", eventId);
        m_Logger.logEvent("DesignEvent", params);

		String FbEv = eventId.replace(':','_');
		m_Logger.logEvent(FbEv);
    }
	
    public static void AddDesignEvent(String eventId, String stack)
    {
        Bundle params = new Bundle();
		if (stack != "")
		{
			params.putString("stack", stack);
		}
		m_Logger.logEvent(eventId, params);

        params.putString("eventId", eventId);		
        m_Logger.logEvent("DesignEvent", params);	
		
		String FbEv = eventId.replace(':','_');
		m_Logger.logEvent(FbEv);		
    }

    public static void AddResourceEvent(int flowtype, String currency, float amount, String itemType, String itemId)
    {
        Bundle params = new Bundle();
        params.putInt("flowtype", flowtype);
        params.putString("currency", currency);
        params.putFloat("amount", amount);
        params.putString("itemType", itemType);
        params.putString("itemId", itemId);
        m_Logger.logEvent("ResourceEvent", params);

    }

    public static void AddBusinessEvent(String currency, double amount, String itemType, String itemId, String cartType
	, String receipt, String store, String signature, String stack)
    {
		
		Log.v("iap", "AddBusinessEvent: currency = " + currency +
									", amount = " + amount +
									", itemType = " + itemType +
									", itemId = " + itemId +
									", cartType = " + cartType +
									", receipt = " + receipt +
									", store = " + store +
									", signature = " + signature);
			
        Bundle params = new Bundle();
        params.putString("currency", currency);
        params.putFloat("amount", (float)amount);
        params.putString("itemType", itemType);
        params.putString("itemId", itemId);
        params.putString("cartType", cartType);
        params.putString("receipt", receipt);
        params.putString("store", store);
        params.putString("signature", signature);

		if (stack != "")
		{
			params.putString("stack", stack);		
		}

        m_Logger.logEvent("BusinessEvent", params);

        Currency javacurrency = Currency.getInstance(Locale.US);
        try
        {
            //Locale locale = new Locale(currency);
            javacurrency = Currency.getInstance(currency);
        }
        catch (ArithmeticException ex)
        {
            Log.v("FB", "can't convert currency string: " + currency);
        }

        m_Logger.logPurchase(BigDecimal.valueOf(amount), javacurrency);
    }
	
    public static void UpdateUserProperty(String key, String value)
    {
        Bundle params = new Bundle();
        params.putString(key, value);		
		
		AppEventsLogger.updateUserProperties(params,  new GraphRequest.Callback()
		{
            @Override
            public void onCompleted(GraphResponse response)
			{
                Log.v("fb", "AppEventsLogger.updateUserProperties onCompleted " + response.getRawResponse());
            }
        });
    }

}