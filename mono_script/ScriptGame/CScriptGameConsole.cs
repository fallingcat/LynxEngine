using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    public class CScriptGameConsole : CGameConsoleSystem
    {
        public CScriptGameConsole()
        {
        }

        public CScriptGameConsole(IntPtr ptr)
            : base(ptr)
        {
        }

        ~CScriptGameConsole()
        {
        }

        new public void OnInit()
        {
            SetPromptStr("ScriptGame");
        }
    }
}
