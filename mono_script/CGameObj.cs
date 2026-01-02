using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    public class CGameObj : CObj
    {
        public CGameObj()
        {
        }

        public CGameObj(IntPtr ptr)
            : base(ptr)
        {
        }

        ~CGameObj()
        {
        }        
    }
}    