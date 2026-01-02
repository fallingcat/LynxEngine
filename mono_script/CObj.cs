using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace LynxEngine
{    
    public class CObj
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern UInt32 _GetID(IntPtr _this);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _SetID(IntPtr _this, UInt32 id);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _SetName(IntPtr _this, String name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _SetPos(IntPtr _this, CVector3 pos);

        protected IntPtr _this;        
        //protected HandleRef HRef;
        
        public CObj()
        {
            _this = IntPtr.Zero;                
        }

        public CObj(IntPtr ptr)
        {
            _this = ptr;            
        }        

        ~CObj()
        {
        }

        public IntPtr GetPtr()
        {
            return _this;
        }

        public void SetPtr(IntPtr ptr)
        {
            _this = ptr;
            //HRef = new HandleRef(this, ptr);
        }

        public void SetID(UInt32 id)
        {
            _SetID(_this, id);
        }

        public UInt32 GetID()
        {
            return _GetID(_this);
        }

        public void SetName(String name)
        {
            _SetName(_this, name);
        }

        public void SetPos(CVector3 pos)
        {
            _SetPos(_this, pos);
        }        
        
        public void OnCreate()
        {
        }

        public void OnInit()
        {
            //Debug.WriteLine("Cobj::SetID()");
        }

        public void OnLoop(float step)
        {
        }

        public void OnRender()
        {
        }

        public void OnQuit()
        {
        }

        public void OnDestory()
        {
        }
    }
}

