package LynxEngine.Android;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.opengl.GLU;
import android.content.res.AssetManager;
import LynxEngine.Android.View;

public class Renderer implements android.opengl.GLSurfaceView.Renderer 
{
	Activity App;
	LynxEngine.Kernel Kernel;
	LynxEngine.Android.View View;
	int RendererType;
	
	public native void Clear(float r, float g, float b);	
	
	public Renderer(Activity app, LynxEngine.Kernel k, LynxEngine.Android.View v, int renderertype)
	{	
		App = app;
		Kernel = k;
		View = v;		
		RendererType = renderertype;
	}	
	
	/*
	 * (non-Javadoc)
	 *
	 * @see
	 * android.opengl.GLSurfaceView.Renderer#onSurfaceCreated(javax.
         * microedition.khronos.opengles.GL10, javax.microedition.khronos.
         * egl.EGLConfig)
	 */
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {		
		View.Init(RendererType);	
	}

	/*
	 * (non-Javadoc)
	 *
	 * @see
	 * android.opengl.GLSurfaceView.Renderer#onDrawFrame(javax.
         * microedition.khronos.opengles.GL10)
	 */
	public void onDrawFrame(GL10 gl) 
	{
		if (Kernel.bInit())
			Kernel.Loop();
		
		//Clear(0.0f, 0.0f, 1.0f);
	}

	/*
	 * (non-Javadoc)
	 *
	 * @see
	 * android.opengl.GLSurfaceView.Renderer#onSurfaceChanged(javax.
         * microedition.khronos.opengles.GL10, int, int)
	 */
	public void onSurfaceChanged(GL10 gl, int width, int height) 
	{				
	}
}
