package LynxEngine.Android;

import com.gameanalytics.sdk.*;
import java.util.*;
import android.app.Activity;
import LynxEngine.Android.JNI;

public class GameAnalytics {
	final static int AE_RES_CURRENCIES = 0;
	final static int AE_RES_ITEMTYPES  = 1;
	final static int AE_CUSTOM_DIM01   = 2;
	final static int AE_CUSTOM_DIM02   = 3;
	final static int AE_CUSTOM_DIM03   = 4;
	  
	public static void Config(String build) {
		com.gameanalytics.sdk.GameAnalytics.configureBuild(build);
		/*
		StringVector currencies = new StringVector();
		currencies.add("gems");
		currencies.add("gold");
		GameAnalytics.configureAvailableResourceCurrencies(currencies);

		StringVector itemTypes = new StringVector();
		itemTypes.add("boost");
		itemTypes.add("lives");
		GameAnalytics.configureAvailableResourceItemTypes(itemTypes);

		// Configure available custom dimensions
		StringVector customDimension01 = new StringVector();
		customDimension01.add("ninja");
		customDimension01.add("samurai");
		GameAnalytics.configureAvailableCustomDimensions01(customDimension01);

		StringVector customDimension02 = new StringVector();
		customDimension02.add("whale");
		customDimension02.add("dolphin");
		GameAnalytics.configureAvailableCustomDimensions02(customDimension02);
		 
		StringVector customDimension03 = new StringVector();
		customDimension03.add("horde");
		customDimension03.add("alliance");
		GameAnalytics.configureAvailableCustomDimensions03(customDimension03);
		*/
	} 

	public static void Init(Activity activity, String gameKey, String secretKey) {		
		// Enable log
        com.gameanalytics.sdk.GameAnalytics.setEnabledInfoLog(true);
        com.gameanalytics.sdk.GameAnalytics.setEnabledVerboseLog(true);
		
		com.gameanalytics.sdk.GameAnalytics.initializeWithGameKey(activity, gameKey, secretKey);
	}
	
	public static void ConfigureAvailableEvent(int type, String[] stringArray) {
		// Debug print
		String debugString = "";
		for (int i=0; i<stringArray.length; ++i) {
			debugString += (stringArray[i] + ",");
		}
		JNI.LOGI("ConfigureAvailableEvent: type = " + type + ", value = [" + debugString + "]");
									
		switch(type)
		{
		case AE_RES_CURRENCIES:
			com.gameanalytics.sdk.GameAnalytics.configureAvailableResourceCurrencies(stringArray);
			break;
		case AE_RES_ITEMTYPES:
			com.gameanalytics.sdk.GameAnalytics.configureAvailableResourceItemTypes(stringArray);
			break;
		case AE_CUSTOM_DIM01:
			com.gameanalytics.sdk.GameAnalytics.configureAvailableCustomDimensions01(stringArray);
			break;
		case AE_CUSTOM_DIM02:
			com.gameanalytics.sdk.GameAnalytics.configureAvailableCustomDimensions02(stringArray);
			break;
		case AE_CUSTOM_DIM03:
			com.gameanalytics.sdk.GameAnalytics.configureAvailableCustomDimensions03(stringArray);
			break;		
		}
	}
	
	public static void AddProgressionEvent(int progressionStatus, String progression01, String progression02, String progression03) {
		if (progression02 == "")
			com.gameanalytics.sdk.GameAnalytics.addProgressionEventWithProgressionStatus(GAProgressionStatus.values()[progressionStatus], progression01);
		else if (progression03 == "")
			com.gameanalytics.sdk.GameAnalytics.addProgressionEventWithProgressionStatus(GAProgressionStatus.values()[progressionStatus], progression01, progression02);
		else
			com.gameanalytics.sdk.GameAnalytics.addProgressionEventWithProgressionStatus(GAProgressionStatus.values()[progressionStatus], progression01, progression02, progression03);
	}

	public static void AddDesignEvent(String eventId) {
		com.gameanalytics.sdk.GameAnalytics.addDesignEventWithEventId(eventId);
	} 

	public static void AddResourceEvent(int flowtype, String currency, float amount, String itemType, String itemId) {
		com.gameanalytics.sdk.GameAnalytics.addResourceEventWithFlowType(flowtype, currency, amount, itemType, itemId);
	} 
	
	public static void AddBusinessEvent(String currency, double amount, String itemType, String itemId, String cartType, String receipt, String store, String signature) {
		
		JNI.LOGI("AddBusinessEvent: currency = " + currency +
									", amount = " + amount +
									", itemType = " + itemType +
									", itemId = " + itemId +
									", cartType = " + cartType +
									", receipt = " + receipt +
									", store = " + store +
									", signature = " + signature);
		amount = amount * 100;					
		com.gameanalytics.sdk.GameAnalytics.addBusinessEventWithCurrency(currency, (int)amount, itemType, itemId, cartType, receipt, store, signature);
	} 
}