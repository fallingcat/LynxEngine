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
import android.app.AlertDialog;
import android.app.AlarmManager;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.Manifest;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.Environment;
import android.content.Intent;
import android.content.Context;
import android.content.DialogInterface;
import android.content.IntentFilter;
import android.content.ActivityNotFoundException;
import android.content.pm.PackageManager;
import java.io.IOException;
import java.io.FileOutputStream;
import java.io.File;
import java.nio.ByteBuffer;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.graphics.*;
import android.graphics.drawable.*;
import android.view.*;
import android.view.ViewGroup.*;
import android.view.View.*;
import android.view.inputmethod.InputMethodManager;
import android.view.inputmethod.EditorInfo;
import android.widget.*;
import android.telephony.TelephonyManager;
import java.text.SimpleDateFormat;
import android.provider.MediaStore;
import android.os.Build;
import android.content.res.Configuration;
import android.annotation.SuppressLint;
import android.content.DialogInterface;
import android.app.Dialog;

import android.webkit.ValueCallback;
import android.webkit.WebChromeClient;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.webkit.WebChromeClient;
import android.os.AsyncTask;
import android.net.Uri;
import java.io.DataInputStream;
import java.net.URL;
import java.net.URLConnection;
import java.util.Arrays;
import android.text.TextWatcher;
import android.text.Editable;
import android.webkit.WebSettings;
import java.lang.Runnable;
import android.util.Log;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.math.BigInteger;
import java.net.HttpURLConnection;
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;

// Facebook
import com.facebook.FacebookRequestError;
import com.facebook.FacebookSdk;
import com.facebook.CallbackManager;
import com.facebook.share.widget.ShareDialog;
import com.facebook.share.model.ShareLinkContent;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.login.LoginResult;
import com.facebook.login.LoginManager;
import com.facebook.GraphRequest;
import com.facebook.GraphResponse;
import com.facebook.AccessToken;
import com.facebook.share.Sharer;
import com.facebook.share.model.SharePhoto;
import com.facebook.share.model.SharePhotoContent;
import com.facebook.appevents.AppEventsLogger;
import com.facebook.applinks.AppLinkData;

import org.json.JSONArray;
import org.json.JSONObject;

// IAP
import LynxEngine.Android.IAP;
import LynxEngine.Android.JNI;
import LynxEngine.Android.util.IabBroadcastReceiver;

// Notification
import com.android.notify.NotificationReceiver;

// firebase dynamic.
import android.support.annotation.NonNull;
import com.google.android.gms.appinvite.AppInvite;
import com.google.android.gms.appinvite.AppInviteInvitationResult;
import com.google.android.gms.appinvite.AppInviteReferral;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.common.api.ResultCallback;
import com.google.firebase.iid.FirebaseInstanceId;

// AdvertisingId;
import com.google.android.gms.ads.identifier.AdvertisingIdClient;
import com.google.android.gms.common.GooglePlayServicesNotAvailableException;
import com.google.android.gms.common.GooglePlayServicesRepairableException;

// Feedback dialog
import LynxEngine.Android.FeedbackDialog;

import java.lang.Thread.UncaughtExceptionHandler;


//andorid 6.0+
import pub.devrel.easypermissions.EasyPermissions;

public class NativeActivity extends android.app.NativeActivity implements EasyPermissions.PermissionCallbacks
{
    final int GET_SKD_PERM_DELAY = 700;
	int NAVBAR_DELAY_HIDE_MILSEC = 4000;	
	// touch 
	GestureDetector mGestureDetector = null;
	
	// static instance ref self.
	static NativeActivity sNativeActivity = null;		
	/** Static method to get NativeActivity instance. */
	public static NativeActivity getInstance() {  return sNativeActivity; }
		
	// iap
	IAP mIAP = null;
		
	// layout	
	PopupDialog popupDialog;
	FeedbackDialog freebackDialog;
	LinearLayout mainLayout;
	RelativeLayout viewLayout;
	Map<String, View> viewMap;
	
	CallbackManager callbackManager;
    ShareDialog shareDialog;
	
    private GoogleApiClient mGoogleApiClient;
	static Pattern unicodeOutliers;	
	
	//webview upload.
    private String mCM;
    private ValueCallback<Uri> mUM;
    private ValueCallback<Uri[]> mUMA;
    private final static int FCR=1;


	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		
		//TryGetAPPPermissions();
		//TryGetSDKPermissions();

		GetAdvertisingIDString();
		
		// Initialize the SDK before executing any other operations,
		// especially, if you're using Facebook UI elements.
		FacebookSdk.sdkInitialize(getApplicationContext());
		FacebookSdk.setIsDebugEnabled(true); 
		AppEventsLogger.activateApp(this);
		AppLinkData.fetchDeferredAppLinkData(this, 
					new AppLinkData.CompletionHandler()
					{
						@Override
						public void onDeferredAppLinkDataFetched(AppLinkData appLinkData)
						{
							// 處理應用程式連結資料
							if (appLinkData != null)
							{
								Bundle bundle = appLinkData.getArgumentBundle();
								Log.i("DEBUG_FACEBOOK_SDK", bundle.toString()); 
							}
							else
							{ 
								Log.i("DEBUG_FACEBOOK_SDK", "AppLinkData is Null"); 
							}
						}
					}
		);

		//firebase dynamic.
        // [START build_api_client]
        // Build GoogleApiClient with AppInvite API for receiving deep links
        mGoogleApiClient = new GoogleApiClient.Builder(this)
                //.enableAutoManage(this, this)
                .addApi(AppInvite.API)
                .build();
        // [END build_api_client]
		
        // [START get_deep_link]
        // Check if this app was launched from a deep link. Setting autoLaunchDeepLink to true
        // would automatically launch the deep link if one is found.
        boolean autoLaunchDeepLink = false;
        AppInvite.AppInviteApi.getInvitation(mGoogleApiClient, this, autoLaunchDeepLink)
                .setResultCallback(
                        new ResultCallback<AppInviteInvitationResult>()
						{
                            @Override
                            public void onResult(@NonNull AppInviteInvitationResult result)
							{
                                if (result.getStatus().isSuccess())
								{
                                    // Extract deep link from Intent
                                    Intent intent = result.getInvitationIntent();
                                    String deepLink = AppInviteReferral.getDeepLink(intent);

                                    // Handle the deep link. For example, open the linked
                                    // content, or apply promotional credit to the user's
                                    // account.
                                    Log.d("firebase", "getInvitation " + deepLink);
									HandleURL(deepLink);

                                }
								else
								{
                                    Log.d("firebase", "getInvitation: no deep link found.");
                                }
                            }
                        });


        // [END get_deep_link]
		

		JNI.OnLoad(this);
		

		// Enter code here
		mGestureDetector = new GestureDetector(this, new GestureListener());

		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		
		// @private NativeContentView mNativeContentView in android.app.NativeActivity
		View nativeContentView = getWindow().getDecorView();
		nativeContentView.setClickable(true);
		//nativeContentView.setOnTouchListener(this);
		
		/// DO NOT block surface listener
		/* getWindow().takeSurface(null); */
		
		/// block input listener from native
        getWindow().takeInputQueue(null);		

		// static self reference 
		sNativeActivity = this;

		getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_ADJUST_PAN);

		runOnUiThread(new Runnable()  {
			@Override
			public void run()  {
				mainLayout = new LinearLayout(sNativeActivity);
				viewLayout = new RelativeLayout(sNativeActivity);
				viewMap = new HashMap<String, View> ();

				setContentView(mainLayout, new LinearLayout.LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT));

				popupDialog = new PopupDialog(sNativeActivity);
				popupDialog.SetBackgroundClickable();
				//RelativeLayout.LayoutParams lp1 = new RelativeLayout.LayoutParams(-1, -1);
				//lp1.addRule(RelativeLayout.CENTER_VERTICAL);
				//lp1.addRule(RelativeLayout.	CENTER_HORIZONTAL);
				popupDialog.setContentView(viewLayout);
				
				popupDialog.setOnKeyListener(new Dialog.OnKeyListener()
				{
					@Override
					public boolean onKey(DialogInterface arg0, int keyCode, KeyEvent event) {
					
						//This is the filter
						if (event.getAction()!= KeyEvent.ACTION_DOWN)
							return false;

						if (keyCode == KeyEvent.KEYCODE_BACK) 
						{
							Log.d("dia", "OnCancelListener ");
							sNativeActivity.onBackPressed();
						}
						return false;
					}
				});
			}
		});


		// Hide navigation bar
		if (android.os.Build.VERSION.SDK_INT >= 19)
		{
		
			final View decorView = getWindow().getDecorView();

			HideNavBar();
			decorView.setOnSystemUiVisibilityChangeListener(new View.OnSystemUiVisibilityChangeListener()
            {
                @Override
                public void onSystemUiVisibilityChange(int flags)
                {
					//Log.d("NAV", "onSystemUiVisibilityChange");
					boolean visable = (flags & View.SYSTEM_UI_FLAG_HIDE_NAVIGATION) == 0;

                    if(visable)
                    {
						//Log.d("NAV", "visable");
						DelayHideNavBar(NAVBAR_DELAY_HIDE_MILSEC);
						//HideNavBar();
                    }
                }
            });
		}
		
		callbackManager = CallbackManager.Factory.create();
        shareDialog = new ShareDialog(this);


		// 表情碼.
		// https://en.wikipedia.org/wiki/Emoji
		// 計算 Surrogate Pair .
		//http://www.russellcottrell.com/greek/utilities/surrogatepaircalculator.htm
		//Pattern unicodeOutliers = Pattern.compile("[\uD83C\uDF00-\uD83D\uDDFF]|[\uD83E\uDD00-\uD83E\uDDFF]|[\uD83D\uDE00-\uD83D\uDE4F]|[\uD83D\uDE80-\uD83D\uDEFF]|[\u2600-\u27BF]", Pattern.UNICODE_CASE | Pattern.CASE_INSENSITIVE);  
		unicodeOutliers = Pattern.compile("[\uD83C\uDF00-\uD83D\uDDFF]|[\uD83E\uDD00-\uD83E\uDDFF]|[\uD83D\uDE00-\uD83D\uDE4F]|[\uD83D\uDE80-\uD83D\uDEFF]", Pattern.UNICODE_CASE | Pattern.CASE_INSENSITIVE);  
        
		/* IAP */
		mIAP = new IAP();					
	}
		
	/** Called when the activity is destroyed. */
	@Override
	public void onDestroy()
	{				
		super.onDestroy();
		
		mIAP.Dispose();
	}

	@Override
	protected void onStart()
	{
		super.onStart();
		mGoogleApiClient.connect();
	}

	@Override
	protected void onStop()
	{
		super.onStop();
		mGoogleApiClient.disconnect();
		
		//maybe clear intent data here is good.
		getIntent().setData(null);
	}

	protected void onPostCreate(Bundle savedInstacceState)
	{
		super.onPostCreate(savedInstacceState);

		DelayHideNavBar(NAVBAR_DELAY_HIDE_MILSEC);
		
		//facebook.
		AccessToken.refreshCurrentAccessTokenAsync();

		//HandleIntentData();
	}

	Handler mHideSystemUiHandler = new  Handler()
	{
		@Override
		public void handleMessage(Message msg)
		{
			HideNavBar();
		}
	};
	/*
	Runnable mHideSystemUiRun = new Runnable()
				{
					@Override
					public void run()
					{
						HideNavBar();
					}
				};
	*/
	private void DelayHideNavBar(int delayMillis)
	{
		if (android.os.Build.VERSION.SDK_INT >= 19)
		{
			mHideSystemUiHandler.removeMessages(0);
			mHideSystemUiHandler.sendEmptyMessageDelayed(0, delayMillis);

			//mHideSystemUiHandler.removeCallbacks(mHideSystemUiRun);
			//mHideSystemUiHandler.postDelayed(mHideSystemUiRun, delayMillis);

		}
	}
	
	@Override
	public void onResume()
	{
		super.onResume();
		//HideNavBar();
		//HidePopupDialogNavBar();
		
		DelayHideNavBar(NAVBAR_DELAY_HIDE_MILSEC);
		HidePopupDialogNavBar();

		mIAP.RegPromoCodeListener();
	}

	@Override
	public void onPostResume()
	{
		super.onPostResume();
	
        Log.d("firebase", "onPostResume ");
		HandleIntentData();
	}

	@Override
	public void onPause()
	{ 
		super.onPause(); 
		AppEventsLogger.deactivateApp(this);
		mIAP.UnRegPromoCodeListener();
	}

	void HidePopupDialogNavBar()
	{
		if (popupDialog != null)
		{
			if (popupDialog.isShowing())
			{
				popupDialog.HideNavBar();
			}
		}
	}

	public void FacebookUploadPic(final String filepath)
	{
		Log.d("FB", "FacebookUploadPic");
		AccessToken ast = AccessToken.getCurrentAccessToken();
		if (ast != null)
		{
			if (ShareDialog.canShow(SharePhotoContent.class))
			{
				/*
				BitmapFactory.Options options = new BitmapFactory.Options();
				options.inPreferredConfig = Bitmap.Config.ARGB_8888;
				options.inPurgeable = true;
				options.inInputShareable = true;
				Bitmap bitmap = BitmapFactory.decodeFile(getFilesDir() + "/" + filepath, options);
				GraphRequest request = GraphRequest.newUploadPhotoRequest(ast, bitmap,
				 new Request.Callback()
					{
						public void onCompleted(Response response)
						{
							Log.d("FB", response.toString());
						}
					}
				);
				
				//Bundle parameters = new Bundle();
				//parameters.putString("fields", fields);
				//request.setParameters(parameters);
				request.executeAsync();
				*/
			
				BitmapFactory.Options options = new BitmapFactory.Options();
				options.inPreferredConfig = Bitmap.Config.ARGB_8888;
				options.inPurgeable = true;
				options.inInputShareable = true;
				Bitmap bitmap = BitmapFactory.decodeFile(getFilesDir() + "/" + filepath, options);
				
				//Bitmap bitmap = Bitmap.createBitmap(64, 64, Bitmap.Config.ARGB_8888);
				//bitmap.eraseColor(Color.argb(255, 0, 0, 255));

				if (bitmap == null)
				{
					JNI.OnFinishFacebookUploadPic("onError");
					return;				
				}

				SharePhoto photo = new SharePhoto.Builder()
				.setBitmap(bitmap)
				.build();

				SharePhotoContent content = new SharePhotoContent.Builder()
					.addPhoto(photo)
					.build();

		
				shareDialog.registerCallback(callbackManager, new FacebookCallback<Sharer.Result>()
						{
							@Override
							public void onSuccess(Sharer.Result sharerResult)
							{
								Log.d("FB", "FacebookUploadPic onSuccess ");
								JNI.OnFinishFacebookUploadPic("onSuccess");
							}

							@Override
							public void onCancel()
							{
								Log.d("FB", "FacebookUploadPic onCancel");
								JNI.OnFinishFacebookUploadPic("onCancel");
							}

							@Override
							public void onError(FacebookException exception)
							{
								Log.d("FB", "FacebookUploadPic onError");
								JNI.OnFinishFacebookUploadPic("onError");
							}
						
						}	
				);

				shareDialog.show(this, content);
			}
		}		
	}

	public String GetFacebookAccessToken()
	{
		String Ret = "";
		Log.d("FB", "GetFacebookAccessToken");
		AccessToken ast = AccessToken.getCurrentAccessToken();
		if (ast != null)
		{
			Ret = ast.getToken();
		}
		
		Log.d("FB", "GetFacebookAccessToken =" + Ret);
		return Ret;	
	}

	public void TryFacebookUploadPic(final String filepath)
	{	
		boolean needsLogin = false;
		boolean alreadyLogin = false;
		
		AccessToken ast = AccessToken.getCurrentAccessToken();
		if (ast != null)
		{
			alreadyLogin = true;
		}
		else
		{
			needsLogin = true;
		}

		
		if (needsLogin)
		{
			Log.d("FB", "TryFacebookUploadPic");
			final boolean doThingsAfterLogin = alreadyLogin;
			LoginManager.getInstance().registerCallback(callbackManager, new FacebookCallback<LoginResult>()
			{
				@Override
				public void onSuccess(LoginResult loginResult)
				{
					Log.d("FB", "onSuccess");
					FacebookUploadPic(filepath);
				}

				@Override
				public void onCancel()
				{
					Log.d("FB", "onCancel");
					if (doThingsAfterLogin)
					{
						FacebookUploadPic(filepath);				
					}
					else
					{
						JNI.OnFinishFacebookUploadPic("onCancel");
					}
				}

				@Override
				public void onError(FacebookException exception)
				{
					Log.d("FB", "onError");
					JNI.OnFinishFacebookUploadPic("onError");
				}
			});
			LoginManager.getInstance().logInWithReadPermissions(this, Arrays.asList(""));
		}
		else
		{
			FacebookUploadPic(filepath);		
		}
	
	}

	public void FacebookShareLink(final String title, final String Desc, final String contenturl, final String imageurl)
	{
		Log.d("FB", "FaceBookShare");
		AccessToken ast = AccessToken.getCurrentAccessToken();
		if (ast != null)
		{
			if (ShareDialog.canShow(ShareLinkContent.class))
			{
				Log.d("FB", "canShow");
				ShareLinkContent linkContent = new ShareLinkContent.Builder()
					.setContentTitle(title)
					.setContentDescription(Desc)
					.setContentUrl(Uri.parse(contenturl))
					.setImageUrl(Uri.parse(imageurl))
					.build();

				shareDialog.registerCallback(callbackManager, new FacebookCallback<Sharer.Result>()
						{
							@Override
							public void onSuccess(Sharer.Result sharerResult)
							{
								Log.d("FB", "ShareLinkContent onSuccess");
								JNI.OnFinishFacebookShareLink("onSuccess");
							}

							@Override
							public void onCancel()
							{
								Log.d("FB", "ShareLinkContent onCancel");
								JNI.OnFinishFacebookShareLink("onCancel");
							}

							@Override
							public void onError(FacebookException exception)
							{
								Log.d("FB", "ShareLinkContent onError");
								JNI.OnFinishFacebookShareLink("onError");
							}
						
						}	
				);

				shareDialog.show(this, linkContent);
			}
		}
	}

	public void tryFacebookShareLink(final String title, final String Desc, final String contenturl, final String imageurl)
	{
		boolean needsLogin = false;
		boolean alreadyLogin = false;
		
		AccessToken ast = AccessToken.getCurrentAccessToken();
		if (ast != null)
		{
			alreadyLogin = true;
		}
		else
		{
			needsLogin = true;
		}

		
		if (needsLogin)
		{
			Log.d("FB", "TryFacebookUploadPic");
			final boolean doThingsAfterLogin = alreadyLogin;
			LoginManager.getInstance().registerCallback(callbackManager, new FacebookCallback<LoginResult>()
			{
				@Override
				public void onSuccess(LoginResult loginResult)
				{
					Log.d("FB", "onSuccess");
					FacebookShareLink(title, Desc, contenturl, imageurl);
				}

				@Override
				public void onCancel()
				{
					Log.d("FB", "onCancel");
					if (doThingsAfterLogin)
					{
						FacebookShareLink(title, Desc, contenturl, imageurl);		
					}
					else
					{
						JNI.OnFinishFacebookShareLink("onCancel");
					}
				}

				@Override
				public void onError(FacebookException exception)
				{
					Log.d("FB", "onError");
					JNI.OnFinishFacebookShareLink("onError");
				}
			});
			LoginManager.getInstance().logInWithReadPermissions(this, Arrays.asList(""));
		}
		else
		{
			FacebookShareLink(title, Desc, contenturl, imageurl);	
		}
	
	
	
	}

	public void ShareWithIntent(String msg, String imagePath, boolean isInternalPath)
	{
		String imgfullpath = "";
		
		Log.v("share", "ShareWithIntent msg =" + msg.toString());
		Log.v("share", "ShareWithIntent imagePath =" + imagePath.toString());
		if (imagePath != "")
		{
			String path = "";
			if (isInternalPath)
			{
				imgfullpath = getFilesDir() + "/" + imagePath;
			}
			else
			{				
				imgfullpath = getApplicationContext().getExternalFilesDir("").toString() + "/" +imagePath;

			}
		}

		ShareWithIntent.ShareImage(this, "", imgfullpath, "", "", msg, isInternalPath);
	
	}

	@Override
	public void onBackPressed()
	{
		Log.d("m_bOnBack", "m_bOnBack onBackPressed");
		JNI.OnBackPressed();
		//JNI.LOGI("NativeActivity::onBackPressed");

		//AddWebView("123", "http://www.google.com", 0, 0, 1000, 1000);
		//MoveView("123", 0, 500, 1000, 1000);
		
		//int margin = (int)(1152*0.05f);
		//OpenWebView("http://page.static.qubot.qubitgames.com/news", 192+margin, margin, 1344-margin, 2048-margin);
		//FaceBookShare("title", "desc", "http://qubitent.com/", "http://qubitent.com/images/games/game_QB.png" );
		//FacebookUploadPic("../texture/ui/loading/loading_00.png");
	}
	
	@Override
	protected void onActivityResult(final int requestCode, final int resultCode, final Intent data)
	{				
		super.onActivityResult(requestCode, resultCode, data);
	
		if (mIAP.HandleActivityResult(requestCode, resultCode, data)) {
			JNI.LOGI("onActivityResult handled by IAB.");
		}
		
        if (callbackManager.onActivityResult(requestCode, resultCode, data)){
            JNI.LOGI("onActivityResult handled by FB.");						
        }
		
		// webview upload.
		if(Build.VERSION.SDK_INT >= 21)
		{ 
            Uri[] results = null;
            //Check if response is positive
            if(resultCode== Activity.RESULT_OK){
                if(requestCode == FCR){
                    if(null == mUMA){
                        return;
                    }
                    if(data == null){
                        //Capture Photo if no image available
                        if(mCM != null){
                            results = new Uri[]{Uri.parse(mCM)};
                        }
                    }else{
                        String dataString = data.getDataString();
                        if(dataString != null){
                            results = new Uri[]{Uri.parse(dataString)};
                        }
                    }
                }
            }
			
			if(null != mUMA)
			{
				mUMA.onReceiveValue(results);
            }
            mUMA = null;
        }else{
            if(requestCode == FCR){
                if(null == mUM) return;
                Uri result = data == null || resultCode != RESULT_OK ? null : data.getData();
				mUM.onReceiveValue(result);				
                mUM = null;
            }
        }				
	}

	@Override 
	public void onWindowFocusChanged(boolean hasFocus)
	 {
		// TODO Auto-generated method stub 
		//Log.d("NAV", "onWindowFocusChanged");
		super.onWindowFocusChanged(hasFocus);
		DelayHideNavBar(NAVBAR_DELAY_HIDE_MILSEC);
		HidePopupDialogNavBar();
		/*
		if (hasFocus)
		{
			DelayHideNavBar(NAVBAR_DELAY_HIDE_MILSEC);
		}
		else
		{
			mHideSystemUiHandler.removeMessages(0);			
		}
		*/
	}

	public void HideNavBar()
	{	
		if (android.os.Build.VERSION.SDK_INT >= 19)
		{
			View decorView = getWindow().getDecorView();
			decorView.setSystemUiVisibility(
				View.SYSTEM_UI_FLAG_LAYOUT_STABLE
				| View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
				| View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
				| View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
				| View.SYSTEM_UI_FLAG_FULLSCREEN
				//| View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
				| View.SYSTEM_UI_FLAG_IMMERSIVE
			);
		}	
		
	}

	public String GetGameVersion() {
		String version = "0.0";
		try {
			version = getPackageManager().getPackageInfo(getPackageName(), 0).versionName;
		} catch (Exception e) {}
		return version;
	}

	// facebook 登出.
	void FacebookLogout()
	{	
        LoginManager.getInstance().logOut();
	}

	// 只做登入取得權限.
	void FacebookLoginWithPermission(final String permission)
	{
		if (!isNetworkAvailable())
		{
			return;
		}

		Log.d("FB", "LoginFacebookWithPermission ");
		boolean needsLogin = false;		
		AccessToken ast = AccessToken.getCurrentAccessToken();
		if (ast != null)
		{
			if (!HasPermissionFacebook(permission))
			{
				Log.d("FB", " Not HasPermissionFacebook ");
				needsLogin = true;
			}			
		}
		else
		{
			needsLogin = true;		
		}
	
		if (needsLogin)
		{
			Log.d("FB", "LoginFacebookWithPermission needsLogin");
			LoginManager.getInstance().registerCallback(callbackManager, new FacebookCallback<LoginResult>()
			{
				@Override
				public void onSuccess(LoginResult loginResult)
				{
					Log.d("FB", "onSuccess");
					JNI.OnFinishFacebookLogin("onSuccess");
				}

				@Override
				public void onCancel()
				{
					Log.d("FB", "onCancel");
					JNI.OnFinishFacebookLogin("onCancel");
				}

				@Override
				public void onError(FacebookException exception)
				{
					Log.d("FB", "onError");
					JNI.OnFinishFacebookLogin("onError");
				}
			});
			LoginManager.getInstance().logInWithReadPermissions(this, Arrays.asList(permission));
		}
	
	}

	// 要求 GraphAPI/me/ 底下的資料.
	void FacebookAccountDataRequest(final String permission, final String fields)
	{	
		AccessToken ast = AccessToken.getCurrentAccessToken();
		if (ast != null)
		{
			GraphRequest request = GraphRequest.newMeRequest(ast, new GraphRequest.GraphJSONObjectCallback()
			{
				@Override
				public void onCompleted(JSONObject object, GraphResponse response)
				{					
					if (object != null)
					{
						Log.v("FB", "newMeRequest:" + object.toString());
						JNI.OnFinishQueryFacebookAccountData(object.toString());
					}
					else
					{
						//FacebookRequestError Fre = response.getError();
						//if (Fre != null)
						//{
						//	Log.v("FB", "fberror code:" + Fre.getErrorCode() + " msg: "+Fre.getErrorMessage());
						//}

						JNI.OnFinishQueryFacebookAccountData("onError");					
					}
				}
			});
				
			Bundle parameters = new Bundle();
			parameters.putString("fields", fields);
			request.setParameters(parameters);
			request.executeAsync();
		}
	}

	
	// 登入並要求 GraphAPI/me/ 底下的資料.
	public void QueryFacebookAccountData(final String permission, final String fields)
	{
	
		boolean needsLogin = false;
		boolean alreadyLogin = false;
		
		AccessToken ast = AccessToken.getCurrentAccessToken();
		if (ast != null)
		{
			Log.d("FB", "already Login ");
			alreadyLogin = true;
			if (!HasPermissionFacebook(permission))
			{
				Log.d("FB", " Not HasPermissionFacebook ");
				needsLogin = true;
			}
			else
			{
				Log.d("FB", " FacebookAccountDataRequest ");
				FacebookAccountDataRequest(permission, fields);
			}

		}
		else
		{
			needsLogin = true;
		}

		
		if (needsLogin)
		{
			Log.d("FB", "QueryFacebookAccountData");
			// 己登入,再去要權限時, 只會回傳 onCancel.
			final boolean doThingsAfterLogin = alreadyLogin;
			LoginManager.getInstance().registerCallback(callbackManager, new FacebookCallback<LoginResult>()
			{
				@Override
				public void onSuccess(LoginResult loginResult)
				{
					Log.d("FB", "onSuccess");
					FacebookAccountDataRequest(permission, fields);
				}

				@Override
				public void onCancel()
				{
					Log.d("FB", "onCancel");
					if (doThingsAfterLogin)
					{
						FacebookAccountDataRequest(permission, fields);						
					}
					else
					{
						JNI.OnFinishQueryFacebookAccountData("onCancel");
					}
				}

				@Override
				public void onError(FacebookException exception)
				{
					Log.d("FB", "onError");
					JNI.OnFinishQueryFacebookAccountData("onError");
					// App code   
				}
			});
			LoginManager.getInstance().logInWithReadPermissions(this, Arrays.asList(permission));
		}
	}
	
	// 要求 me/frineds 的資料.
	void FacebookFriendsDataRequest(final String permission, final String fields)
	{
		if (!isNetworkAvailable())
		{
			JNI.OnFinishQueryFacebookFriendsData("onError");
			return;
		}

		AccessToken ast = AccessToken.getCurrentAccessToken();
		if (ast != null)
		{

			GraphRequest request = GraphRequest.newMyFriendsRequest(ast, new GraphRequest.GraphJSONArrayCallback()
			{
				@Override
				public void onCompleted(JSONArray jsonArray, GraphResponse response)
				{
					Log.d("FB", jsonArray.toString());
					JNI.OnFinishQueryFacebookFriendsData(jsonArray.toString());
				}
			});
				
			Bundle parameters = new Bundle();
			if (fields != "")
			{
				parameters.putString("fields", fields);
				request.setParameters(parameters);
			}
			request.executeAsync();	
		}
	}

	// 檢查 FB 玩家給我們遊戲的權限.
	boolean HasPermissionFacebook(final String permission)
	{
		AccessToken ast = AccessToken.getCurrentAccessToken();

		if (ast == null)
		{
			return false;
		}
	
		Set<String> perms = ast.getPermissions();
		List<String> wants = Arrays.asList(permission.split(","));
		
		for (String string : perms)
		{
			Log.d("FB", "have " + string);
		}


		for (String string : wants)
		{
			Log.d("FB", "want " + string);

			if (!perms.contains(string) && string != "")
			{
				return false;
			}
		}

		return true;
	}

	// 登入並取得有在玩本遊戲的玩家朋友資料.
	public void QueryFacebookFriendsData(final String permission, final String fields)
	{
		boolean needsLogin = false;
		boolean alreadyLogin = false;
		
		AccessToken ast = AccessToken.getCurrentAccessToken();
		if (ast != null)
		{
			Log.d("FB", "already Login ");
			alreadyLogin = true;
			if (!HasPermissionFacebook(permission))
			{
				Log.d("FB", " Not HasPermissionFacebook ");
				needsLogin = true;
			}
			else
			{
				Log.d("FB", " FacebookFriendsDataRequest ");
				FacebookFriendsDataRequest(permission, fields);
			}
		}
		else
		{
			needsLogin = true;		
		}


		if (needsLogin)
		{
			Log.d("FB", "QueryFacebookFriendsData");
			// 己登入,再去要權限時, 只會回傳 onCancel.
			final boolean doThingsAfterLogin = alreadyLogin;
			LoginManager.getInstance().registerCallback(callbackManager, new FacebookCallback<LoginResult>()
			{
				@Override
				public void onSuccess(LoginResult loginResult)
				{
					Log.d("FB", "onSuccess");
					FacebookFriendsDataRequest(permission, fields);
				}

				@Override
				public void onCancel()
				{
					Log.d("FB", "onCancel");
					if (doThingsAfterLogin)
					{
						FacebookFriendsDataRequest(permission, fields);						
					}
					else
					{
						JNI.OnFinishQueryFacebookFriendsData("onCancel");					
					}

				}

				@Override
				public void onError(FacebookException exception)
				{
					Log.d("FB", "onError");
					JNI.OnFinishQueryFacebookFriendsData("onError");
					// App code   
				}
			});
			LoginManager.getInstance().logInWithReadPermissions(this, Arrays.asList(permission));
		}
	}
	/**
	 * Override input event from native is possible, but it will use
	 * private(hide) android class (ref:http://blog.csdn.net/wise2008/article/details/9129341)
	 * and rebuild classes.jar with private android class
	 **
	@Override
	public void onInputQueueCreated(InputQueue queue) {
		Log.i(tag, "onInputQueueCreated");
		
		InputChannel mInputChannel=queue.getInputChannel();
		InputQueue mInputQueue=queue;
		InputQueue.registerInputChannel(mInputChannel, mInputHandler, Looper.myQueue());
		
		super.onInputQueueCreated(queue);
    }

    @Override
    public void onInputQueueDestroyed(InputQueue queue) {
        Log.i(tag, "onInputQueueDestroyed");
		super.onInputQueueDestroyed(queue);
    }
	**/

	/*public boolean onTouch(View view, MotionEvent event) {
		final int pointerCount = event.getPointerCount();
		final int action = event.getAction();
		final int actionMasked = event.getActionMasked();
		JNI.LOGI("NativeActivity::onTouch, pointer[" + pointerCount + "], action=" 
			+ action + " , actionMasked = " + actionMasked);

		return mGestureDetector.onTouchEvent(event);
	}*/

	public boolean onTouchEvent(MotionEvent event)
	{
		final int pointerCount = event.getPointerCount();
		final int action = event.getAction();
		
		/*if (action == MotionEvent.ACTION_UP)
			JNI.OnTouch(MotionEvent.ACTION_UP, event);*/
		switch (action)
		{
			case MotionEvent.ACTION_UP:
			case MotionEvent.ACTION_CANCEL:
			case MotionEvent.ACTION_MOVE:
			case MotionEvent.ACTION_DOWN:
				JNI.OnTouchEvent(action, event);
				break;
		}
		mGestureDetector.onTouchEvent(event);
		
		
		switch (action)
		{
			case MotionEvent.ACTION_DOWN:
				HideNavBar();
				break;
		}

		return false; 
	}
	

	/*
	 * implement OpenURL from native to Java
	 */
	/*public void OpenURL(final String _URL, 
						final String _Title, 
	                    final String _ButtonTitle,
	                    int _Left, int _Top, int _Right, int _Bottom,
	                    int _ButtonWidth, int _ButtonHeight,
						int _BackgroundColorRGB,
						int _BackgroundColorAlpha)
	{
		Intent intent = new Intent(NativeActivity.sNativeActivity, WebViewActivity.class); 
		intent.putExtra(WebViewActivity.INTENT_PARAM_URL,			 _URL);
		intent.putExtra(WebViewActivity.INTENT_TITLE_TEXT,			 _Title);
		intent.putExtra(WebViewActivity.INTENT_BUTTON_TEXT,			 _ButtonTitle);
		intent.putExtra(WebViewActivity.INTENT_PARAM_VIEW_LEFT,		 _Left);
		intent.putExtra(WebViewActivity.INTENT_PARAM_VIEW_TOP,		 _Top);
		intent.putExtra(WebViewActivity.INTENT_PARAM_VIEW_RIGHT,		 _Right);
		intent.putExtra(WebViewActivity.INTENT_PARAM_VIEW_BOTTOM,	 _Bottom);
		intent.putExtra(WebViewActivity.INTENT_PARAM_BUTTON_W,		 _ButtonWidth);
		intent.putExtra(WebViewActivity.INTENT_PARAM_BUTTON_H,        _ButtonHeight);
		intent.putExtra(WebViewActivity.INTENT_PARAM_BACKGROUNDCOLOR_RGB, _BackgroundColorRGB);
		intent.putExtra(WebViewActivity.INTENT_PARAM_BACKGROUNDCOLOR_ALPHA, _BackgroundColorAlpha);

		NativeActivity.sNativeActivity.startActivity(intent);
	}*/

	public void OpenURL(String url) {
		Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
		
        PackageManager packageManager = getPackageManager();
        if (intent.resolveActivity(packageManager) != null)
		{
			startActivity(intent);
        }
		else
		{
            Log.d("OpenURL", "Cannot handle this intent");
        }
		
	}
	
	public Typeface getTypeface (String name) {
		Typeface typeface = null;
		try {
			typeface = Typeface.createFromFile(getFilesDir() + "/font/" + name);
		} catch (Exception ex) {}
		try {
			typeface = Typeface.createFromAsset(getAssets(), "font/" + name);
		} catch (Exception ex) {}
		return typeface;
	}

	public void SavePNG (String name, byte[] data, int length, int width, int height) {
		ByteBuffer buffer = ByteBuffer.wrap(data);
		Bitmap bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
		bitmap.copyPixelsFromBuffer(buffer);
		Matrix matrix = new Matrix();
		matrix.preScale(1, -1);
		bitmap = Bitmap.createBitmap(bitmap, 0, 0, width, height, matrix, false);

		name = NativeActivity.EmojiFilter(name);

		String Filename;		
		if (name.charAt(0) == '.' && name.charAt(1) == '.' && name.charAt(2) == '/')
			Filename = name.substring(3);
		else if (name.charAt(0) == '.' && name.charAt(1) == '/')
			Filename = name.substring(2);
		else
			Filename = name;
		
		File file = new File(getFilesDir() + "/" + Filename);
		file.getParentFile().mkdirs();
		FileOutputStream out = null;
		try {
			out = new FileOutputStream(file);
			bitmap.compress(Bitmap.CompressFormat.PNG, 100, out);
		} catch (IOException ex) {
			return;
		} finally {
			try {
				if (out != null) {
					out.close();
				}
			} catch (IOException e) {
				return;
			}
		}
	}

	public void MakeDirectories (String name)
	{
		File file = new File(getFilesDir() + "/" + name);
		file.getParentFile().mkdirs();
	}

	public void OpenEditText (final String name, final String str, final long ptr) {
		runOnUiThread(new Runnable()  {
			@Override
			public void run()  {				

				final EditText editText = new EditText(sNativeActivity);
				//viewMap.put(name, editText);//遊戲不會主動呼叫 ReleaseView, 所以不用加入.
				editText.setSingleLine();
				editText.setTextSize(24);
				editText.setBackgroundColor(android.graphics.Color.WHITE);
				editText.setImeOptions(EditorInfo.IME_ACTION_DONE);

				final InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);

				final Button button = new Button(sNativeActivity);
				button.setBackgroundColor(android.graphics.Color.TRANSPARENT);

				viewLayout.addView(button, new RelativeLayout.LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT));
				RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.WRAP_CONTENT);
				//params.addRule(RelativeLayout.CENTER_IN_PARENT); 
				viewLayout.addView(editText, params);

				button.setOnClickListener(new View.OnClickListener() {
					public void onClick(View v)
					{
						//有些機會當機, 改成讓 onFocusChange 時移 view 就好.
						/*
						SafeLayoutRemoveView(viewLayout, editText);
						SafeLayoutRemoveView(viewLayout, button);
						*/
						popupDialog.CloseDialog();
					}
				});

				editText.postDelayed(new Runnable()
				{
					@Override
					public void run()
					{
						editText.requestFocus();
						imm.showSoftInput(editText, InputMethodManager.SHOW_IMPLICIT);
						editText.setOnFocusChangeListener(new OnFocusChangeListener() {
							@Override
							public void onFocusChange(View v, boolean hasFocus)
							{
								/*if (hasFocus) {
									editText.setPadding(0, 0, 0, viewLayout.getRootView().getHeight()- viewLayout.getHeight());
								}*/
								if (!hasFocus)
								{
									JNI.UpdateText(ptr, null);
									SafeLayoutRemoveView(viewLayout, editText);
									SafeLayoutRemoveView(viewLayout, button);

									//popupWindow.setFocusable(false);
									popupDialog.CloseDialog();
								}

								//if (!hasFocus && android.os.Build.VERSION.SDK_INT >= 19)	
								HideNavBar();
							}
						});
					}
				}, 100);
				editText.append(str); 

				editText.setOnEditorActionListener(new TextView.OnEditorActionListener() { 
					@Override 
					public boolean onEditorAction(TextView v, int actionId, KeyEvent event) { 
						if (actionId == EditorInfo.IME_ACTION_DONE)
						{						
							String newstr = EmojiFilter(editText.getText().toString());
							//String newstr = editText.getText().toString();
							newstr = PercentFilter(newstr);
							JNI.UpdateText(ptr, newstr);
							//popupDialog.setFocusable(false);
							popupDialog.CloseDialog();
						}
						return false;
					}
				});

				editText.addTextChangedListener(new TextWatcher()
				{
					@Override
					public void afterTextChanged(Editable s)
					{
		
					}

					@Override    
					public void beforeTextChanged(CharSequence s, int start,
						int count, int after)
					{
					}
					@Override    
					public void onTextChanged(CharSequence s, int start, int before, int count)
					{
						//Log.d("emo", " start="+ start + " before=" + before + " count:" + count);
						//editText.removeTextChangedListener(this);
						String newstr = EmojiFilter(editText.getText().toString());						
						//String newstr = editText.getText().toString();						
						//editText.setText(newstr);
						//editText.setSelection(newstr.length());					
						//editText.addTextChangedListener(this);
						newstr = PercentFilter(newstr);
						JNI.UpdateText(ptr, newstr);
					}

				});

				popupDialog.showAtLocation(0, 0);
				//popupWindow.update();
			}
		});
	}

	public String ToHex(String arg)
	{
		return String.format("%x", new BigInteger(1, arg.getBytes()));
	}
	
	static String PercentFilter(String text)
	{
        return text.replace("\u0025", "\u0025\u0025");
	}

	static String EmojiFilter(String text)
	{
		Matcher unicodeOutlierMatcher = unicodeOutliers.matcher(text);
        return unicodeOutlierMatcher.replaceAll("\u2605");
	}
	
	void SafeLayoutRemoveView(ViewGroup vg, View taraget)
	{
		if(vg.indexOfChild(taraget) != -1)
		{
			//Log.d("crash", "ViewGroup has view");
			vg.removeView(taraget);
		}

	}

	public void AddAnimationView (final String name, final String filename, final String ext, final int num, final int dur, final int x1, final int y1, final int x2, final int y2) {
		runOnUiThread(new Runnable()  {
			@Override
			public void run()  {
				ImageView imgView = new ImageView(sNativeActivity);
				viewMap.put(name, imgView);
				AnimationDrawable anim = new AnimationDrawable();
				anim.setOneShot(false);
				imgView.setBackground(anim);
				for (int i=0; i<num; i++) {
					Drawable d = null;
					try {
						 d = Drawable.createFromPath(getFilesDir() + "/" + filename + String.format("%02d", i) + "." + ext);
					}
					/*if (d == null)*/
					catch (Exception ex) {;}
					if (d == null) {
						try {
							d = Drawable.createFromStream(getAssets().open(filename + String.format("%02d", i) + "." + ext), null);
						}
						catch (Exception ex) {;}
					}
					if (d != null)
						anim.addFrame(d, dur);
				}
				if (anim.isRunning()){
					anim.stop();
				}
				
				anim.start();

				imgView.setX(x1);
				imgView.setY(y1);
				viewLayout.addView(imgView, new LinearLayout.LayoutParams(x2-x1, y2-y1));

				popupDialog.showAtLocation(0, 0);

				//popupWindow.update();
			}
		});
	}

	public void OpenWebView (final String url, final int x1, final int y1, final int x2, final int y2) {
		runOnUiThread(new Runnable()  {
			@Override
			public void run()  {
				final PopupWindow popupForWebView = new PopupWindow(sNativeActivity);
				popupForWebView.setWindowLayoutMode(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT);
				popupForWebView.setClippingEnabled(false);
				popupForWebView.setBackgroundDrawable(new ColorDrawable(android.graphics.Color.TRANSPARENT));

				LinearLayout root = new LinearLayout(sNativeActivity);

				LinearLayout layout = new LinearLayout(sNativeActivity);
				layout.setOrientation(LinearLayout.VERTICAL);
				layout.setBackgroundColor(android.graphics.Color.GRAY);

				RelativeLayout topBar = new RelativeLayout(sNativeActivity);

				final WebView webView = new WebView(sNativeActivity);
				

				webView.getSettings().setJavaScriptEnabled(true);
				webView.getSettings().setDefaultZoom(WebSettings.ZoomDensity.FAR);
				//webView.getSettings().setSupportZoom(true);
				webView.getSettings().setUseWideViewPort(true);
				//webView.requestFocus(View.FOCUS_DOWN);				
				webView.getSettings().setJavaScriptEnabled(true);
				webView.getSettings().setAllowFileAccess(true);
				
				if(Build.VERSION.SDK_INT >= 21){
					webView.getSettings().setMixedContentMode(0);
					webView.setLayerType(View.LAYER_TYPE_HARDWARE, null);
				}else if(Build.VERSION.SDK_INT >= 19){
					webView.setLayerType(View.LAYER_TYPE_HARDWARE, null);
				}else if(Build.VERSION.SDK_INT >=11 && Build.VERSION.SDK_INT < 19){
					webView.setLayerType(View.LAYER_TYPE_SOFTWARE, null);
				}

				webView.loadUrl(url);
				webView.setWebViewClient(new WebViewClient() {
					@Override
					public boolean shouldOverrideUrlLoading(WebView view, String url) {
						if(url.startsWith("qubot"))
						{
							HandleURL(url);
						}
						else
						{
							view.loadUrl(url);
						}
						return true;
					}
					
					public void onReceivedError(WebView view, int errorCode, String description, String failingUrl)
					{
						Toast.makeText(getApplicationContext(), "Failed loading app!", Toast.LENGTH_SHORT).show();
					}
				});

				
				webView.setWebChromeClient(new WebChromeClient(){
					//For Android 3.0+
					public void openFileChooser(ValueCallback<Uri> uploadMsg){
						mUM = uploadMsg;
						Intent i = new Intent(Intent.ACTION_GET_CONTENT);
						i.addCategory(Intent.CATEGORY_OPENABLE);
						i.setType("*/*");
						startActivityForResult(Intent.createChooser(i,"File Chooser"), FCR);
					}
					// For Android 3.0+, above method not supported in some android 3+ versions, in such case we use this
					public void openFileChooser(ValueCallback uploadMsg, String acceptType){
						mUM = uploadMsg;
						Intent i = new Intent(Intent.ACTION_GET_CONTENT);
						i.addCategory(Intent.CATEGORY_OPENABLE);
						i.setType("*/*");
						startActivityForResult(
								Intent.createChooser(i, "File Browser"),
								FCR);
					}
					//For Android 4.1+
					public void openFileChooser(ValueCallback<Uri> uploadMsg, String acceptType, String capture){
						mUM = uploadMsg;
						Intent i = new Intent(Intent.ACTION_GET_CONTENT);
						i.addCategory(Intent.CATEGORY_OPENABLE);
						i.setType("*/*");
						startActivityForResult(Intent.createChooser(i, "File Chooser"), FCR);
					}
					//For Android 5.0+
					public boolean onShowFileChooser(
							WebView webView, ValueCallback<Uri[]> filePathCallback,
							WebChromeClient.FileChooserParams fileChooserParams){
						if(mUMA != null){
							mUMA.onReceiveValue(null);
						}
						mUMA = filePathCallback;
						Intent takePictureIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
						if(takePictureIntent.resolveActivity(getPackageManager()) != null){
							File photoFile = null;
							try{
								photoFile = createImageFile();
								takePictureIntent.putExtra("PhotoPath", mCM);
							}catch(IOException ex){
								Log.e("upload", "Image file creation failed", ex);
							}
							if(photoFile != null){
								mCM = "file:" + photoFile.getAbsolutePath();
								takePictureIntent.putExtra(MediaStore.EXTRA_OUTPUT, Uri.fromFile(photoFile));
							}else{
								takePictureIntent = null;
							}
						}
						Intent contentSelectionIntent = new Intent(Intent.ACTION_GET_CONTENT);
						contentSelectionIntent.addCategory(Intent.CATEGORY_OPENABLE);
						contentSelectionIntent.setType("*/*");
						Intent[] intentArray;
						if(takePictureIntent != null){
							intentArray = new Intent[]{takePictureIntent};
						}else{
							intentArray = new Intent[0];
						}

						Intent chooserIntent = new Intent(Intent.ACTION_CHOOSER);
						chooserIntent.putExtra(Intent.EXTRA_INTENT, contentSelectionIntent);
						chooserIntent.putExtra(Intent.EXTRA_TITLE, "Image Chooser");
						chooserIntent.putExtra(Intent.EXTRA_INITIAL_INTENTS, intentArray);
						startActivityForResult(chooserIntent, FCR);
						return true;
					}
				});




				Button BackButton = new Button(sNativeActivity);
				RelativeLayout.LayoutParams BackParams = new RelativeLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
				BackButton.setBackgroundColor(android.graphics.Color.TRANSPARENT);
				BackButton.setText("Back");
				BackButton.setOnClickListener(new View.OnClickListener() {
					public void onClick(View v) {
						if (webView.canGoBack()) {
							webView.goBack();
						}
					}
				});

				TextView Title = new TextView(sNativeActivity);
				RelativeLayout.LayoutParams TitleParams = new RelativeLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
				TitleParams.addRule(RelativeLayout.CENTER_IN_PARENT);
				Title.setText("TITLE");
				Title.setTextSize(36);

				Button CloseButton = new Button(sNativeActivity);
				RelativeLayout.LayoutParams CloseParams = new RelativeLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
				CloseParams.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
				CloseButton.setBackgroundColor(android.graphics.Color.TRANSPARENT);
				CloseButton.setText("OK");
				CloseButton.setOnClickListener(new View.OnClickListener() {
					public void onClick(View v) {
						popupForWebView.dismiss();
					}
				});

				/*webView.setX(x1);
				webView.setY(y1);
				layout.addView(webView, new LinearLayout.LayoutParams(x2-x1, (int)((y2)*7/8f)));
				button.setX(x1);
				//button.setY(y1 + (int)((y2-y1)*6/7f));
				layout.addView(button, new LinearLayout.LayoutParams(x2-x1, (int)((y2)*1/8f)));*/
				layout.setX(x1);
				layout.setY(y1);
				root.addView(layout, new LinearLayout.LayoutParams(x2-x1, y2-y1));

				layout.addView(topBar, new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
				topBar.addView(BackButton, BackParams);
				topBar.addView(Title, TitleParams);
				topBar.addView(CloseButton, CloseParams);
				layout.addView(webView, new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
				
				popupForWebView.setContentView(root);
				popupForWebView.showAtLocation(mainLayout, Gravity.NO_GRAVITY, 0, 0);
				popupForWebView.update();
			}
		});
	}

	public void AddWebView (final String name, final String htmlOrUrl, final int x1, final int y1, final int x2, final int y2, final boolean bNotFullScreen) {
		runOnUiThread(new Runnable()  {
			@Override
			public void run()  {
				
				View findview = viewMap.get(name);
				if (findview != null)
				{
					Log.d("Qubot", "AddWebView duplicate name in viewMap:" + name);
				}
				else
				{
					//popupWindow.setFocusable(bfocus);

					final WebView webView = new WebView(sNativeActivity);
					webView.setWebViewClient(new WebViewClient()  {
						@Override
						public boolean shouldOverrideUrlLoading(WebView view, String url) {
							if(url.startsWith("qubot"))
							{
								HandleURL(url);
							}
							else
							{
								webView.loadUrl(url);
							}
							return true;
						}
						
						public void onReceivedError(WebView view, int errorCode, String description, String failingUrl){
							Toast.makeText(getApplicationContext(), "Failed loading app!", Toast.LENGTH_SHORT).show();
						}

					});
				

					webView.setWebChromeClient(new WebChromeClient(){
						//For Android 3.0+
						public void openFileChooser(ValueCallback<Uri> uploadMsg){
							mUM = uploadMsg;
							Intent i = new Intent(Intent.ACTION_GET_CONTENT);
							i.addCategory(Intent.CATEGORY_OPENABLE);
							i.setType("*/*");
							startActivityForResult(Intent.createChooser(i,"File Chooser"), FCR);
						}
						// For Android 3.0+, above method not supported in some android 3+ versions, in such case we use this
						public void openFileChooser(ValueCallback uploadMsg, String acceptType){
							mUM = uploadMsg;
							Intent i = new Intent(Intent.ACTION_GET_CONTENT);
							i.addCategory(Intent.CATEGORY_OPENABLE);
							i.setType("*/*");
							startActivityForResult(
									Intent.createChooser(i, "File Browser"),
									FCR);
						}
						//For Android 4.1+
						public void openFileChooser(ValueCallback<Uri> uploadMsg, String acceptType, String capture){
							mUM = uploadMsg;
							Intent i = new Intent(Intent.ACTION_GET_CONTENT);
							i.addCategory(Intent.CATEGORY_OPENABLE);
							i.setType("*/*");
							startActivityForResult(Intent.createChooser(i, "File Chooser"), FCR);
						}
						//For Android 5.0+
						public boolean onShowFileChooser(
								WebView webView, ValueCallback<Uri[]> filePathCallback,
								WebChromeClient.FileChooserParams fileChooserParams){
							if(mUMA != null){
								mUMA.onReceiveValue(null);
							}
							mUMA = filePathCallback;
							Intent takePictureIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
							if(takePictureIntent.resolveActivity(getPackageManager()) != null){
								File photoFile = null;
								try{
									photoFile = createImageFile();
									takePictureIntent.putExtra("PhotoPath", mCM);
								}catch(IOException ex){
									Log.e("upload", "Image file creation failed", ex);
								}
								if(photoFile != null){
									mCM = "file:" + photoFile.getAbsolutePath();
									takePictureIntent.putExtra(MediaStore.EXTRA_OUTPUT, Uri.fromFile(photoFile));
								}else{
									takePictureIntent = null;
								}
							}
							Intent contentSelectionIntent = new Intent(Intent.ACTION_GET_CONTENT);
							contentSelectionIntent.addCategory(Intent.CATEGORY_OPENABLE);
							contentSelectionIntent.setType("*/*");
							Intent[] intentArray;
							if(takePictureIntent != null){
								intentArray = new Intent[]{takePictureIntent};
							}else{
								intentArray = new Intent[0];
							}

							Intent chooserIntent = new Intent(Intent.ACTION_CHOOSER);
							chooserIntent.putExtra(Intent.EXTRA_INTENT, contentSelectionIntent);
							chooserIntent.putExtra(Intent.EXTRA_TITLE, "Image Chooser");
							chooserIntent.putExtra(Intent.EXTRA_INITIAL_INTENTS, intentArray);
							startActivityForResult(chooserIntent, FCR);
							return true;
						}
					});

					webView.setBackgroundColor(android.graphics.Color.TRANSPARENT);
					webView.setOnTouchListener(      
						new View.OnTouchListener()
						{
							public boolean onTouch(View v, MotionEvent event)
							{
								switch (event.getAction())
								{
									case 0: 
									case 1: 
									if (!v.hasFocus())
									{
										v.requestFocus();
									}
									HidePopupDialogNavBar();
								break;
							}
							return false;
						}
					});


					viewMap.put(name, webView);
					webView.setFocusable(true);
					webView.setLongClickable(true);

					webView.getSettings().setJavaScriptEnabled(true);
					webView.getSettings().setDefaultZoom(WebSettings.ZoomDensity.FAR);
					/*webView.getSettings().setBuiltInZoomControls(true);
					webView.getSettings().setDisplayZoomControls(false);
					webView.getSettings().setSupportZoom(true);*/					

					if(Build.VERSION.SDK_INT >= 21)
					{
						webView.getSettings().setMixedContentMode(0);
						webView.setLayerType(View.LAYER_TYPE_HARDWARE, null);
					}
					else if(Build.VERSION.SDK_INT >= 19)
					{
						webView.setLayerType(View.LAYER_TYPE_HARDWARE, null);
					}
					else if(Build.VERSION.SDK_INT >=11 && Build.VERSION.SDK_INT < 19)
					{
						webView.setLayerType(View.LAYER_TYPE_SOFTWARE, null);
					}
					webView.getSettings().setDatabaseEnabled(true);
					webView.getSettings().setDomStorageEnabled(true);
					webView.getSettings().setAllowFileAccess(true);
					webView.getSettings().setGeolocationEnabled(true);
					if (android.os.Build.VERSION.SDK_INT >= 8)
					{
						webView.getSettings().setPluginState(WebSettings.PluginState.ON);
					}
					if (android.os.Build.VERSION.SDK_INT >= 11)
					{
						webView.getSettings().setDisplayZoomControls(false);
					}
					if (android.os.Build.VERSION.SDK_INT >= 16)
					{
						webView.getSettings().setAllowFileAccessFromFileURLs(true);
						webView.getSettings().setAllowUniversalAccessFromFileURLs(true);
					}
					webView.getSettings().setUseWideViewPort(true);	

					/*
					webView.setHapticFeedbackEnabled(false);
					webView.setOnLongClickListener(new OnLongClickListener()
						{
							@Override
							public boolean onLongClick(View v)
							{
								Log.d("crash", "");
								return true;
							}
						}
					);*/


					if (htmlOrUrl.startsWith("http"))
					{
						webView.loadUrl(htmlOrUrl);
					}
					else
					{						
						String htmlString = "<!DOCTYPE html><html lang=\"en\">"							   
							   + "<head><meta charset=\"utf-8\">"
							   + "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">"
							   + "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no\">"
							   + "<meta name=\"description\" content=\"Qubit Entertainment Ltd.\">"
							   + "<meta name=\"author\" content=\"Qubit Entertainment Ltd.\">"
							   + "<title>Qubot - Qubit Entertainment Ltd.</title></head>"
							   + "<body>"
							   + htmlOrUrl
							   + "</body></html>";
						
						webView.loadDataWithBaseURL(null, htmlString, "text/html", "utf-8", null);
						//webView.loadData(htmlOrUrl, "text/html", null);
					}

					webView.setBackgroundColor(Color.TRANSPARENT);
					
					if (bNotFullScreen)
					{
						webView.setX(0);
						webView.setY(0);
					}
					else
					{
						webView.setX(x1);
						webView.setY(y1);
					}
					
					viewLayout.addView(webView, new RelativeLayout.LayoutParams(x2-x1, y2-y1));
				}


				//popupWindow.showAtLocation(viewLayout, Gravity.NO_GRAVITY, 0, 0);
				//popupWindow.update();

				if (bNotFullScreen)
				{
					popupDialog.showAtLocationHeightSubNavbar(x1, y1, x2 - x1, y2 - y1);
				}
				else
				{
					popupDialog.showAtLocation(0, 0);
				}

			}
		});
	}

	void HandleIntentData()
	{
		Uri datauri = getIntent().getData();
		if (datauri != null)
		{
			for(String key : datauri.getQueryParameterNames())
			{
				String value = datauri.getQueryParameter(key);
				Log.d("parm", "key=" + key + " val=" + value);
				JNI.HandleURL(key, value);
			}

			//don't clear here. someone still need data.
			//getIntent().setData(null);
		}
	}

	void HandleURL(String url) {
		try
		{
			Uri uri = Uri.parse(url);
			/*String displayMode = uri.getQueryParameter("displayMode");
			JNI.GameHomePageGoTo(displayMode);
			Toast toast = Toast.makeText(sNativeActivity, displayMode, Toast.LENGTH_LONG);
			toast.show();*/
			for(String key : uri.getQueryParameterNames()) {
				String value = uri.getQueryParameter(key);
				JNI.HandleURL(key, value);
			}
		}
		catch (Exception e)
		{
			Log.d("", "Webview Error:" + e.getMessage());;
		}
	}

	public void ClosePopupWindow () {
		runOnUiThread(new Runnable()  {
			@Override
			public void run()  {
				if (popupDialog != null) {
					for(Map.Entry<String, View> entry : viewMap.entrySet()) {
						entry.getValue().setBackground(null);
						viewLayout.removeView(entry.getValue());
					}
					/*viewLayout.removeAllViews();*/
					//viewLayout = new LinearLayout(sNativeActivity);
					viewMap.clear();
					popupDialog.CloseDialog();
				}
			}
		});
	}

	public void MoveView (final String name, final int x1, final int y1, final int x2, final int y2, final boolean notFullScreen) {
		runOnUiThread(new Runnable()  {
			@Override
			public void run()  {
				View view = viewMap.get(name);
				if (view != null)
				{
					//Log.d("web", "MoveView " + name);
					
					// 自動判斷易誤判,還是改成由使用者控制.
					/*
					boolean notFullScreen = view instanceof WebView;
					*/
					if (notFullScreen)
					{						
						view.setX(0);
						view.setY(0);						
						popupDialog.SetPosition(x1, y1, x2 - x1, y2 - y1);
						// view.layout(x1, y1, x2 - x1, y2 - y1);
						// popupDialog.getWindow().addFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE);
					}
					else
					{
						view.setX(x1);
						view.setY(y1);					
					}

					//Log.d("web", "MoveView " + name + "," + x1 + "," + y1 + "," + (x2 - x1) + "," + (y2 - y1));
					
					RelativeLayout.LayoutParams params = (RelativeLayout.LayoutParams)view.getLayoutParams();
					params.width = x2-x1;
					params.height = y2-y1;
					view.setLayoutParams(params);
				}
			}
		});
	}
	
	public void ShowView (final String name, final boolean bShow) {
		runOnUiThread(new Runnable()  {
			@Override
			public void run()  {
				if (bShow) {
					viewMap.get(name).setVisibility(View.VISIBLE);
					popupDialog.showAtLocation(0, 0);
				}
				else {
					viewMap.get(name).setVisibility(View.GONE);
					boolean flag = true;
					for(Map.Entry<String, View> entry : viewMap.entrySet()) {
						if (entry.getValue().getVisibility() == View.VISIBLE) {
							flag = false;
							break;
						}
					}
					if (flag) {
						popupDialog.CloseDialog();
					}
				}
			}
		});
	}

	public void RemoveView (final String name) {
		runOnUiThread(new Runnable()  {
			@Override
			public void run()  {
                if (viewMap.get(name) != null) {
				    viewLayout.removeView(viewMap.get(name));
                }
				boolean flag = true;
				for(Map.Entry<String, View> entry : viewMap.entrySet()) {
					if (entry.getValue().getVisibility() == View.VISIBLE) {
						flag = false;
						break;
					}
				}
				if (flag)
					popupDialog.CloseDialog();
			}
		});
	}

	public void ReleaseView (final String name) {
		runOnUiThread(new Runnable()  {
			@Override
			public void run()  {
                if (viewMap.get(name) != null) {
                    viewLayout.removeView(viewMap.get(name));
                    viewMap.remove(name);
                }
				if (popupDialog != null)
				{
					boolean flag = true;
					for(Map.Entry<String, View> entry : viewMap.entrySet())
					{
						if (entry.getValue().getVisibility() == View.VISIBLE)
						 {
							flag = false;
							break;
						}
					}
					if (flag)
					{
						popupDialog.CloseDialog();
					}
				}

			}
		});
	}

	public void AddColorBackground (final String name, final int color)
	{
		runOnUiThread(new Runnable()  {
			@Override
			public void run()  {
				View background = new View(sNativeActivity);
				viewMap.put(name, background);
				background.setBackgroundColor(color);
				viewLayout.addView(background, new LinearLayout.LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT));
			}
		});
	}
			
	/**
     * Implement QueryInventoryAsync from native to Java
	 *
	 * Need create a ui thread to process QueryInventory.
     */ 	
	public void QueryInventoryAsync(final String base64EncodedPublicKey, final String[] skus) {
		JNI.LOGI("QueryInventoryAsync");
						
		if (null != mIAP) {
			final List<String> skuList = Arrays.asList(skus);
			// final String publicKey = base64EncodedPublicKey;
			
			runOnUiThread(new Runnable() {
				@Override
				public void run()  {							
					mIAP.QueryInventory(sNativeActivity, base64EncodedPublicKey, skuList);
				}
			});
		}
	}

	/**
     * Implement LaunchPurchaseFlow from native to Java
	 *
	 * There's no necessary to create a ui thread. IabHelper use Intent to process purchase.
	 */
	public void LaunchPurchaseFlow(final String sku, final int requestCode, final String payload) {
		JNI.LOGI("LaunchPurchaseFlow");
		
		if (null != mIAP) {	
			mIAP.PurchaseInventory(sNativeActivity, sku, requestCode, payload);
		}		
	}

	/**
     * Implement ConsumePurchase from native to Java
	 *	 
	 */	
	public void ConsumePurchase()  {
		JNI.LOGI("ConsumePurchase");

		if (null != mIAP) {	
			mIAP.ConsumePurchase();
		}	
	}				
	
	/**
     * Implement ConsumePromoCodePurchase from native to Java
	 *	 
	 */	
	public void ConsumePromoCodePurchase()  {
		JNI.LOGI("ConsumePromoCodePurchase");

		if (null != mIAP) {	
			mIAP.ConsumePromoCodePurchase();
		}	
	}

	/**
     * Implement LocalNotification for add notification by tag, id
	 *
	 * @param
	 *      id       : ID for different game type notification.
	 *		title    : Title of notification.
	 *		message  : Context of notification.
	 *		ringtone : custom rinetone resource uri, use default ringtone if it's empty string.
	 *				   the resource of ringtone should locate at /res/raw.
	 *      interval : launch time in seconds for notification.
	 */	
	public void ScheduleLocalNotification(int id, String title, String message, String ringtone, int interval) {
		JNI.LOGI("ScheduleLocalNotification : " + id + "," + title + "," + message + "," + interval);
					
		PendingIntent sender = getPendingIntent(
					title, 
					message, 
					ringtone,
					NotificationReceiver.LYNXENGINE_ANDROID_NOTIFICATION_TAG, //tag					
					id);		
				
		Calendar calendar = Calendar.getInstance();
		calendar.setTimeInMillis(System.currentTimeMillis());
		calendar.add(Calendar.SECOND, interval);
        
        AlarmManager am = (AlarmManager)getSystemService(Context.ALARM_SERVICE);
        am.setExact(AlarmManager.RTC_WAKEUP, calendar.getTimeInMillis(), sender);
	}
	
	/**     
	 * Implement LocalNotification for cancel notification by tag, id
	 *
	 */	
	public void CancelLocalNotification(int id) {
		JNI.LOGI("CancelLocalNotification : id = " + id);
		
		PendingIntent sender = getPendingIntent(
					null, 
					null, 
					null, 
					NotificationReceiver.LYNXENGINE_ANDROID_NOTIFICATION_TAG, //tag
					id);
					
		AlarmManager am = (AlarmManager)getSystemService(Context.ALARM_SERVICE);
        am.cancel(sender);
		
		NotificationManager nm = (NotificationManager)getApplicationContext().getSystemService(Context.NOTIFICATION_SERVICE);
		nm.cancel(
			NotificationReceiver.LYNXENGINE_ANDROID_NOTIFICATION_TAG, //tag
			id);        
	}
	
	/**     
	 * Implement LocalNotification of getPendingIntent for construct intent extra parameters
	 *
	 */	
	private PendingIntent getPendingIntent(String title, String message, String ringtone, String tag, int id) {	
		Intent intent = new Intent("android.media.action.DISPLAY_NOTIFICATION");		
        intent.addCategory("android.intent.category.DEFAULT");		
		intent.putExtra("notify_tag"  		, tag    );
		intent.putExtra("notify_id"   		, id     );
		intent.putExtra("notify_title"		, title  );
		intent.putExtra("notify_msg"  		, message);
		intent.putExtra("notify_sound"  	, ringtone);
		
        PendingIntent sender = PendingIntent.getBroadcast(
			this, 
			id, // id as requestCode
			intent, 
			PendingIntent.FLAG_UPDATE_CURRENT);
			
		return sender;
	}
	
	/**
	 * implement OpenAlertDialog from native to Java
	 *
	 * The system will dismiss dialog when user touches any button of dialog
	 * except radio buttons or checkboxes. 
	 * http://developer.android.com/guide/topics/ui/dialogs.html#DismissingADialog
	 * if there's still problem for Runnable can not solve(such as leak or keep single instance), 
	 * consider use activity.
	 *
	 * item index = 0 ~ N
	 * cancel index = -2
	 * middle index = -3
	 * ok index = -1
	 */
	static Runnable mRunnable = null;	
	public void OpenAlertDialog(final String title, final String message, 
		final String ok, final String middle, final String cancel, final String[] arrayParamStr) {
		
		mRunnable = new Runnable(){
            public void run() {
				AlertDialog.Builder builder = new AlertDialog.Builder(sNativeActivity);
                builder.setTitle(title);
				
				DialogInterface.OnClickListener onClick = new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which) {
						Log.i("com.alert.dialog", "alert dialog, index = " + which);

						JNI.OnAlertDialogClick(which);
					}
				};
                builder.setPositiveButton(ok     , onClick);
				builder.setNeutralButton(middle  , onClick);
				builder.setNegativeButton(cancel , onClick);
				// builder.setMessage(message);
				builder.setItems(arrayParamStr   , onClick);

                builder.show();
            }};

		this.runOnUiThread(mRunnable);
	}

	private class Downloader extends AsyncTask<String, Integer, Boolean> {
		protected Boolean doInBackground(String... strs) {
			try {
				String url = strs[0];
				String name = strs[1];
				Boolean resumable = Boolean.parseBoolean(strs[2]);

				URL u = new URL(url);
				String Filename;		
				if (name.charAt(0) == '.' && name.charAt(1) == '.' && name.charAt(2) == '/')
					Filename = name.substring(3);
				else if (name.charAt(0) == '.' && name.charAt(1) == '/')
					Filename = name.substring(2);
				else
					Filename = name;
				
				if (resumable)
				{	
					JNI.LOGI("using resume downloading ...");
													
					String localFilename = getFilesDir() + "/" + Filename;
					int localFileLength = 0;
					HttpURLConnection urlConnection = (HttpURLConnection)u.openConnection();
					// urlConnection.setRequestMethod("GET");
					urlConnection.setRequestProperty("Accept-Encoding", "identity");
															
					File localTempFile = new File(localFilename);					
					if (localTempFile.exists()) {
						localFileLength = (int) localTempFile.length();
						JNI.LOGI("localTempFile exists with bytes:" + localFileLength);						
						urlConnection.setRequestProperty("Range", "bytes=" + (localTempFile.length()) + "-");
					}
					else {
						JNI.LOGI("localTempFile " + localFilename + " not exists. Start with bytes:" + localFileLength);
						urlConnection.setRequestProperty("Range", "bytes=" + localFileLength + "-");
					}
							
					// urlConnection.setDoInput(true);
					// urlConnection.setDoOutput(true);
					urlConnection.connect();
				
					File file = new File(localFilename);
					file.getParentFile().mkdirs();
												
					BufferedInputStream inStream = new BufferedInputStream(urlConnection.getInputStream());
					FileOutputStream fos = (localFileLength<=0)
						? new FileOutputStream(localFilename)
						: new FileOutputStream(localFilename, true);
					BufferedOutputStream outStream = new BufferedOutputStream(fos, 4096);				
					byte[] buffer = new byte[4096];
				
					int file_size = urlConnection.getContentLength()+localFileLength;
					JNI.LOGI("getContentLength in bytes:" + file_size);				
					JNI.SetDownloadTotalSize(file_size);
				
					int bufferLength = 0;
					int totalLength = localFileLength;
					while ((bufferLength = inStream.read(buffer, 0, 4096)) >= 0) 
					{
						outStream.write(buffer, 0, bufferLength);
						totalLength += bufferLength;
						JNI.SetDownloadProgressSize(totalLength);
					
						if (isCancelled()) {
							break;
						}					
					}

					// dis.close();
					inStream.close();
					outStream.close();
					fos.flush();
					fos.close();
				}
				else
				{					
					DataInputStream dis = new DataInputStream(u.openStream());
				
					URLConnection urlConnection = u.openConnection();
					urlConnection.connect();
					int file_size = urlConnection.getContentLength();
					JNI.SetDownloadTotalSize(file_size);

					byte[] buffer = new byte[4096];
											
					File file = new File(getFilesDir() + "/" + Filename);
					file.getParentFile().mkdirs();

					FileOutputStream fos = new FileOutputStream(file);

					int bufferLength = 0;
					int totalLength = 0;
					while ((bufferLength = dis.read(buffer)) != -1) {
						fos.write(buffer, 0, bufferLength);
						totalLength += bufferLength;
						JNI.SetDownloadProgressSize(totalLength);
						//publishProgress(totalLength);
						if (isCancelled()) {
							break;
						}
					}

					dis.close();
					fos.flush();
					fos.close();
				}
            } catch (Exception e) {
                return false;
            }
            return true;
		}

		/*protected void onProgressUpdate(Integer... progress) {
			
		}*/

		/*protected void onPostExecute(Long result) {
			
		}*/
	}

	// support HttpURLConnection download with resume flag.
	public boolean DownloadURLtoFile(String url, String name, boolean resume) {
		try {
			return new Downloader().execute(url, name, String.valueOf(resume)).get();
		}
		catch (Exception ex) {
			return false;
		}
	}

	// check netowrk.
	public boolean isNetworkAvailable()
	{
		ConnectivityManager connectivityManager = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo activeNetworkInfo = connectivityManager.getActiveNetworkInfo();
		return activeNetworkInfo != null && activeNetworkInfo.isConnected();
	}

	// device sleep mode	
	public void SetDeviceSleepMode(boolean b) {		
		final boolean enable = b;

		runOnUiThread(new Runnable()  {
			@Override
			public void run()  {
				if (enable){
					getWindow().clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
				}
				else {
					getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
				}
			}
		});		
	}

	
	// get 	getAdvertisingIdInfo.
	public void GetAdvertisingIDString()
	{
		Log.d("adv", "GetAdvertisingIDString");
		AsyncTask<Void, Void, String> task = new AsyncTask<Void, Void, String>()
		{
			@Override
			protected String doInBackground(Void... params)
			{
				AdvertisingIdClient.Info idInfo = null;
				try {
					idInfo = AdvertisingIdClient.getAdvertisingIdInfo(getApplicationContext());
				} catch (GooglePlayServicesNotAvailableException e) {
					e.printStackTrace();
				} catch (GooglePlayServicesRepairableException e) {
					e.printStackTrace();
				} catch (IOException e) {
					e.printStackTrace();
				}
				String advertId = null;
				try{
					advertId = idInfo.getId();
				}catch (NullPointerException e){
					e.printStackTrace();
				}
				

				return advertId;
			}

			@Override
			protected void onPostExecute(String advertId)
			{
				if (advertId != null)
				{
					Log.d("adv", "GetAdvertisingIDString =" + advertId);
					JNI.SetAdvertisingIDString(advertId);
				}
			}

		};
		task.execute();		
	}
	
	
	public String GetFirebaseAccessToken()
	{
		String Ret = "";
		Log.d("Firebase", "GetFirebaseAccessToken");
		if (FirebaseInstanceId.getInstance() != null)
		{
			Log.d("Firebase", "GetFirebaseAccessToken getInstance");
			if (FirebaseInstanceId.getInstance().getToken() != null)
			{
				Log.d("Firebase", "GetFirebaseAccessToken getInstance getToken");
				Ret = FirebaseInstanceId.getInstance().getToken();
			}
		}
		
		Log.d("Firebase", "GetFirebaseAccessToken =" + Ret);
		return Ret;	
	}

	public void OpenRatingDialog(final String title, final String text, final String positiveText, final String negativeText, final String neverText)
	{
		runOnUiThread(new Runnable()  {
			@Override
			public void run()  {
				freebackDialog = new FeedbackDialog(sNativeActivity, "lynxengine@qubitent.com");
				freebackDialog.setAppIconVisible(true)
							  .setTitle(title)
							  .setRateText(text)
							  .setButtonHint(positiveText, negativeText, neverText)							  
							  .setUpperBound(2)
							  .forceOpenDialog()
							  .showAfter(0);		
			}
		});		
	}

	
    // android 6.0+ 試著本遊戲要用的權限.
    private void TryGetAPPPermissions()
    {
        String[] perms =
                {
                        Manifest.permission.READ_EXTERNAL_STORAGE
                        , Manifest.permission.WRITE_EXTERNAL_STORAGE
                };

        if (EasyPermissions.hasPermissions(this, perms))
        {
            // Already have permission, do the thing
            // ...
            TryGetSDKPermissions();
        }
        else
        {
            // Do not have permissions, request them now
			//R.string.extrw.			
            EasyPermissions.requestPermissions(this, this.getString(getResources().getIdentifier("extrw", "string", getPackageName())),
                    0, perms);
        }
    }


    Handler mGetSDKPermissionsHandler = new  Handler()
    {
        @Override
        public void handleMessage(Message msg)
        {
            TryGetSDKPermissions();
        }
    };

    private  void DelayGetSDKPermissionsHandler()
    {
        mGetSDKPermissionsHandler.removeMessages(0);
        mGetSDKPermissionsHandler.sendEmptyMessageDelayed(0, GET_SKD_PERM_DELAY);

    }

    // andorid 6.0+ 試著取得第三方 SDK 要用的權限.
    private void TryGetSDKPermissions()
    {
        String[] perms =
                {
                        Manifest.permission.READ_PHONE_STATE
                };

        if (EasyPermissions.hasPermissions(this, perms))
        {
            // Already have permission, do the thing
            // ...
        }
        else
        {
            // Do not have permissions, request them now
			//R.string.readphonestate.
            EasyPermissions.requestPermissions(this, this.getString(getResources().getIdentifier("readphonestate", "string", getPackageName())),
                    0, perms);
        }
    }

	
    // android 6.0+ 試著本遊戲要用的權限.
    @Override
    public void onPermissionsGranted(int requestCode, List<String> perms)
    {
        if (perms.contains(Manifest.permission.READ_EXTERNAL_STORAGE) || perms.contains(Manifest.permission.WRITE_EXTERNAL_STORAGE))
        {
            DelayGetSDKPermissionsHandler();
        }

    }
	
    // android 6.0+ 試著本遊戲要用的權限.
    @Override
    public void onPermissionsDenied(int requestCode, List<String> perms)
    {
        // Some permissions have been denied.
    }

	
    // android 6.0+ 試著本遊戲要用的權限.
    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults)
    {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        // Forward results to EasyPermissions send to EasyPermissions
        EasyPermissions.onRequestPermissionsResult(requestCode, permissions, grantResults, this);
    }
	
    public String GetNetworkType()
    {
        String Ret = "";
        ConnectivityManager manager = (ConnectivityManager) getSystemService(CONNECTIVITY_SERVICE);
		if (manager != null)
		{
			NetworkInfo networkInfo = manager.getActiveNetworkInfo();
			if (networkInfo != null)
			{
				Ret = networkInfo.getTypeName();
			}
		}

        Log.v("networktype", Ret);
        return Ret;
    }
	
    // 取得 電信商的名稱.
    public String GetNetworkOperatorName()
    {
        String Ret = "";

        String[] perms =
                {
                        Manifest.permission.READ_PHONE_STATE
                };

        if (EasyPermissions.hasPermissions(this, perms))
        {
            TelephonyManager telManager = (TelephonyManager) getSystemService(TELEPHONY_SERVICE);
			if (telManager != null)
			{
				Ret = telManager.getNetworkOperatorName();
			}
        }
        Log.v("networktype", Ret);

        return Ret;
    }
	
	//web view upload.
    public class Callback extends WebViewClient{
        public void onReceivedError(WebView view, int errorCode, String description, String failingUrl){
            Toast.makeText(getApplicationContext(), "Failed loading app!", Toast.LENGTH_SHORT).show();
        }
    }
    // Create an image file
    private File createImageFile() throws IOException{
        @SuppressLint("SimpleDateFormat") String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
        String imageFileName = "img_"+timeStamp+"_";
        File storageDir = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES);
        return File.createTempFile(imageFileName,".jpg",storageDir);
    }

	
    @Override
    public void onConfigurationChanged(Configuration newConfig){
        super.onConfigurationChanged(newConfig);
    }

	public void MoveToBackGround()
	{
		moveTaskToBack (true);
	}
}

