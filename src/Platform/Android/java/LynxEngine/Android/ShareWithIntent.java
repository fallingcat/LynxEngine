package LynxEngine.Android;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.ResolveInfo;
import android.net.Uri;
import android.os.Parcelable;
import android.support.v4.content.FileProvider;
import android.util.Log;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import android.graphics.BitmapFactory;
import android.graphics.Bitmap;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import android.graphics.Color;
/**
 * Created by Qubit on 2016/8/17.
 */
public class ShareWithIntent
{
    static void ShareImage(Activity activity, String nameApp, String imageFullPath, String selectAppMsg, String subjectMsg, String textMsg, boolean isInternal)
    {
		copyData(activity);
		CopytoSD(activity);

        Uri uri = null;
		
		try
		{
			if (isInternal)
			{
				if (imageFullPath != "")
				{
					File newFile = new File(imageFullPath);
					Log.v("share", "newFile =" + newFile.toString());

					if (newFile.exists())
					{
						uri = FileProvider.getUriForFile(activity.getApplicationContext(), "com.qubitent.file.fileprovider", newFile);
						Log.v("share", "uri = " + uri.toString());
					}
					else
					{					
						Log.v("share", "file not exists" + imageFullPath);
					}
				}
					
			}
			else
			{
				if (imageFullPath != "")
				{
					File newFile = new File(imageFullPath);
					if (newFile.exists())
					{
						uri = Uri.fromFile(newFile);
						Log.v("share", "uri = " + uri.toString());
					}
					else
					{
				
						Log.v("share", "file not exists" + imageFullPath);
					}
				}
			}
        }
		catch(Exception e)
		{
			Log.v("share","Exception while sending image on " +  e.getMessage());
		}

        ShareImg(activity, nameApp, uri, selectAppMsg, subjectMsg, textMsg);
    }



    private static void ShareImg(Activity activity, String nameApp, Uri uri, String selectAppMsg, String subjectMsg, String textMsg)
    {
        try
        {
            List<Intent> targetedShareIntents = new ArrayList<Intent>();
            Intent share = new Intent(android.content.Intent.ACTION_SEND);
            share.setType("image/*");
            List<ResolveInfo> resInfo = activity.getPackageManager().queryIntentActivities(share, 0);
            if (!resInfo.isEmpty())
            {
                for (ResolveInfo info : resInfo)
                {
                    Intent targetedShare = new Intent(android.content.Intent.ACTION_SEND);

                    targetedShare.setType("image/*"); // put here your mime type
                    if (info.activityInfo.packageName.toLowerCase().contains(nameApp) || info.activityInfo.name.toLowerCase().contains(nameApp))
                    {
                        targetedShare.putExtra(Intent.EXTRA_SUBJECT, subjectMsg);
                        targetedShare.putExtra(Intent.EXTRA_TEXT, textMsg);
						if (uri != null)
						{
							targetedShare.putExtra(Intent.EXTRA_STREAM, uri);
						}
                        targetedShare.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
                        targetedShare.setPackage(info.activityInfo.packageName);
                        targetedShareIntents.add(targetedShare);
                    }
                }

                Intent chooserIntent = Intent.createChooser(targetedShareIntents.remove(0), selectAppMsg);
                chooserIntent.putExtra(Intent.EXTRA_INITIAL_INTENTS, targetedShareIntents.toArray(new Parcelable[]{}));
                activity.startActivity(chooserIntent);
            }
        }
        catch(Exception e){
            Log.v("share","Exception while sending image on" + nameApp + " "+  e.getMessage());
        }
    }

	
    private static void copyData(Activity activity)
    {
        String path = "/data/data/" + activity.getPackageName() + "/files/texture/ui/loading/";
        String Name = "connecting_00.png";

        File folder = new File(path);

        if (!folder.exists())
        {
            Log.v("file", "folder.mkdirs()");
            folder.mkdirs();
        }

        try {

            //OutputStream myOutput = getApplicationContext().openFileOutput("savedpic.jpg", Context.MODE_WORLD_READABLE | Context.MODE_WORLD_WRITEABLE);

            OutputStream myOutput = new FileOutputStream(folder.toString() + "/" + Name);
            byte[] buffer = new byte[1024];
            int length;
            InputStream myInput = activity.getApplicationContext().getAssets().open("texture/ui/loading/connecting_00.png");
            while ((length = myInput.read(buffer)) > 0) {
                myOutput.write(buffer, 0, length);
            }
            myInput.close();
            myOutput.flush();
            myOutput.close();
        }
        catch(Exception e)
        {
            Log.v("file", e.toString());
        }
    }


    private static void CopytoSD(Activity activity)
    {
        File folder = activity.getApplicationContext().getExternalFilesDir("texture/ui/loading/");
        String Name = "connecting_00.png";
		
		Log.v("share",  "CopytoSD " + folder);
        if (!folder.exists())
        {
            Log.v("file", "folder.mkdirs()");
            folder.mkdirs();
        }
				
        try {
		
            OutputStream myOutput = new FileOutputStream(folder.toString() + "/" + Name);
            byte[] buffer = new byte[1024];
            int length;
            InputStream myInput = activity.getApplicationContext().getAssets().open("texture/ui/loading/connecting_00.png");
            while ((length = myInput.read(buffer)) > 0) {
                myOutput.write(buffer, 0, length);
            }
            myInput.close();
            myOutput.flush();
            myOutput.close();
        }
        catch(Exception e)
        {
            Log.v("file", e.toString());
        }
    }
	
}
