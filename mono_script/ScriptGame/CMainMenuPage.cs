using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    public class CMainMenuPage : CUIPage
    {
        const int ID_GAMES 			    = 0;
        const int ID_OPTION			    = 1;
        const int ID_BUY_FULL_VERSION   = 2;

        protected CSound PressedSound = null;

        new public void OnCreate()
        {
            CRect Rect = new CRect(0, 140, 480, 200);
            CVector2 ButtonSize = new CVector2(200, 50);
            CButton Button = AddButton("../texture/button.tga", Rect, ButtonSize, ID_GAMES, "Games");
            Button.SetTargetName("SelectGamePage");            

            Rect.y1 = 200;
            Rect.y2 = 260;
            Button = AddButton("../texture/button.tga", Rect, ButtonSize, ID_OPTION, "Option");
            Button.SetTargetName("OptionPage");            

            Rect.y1 = 260;
            Rect.y2 = 320;
            Button = AddButton("../texture/button.tga", Rect, ButtonSize, ID_BUY_FULL_VERSION, "Buy Full Version");
            Button.SetTargetName("SelectGamePage");            

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
                    if (PressedButton.GetID() != ID_BUY_FULL_VERSION)
                        Quit();
                    else
                        SetPressedControl(null);
                }
            }
            else
            {        
                GlobalVar.SystemMouse.Poll();

                for (int i=0; i<GetNumControls(); i++)            
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
                CColor4i Color = new CColor4i(62, 60, 58, 255);
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
