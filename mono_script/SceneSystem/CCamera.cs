using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    public class CCamera : CDynamicObj
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern bool _Create(IntPtr _this);

        public CCamera()
        {
        }

        public CCamera(IntPtr ptr)
            : base(ptr)
        {
        }

        ~CCamera()
        {
        }

        public bool Create()
        {
            return _Create(_this);
        }
    }
}