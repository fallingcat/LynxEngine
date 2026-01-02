using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    public class CContainer : CObj
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _Render(IntPtr _this, CCameraContainer camera);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _UpdateTransform(IntPtr _this);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _SetPosition(IntPtr _this, CVector3 pos);
        
        public CContainer()
        {
        }

        public CContainer(IntPtr ptr)
            : base(ptr)
        {
        }

        ~CContainer()
        {
        }

        public void Render(CCameraContainer camera)
        {
            _Render(_this, camera);
        }

        public void UpdateTransform()
        {
            _UpdateTransform(_this);
        }

        public void SetPosition(CVector3 pos)
        {
            _SetPosition(_this, pos);
        }
    }
}
