using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    public class CScene : CObj
    {
        public enum RENDERMODE
        {
            RENDER_NORMAL = 0,
            RENDER_DEPTHCOMPLEX,
            RENDER_LIGHTINGONLY,
            RENDER_SSAOONLY,
            RENDER_SIMPLE,
            RENDER_SIMPLELIGHTING,
            RENDER_WIREFRAME,
            RENDER_ONELIGHT,
            NUM_RENDERMODE,
        };

        public enum SORTMODE
        {
            SORT_BY_DISTANCE = 0,
            SORT_BY_MATERIAL,
            SORT_BY_DEPTH_LAYER,
        };

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern UInt32 _GetVersion(IntPtr _this);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern bool _Setup(IntPtr _this);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _SetRenderMode(IntPtr _this, RENDERMODE mode);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _LoadBackgroundTexture(IntPtr _this, String filename);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _SetBackgroundTextureUVOffset(IntPtr _this, float u0, float v0, float u1, float v1);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _SetSortMode(IntPtr _this, SORTMODE mode);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern CCamera _CreateCamera(IntPtr _this);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _SetCurrentCamera(IntPtr _this, CCamera c);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern CDynamicObj _CreateDynamicEntity(IntPtr _this);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _AddDynamicObj(IntPtr _this, CDynamicObj dynobj);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern CDynamicObj _GetDynamicObjByName(IntPtr _this, String name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern CDynamicObj _GetDynamicObjByIndex(IntPtr _this, UInt32 i);              

        public CScene()
        {
        }

        public CScene(IntPtr ptr)
            : base(ptr)
        {
        }

        ~CScene()
        {
        }

        public UInt32 GetVersion()
        {
            return _GetVersion(_this);
        }

        public bool Setup()
        {
            return _Setup(_this);
        }

        public void SetRenderMode(RENDERMODE mode)
        {
            _SetRenderMode(_this, mode);
        }

        public void LoadBackgroundTexture(String filename)
        {
            _LoadBackgroundTexture(_this, filename);
        }

        public void SetBackgroundTextureUVOffset(float u0, float v0, float u1, float v1)
        {
            _SetBackgroundTextureUVOffset(_this, u0, v0, u1, v1);
        }

        public void SetSortMode(SORTMODE mode)
        {
            _SetSortMode(_this, mode);
        }

        public CCamera CreateCamera()
        {
            return _CreateCamera(_this);
        }

        public void SetCurrentCamera(CCamera c)
        {
            _SetCurrentCamera(_this, c);
        }

        public CDynamicObj CreateDynamicEntity()
        {
            return _CreateDynamicEntity(_this);
        }

        public void AddDynamicObj(CDynamicObj dynobj)
        {
            _AddDynamicObj(_this, dynobj);
        }

        public CDynamicObj GetDynamicObjByName(String name)
        {
            return _GetDynamicObjByName(_this, name);
        }

        public CDynamicObj GetDynamicObjByIndex(UInt32 i)
        {
            return _GetDynamicObjByIndex(_this, i);
        }

        new public void OnInit()
        {
        }
    }
}