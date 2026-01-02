using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    public class CScriptGameEngine : CEngine
    {
        public CScriptGameEngine()
        {
        }

        public CScriptGameEngine(IntPtr ptr)
        {
        }

        new public void OnInit()
        {
            SetName("ScriptGameEngine");
            CreatePhysicsWorld();
        }
    }
}
