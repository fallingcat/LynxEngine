using System;
using System.Runtime.CompilerServices;
using LynxEngine;


namespace LynxEngine
{
    public class CInputDevice : CObj
    {
        public CInputDevice()
        {
        }

        public CInputDevice(IntPtr ptr)
            : base(ptr)
        {
        }

        ~CInputDevice()
        {
        }
    }
}
