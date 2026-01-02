package LynxEngine;

public class Kernel
{  		
	static 
	{
		//System.loadLibrary(".\\LynxEngine_Win32_Web_ud");
		//String oldJLP = System.getProperty("java.library.path");
		System.setProperty("java.library.path", "E:\\MyWork\\Lynx System\\SampleWebGame\\bin");
		//System.loadLibrary("LynxEngine_JNI");
		System.loadLibrary("LynxActivity");
		//System.setProperty("java.library.path", oldJLP);			
	}
	
	public native void NativeInit(java.lang.Object context, int width, int height, int rendertype);	
	public native void NativeRelease();
	public native void Loop();
	public native void SetMousePollData(int x, int y, int mask);
	public native void SetKeyboardPollData(int key, boolean bpress);
	public native void SetCharPressed(int c);
	public native void Clear(float r, float g, float b);
		
	public static final int VOID_RENDERER 	= -1;
	public static final int NULL_RENDERER 	=  0;
	public static final int D3D9_RENDERER 	=  1;
	public static final int D3D10_RENDERER 	=  2;
	public static final int D3D11_RENDERER 	=  3;
	public static final int OGL_RENDERER 	=  4;
	public static final int OGLES_RENDERER 	=  5;
	public static final int OGLES2_RENDERER =  6;
		
	boolean bInit = false;
	
	public boolean bInit() {return bInit; }
	public void Init(java.lang.Object context, int width, int height, int rendertype)
	{
		NativeInit(context, width, height, rendertype);
		bInit = true;		
	}	
}