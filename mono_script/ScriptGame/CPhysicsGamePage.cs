using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    //-------------------------------------------------------------------------------------------------------
    //
    //  說明:   
    //-------------------------------------------------------------------------------------------------------
    public class CPhysicsGamePage : CUIPage
    {
        bool GameConsoleIsRunning = false;
        CSound BGM;
        CVector2[] BackgroundUV = new CVector2[2];
        CScene Scene;

        new public void OnCreate()
        {
            GlobalVar.Engine.OpenPackage("../Game2Data.zip");

            Scene = GlobalVar.SceneSystem.AddScene();
            Scene.SetName("Physics Game Scene");

            CCamera Camera = Scene.CreateCamera();
            Camera.Create();
            Scene.SetCurrentCamera(Camera);
            CCameraContainer CameraContainer = (CCameraContainer)Camera.GetContainer();

            CVector3 CameraPos = new CVector3(10, 10, -135);
            CameraContainer.SetPosition(CameraPos);
            CVector3 CameraLookPos = new CVector3(10, 10, 0);
            CameraContainer.LookAt(CameraLookPos);
            CameraContainer.UpdateProjectionMatrix((float)(GlobalVar.GraphicsSystem.GetBackbufferWidth()) / (float)(GlobalVar.GraphicsSystem.GetBackbufferHeight()));
            CameraContainer.UpdateViewMatrix();

            Scene.LoadBackgroundTexture("../texture/scene/map01/background0.tga");
            BackgroundUV[0] = new CVector2();
            BackgroundUV[1] = new CVector2();
            BackgroundUV[0].x = 0.0f; BackgroundUV[0].y = 0.0f;
            BackgroundUV[1].x = -0.5f; BackgroundUV[1].y = 0.0f;
            Scene.SetBackgroundTextureUVOffset(BackgroundUV[0].x, BackgroundUV[0].y, BackgroundUV[1].x, BackgroundUV[1].y);

            CDynamicObj DynObj = Scene.CreateDynamicEntity();
            DynObj.CreateSprite("Box", "../texture/scene/map01/box.tga", 64, 64);
            DynObj.SetDepthLayer(CRenderableObj.DEPTHLAYERTYPE.DEPTH_LAYER_5);
            CSpriteContainer SpriteContainer = (CSpriteContainer)DynObj.GetContainer();
            SpriteContainer.SetOriginType(CSpriteContainer.ORIGINTYPE.OT_CENTER);
            SpriteContainer.SetAlignmentType(0);
            CVector3 BoxPos = new CVector3(240, 310, 0);
            DynObj.SetPosition(BoxPos);
            DynObj.UpdateTransform();
            DynObj.CreatePhysics(CPhysicsSystem.SHAPETYPE.BOX);
            Scene.AddDynamicObj(DynObj);

            CDynamicObj Player = Scene.CreateDynamicEntity();
            Player.CreateModel("Player", "../model/dynobj/boy/boy.mdl");

            Player.SetDepthLayer(CRenderableObj.DEPTHLAYERTYPE.DEPTH_LAYER_3);
            Player.Yaw(-90.0f, MATHORDER.LYNX_MATH_POST);
            CVector3 PlayerPos = new CVector3(0, 100, 0);
            Player.SetPosition(PlayerPos);
            Player.CreatePhysics(CPhysicsSystem.SHAPETYPE.BOX);
            Scene.AddDynamicObj((CDynamicObj)Player);

            //Player.States = new CPlayer.CState[3];
            //Player.States[(uint)CPlayer.STATE.RUN] = new CPlayer.CState();
            //Player.States[(uint)CPlayer.STATE.RUN].Animation = Player.LoadAnimation("../model/dynobj/boy/run.ani");
            //Player.States[(uint)CPlayer.STATE.RUN].Sound = GlobalVar.SoundSystem.LoadSound("Run", "../sound/run.wav");
            //Player.States[(uint)CPlayer.STATE.RUN].Sound.SetLoops(-1);

            //Player.States[(uint)CPlayer.STATE.JUMP] = new CPlayer.CState();
            //Player.States[(uint)CPlayer.STATE.JUMP].Animation = Player.LoadAnimation("../model/dynobj/boy/jump.ani");
            //Player.States[(uint)CPlayer.STATE.JUMP].Sound = GlobalVar.SoundSystem.LoadSound("Run", "../sound/jump.wav");

            //Player.States[(uint)CPlayer.STATE.ATTACK1] = new CPlayer.CState();
            //Player.States[(uint)CPlayer.STATE.ATTACK1].Animation = Player.LoadAnimation("../model/dynobj/boy/attack1.ani");
            //Player.States[(uint)CPlayer.STATE.ATTACK1].Sound = GlobalVar.SoundSystem.LoadSound("Run", "../sound/attack1.wav");

            ////Player.CreatePhysics(CPhysicsSystem.SHAPETYPE.BOX);

            //BGLayers[0] = new CBGLayer();
            //BGLayers[0].ObjectWidth = 160;
            //BGLayers[0].ObjectHeight = 320;
            //BGLayers[0].ObjectInterval = 110;
            //BGLayers[0].PosY = 320 - 35 - 320;
            //BGLayers[0].ScrollingSpeed = 1;
            //BGLayers[0].CreateObjects(Scene, 6, "Tree", "../texture/scene/map00/tree_23.tga", CRenderableObj.DEPTHLAYERTYPE.DEPTH_LAYER_5);

            //BGLayers[1] = new CBGLayer();
            //BGLayers[1].ObjectWidth = 256;
            //BGLayers[1].ObjectHeight = 256;
            //BGLayers[1].ObjectInterval = 256;
            //BGLayers[1].PosY = 320 - 35 - 256;
            //BGLayers[1].ScrollingSpeed = 2.5f;
            //BGLayers[1].CreateObjects(Scene, 3, "Tree2", "../texture/scene/map00/tree_01.tga", CRenderableObj.DEPTHLAYERTYPE.DEPTH_LAYER_4);

            //BGLayers[2] = new CBGLayer();
            //BGLayers[2].ObjectWidth = 128;
            //BGLayers[2].ObjectHeight = 64;
            //BGLayers[2].ObjectInterval = 110;
            //BGLayers[2].PosY = 320 - 22 - 64;
            //BGLayers[2].ScrollingSpeed = 6.0f;
            //BGLayers[2].CreateObjects(Scene, 6, "Bush", "../texture/scene/map00/bush_08.tga", CRenderableObj.DEPTHLAYERTYPE.DEPTH_LAYER_3);

            //BGLayers[3] = new CBGLayer();
            //BGLayers[3].ObjectWidth = 64;
            //BGLayers[3].ObjectHeight = 64;
            //BGLayers[3].ObjectInterval = 64;
            //BGLayers[3].PosY = 320 - 40;
            //BGLayers[3].ScrollingSpeed = 6.0f;
            //BGLayers[3].CreateObjects(Scene, 9, "GroundTile", "../texture/scene/map00/ground.tga", CRenderableObj.DEPTHLAYERTYPE.DEPTH_LAYER_2);

            BGM = GlobalVar.SoundSystem.LoadSound("BGM", "../sound/BGM.wav");
            BGM.SetLoops(-1);

            Scene.SetRenderMode(CScene.RENDERMODE.RENDER_SIMPLE);
            Scene.SetSortMode(CScene.SORTMODE.SORT_BY_DEPTH_LAYER);
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

            //Player.SetState(CPlayer.STATE.RUN);

            //for (int i = 0; i < BGLayers.GetLength(0); i++)
            //{
            //    for (int j = 0; j < BGLayers[i].Objects.GetLength(0); j++)
            //    {
            //        BGLayers[i].Pos[j] = (int)(j * BGLayers[i].ObjectInterval);
            //    }
            //}

            BGM.SetVolume(0.6f);
            BGM.Play();
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