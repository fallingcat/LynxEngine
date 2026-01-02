using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    public class CSceneSystem : CSystem
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern CScene _AddScene(IntPtr _this);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _RemoveAllScenes(IntPtr _this);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern CScene _GetCurrentScene(IntPtr _this);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern CScene _GetScene(IntPtr _this, UInt32 i);

        public CSceneSystem()
        {
            GlobalVar.SceneSystem = this;
        }

        public CSceneSystem(IntPtr ptr)
            : base(ptr)
        {
            GlobalVar.SceneSystem = this;
        }

        ~CSceneSystem()
        {
        }

        public CScene AddScene()
        {
            return _AddScene(_this); 
        }

        public void RemoveAllScenes()
        {
            _RemoveAllScenes(_this);
        }

        public CScene _GetCurrentScene()
        {
            return _GetCurrentScene(_this);
        }

        public CScene _GetScene(UInt32 i)
        {
            return _GetScene(_this, i);
        }
    }
}
