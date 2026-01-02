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
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.PackageInfo;
import android.content.BroadcastReceiver;
import android.content.IntentFilter;

import java.util.List;

import LynxEngine.Android.JNI;
import LynxEngine.Android.util.IabBroadcastReceiver.IabBroadcastListener;
import LynxEngine.Android.util.IabHelper;
import LynxEngine.Android.util.IabException;
import LynxEngine.Android.util.IabBroadcastReceiver;
import LynxEngine.Android.util.IabHelper.IabAsyncInProgressException;
import LynxEngine.Android.util.IabResult;
import LynxEngine.Android.util.Inventory;
import LynxEngine.Android.util.Purchase;

public class IAP
{		
	public final static int BILLING_SERVICES_NOT_INSTALL   	   = 30002; // SKRC_SERVICE_NOT_INSTALL
	public final static int BILLING_RESPONSE_IABHELPER_IS_NULL = 30003; // SKRC_INIT_IAP_FAILED
	public final static int BILLING_RESPONSE_ASYNC_EXCEPTION   = 30004;	// SKRC_IABHELPER_EXCEPITION	
	static final String[] GooglePlayStorePackageName = {"com.google.market", "com.android.vending"};
	IabHelper mHelper;	
	
	/**
     * SetUp IabHelper.     
     */
	public void SetUp() {
		if (mHelper != null) {
            mHelper.disposeWhenFinished();
            mHelper = null;
        }		
	}
	
	/**
     * Dispose of object, releasing resources.     
     */
	public void Dispose() {
		if (mHelper != null) {
            mHelper.disposeWhenFinished();
            mHelper = null;
        }		
	}
		
	/**
	 * implementation of CheckGooglePlayStoreInstalled
	 */
	public boolean CheckGooglePlayStoreInstalled() {
		JNI.LOGI("Check GooglePlayStore package installed");
		
		boolean googlePlayStoreInstalled = false;

		PackageManager packageManager =  NativeActivity.getInstance().getApplication().getPackageManager();
		List<PackageInfo> packages = packageManager.getInstalledPackages(PackageManager.GET_UNINSTALLED_PACKAGES);
		
		for (PackageInfo packageInfo : packages) {			
			for (int i=0 ; i<GooglePlayStorePackageName.length ; ++i)
	        if (packageInfo.packageName.equals(GooglePlayStorePackageName[i])) {
	            googlePlayStoreInstalled = true;
	            break;
	        }		    
		}

		return googlePlayStoreInstalled;
	}
	
	/**
     * OnError. handle any error in iap.
	 * native method OnIapPurchaseFailed is called when iap error occured.     
     */    
	void OnError(int code, String message) {		
		JNI.LOGI("OnError: (" + code + "," + message + ")");
		
		if (code != IabHelper.IABHELPER_USER_CANCELLED) {
			// error report		
			JNI.OnIapPurchaseFailed(code, message);
		}
		
		// end of purchase flow
		JNI.OnIapPurchaseFinished(code, "Purchase flow finished with response error.");
	}
	
	/**
     * Query inventory. onQueryInventoryFinished method is called when a query is finished.
     * IabHelper needs setup before starting query.
     */    
	public void QueryInventory(Context context, String encodedPublicKey, final List<String> Skus)
	{		
		if (false == CheckGooglePlayStoreInstalled())
		{
			OnError(BILLING_SERVICES_NOT_INSTALL,"GooglePlay services not installed.");
			return;
		}
		
		if (null == mHelper) {
			mHelper = new IabHelper(context, encodedPublicKey);
			mHelper.enableDebugLogging(true);
		}
		
		JNI.LOGI("IabHelper Starting setup.");
		
		if (false == mHelper.isSetup()) {
			mHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
				public void onIabSetupFinished(IabResult result) {
					JNI.LOGI("IabHelper Setup finished.");

					if (!result.isSuccess()) {
						OnError(result.getResponse(), "Setup in-app billing failed.");
						return;
					}

					// Have we been disposed of in the meantime? If so, quit.
					if (mHelper == null) {
						OnError(BILLING_RESPONSE_IABHELPER_IS_NULL, "IabHelper might be disposed.");
						return;
					}

					JNI.LOGI("IabHelper setup successful. Starting querying inventory.");
																																			
					try {
						mHelper.queryInventoryAsync(true, Skus, null, mQueryInventoryListener);
					} catch (IabAsyncInProgressException e) {
						OnError(BILLING_RESPONSE_ASYNC_EXCEPTION, "Error querying inventory. Another async operation in progress.");						
					}	
				}
			});
		}
		else 
		{			
			JNI.LOGI("IabHelper already setup. Starting querying inventory.");
			
			try {
				mHelper.queryInventoryAsync(true, Skus, null, mQueryInventoryListener);
			} catch (IabAsyncInProgressException e) {
				OnError(BILLING_RESPONSE_ASYNC_EXCEPTION, "Error querying inventory. Another async operation in progress.");				
			}
		}
	}
	
	/**
     * Purchase inventory. 
	 *
	 * There're 2 steps in the purchase process: 
	 *	 purchase and consume inventory. disassemble to 2 different api.
	 * onIabPurchaseFinished method is called when a purchase is finished.     
     */
	public void PurchaseInventory(Activity activity, final String sku, final int requestCode, final String payload)
	{
		if (null == mHelper || false == mHelper.isSetup()) {
			OnError(BILLING_RESPONSE_IABHELPER_IS_NULL, "IabHelper init failed while in purchase.");			
			return;
		}
			
		try {
            mHelper.launchPurchaseFlow(activity, sku, requestCode, mPurchaseFinishedListener, payload);
        } catch (IabAsyncInProgressException e) {
			OnError(BILLING_RESPONSE_ASYNC_EXCEPTION, "Error launching purchase flow. Another async operation in progress.");            
        }
	}
	
	/**
     * Consume purchase. 
	 *
	 * call InternalConsumeProcess(false).	 
     */
	public boolean ConsumePurchase() { return InternalConsumeProcess(false); }
	
	/**
     * Consume promocode purchase. 
	 *
	 * call InternalConsumeProcess(true).	 
     */
	public boolean ConsumePromoCodePurchase() { return InternalConsumeProcess(true); }

	/**
     * InternalConsumeProcess purchase.
	 * onConsumeFinished method is called when consumption is finished.	 
     * IabHelper needs setup before starting query.
	 *
	 * return ture if internal process is finished.
     */    
	public boolean InternalConsumeProcess(boolean consumePromoCode)
	{		
		if (null == mHelper || false == mHelper.isSetup()) {
			OnError(BILLING_RESPONSE_IABHELPER_IS_NULL, "IabHelper init failed while in internalconsumeprocess.");
			return true;
		}
		
		// check if there's purchase or not.
		boolean hasNoPurchase = true;		
		
		try 
		{
			Inventory inv = mHelper.queryInventory();
			List<String> skus = inv.getAllOwnedSkus();
			
			for (String sku : skus) {
				Purchase purchase = inv.getPurchase(sku);
				
				if (null != purchase) 
				{	
					// there's purchase needs consume
					hasNoPurchase = false;
											        					
					String orderId = purchase.getOrderId();
					String payload = purchase.getDeveloperPayload();
					JNI.LOGI("Consuming purchase=" + sku + ", orderid=" + orderId + ", DeveloperPayload=" + payload + " from previous purchase.");
		
					// orderId isEmpty : TEST account buy IAP or using promocode
					// payload isEmpty : only using promocode
					if (payload.isEmpty() && orderId.isEmpty())
					{	
						JNI.LOGI("Consuming purchase for promocode.");

						// using addByIAP in 1.00.8603, NEED NEW API for promocode verification.
						if (JNI.OnVerifyPurchasePromoCode(purchase.getItemType(), purchase.toString(), purchase.getOriginalJson(), purchase.getSignature()))					
						{
							JNI.LOGI("OnVerifyPurchasePromoCode succeed for promocode.");

							try {
								mHelper.consumeAsync(purchase, mConsumeFinishedListener);
							} catch (IabAsyncInProgressException e) {
								OnError(BILLING_RESPONSE_ASYNC_EXCEPTION, "Error consuming promocode " + sku + ". Another async operation in progress.");
							}
						}
					}				
					else
					{
						JNI.LOGI("Consuming purchase for iap.");

						if (JNI.OnVerifyDeveloperPayload(purchase.getItemType(), purchase.toString(), purchase.getOriginalJson(), purchase.getSignature()))
						{
							JNI.LOGI("OnVerifyDeveloperPayload succeed.");

							try {
								mHelper.consumeAsync(purchase, mConsumeFinishedListener);
							} catch (IabAsyncInProgressException e) {
								OnError(BILLING_RESPONSE_ASYNC_EXCEPTION, "Error consuming " + sku + ". Another async operation in progress.");
							}
						}
					}

					// handle one purchase at a time.
					break;
				} // End of purchase != null
			} // End for
		} 
		catch (IabException e) {
			final int responseCode = e.getResult().getResponse();
			final String message = e.getResult().getMessage();		
			OnError(BILLING_RESPONSE_ASYNC_EXCEPTION, "Error consuming query purchase, reason = " + message + "(" + responseCode + ").");
		}
		
		return hasNoPurchase;		
	}

	// Callback for when a query is finished	
    IabHelper.QueryInventoryFinishedListener mQueryInventoryListener = new IabHelper.QueryInventoryFinishedListener() {
        public void onQueryInventoryFinished(IabResult result, Inventory inventory) {
            JNI.LOGI("Query inventory callback.");

            // Have we been disposed of in the meantime? If so, quit.
            if (mHelper == null) {
				OnError(BILLING_RESPONSE_IABHELPER_IS_NULL, "IabHelper init failed while in query inventory.");
				return;
			}
			
            // Is it a failure?
            if (result.isFailure()) {
				OnError(result.getResponse(), "Query inventory failed: " + result);
                return;
            }

			JNI.OnQueryInventoryFinished(inventory.getJSONString());

            JNI.LOGI("Query inventory successful. Checking inventory consumption.");			
			ConsumePurchase();			
        }
    };
	
	// Callback for when a purchase is finished
    IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener() {
        public void onIabPurchaseFinished(IabResult result, Purchase purchase) {
            JNI.LOGI("Purchase callback: " + result + ", purchase: " + purchase);

            // if we were disposed of in the meantime, quit.
            if (mHelper == null) {
				OnError(BILLING_RESPONSE_IABHELPER_IS_NULL, "IabHelper init failed while in purchase.");
				return;
			}

            if (result.isFailure()) {
				OnError(result.getResponse(), "Purchase failed: " + result);
                return;
            }
			
			JNI.LOGI("Purchase successful. Starting inventory consumption.");
			ConsumePurchase();
        }
    };
			
	// Called when consumption is complete
    IabHelper.OnConsumeFinishedListener mConsumeFinishedListener = new IabHelper.OnConsumeFinishedListener() {
        public void onConsumeFinished(Purchase purchase, IabResult result) {
            JNI.LOGI("Consumption callback. purchase: " + purchase + ", result: " + result);

            // if we were disposed of in the meantime, quit.
            if (mHelper == null) {
				OnError(BILLING_RESPONSE_IABHELPER_IS_NULL, "IabHelper init failed while in consume.");
				return;
			}
			
            // We know this is the "gas" sku because it's the only one we consume,
            // so we don't check which sku was consumed. If you have more than one
            // sku, you probably should check...
            if (result.isFailure()) {
				OnError(result.getResponse(), "Consume failed: " + result);
				return;
            }
						
			JNI.OnConsumeFinished(result.getResponse(), result.getMessage(), purchase.getItemType(), purchase.toString());
            JNI.LOGI("Consume successful. Check if there's still any purchase needs to consume.");            		
						
			// Check if there's still any purchase needs to consume.
			if (true == ConsumePurchase())
			{				
				JNI.LOGI("NO purchase needs to consume . Finish purchase flow.");            		
				JNI.OnIapPurchaseFinished(IabHelper.BILLING_RESPONSE_RESULT_OK	,"Purchase flow finished.");
			}
        }
    };
	
	/**
     * HandleActivityResult
	 *
	 */
	public boolean HandleActivityResult(final int requestCode, final int resultCode, final Intent data) {
        if (null != mHelper) {
			return mHelper.handleActivityResult(requestCode, resultCode, data);			
		}
		return false;
	}

	
	// android pomo code.
    private BroadcastReceiver m_inAppPromotionsReceiver = new BroadcastReceiver()
	{
        @Override
        public void onReceive(Context context, Intent intent)
		{
            // internalConsumeProcess();
			ConsumePromoCodePurchase();
        }
    };

	// call on activity onResume.
	public void RegPromoCodeListener()
	{	
	    IntentFilter promoFilter = new IntentFilter("com.android.vending.billing.PURCHASES_UPDATED");
        NativeActivity.getInstance().registerReceiver(m_inAppPromotionsReceiver, promoFilter);        
        // internalConsumeProcess();
	}
	
	// call on Activity onPause.
	public void UnRegPromoCodeListener()
	{
		NativeActivity.getInstance().unregisterReceiver(m_inAppPromotionsReceiver);
	}		
}
