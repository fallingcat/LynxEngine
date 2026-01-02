package LynxEngine;

import java.applet.Applet;
import java.awt.*;

public class LynxApplet extends java.applet.Applet implements Runnable
{
	protected int RendererType;
	Thread animator;
	LynxEngine.Canvas MainCanvas;
	
	public native void NativeInit();	
	
	public void init() 
	{
		setLayout (null);		
		MainCanvas = new LynxEngine.Canvas(RendererType);
		MainCanvas.setBounds(0, 0, getWidth(), getHeight());
     	add("Center", MainCanvas);
     	NativeInit();
	}	
	
	public void start() 
	{
		animator = new Thread(this);
		animator.start();		
	}
		
	/**
     * This method is called when the applet is no longer
     * visible. Set the animator variable to null so that the
     * thread will exit before displaying the next frame.
     */
    public void stop() 
    {
    	animator = null;
    }    
    
    public void paint(Graphics g) 
    {
    	MainCanvas.paint(g);      	
    }
    
	/**
     * This method is called by the thread that was created in
     * the start method. It does the main animation.
     */
    public void run() 
    {
    	long T0 = System.currentTimeMillis();
    	while (Thread.currentThread() == animator) 
    	{
    	    // Display the next frame of animation.
    	    repaint();    	    
    	    // Delay for a while
    	    try {
    	    	T0 += 1000.0/600.0;
    			Thread.sleep(Math.max(0, T0 - System.currentTimeMillis()));
    	    } catch (InterruptedException e) {
    	    	break;
    	    }    	    
    	}    	
    }
    
}