using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    public class CSelectGamePage : CUIPage
    {
        const int SCROLLING_GAME  	= 0;
        const int PHYSICS_GAME		= 1;
        const int THIRDPERSON_GAME  = 2;
        const int FPS_GAME          = 3;
        const int GAME5		        = 4;
        const int GAME6             = 5;

        protected CSound PressedSound = null;

        new public void OnCreate()
        {
            CRect Rect = new CRect(0, 10, 480, 70);
            CVector2 ButtonSize = new CVector2(400, 50);
            CButton Button = AddButton("../texture/button.tga", Rect, ButtonSize, SCROLLING_GAME, "Scrolling Game");
            Button.SetTargetName("ScrollingGamePage");

            Rect.y1 = 70;
            Rect.y2 = 130;
            Button = AddButton("../texture/button.tga", Rect, ButtonSize, PHYSICS_GAME, "Physics Game");
            Button.SetTargetName("PhysicsGamePage");

            Rect.y1 = 130;
            Rect.y2 = 190;
            Button = AddButton("../texture/button.tga", Rect, ButtonSize, THIRDPERSON_GAME, "3rd Person Game");
            Button.SetTargetName("3rdPersonGamePage");

            Rect.y1 = 190;
            Rect.y2 = 250;
            Button = AddButton("../texture/button.tga", Rect, ButtonSize, FPS_GAME, "FPS Game");
            Button.SetTargetName("SelectGamePage");

            Rect.y1 = 250;
            Rect.y2 = 310;
            Button = AddButton("../texture/button.tga", Rect, ButtonSize, GAME5, "Game 5");
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
                    if (PressedButton.GetID() == SCROLLING_GAME || PressedButton.GetID() == PHYSICS_GAME || PressedButton.GetID() == THIRDPERSON_GAME)
                        Quit();
                    else
                        SetPressedControl(null);
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
