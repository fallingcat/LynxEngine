using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    public class CDynamicObj : CRenderableObj
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _CreateModel(IntPtr _this, String name, String filename);        

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _CreateSprite(IntPtr _this, String name, String filename, float w, float h);        

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _CreatePhysics(IntPtr _this, UInt32 type);                      

        public CDynamicObj()
        {
        }

        public CDynamicObj(IntPtr ptr)
            : base(ptr)
        {
        }

        ~CDynamicObj()
        {
        }

        public void CreateModel(String name, String filename)
        {
            _CreateModel(_this, name, filename);        
        }

        public void CreateSprite(String name, String filename, float w, float h)
        {
            _CreateSprite(_this, name, filename, w, h);
        }

        public void CreatePhysics(CPhysicsSystem.SHAPETYPE type)
        {
            _CreatePhysics(_this, (uint)type);
        }

        new public void OnInit()
        {            
        }
    }
}