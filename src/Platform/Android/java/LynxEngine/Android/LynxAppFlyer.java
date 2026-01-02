package LynxEngine.Android;

import java.util.*;
import android.app.Activity;

import android.content.SharedPreferences;
import android.util.Log;
import android.provider.Settings.Secure;
import android.telephony.TelephonyManager;
import java.lang.*;

import com.appsflyer.AFInAppEventParameterName;
import com.appsflyer.AFInAppEventType;
import com.appsflyer.AppsFlyerConversionListener;
import com.appsflyer.AppsFlyerLib;
import android.Manifest;

import pub.devrel.easypermissions.EasyPermissions;

/**
 * Created by Qubit on 2017/3/14.
 */

public class LynxAppFlyer
{

    static final String TAG = "AF";

    static SharedPreferences prefs = null;

    static Activity mActivity;
    static String DevKey = "";
	static boolean initaled = false;
    public static void Config(String build)
    {
    }

    public static void Init(Activity activity, String key)
    {
        DevKey = key;
        mActivity = activity;
        prefs = mActivity.getSharedPreferences(mActivity.getPackageName(), mActivity.MODE_PRIVATE);


        String android_id = Secure.getString(activity.getContentResolver(), Secure.ANDROID_ID);
        AppsFlyerLib.getInstance().setAndroidIdData(android_id);
		StartTracking();
		/*
        if (prefs.getBoolean("LynxAppFlyerfirstrun", true))
        {
            Log.v("AF", "LynxAppFlyerfirstrun true" );
            prefs.edit().putBoolean("LynxAppFlyerfirstrun", false).commit();
            //第一次 run 先問權限後再 start track.
            TryGetReadPhoneStatePermission();

        }
        else
        {
            Log.v("AF", "LynxAppFlyerfirstrun false" );
            StartTracking();
        }*/
		
		initaled = true;

    }

    static void StartTracking()
    {
		if (!initaled)
		{
			return;
		}
		
        Log.v("AF", "StartTracking" );
        AppsFlyerLib.getInstance().startTracking(mActivity.getApplication(), DevKey);
		
		AppsFlyerLib.getInstance().sendDeepLinkData(mActivity);
		
        AppsFlyerLib.getInstance().registerConversionListener(mActivity, new AppsFlyerConversionListener()
        {
            @Override
            public void onInstallConversionDataLoaded(Map<String, String> conversionData)
            {
                for (String attrName : conversionData.keySet())
                {
                    Log.d(AppsFlyerLib.LOG_TAG, "onInstallConversionDataLoaded attribute: " + attrName + " = " +
                            conversionData.get(attrName));
                }

                //SCREEN VALUES//
                //final String install_type = "Install Type: " + conversionData.get("af_status");
                //final String media_source = "Media Source: " + conversionData.get("media_source");
                //final String install_time = "Install Time(GMT): " + conversionData.get("install_time");
                //final String click_time = "Click Time(GMT): " + conversionData.get("click_time");
            }

            @Override
            public void onInstallConversionFailure(String errorMessage)
            {
                Log.d(AppsFlyerLib.LOG_TAG, "error onInstallConversionFailure getting conversion data: " + errorMessage);
            }

            @Override
            public void onAppOpenAttribution(Map<String, String> conversionData)
            {
                Log.d(AppsFlyerLib.LOG_TAG, "onAppOpenAttribution");
                for (String attrName : conversionData.keySet())
                {
                    Log.d(AppsFlyerLib.LOG_TAG, "onAppOpenAttribution attribute: " + attrName + " = " +
                            conversionData.get(attrName));
                }
            }

            @Override
            public void onAttributionFailure(String errorMessage)
            {
                Log.d(AppsFlyerLib.LOG_TAG, "error onAttributionFailure : " + errorMessage);
            }
        });
		
    }

    static private void TryGetReadPhoneStatePermission()
    {
		if (!initaled)
		{
			return;
		}
		
        String[] perms = { Manifest.permission.READ_PHONE_STATE};

        if (EasyPermissions.hasPermissions(mActivity, perms))
        {
            // Already have permission, do the thing
            // ...
            OnGetReadPhoneStatePermission();
        }
        else
        {
            // Do not have permissions, request them now
			//R.string.readphonestate
            EasyPermissions.requestPermissions(mActivity, mActivity.getString(mActivity.getResources().getIdentifier("readphonestate", "string", mActivity.getPackageName())),
                    0, perms);
        }
    }

    static private void OnGetReadPhoneStatePermission()
    {
		if (!initaled)
		{
			return;
		}
		
        TelephonyManager mngr = (TelephonyManager)mActivity.getSystemService(mActivity.TELEPHONY_SERVICE);
        String IMEI = mngr.getDeviceId();
        AppsFlyerLib.getInstance().setImeiData(IMEI);
        Log.v("AF", "IMEI : " + IMEI);

        StartTracking();
    }


    static public void onPermissionsGranted(int requestCode, List<String> perms)
    {
		if (!initaled)
		{
			return;
		}
		
        // Some permissions have been granted一些已授予的权限
        Log.d(TAG, "onPermissionsGranted:" + requestCode + ":" + perms.size());
        if (perms.contains(Manifest.permission.READ_PHONE_STATE))
        {
            OnGetReadPhoneStatePermission();
        }

    }

    static public void onPermissionsDenied(int requestCode, List<String> perms)
    {
		if (!initaled)
		{
			return;
		}
		
        // Some permissions have been denied一些被拒绝的权限
        Log.d(TAG, "onPermissionsGranted:" + requestCode + ":" + perms.size());
    }



    public static void SetUserID(String userid)
    {
    }

    public static void ConfigureAvailableEvent(int type, String[] stringArray)
    {
    }

    public static void AddProgressionEvent(int progressionStatus, String progression01, String progression02, String progression03)
    {
		if (!initaled)
		{
			return;
		}
		
        Map<String, Object> eventValue = new HashMap<String, Object>();
        eventValue.put("progressionStatus", progressionStatus);
        eventValue.put("progression01", progression01);
        eventValue.put("progression02", progression02);
        eventValue.put("progression03", progression03);

        AppsFlyerLib.getInstance().trackEvent(mActivity, "ProgressionEvent", eventValue);

    }

    public static void AddDesignEvent(String eventId)
    {
		if (!initaled)
		{
			return;
		}
		
        Map<String, Object> eventValue = new HashMap<String, Object>();
        eventValue.put("eventId", eventId);
        AppsFlyerLib.getInstance().trackEvent(mActivity, "DesignEvent", eventValue);
        AppsFlyerLib.getInstance().trackEvent(mActivity, eventId, eventValue);
    }

    public static void AddDesignEvent(String eventId, String stack)
    {
		if (!initaled)
		{
			return;
		}
		
        Map<String, Object> eventValue = new HashMap<String, Object>();
        eventValue.put("eventId", eventId);

        if (stack != "")
        {
            eventValue.put("stack", stack);
        }

        AppsFlyerLib.getInstance().trackEvent(mActivity, "DesignEvent", eventValue);
        AppsFlyerLib.getInstance().trackEvent(mActivity, eventId, eventValue);
    }

    public static void AddResourceEvent(int flowtype, String currency, float amount, String itemType, String itemId)
    {		
		if (!initaled)
		{
			return;
		}
		
        Map<String, Object> eventValue = new HashMap<String, Object>();
        eventValue.put("flowtype", flowtype);
        eventValue.put("currency", currency);
        eventValue.put("amount", amount);
        eventValue.put("itemType", itemType);
        eventValue.put("itemId", itemId);
        AppsFlyerLib.getInstance().trackEvent(mActivity, "ResourceEvent", eventValue);

    }

    public static void AddBusinessEvent(String currency, double amount, String itemType, String itemId, String cartType
            , String receipt, String store, String signature, String stack)
    {

		if (!initaled)
		{
			return;
		}
		
        Log.v("iap", "AddBusinessEvent: currency = " + currency +
                ", amount = " + amount +
                ", itemType = " + itemType +
                ", itemId = " + itemId +
                ", cartType = " + cartType +
                ", receipt = " + receipt +
                ", store = " + store +
                ", signature = " + signature);

        Map<String, Object> eventValue = new HashMap<String, Object>();
        eventValue.put(AFInAppEventParameterName.REVENUE, amount);
        eventValue.put(AFInAppEventParameterName.CONTENT_TYPE,itemType);
        eventValue.put(AFInAppEventParameterName.CONTENT_ID, itemId);
        eventValue.put(AFInAppEventParameterName.CURRENCY, currency);

        if (stack != "")
        {
            eventValue.put("stack", stack);
        }

        AppsFlyerLib.getInstance().trackEvent(mActivity, AFInAppEventType.PURCHASE, eventValue);


    }

    public static void UpdateUserProperty(String key, String value)
    {
		if (!initaled)
		{
			return;
		}
		
        //Bundle params = new Bundle();
        //params.putString(key, value);

    }
	
    public static void AddEvent(String eventname)
    {
		
		if (!initaled)
		{
			return;
		}
        //Bundle params = new Bundle();
        //params.putString(key, value);
		
        Map<String, Object> eventValue = new HashMap<String, Object>();
        AppsFlyerLib.getInstance().trackEvent(mActivity, eventname, eventValue);
    }
}
