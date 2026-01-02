package LynxEngine.Android;

import android.os.Handler;
import android.os.Message;
import android.app.Activity;
import android.app.Dialog;
import android.content.Context;
import android.view.Gravity;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.InputEvent;
import android.view.KeyEvent;
import android.view.Display;
import android.view.MotionEvent;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.graphics.drawable.ColorDrawable;
import android.graphics.Point;
import android.util.Log;
//import com.qubitent.qubot.test.R;
import java.lang.reflect.InvocationTargetException;

public class PopupDialog extends Dialog
{
	int NAVBAR_DELAY_HIDE_MILSEC = 100;
	private final Context mContext;

	public PopupDialog(Context context)
	{
		//super(context, 16973841);
		//super(context, R.style.Dialog);		
		super(context);

		
		mContext=context;
		
		//getWindow().setFlags();
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN);
		getWindow().clearFlags(WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN);
		getWindow().clearFlags(WindowManager.LayoutParams.FLAG_LAYOUT_INSET_DECOR);
		getWindow().setBackgroundDrawable(new ColorDrawable(android.graphics.Color.TRANSPARENT));

		SetBackgroundClickable();
		//getWindow().getAttributes().windowAnimations = R.style.dialog_animation;
		//getWindow().getAttributes().windowAnimations = R.style.dialog_animation;
		// anim ID 來自 https://developer.android.com/reference/android/R.anim.html  
		Animation in = AnimationUtils.loadAnimation(mContext, 17432576);
        in.setDuration(0);
		getWindow().setWindowAnimations(17432576);
		
		WindowManager.LayoutParams wmlp = getWindow().getAttributes();
		String className = "android.view.WindowManager$LayoutParams";
		try {
			Class layoutParamsClass = Class.forName(className);

			java.lang.reflect.Field privateFlags = layoutParamsClass.getField("privateFlags");
			java.lang.reflect.Field noAnim = layoutParamsClass.getField("PRIVATE_FLAG_NO_MOVE_ANIMATION");

			int privateFlagsValue = privateFlags.getInt(wmlp);
			int noAnimFlag = noAnim.getInt(wmlp);
			privateFlagsValue |= noAnimFlag;

			privateFlags.setInt(wmlp, privateFlagsValue);

			// Dynamically do stuff with this class
			// List constructors, fields, methods, etc.

		} catch (ClassNotFoundException e) {
			//Logger.l.e(e.toString());
			// Class not found!
						Log.d("dia", "ref Class not found! ");
		} catch (Exception e) {
			//Logger.l.e(e.toString());
			// Unknown exception
						Log.d("dia", "ref Unknown exception");
		}
	}

	public boolean onKeyDown(int keyCode, KeyEvent event)
	{
		if (keyCode == 4) {
			return false;
		}
		return super.onKeyDown(keyCode, event);
	}


	public void showAtLocation(int x,int y)
	{
		
		//Display display = ((Activity)mContext).getWindowManager().getDefaultDisplay();
		//int width = display.getWidth();
		//int height = display.getHeight();
		WindowManager.LayoutParams wmlp = getWindow().getAttributes();
		//左上為基準點.
		wmlp.gravity = Gravity.TOP | Gravity.LEFT; 
		wmlp.x = x;
		wmlp.y = y;
		
		Point screensize = getRealScreenSize(mContext);
		//Point navsize = getNavigationBarSize(mContext);


		wmlp.width = screensize.x;
		wmlp.height = screensize.y;
		getWindow().setAttributes(wmlp);
		show();
		DelayHideNavBar(NAVBAR_DELAY_HIDE_MILSEC);
	}
	
	public void showAtLocationHeightSubNavbar(int x,int y, int width, int height)
	{
		
		//Log.d("web", "showAtLocationHeightSubNavbar x:" + x + " y:" + y + " width:" + width + " height:" + height);
		//Display display = ((Activity)mContext).getWindowManager().getDefaultDisplay();
		//int width = display.getWidth();
		//int height = display.getHeight();
		WindowManager.LayoutParams wmlp = getWindow().getAttributes();
		//左上為基準點.
		wmlp.gravity = Gravity.TOP | Gravity.LEFT; 
		wmlp.x = x;
		wmlp.y = y;

		Point navsize = getNavigationBarSize(mContext);
		Point screensize = getRealScreenSize(mContext);


		wmlp.width = width;//screensize.x;
		//wmlp.height = height - (int)(navsize.y * 0.5F);
		wmlp.height = height;//(int)(screensize.y * 0.92F);
		getWindow().setAttributes(wmlp);
		show();
		DelayHideNavBar(NAVBAR_DELAY_HIDE_MILSEC);
	}

	public void SetPosition(final int x, final int y, final int width, final int height)
	{
		
		//Log.d("web", "SetPosition" + " x:" + x + " y:" + y + " width:" + width + " height:" + height);
		WindowManager.LayoutParams wmlp = getWindow().getAttributes();
		//左上為基準點.
		wmlp.gravity = Gravity.TOP | Gravity.LEFT; 
		wmlp.x = x;
		wmlp.y = y;
		wmlp.width = width;
		wmlp.height = height;
	
		getWindow().setAttributes(wmlp);
	
	}

	//from http://stackoverflow.com/questions/20264268/how-to-get-height-and-width-of-navigation-bar-programmatically
	public Point getNavigationBarSize(Context context)
	{
		Point appUsableSize = getAppUsableScreenSize(context);
		Point realScreenSize = getRealScreenSize(context);

		// navigation bar on the right
		if (appUsableSize.x < realScreenSize.x) {
			return new Point(realScreenSize.x - appUsableSize.x, appUsableSize.y);
		}

		// navigation bar at the bottom
		if (appUsableSize.y < realScreenSize.y) {
			return new Point(appUsableSize.x, realScreenSize.y - appUsableSize.y);
		}

		// navigation bar is not present
		return new Point();
	}

	public Point getAppUsableScreenSize(Context context) {
		WindowManager windowManager = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
		Display display = windowManager.getDefaultDisplay();
		Point size = new Point();
		display.getSize(size);
		return size;
	}

	public Point getRealScreenSize(Context context) {
		WindowManager windowManager = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
		Display display = windowManager.getDefaultDisplay();
		Point size = new Point();

		if (android.os.Build.VERSION.SDK_INT >= 17) {
			display.getRealSize(size);
		} else if (android.os.Build.VERSION.SDK_INT >= 14) {
			try {
				size.x = (Integer) Display.class.getMethod("getRawWidth").invoke(display);
				size.y = (Integer) Display.class.getMethod("getRawHeight").invoke(display);
			} catch (IllegalAccessException e) {} catch (InvocationTargetException e) {} catch (NoSuchMethodException e) {}
		}

		return size;
	}

	Handler mHideSystemUiHandler = new  Handler()
												{
													@Override
													public void handleMessage(Message msg)
													{
														HideNavBar();
													}
												};

	void DelayHideNavBar(int delayMillis)
	{
		if (android.os.Build.VERSION.SDK_INT >= 19)
		{
			mHideSystemUiHandler.removeMessages(0);
			mHideSystemUiHandler.sendEmptyMessageDelayed(0, delayMillis);
		}


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

	public void SetBackgroundClickable()
	{
			Window dialogWindow = getWindow();

            // Make the dialog possible to be outside touch
            dialogWindow.setFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL,
                    WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL);
            dialogWindow.clearFlags(WindowManager.LayoutParams.FLAG_DIM_BEHIND);

	

	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event)  
	{  

       HideNavBar(); 
       return super.onTouchEvent(event);  
	}  

	public void CloseDialog()
	{
		hide();
	}
}