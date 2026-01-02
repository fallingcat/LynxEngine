using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    public class CGameConsoleSystem : CSystem
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern bool _IsRunning(IntPtr _this);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _SetPromptStr(IntPtr _this, String str);

        public CGameConsoleSystem()
        {
            GlobalVar.GameConsoleSystem = this;
        }

        public CGameConsoleSystem(IntPtr ptr)
            : base(ptr)
        {
            GlobalVar.GameConsoleSystem = this;
        }

        ~CGameConsoleSystem()
        {
        }

        public bool IsRunning()
        {
            return _IsRunning(_this);
        }

        public void SetPromptStr(String str)
        {
            _SetPromptStr(_this, str);
        }
    }
}
