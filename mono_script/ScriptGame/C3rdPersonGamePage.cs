using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    //-------------------------------------------------------------------------------------------------------
    //
    //  說明:   
    //-------------------------------------------------------------------------------------------------------
    public class C3rdPersonGamePage : CUIPage
    {
        bool GameConsoleIsRunning = false;
        //CSound BGM;
        CScene Scene;

        new public void OnCreate()
        {
            GlobalVar.Engine.OpenPackage("../Game3Data.zip");

            Scene = GlobalVar.SceneSystem.AddScene();
            Scene.SetName("3rd Person Game Scene");            
            
            Scene.SetRenderMode(CScene.RENDERMODE.RENDER_SIMPLE);
            Scene.SetSortMode(CScene.SORTMODE.SORT_BY_MATERIAL);
            Scene.Setup();
        }
        //-------------------------------------------------------------------------------------------------------
        //
        //  說明:   
        //-------------------------------------------------------------------------------------------------------         
        new public void OnInit()
        {
            GlobalVar.SystemMouse = (CMouse)(GlobalVar.InputSystem.FindDevice("System Mouse"));
            GlobalVar.Engine.DeleteLastUIPage();
            
            //BGM.SetVolume(0.6f);
            //BGM.Play();
        }
        //-------------------------------------------------------------------------------------------------------
        //
        //  說明:   
        //-------------------------------------------------------------------------------------------------------
        new public void OnLoop(float step)
        {
            GlobalVar.SystemMouse.Poll();

            if (GlobalVar.GameConsoleSystem.IsRunning())
            {
                GameConsoleIsRunning = true;
                //Player.States[(int)Player.State].Sound.Pause();
            }
            else
            {
                //AnimationLoop();

                if (GameConsoleIsRunning)
                {
                    //if (!Player.States[(int)Player.State].Sound.IsPlaying())
                    //{
                    //    Player.States[(int)Player.State].Sound.Play();
                    //}
                    GameConsoleIsRunning = false;
                }
            }
        }
        //-------------------------------------------------------------------------------------------------------
        //
        //  說明:   
        //-------------------------------------------------------------------------------------------------------
        new public void OnRender()
        {
            GlobalVar.Engine.Render();
        }
        //-------------------------------------------------------------------------------------------------------
        //
        //  說明:   
        //-------------------------------------------------------------------------------------------------------
        new public void OnQuit()
        {
            base.OnQuit();
        }
    }
}