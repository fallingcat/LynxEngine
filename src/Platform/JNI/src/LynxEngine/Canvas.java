package LynxEngine;

import java.awt.*;
import java.awt.event.*;

public class Canvas extends java.awt.Canvas implements MouseListener, MouseMotionListener, KeyListener
{  	 
	protected int RendererType;
	LynxEngine.Kernel Kernel;
	public int CursorX, CursorY, ButtonMask;
	
	static 
	{		
	}	
	
	public void paint(Graphics g)
	{
		if (!Kernel.bInit())
			Kernel.Init((java.lang.Object)this, getWidth(), getHeight(), RendererType);
		Kernel.Loop();
	}
	
	//
	public Canvas(int renderertype) 
	{				
		RendererType = renderertype;
		ButtonMask = 0;
		Kernel = new LynxEngine.Kernel();
		//if (!Kernel.bInit())
		//	Kernel.Init(this, 480, 320);
		
     	addMouseListener( this );
        addMouseMotionListener( this );
        addKeyListener( this );
	}	
	
	/*public void paint(Graphics g)
	{
		g.setColor(Color.black);
		g.drawString("Pos " + CursorX + CursorY, 0, 30);		
	}
	*/	
	
	public void mouseEntered( MouseEvent e ) 
    {
    	// called when the pointer enters the applet's rectangular area
    }
    public void mouseExited( MouseEvent e ) 
    {
    	// called when the pointer leaves the applet's rectangular area
    }
    public void mouseClicked( MouseEvent e ) 
    {
        // called after a press and release of a mouse button
        // with no motion in between
        // (If the user presses, drags, and then releases, there will be
        // no click event generated.)
    }
    public void mousePressed( MouseEvent e ) 
    {  
    	// called after a button is pressed down    	
        // "Consume" the event so it won't be processed in the
        // default manner by the source which generated it.
    	//Kernel.SetMouseCursorPos(100, 133);
        //e.consume();
    	
    	int Button = e.getButton();
    	switch (Button)
    	{
    		case MouseEvent.NOBUTTON:
    			ButtonMask = 0;
    			break;    			
    		case MouseEvent.BUTTON1:
    			ButtonMask = 1;
    			break;  			
    		case MouseEvent.BUTTON3:
    			ButtonMask = 4;
    			break;	   		
    	}
    	
    	if (Kernel.bInit())    		
    		Kernel.SetMousePollData(CursorX, CursorY, ButtonMask);
    }
    public void mouseReleased( MouseEvent e ) 
    {  
    	// called after a button is released
        //e.consume();
    }
    public void mouseMoved( MouseEvent e ) 
    {  
    	// called during motion when no buttons are down
    	CursorX = e.getX();
    	CursorY = e.getY();
    	ButtonMask = 0;
    	if (Kernel.bInit())
    		Kernel.SetMousePollData(CursorX, CursorY, ButtonMask);        
    }
    public void mouseDragged( MouseEvent e ) 
    {  
    	// called during motion with buttons down
    	int mx = e.getX();
    	int my = e.getY();
        e.consume();
    }    
    
    public void keyPressed( KeyEvent e ) 
    { 
    	if (Kernel.bInit())
    	{
    		int Key = e.getKeyCode();
    		Kernel.SetKeyboardPollData(Key, true);
    	}    	
    }
    public void keyReleased( KeyEvent e ) 
    { 
    	if (Kernel.bInit())
    	{
    		int Key = e.getKeyCode();
    		Kernel.SetKeyboardPollData(Key, false);
    	}    	
    }
    
    public void keyTyped( KeyEvent e ) 
    {
    	int c = e.getKeyChar();
    	if ( c != KeyEvent.CHAR_UNDEFINED ) 
    	{
    		if (Kernel.bInit())       	
    			Kernel.SetCharPressed(c);
    	}       
    }
}