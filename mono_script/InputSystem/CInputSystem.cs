using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    public class CInputSystem : CSystem
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern CInputDevice _FindDevice(IntPtr _this, String name);        

        public CInputSystem()
        {
            GlobalVar.InputSystem = this;
        }

        public CInputSystem(IntPtr ptr)
            : base(ptr)
        {
            GlobalVar.InputSystem = this;
        }

        ~CInputSystem()
        {
        }

        public CInputDevice FindDevice(String name)
        {
            return _FindDevice(_this, name);
        }
    }
}