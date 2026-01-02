
package LynxEngine.Android;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Typeface;
import android.graphics.Rect;
import android.graphics.Paint.Style;
import android.text.TextPaint;
import android.text.StaticLayout;
import android.text.Layout.Alignment;
import android.graphics.drawable.Drawable;
import android.opengl.GLUtils;
import java.nio.ByteBuffer;
import java.nio.charset.Charset;
import android.graphics.Matrix;
import java.lang.reflect.*;
import android.util.Log;

public class BitmapContext {
	private int m_Width;
    private int m_Height;
    private Bitmap m_Bitmap;
    private Canvas m_Canvas;
	private TextPaint m_TextPaint;
	private TextPaint m_StrokePaint;
    public BitmapContext(int width, int height) {
        m_Width = width;
        m_Height = height;
		m_Bitmap = Bitmap.createBitmap (m_Width, m_Height, Bitmap.Config.ARGB_8888);
		m_Canvas = new Canvas (m_Bitmap);
		m_TextPaint = new TextPaint();
		m_TextPaint.setAntiAlias(true);
		m_StrokePaint = new TextPaint();
		m_StrokePaint.setAntiAlias(true);		
    }

	public byte[] getBitmap (String text, Typeface typeface, 
		int size, int color, int alignment, int x1, int y1, int x2, int y2, 
		boolean bShadow, 
		boolean bOutline, int strokeColor, int strokeWidth) 
	{		
		String align_h = "ALIGN_CENTER", align_v = "ALIGN_CENTER";
		if ((alignment & 0x01) != 0)
			align_h = "ALIGN_NORMAL";
		else if ((alignment & 0x02) != 0)
			align_h = "ALIGN_CENTER";
		else if ((alignment & 0x04) != 0)
			align_h = "ALIGN_OPPOSITE";
		if ((alignment & 0x10) != 0)
			align_v = "ALIGN_NORMAL";
		else if ((alignment & 0x20) != 0)
			align_v = "ALIGN_CENTER";
		else if ((alignment & 0x40) != 0)
			align_v = "ALIGN_OPPOSITE";

		//text = NativeActivity.EmojiFilter(text);

		m_TextPaint.setTextSize (size);
		m_TextPaint.setTypeface (typeface);
		m_Bitmap.eraseColor (0);
		m_Canvas.drawColor (0);
		StaticLayout textLayout;

		Rect bounds = new Rect();
		m_TextPaint.getTextBounds(text, 0, text.length(), bounds);

		if (bShadow)
			m_TextPaint.setShadowLayer(0, 2,2,0x99000000);
		else
			m_TextPaint.clearShadowLayer();

		Matrix MirrorM = new Matrix();
		MirrorM.setScale(1.0f, -1.0f);
		MirrorM.postTranslate(0.0f, m_Height);

		if (bOutline)
		{
			m_StrokePaint.setTextSize (size);
			m_StrokePaint.setTypeface (typeface);			
			m_StrokePaint.setColor (strokeColor);
			m_StrokePaint.setStyle(Paint.Style.STROKE);
			m_StrokePaint.setStrokeWidth(strokeWidth);
			
			textLayout = new StaticLayout(text, m_StrokePaint, x2-x1, Alignment.valueOf(align_h), 1.0f, 0.0f, false);
			m_Canvas.save();			
			
			m_Canvas.setMatrix(MirrorM);
			
			if (align_v.equals ("ALIGN_NORMAL")) {
				m_Canvas.translate (x1, y1);				
			}
			else if (align_v.equals("ALIGN_CENTER")) {
				int h = textLayout.getHeight();
				m_Canvas.translate (x1, (y1+y2-h)/2<0?0:(y1+y2-h)/2);
			}
			else if (align_v.equals("ALIGN_OPPOSITE")) {
				int h = textLayout.getHeight();
				m_Canvas.translate (x1, h-y2+y1<0?0:h-y2+y1);
			}			
			textLayout.draw (m_Canvas);
			m_Canvas.restore();					
		}
		
		if (bShadow) 
		{
			m_TextPaint.setColor (0x99000000);
			textLayout = new StaticLayout(text, m_TextPaint, x2-x1, Alignment.valueOf(align_h), 1.0f, 0.0f, false);
			m_Canvas.save();			

			m_Canvas.setMatrix(MirrorM);

			if (align_v.equals ("ALIGN_NORMAL")) {
				m_Canvas.translate (x1+2, y1+2);
				//m_Canvas.translate (x1+2, (y1+y2-bounds.height())/2<0?2:(y1+y2-bounds.height())/2+2);
			}
			else if (align_v.equals("ALIGN_CENTER")) {
				int h = textLayout.getHeight();
				m_Canvas.translate (x1+2, (y1+y2-h)/2<0?2:(y1+y2-h)/2+2);
			}
			else if (align_v.equals("ALIGN_OPPOSITE")) {
				int h = textLayout.getHeight();
				m_Canvas.translate (x1+2, h-y2+y1<0?2:h-y2+y1+2);
			}			
			textLayout.draw (m_Canvas);
			m_Canvas.restore();
		}
					
		m_TextPaint.setColor (color);
		textLayout = new StaticLayout(text, m_TextPaint, x2-x1, Alignment.valueOf(align_h), 1.0f, 0.0f, false);
		m_Canvas.save();		

		m_Canvas.setMatrix(MirrorM);

		if (align_v.equals ("ALIGN_NORMAL")) {
			m_Canvas.translate (x1, y1);
			//m_Canvas.translate (x1, (y1+y2-bounds.height())/2<0?0:(y1+y2-bounds.height())/2);
		}
		else if (align_v.equals("ALIGN_CENTER")) {
			int h = textLayout.getHeight();
			m_Canvas.translate (x1, (y1+y2-h)/2<0?0:(y1+y2-h)/2);
		}
		else if (align_v.equals("ALIGN_OPPOSITE")) {
			int h = textLayout.getHeight();
			m_Canvas.translate (x1, h-y2+y1<0?0:h-y2+y1);
		}		
		textLayout.draw (m_Canvas);
		m_Canvas.restore();

		ByteBuffer buffer = ByteBuffer.allocateDirect(m_Bitmap.getByteCount());	
		m_Bitmap.copyPixelsToBuffer(buffer);
		byte[] array = buffer.array();

		try
		{
			destroyDirectByteBuffer(buffer);
		}
		catch (IllegalArgumentException |IllegalAccessException|
            InvocationTargetException| SecurityException| NoSuchMethodException e)
		{
            Log.v("BitmapContext", e.toString());
		}
		return array;
	}
	
    public void destroyDirectByteBuffer(ByteBuffer toBeDestroyed)
            throws IllegalArgumentException, IllegalAccessException,
            InvocationTargetException, SecurityException, NoSuchMethodException
    {

        Method cleanerMethod = toBeDestroyed.getClass().getMethod("cleaner");
        if (cleanerMethod != null) {
            cleanerMethod.setAccessible(true);
            Object cleaner = cleanerMethod.invoke(toBeDestroyed);
            if (cleaner != null) {
                Method cleanMethod = cleaner.getClass().getMethod("clean");
                if (cleanMethod != null) {
                    cleanMethod.setAccessible(true);
                    cleanMethod.invoke(cleaner);
                }
            }
        }
    }

}