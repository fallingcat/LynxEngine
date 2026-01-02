using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    public class CCameraContainer : CContainer
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _LookAt(IntPtr _this, CVector3 pos);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _UpdateViewMatrix(IntPtr _this);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _UpdateProjectionMatrix(IntPtr _this, float ratio);
        
        public CCameraContainer()
        {
        }

        public CCameraContainer(IntPtr ptr)
            : base(ptr)
        {
        }

        ~CCameraContainer()
        {
        }

        public void LookAt(CVector3 pos)
        {
            _LookAt(_this, pos);
        }

        public void UpdateViewMatrix()
        {
            _UpdateViewMatrix(_this);
        }

        public void UpdateProjectionMatrix(float ratio)
        {
            _UpdateProjectionMatrix(_this, ratio);
        }
    }
}