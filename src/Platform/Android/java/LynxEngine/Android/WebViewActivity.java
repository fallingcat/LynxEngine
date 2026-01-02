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
import android.content.Intent;
// import android.util.Log;
import android.graphics.Color;
import android.view.Display;
import android.view.Window;
import android.view.Gravity;
import android.view.View;
import android.view.WindowManager;
import android.view.ViewGroup.LayoutParams;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.FrameLayout;
import android.widget.Button;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import LynxEngine.Android.JNI;


public class WebViewActivity extends Activity
{
    private static WebViewActivity msWebViewActivity;
    private WebView mWebView;
	private Button mButton;

	/// const property
	static final int	LAYOUT_WEBVIEW_ID = 0x0035;	
	static final int	LAYOUT_BUTTON_ID = 0x0015;
	
	static final String LAYOUT_INVISIBLE_TEXT = "Empty";
	static final String INTENT_PARAM_URL = "url";
	static final String INTENT_TITLE_TEXT = "TitleBar_Text";
	static final String INTENT_BUTTON_TEXT = "CloseButton_Text"; 
	static final String INTENT_PARAM_VIEW_LEFT = "View_Left";
	static final String INTENT_PARAM_VIEW_TOP = "View_Top";
	static final String INTENT_PARAM_VIEW_RIGHT = "View_Right";
	static final String INTENT_PARAM_VIEW_BOTTOM = "View_Bottom";
	static final String INTENT_PARAM_BUTTON_W = "Button_Width";
	static final String INTENT_PARAM_BUTTON_H = "Button_Height";
	static final String INTENT_PARAM_BACKGROUNDCOLOR_RGB = "BackgroundColor_RGB";
	static final String INTENT_PARAM_BACKGROUNDCOLOR_ALPHA = "BackgroundColor_Alpha";

	String mURL;
	String mViewTitle;
	String mButtonTitle;
	int mViewLeft;
	int mViewTop;
	int mViewRight;
	int mViewBottom;
	int mButtonWidth;
	int mButtonHeight;
	int mBackGroundColorRGB;
	int mBackGroundColorAlpha;
	boolean mShowButton = false;

	void ProcessIntentParams()
	{
		Intent intent = getIntent(); 

		mURL = intent.getStringExtra(WebViewActivity.INTENT_PARAM_URL);
		mViewTitle = intent.getStringExtra(WebViewActivity.INTENT_TITLE_TEXT);
		mButtonTitle = intent.getStringExtra(WebViewActivity.INTENT_BUTTON_TEXT);
		mViewLeft = intent.getIntExtra (WebViewActivity.INTENT_PARAM_VIEW_LEFT, 0);
		mViewTop = intent.getIntExtra (WebViewActivity.INTENT_PARAM_VIEW_TOP, 0);
		mViewRight = intent.getIntExtra (WebViewActivity.INTENT_PARAM_VIEW_RIGHT, 0);
		mViewBottom = intent.getIntExtra (WebViewActivity.INTENT_PARAM_VIEW_BOTTOM, 0);
		mButtonWidth = intent.getIntExtra (WebViewActivity.INTENT_PARAM_BUTTON_W, 10);
		mButtonHeight = intent.getIntExtra (WebViewActivity.INTENT_PARAM_BUTTON_H, 10);
		mBackGroundColorRGB = intent.getIntExtra (WebViewActivity.INTENT_PARAM_BACKGROUNDCOLOR_RGB, 0xFFFFFFFF);
		mBackGroundColorAlpha = intent.getIntExtra (WebViewActivity.INTENT_PARAM_BACKGROUNDCOLOR_ALPHA, 0xFFFFFFFF);

		mShowButton = (mButtonTitle.equals(WebViewActivity.LAYOUT_INVISIBLE_TEXT)) ? false : true;
	}

    public WebViewActivity()
    {
	    super();
	    msWebViewActivity = this;
		
		JNI.LOGI("WebViewActivity()");
    }

	@Override
	public void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		Intent intent = getIntent(); 

		if(intent != null)
		{
			JNI.LOGI("onCreate(), create webView");
		
			ProcessIntentParams();

			// action bar title
			if (mViewTitle.equals(WebViewActivity.LAYOUT_INVISIBLE_TEXT)) {
				requestWindowFeature(Window.FEATURE_NO_TITLE);
			} 
			else {
				setTitle(mViewTitle);
			}
			
			// create layout
			LinearLayout layout = new LinearLayout(this);
			layout.setOrientation(LinearLayout.VERTICAL);
			// backgroundColor
			layout.setBackgroundColor(Color.argb(
				mBackGroundColorAlpha,
				(mBackGroundColorRGB >> 16) & 0xFF,
				(mBackGroundColorRGB >>  8) & 0xFF,
				(mBackGroundColorRGB        & 0xFF)));
			LayoutParams layoutParam = new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT); 
			
			// create webView
			mWebView = new WebView(this);
			mWebView.setId(WebViewActivity.LAYOUT_WEBVIEW_ID);
			mWebView.loadUrl(mURL);
			mWebView.setWebViewClient(new WebViewClient()       
			{
				@Override
				public boolean shouldOverrideUrlLoading(WebView view, String url) 
				{
					JNI.LOGI("shouldOverrideUrlLoading = " + url);
					
					view.loadUrl(url);
					return true;
				}
			});
			layout.addView(mWebView);
			
			if (mShowButton) {
				// create close button
				mButton = new Button(this);
				mButton.setText(mButtonTitle);
				mButton.setId(WebViewActivity.LAYOUT_BUTTON_ID);
				mButton.setOnClickListener(new View.OnClickListener() 
				{
					@Override
					public void onClick(View v) {
						JNI.LOGI("setOnClickListener(), button in webView");
					
						finish();

						/**
						// implement for back previous page
						//
						if (v.getId() == UIActivity.LAYOUT_BUTTON_ID) {
							if (mWebView.canGoBack()) {
								mWebView.goBack();
							} else {
								finish();
							}
						}
						*/
					} // End of onClick
				}); // End of setOnClickListener
				layout.addView(mButton);
			}
			
			setContentView(layout, layoutParam);
		}
	}

	/*
	 *  in onAttachedToWindow(), do layout setting for WebView
	 */
	@Override
	public void onAttachedToWindow() 
	{
		super.onAttachedToWindow();
        
		Display display = getWindowManager().getDefaultDisplay();

		// layout of rootView
		View view = getWindow().getDecorView();
		view.setTop(mViewTop);
		view.setLeft(mViewLeft); 
		
		WindowManager.LayoutParams lp = (WindowManager.LayoutParams) view.getLayoutParams();
		lp.gravity = Gravity.TOP | Gravity.LEFT;
		lp.width = mViewRight - mViewLeft; // display.getWidth() - paddingWidth;
		lp.height = mViewBottom - mViewTop; // display.getHeight() - paddingHeight;
		getWindowManager().updateViewLayout(view, lp);
		
		// layout of webView
		LinearLayout.LayoutParams webViewParam = new LinearLayout.LayoutParams(
			LayoutParams.FILL_PARENT, LayoutParams.WRAP_CONTENT, 9.0f);
		mWebView.setLayoutParams(webViewParam);

		if (mShowButton) {
			// layout of button
			LinearLayout.LayoutParams buttonParam = new LinearLayout.LayoutParams(
				LayoutParams.FILL_PARENT, LayoutParams.WRAP_CONTENT);
			buttonParam.gravity = Gravity.CENTER_VERTICAL | Gravity.CENTER_HORIZONTAL;
			buttonParam.width = mButtonWidth;
			buttonParam.height = mButtonHeight;		
			mButton.setLayoutParams(buttonParam);
		}

		JNI.LOGI("onAttachedToWindow(), layout setting");
	}

	/*
	 *  in onDetachedFromWindow(), release resources of WebView
	 */
	@Override
	public void onDetachedFromWindow() 
	{
		super.onDetachedFromWindow();

		FrameLayout layout = (FrameLayout)getWindow().getDecorView().getRootView();
		layout.removeView(mWebView);
		mWebView.removeAllViews();
		mWebView.destroy();

		JNI.LOGI("onDetachedFromWindow(), release webView");
	}
}
