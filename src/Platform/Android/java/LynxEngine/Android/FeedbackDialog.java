
package LynxEngine.Android;

import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.Resources;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.app.AlertDialog;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import LynxEngine.Android.JNI;

public class FeedbackDialog  implements DialogInterface.OnClickListener {
	
    private final static String DEFAULT_TITLE = "Rate this app";
    private final static String DEFAULT_TEXT = "Would you like to leave a comment?";
    private final static String DEFAULT_POSITIVE = "Ok";
    private final static String DEFAULT_NEGATIVE = "Not Now";
    private final static String DEFAULT_NEVER = "Never";
    private final static String SHAREDPREFERENCE_ACCESSCOUNT = "AccessCount";
    private static final String SHAREDPREFERENCE_DISABLE = "disabled";
    private static final String TAG = FeedbackDialog.class.getSimpleName();
    private final Context context;
    private boolean isForceMode = false;
    SharedPreferences sharedPrefs;
    private String supportEmail;
	private String promptTitle = DEFAULT_TITLE;
    private String promptText = DEFAULT_TEXT;
    private String promptPositive = DEFAULT_POSITIVE;
    private String promptNegative = DEFAULT_NEGATIVE;
    private String promptNever = DEFAULT_NEVER;
    private TextView contentTextView;    
    private AlertDialog alertDialog;
    private View dialogView;
	private boolean isShowAppIcon = true;
    private int upperBound = 4;
    
    public FeedbackDialog(Context context,String supportEmail) {
        this.context = context;
        sharedPrefs = context.getSharedPreferences(context.getPackageName(),Context.MODE_PRIVATE);
        this.supportEmail = supportEmail;
    }

	/**
     * Call the method go get resource id instead of using R.id or R.layout sometimes it needs resource id in library.
     * @param resName = resource name
	 * @param resType = resource type, eg. "id"' "layout"
     * @return resource id
     */
	private int getResourceId(String resName, String resType) {	
		Resources res = context.getResources();
		return res.getIdentifier(resName, resType, context.getPackageName());
	}

	/**
     * Call the method to get resource id of app icon.
     * @param 
     * @return resource id of app icon
     */
	public int getAppIconResourceId() {
        int appIconResId = -1;
        String packageName = context.getPackageName();
        final PackageManager pm = context.getPackageManager();
        final ApplicationInfo applicationInfo;
        try {
            applicationInfo = pm.getApplicationInfo(packageName, PackageManager.GET_META_DATA);
            appIconResId = applicationInfo.icon;
        } catch (PackageManager.NameNotFoundException e) {
            //do nothing here
        }
        return appIconResId;
    }

    private void build() {
        AlertDialog.Builder builder = new AlertDialog.Builder(context);
        LayoutInflater inflater = LayoutInflater.from(context);

		try 
		{
			dialogView = inflater.inflate(getResourceId("rating_dialog", "layout"), null);
        
			contentTextView = (TextView)dialogView.findViewById(getResourceId("text_content", "id"));
			contentTextView.setText(promptText);
        
			ImageView appIcon = (ImageView) dialogView.findViewById(getResourceId("dialog_icon", "id"));
			if (isShowAppIcon) {
				appIcon.setImageResource(getAppIconResourceId());
				appIcon.setVisibility(View.VISIBLE);
			} else {
				appIcon.setVisibility(View.GONE);
			}

			builder.setTitle(promptTitle)
				   .setView(dialogView);
			if (!promptNegative.isEmpty()) builder.setNegativeButton(promptNegative, this);
			if (!promptPositive.isEmpty()) builder.setPositiveButton(promptPositive, this);
			if (!promptNever.isEmpty()) builder.setNeutralButton(promptNever, this);
							
			// JNI.LOGE("FeedbackDialog.build, message = " + promptNegative + "," + promptPositive + "," + promptNever);				
								
			alertDialog = builder.create();
		}
		catch (Exception e)
		{
			JNI.LOGE("Exception: invoke method FeedbackDialog.build, message = " + e.getMessage());
		}        
    }

    private void disable() {
        SharedPreferences shared = context.getSharedPreferences(context.getPackageName(), Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = shared.edit();
        editor.putBoolean(SHAREDPREFERENCE_DISABLE, true);
        editor.apply();
    }

    private void openMarket() {
        final String appPackageName = context.getPackageName();
        try {
            context.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("market://details?id=" + appPackageName)));
        } catch (android.content.ActivityNotFoundException e) {
            context.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("https://play.google.com/store/apps/details?id=" + appPackageName)));
        }
    }

    private void sendEmail() {
        final Intent emailIntent = new Intent(Intent.ACTION_SEND);
        emailIntent.setType("text/email");
        emailIntent.putExtra(Intent.EXTRA_EMAIL,new String[] {supportEmail});
        emailIntent.putExtra(Intent.EXTRA_SUBJECT, "App Report ("+context.getPackageName()+")");
        emailIntent.putExtra(Intent.EXTRA_TEXT, "");
        context.startActivity(Intent.createChooser(emailIntent, "Send mail..."));
    }

    private void show() {
        boolean disabled  = sharedPrefs.getBoolean(SHAREDPREFERENCE_DISABLE, false);
        if(!disabled){
            build();
            alertDialog.show();
        }
    }

    public void showAfter(int numOfAccess){
        build();

        SharedPreferences.Editor editor = sharedPrefs.edit();
        int accessCount = sharedPrefs.getInt(SHAREDPREFERENCE_ACCESSCOUNT, 0);
        editor.putInt(SHAREDPREFERENCE_ACCESSCOUNT, accessCount + 1);
        editor.apply();

        if(accessCount + 1 >= numOfAccess) {
            show();
        }
    }

    @Override
    public void onClick(DialogInterface dialogInterface, int i) {
        if(i == DialogInterface.BUTTON_POSITIVE){
            openMarket();            
            disable();            
        }
        if(i == DialogInterface.BUTTON_NEUTRAL){
            disable();
        }
        if(i == DialogInterface.BUTTON_NEGATIVE){
            SharedPreferences.Editor editor = sharedPrefs.edit();
            editor.putInt(SHAREDPREFERENCE_ACCESSCOUNT, 0);
            editor.apply();
        }

		JNI.OnClickRatingDialog(i);
        alertDialog.hide();
    }
	
	public FeedbackDialog setAppIconVisible(boolean isVisible) {
        this.isShowAppIcon = isVisible;
        return this;
    }

    public FeedbackDialog setTitle(String title) {
        this.promptTitle = title;
        return this;
    }

    public FeedbackDialog setSupportEmail(String supportEmail) {
        this.supportEmail = supportEmail;
        return this;
    }

    public FeedbackDialog setRateText(String rateText){
        this.promptText = rateText;
        return this;
    }

	public FeedbackDialog setButtonHint(String positiveText, String negativeText, String neverText)
	{
		this.promptPositive = positiveText;
		this.promptNegative = negativeText;
		this.promptNever = neverText;
		return this;
	}
	 	 
    /**
     * Call the method when it needs to overwrite SHAREDPREFERENCE_DISABLE flag and open dialog again.
     * @param 
     * @return the dialog
     */
    public FeedbackDialog forceOpenDialog(){
		SharedPreferences shared = context.getSharedPreferences(context.getPackageName(), Context.MODE_PRIVATE);
		SharedPreferences.Editor editor = shared.edit();
		editor.putBoolean(SHAREDPREFERENCE_DISABLE, false);
		editor.apply();
				   
        return this;
    }

    /**
     * Set the upper bound for the rating.
     * If the rating is >= of the bound, the market is opened.
     * @param bound the upper bound
     * @return the dialog
     */
    public FeedbackDialog setUpperBound(int bound){
        this.upperBound = bound;
        return this;
    }
}
