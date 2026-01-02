package com.android.notify;

import android.os.Bundle;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.ActivityManager;
import android.app.ActivityManager.RunningTaskInfo;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.support.v4.app.NotificationCompat;
import android.support.v4.app.TaskStackBuilder;
import android.media.RingtoneManager;
import android.net.Uri;
import java.util.List;

import LynxEngine.Android.JNI;
import LynxEngine.Android.NativeActivity;

/**
 * NotificationReceiver for receive notification.
 *
 * @description
 *	- package name should be 'lowercase' string in <receiver> tag for AndroidManifest.xml.
 *  - it needs resource (a.k.a R.java) for creating NotificationCompat.Builder.
 *
 * @require method - protected NotificationCompat.Builder getBuilder(Context context);
 *  - should override this method in game.so for related resource.
 *
 */
public class NotificationReceiver extends BroadcastReceiver {
	public final static String LYNXENGINE_ANDROID_NOTIFICATION_TAG = "LynxEngineAndroidNotificationTAG";

	public boolean isActivityRunning(Context ctx) {
        ActivityManager activityManager = (ActivityManager) ctx.getSystemService(Context.ACTIVITY_SERVICE);
        List<RunningTaskInfo> tasks = activityManager.getRunningTasks(Integer.MAX_VALUE);

        for (RunningTaskInfo task : tasks) {
            if (ctx.getPackageName().equalsIgnoreCase(task.baseActivity.getPackageName())) 
                return true;                                  
        }
        return false;
    }

	@Override
    public void onReceive(Context context, Intent intent) {		
		String packageName = context.getPackageName();
		JNI.LOGI("LocalNotificationReceiver::onReceive package : " + packageName); 
		
		dumpIntent(intent);
		
		if (false == isActivityRunning(context)) //|| false == JNI.IsApplicationInForeground())
		{
			Class<?> c = null;
			try {
				String className = packageName + ".NativeActivity";			 			
				c = Class.forName(className);
			} 
			catch (ClassNotFoundException e) { e.printStackTrace();}					
			
			Intent notificationIntent = new Intent(context, c);
			
			TaskStackBuilder stackBuilder = TaskStackBuilder.create(context);
			stackBuilder.addParentStack(c);
			stackBuilder.addNextIntent(notificationIntent);
			
			PendingIntent pendingIntent = stackBuilder.getPendingIntent(0, PendingIntent.FLAG_UPDATE_CURRENT);
					
			NotificationCompat.Builder builder = getBuilder(context, intent);
			if (null != builder)
			{
				builder.setContentIntent(pendingIntent);
				
				String tag = intent.getStringExtra("notify_tag");
				int id = intent.getIntExtra("notify_id",0);
				
				NotificationManager notificationManager = (NotificationManager)context.getSystemService(Context.NOTIFICATION_SERVICE);
				notificationManager.notify(tag, id, builder.build());
			}
		} // End of IsApplicationInForeground
    }
	
	private void dumpIntent(Intent intent)
	{
		Bundle bundle = intent.getExtras();
		for (String key : bundle.keySet()) 
		{
			Object value = bundle.get(key);
			
			JNI.LOGI(String.format("%s : %s (%s)", 
				key,
				value.toString(),
				value.getClass().getName()));			
		}
	}
	
	protected NotificationCompat.Builder getBuilder(Context context, Intent intent) 
	{ 
	/**
	 *	If app is finished and NOT in background, am will call the interface from engine.
	 *  otherwise, call the intreface from QubotWar.
	 *
	 */
		JNI.LOGI("NotificationCompat.Builder getBuilder from LynxEngine");

		String title = intent.getStringExtra("notify_title");
		String message = intent.getStringExtra("notify_msg");	
		String sound = intent.getStringExtra("notify_sound");		
		final int resourceId = context.getResources().getIdentifier("splash", "drawable", context.getPackageName());
		
		NotificationCompat.Builder builder = new NotificationCompat.Builder(context);
        builder.setContentTitle(title)                
			   .setContentText(message)
               .setTicker(context.getPackageName())				
			   // default Vibratation.
			   .setVibrate(new long[] { 1000, 1000, 1000, 1000, 1000 })
			   // default Ringtone.
			   .setSound(RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION))			   
			   // custom Ringtone.
			   .setSound(Uri.parse("android.resource://" + context.getPackageName() + "/raw/ringtone"))
			   // default SmallIcon involve R.
			   .setSmallIcon(resourceId)
			   // default LargeIcon
			   .setLargeIcon(BitmapFactory.decodeResource(context.getResources(), resourceId));
		
		// default SmallIcon
		// .setSmallIcon(R.drawable.splash);	   
		// default LargeIcon
		// .setLargeIcon(BitmapFactory.decodeResource(context.getResources(), R.drawable.splash));
			   	 
		return builder;
	}
}
