package LynxEngine.Android;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;
import android.content.res.AssetManager;

import LynxEngine.Kernel;
import LynxEngine.Android.View;
import LynxEngine.Android.Renderer;

public class LynxActivity extends android.app.Activity 
{
	protected int RendererType;
	LynxEngine.Kernel Kernel;
	LynxEngine.Android.View View;
	LynxEngine.Android.Renderer Renderer;	
	AssetManager AssetMang;
	
	public native void NativeInit(AssetManager a);
	
	/** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) 
    {
    	super.onCreate(savedInstanceState);
                
        this.requestWindowFeature(Window.FEATURE_NO_TITLE); // (NEW)
        this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, 
        		                  WindowManager.LayoutParams.FLAG_FULLSCREEN); // (NEW)
        
        Kernel = new LynxEngine.Kernel();        
        View = new LynxEngine.Android.View(this, Kernel);
        AssetMang = getAssets();
        this.NativeInit(AssetMang);
		RendererType = LynxEngine.Kernel.OGLES2_RENDERER;
        Renderer = new LynxEngine.Android.Renderer(this, Kernel, View, RendererType);        
        View.setRenderer(Renderer);        
  		setContentView(View);
    }
}
