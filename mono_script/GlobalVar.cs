using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    static public class GlobalVar
    {
        public static CEngine Engine = null;
        public static CGameConsoleSystem GameConsoleSystem = null;
        public static CGraphicsSystem GraphicsSystem = null;
        public static CSoundSystem SoundSystem = null;
        public static CInputSystem InputSystem = null;
        public static CSceneSystem SceneSystem = null;
        public static CPhysicsSystem PhysicsSystem = null;
        public static CMouse SystemMouse = null;
    }
}
