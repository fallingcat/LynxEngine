package LynxEngine.Android;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.ArrayList;

/**
 * Log Object 
 * 
 * @description
 * 1. Fetch log from logcat and output to a file. 
 *    
 * @author Kasaki Tzeng
 */
public class Log
{
	public static final String cmdLine = "logcat -f";
	public static void dumpLogcat()
	{		
		try
		{						
			String fileName = "";
			// File file = new File(context.getInternalCacheDir(), fileName);
			
			// @SuppressWarning("unused")
			Process process=Runtime.getRuntime().exec(cmdLine);
			
			
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}		
	}
}
