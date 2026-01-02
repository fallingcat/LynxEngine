using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    public class CGraphicsSystem : CSystem
    {
        public const int RENDER_BUFFER  = 0x1;
        public const int DEPTH_BUFFER   = 0x2;
        public const int STENCIL_BUFFER = 0x4;

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _Print(IntPtr _this, int x, int y, String str);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _PrintWithColor(IntPtr _this, int x, int y, String str, CColor4i color);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern int _GetBackbufferWidth(IntPtr _this);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern int _GetBackbufferHeight(IntPtr _this);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _ClearRenderBuffer(IntPtr _this, int flag, CColor4i color, float z, int s);        

        public CGraphicsSystem()
        {
            GlobalVar.GraphicsSystem = this;
        }

        public CGraphicsSystem(IntPtr ptr)
            : base(ptr)
        {
            GlobalVar.GraphicsSystem = this;
        }

        ~CGraphicsSystem()
        {
            GlobalVar.GraphicsSystem = null;
        }

        public void Print(int x, int y, String str)
        {
            _Print(_this, x, y, str);
        }

        public void PrintWithColor(int x, int y, String str, CColor4i color)
        {
            _PrintWithColor(_this, x, y, str, color);
        }

        public int GetBackbufferWidth()
        {
            return _GetBackbufferWidth(_this);
        }

        public int GetBackbufferHeight()
        {
            return _GetBackbufferHeight(_this);
        }

        public void ClearRenderBuffer(int flag, CColor4i color, float z, int s)
        {
            _ClearRenderBuffer(_this, flag, color, z, s);
        }

        new public void OnInit()
        {            
        }
    }
}
