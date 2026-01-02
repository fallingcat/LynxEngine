using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    public class CEngine : CObj
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern bool _OpenPackage(IntPtr _this, String name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _ClosePackage(IntPtr _this);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern CUIPage _CreateUIPage(IntPtr _this, String name, String scriptfile, String scriptname);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _SetCurrentUIPage(IntPtr _this, CUIPage page);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern CUIPage _FindUIPage(IntPtr _this, String name);        

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _DeleteLastUIPage(IntPtr _this);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _Render(IntPtr _this);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _OpenScene(IntPtr _this, String name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _CreatePhysicsWorld(IntPtr _this);

        public CEngine()
        {
            GlobalVar.Engine = this;
        }

        public CEngine(IntPtr ptr)
            : base(ptr)
        {
            GlobalVar.Engine = this;
        }

        ~CEngine()
        {
            GlobalVar.Engine = null;
        }

        public bool OpenPackage(String name)
        {
            return _OpenPackage(_this, name);
        }

        public void ClosePackage(String name)
        {
            _ClosePackage(_this);
        }

        public CUIPage CreateUIPage(String name, String scriptfile, String scriptname)
        {
            return _CreateUIPage(_this, name, scriptfile, scriptname);
        }

        public void SetCurrentUIPage(CUIPage page)
        {
            _SetCurrentUIPage(_this, page);
        }

        public CUIPage FindUIPage(String name)
        {
            return _FindUIPage(_this, name);
        }

        public void DeleteLastUIPage()
        {
            _DeleteLastUIPage(_this);
        }

        public void Render()
        {
            _Render(_this);
        }

        public void OpenScene(String name)
        {
            _OpenScene(_this, name);
        }

        public void CreatePhysicsWorld()
        {
            _CreatePhysicsWorld(_this);
        }
        
        new public void OnCreate()
        {
        }

        new public void OnInit()
        {
        }
    }
}
