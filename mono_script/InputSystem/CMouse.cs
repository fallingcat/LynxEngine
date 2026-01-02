using System;
using System.Runtime.CompilerServices;
using LynxEngine;


namespace LynxEngine
{
    public class CMouse : CInputDevice
    {
        public const int LBUTTON = 1;
        public const int MBUTTON = 2;
        public const int RBUTTON = 4;

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _Poll(IntPtr _this);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern bool _ButtonStatus(IntPtr _this, int flag);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern CPoint2 _GetPos(IntPtr _this);
                
        public CMouse()
        {
        }

        public CMouse(IntPtr ptr)
            : base(ptr)
        {
        }

        ~CMouse()
        {
        }

        public void Poll()
        {
            _Poll(_this);
        }

        public bool ButtonStatus(int flag)
        {
            return _ButtonStatus(_this, flag);
        }

        public CPoint2 GetPos()
        {
            return _GetPos(_this);
        }
    }
}