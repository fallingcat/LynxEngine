using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    public class CControl : CObj
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _SetAlignmentType(IntPtr _this, UInt32 flag);        

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _SetPosition(IntPtr _this, CVector3 pos);        

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern bool _HitTest(IntPtr _this, CPoint2 pos);        

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _OnHit(IntPtr _this);        

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _SetTargetName(IntPtr _this, String name);        

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern String _GetTargetName(IntPtr _this);        

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern bool _DoesAcceptInput(IntPtr _this);        

        public CControl()
        {
        }

        public CControl(IntPtr ptr)
            : base(ptr)
        {
        }

        ~CControl()
        {
        }

        public void SetAlignmentType(UInt32 flag)
        {
            _SetAlignmentType(_this, flag);
        }

        public void SetPosition(CVector3 pos)
        {
            _SetPosition(_this, pos);
        }

        public bool HitTest(CPoint2 pos)
        {
            return _HitTest(_this, pos);            
        }

        public void OnHit()
        {
            _OnHit(_this);
        }

        public void SetTargetName(String name) 
        {
            _SetTargetName(_this, name);
        }

        public String GetTargetName()
        {
            return _GetTargetName(_this);
        }
    
        public bool DoesAcceptInput()
        {
            return _DoesAcceptInput(_this);        
        }
    }
}
