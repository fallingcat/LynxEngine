using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    public class COptionPage : CUIPage
    {
        const int ID_MUSIC			    = 0;
        const int ID_SOUND				= 1;
        const int ID_OPENGL2            = 2;
        const int ID_BACK               = 3;
        
        protected CSound PressedSound = null;        

        new public void OnCreate()
        {
            CRect Rect = new CRect(0, 40, 240, 100);
            CVector2 ButtonSize = new CVector2(200, 50);
            CButton Button = AddButton("../texture/button.tga", Rect, ButtonSize, ID_MUSIC, "Music");
            Button.SetTargetName("SelectGamePage");

            Rect.y1 = 100;
            Rect.y2 = 160;
            Button = AddButton("../texture/button.tga", Rect, ButtonSize, ID_SOUND, "Sound");
            Button.SetTargetName("OptionPage");

            Rect.y1 = 160;
            Rect.y2 = 220;
            Button = AddButton("../texture/button.tga", Rect, ButtonSize, ID_OPENGL2, "OpenGL 2.0");
            Button.SetTargetName("MainMenuPage");

            Rect.x1 = 480 - 120;
            Rect.y1 = 260;
            Rect.x2 = 480 - 20;
            Rect.y2 = 320;
            ButtonSize.x = 100;
            ButtonSize.y = 50;
            Button = AddButton("../texture/button.tga", Rect, ButtonSize, ID_BACK, "Back");
            Button.SetTargetName("MainMenuPage");

            PressedSound = GlobalVar.SoundSystem.LoadSound("Pressed", "../sound/button-14.wav");
        }

        new public void OnInit()
        {
            GlobalVar.SystemMouse = (CMouse)(GlobalVar.InputSystem.FindDevice("System Mouse"));
            GlobalVar.Engine.DeleteLastUIPage();
        }

        new public void OnLoop(float step)
        {
            CControl PressedButton = GetPressedControl();     

            if (PressedButton != null)
            {
                if (PressedButton.DoesAcceptInput())
                {
                    //CEngine:SetSwipeSpeed(30.0)
                    Quit();
                }
            }
            else
            {
                GlobalVar.SystemMouse.Poll();

                for (int i = 0; i < GetNumControls(); i++)
                {
                    CButton Button = (CButton)GetControlByIndex(i);
                    if (GlobalVar.SystemMouse.ButtonStatus(CMouse.LBUTTON))
                    {
                        CPoint2 CursorPos = GlobalVar.SystemMouse.GetPos();
                        if (Button.HitTest(CursorPos))
                        {
                            Button.OnHit();
                            PressedSound.Play();
                            SetPressedControl(Button);            
                        }
                    }
                }
            }
        }

        new public void OnRender()
        {
            if (GlobalVar.GraphicsSystem != null)
            {
                CColor4i Color = new CColor4i(62, 160, 158, 255);
                GlobalVar.GraphicsSystem.ClearRenderBuffer(CGraphicsSystem.RENDER_BUFFER | CGraphicsSystem.DEPTH_BUFFER, Color, 1.0f, 0);
                RenderControls();
            }
        }

        new public void OnQuit()
        {
            base.OnQuit();
        }
    }
}