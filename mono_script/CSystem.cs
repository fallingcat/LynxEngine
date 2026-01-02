using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    public class CSystem : CObj
    {
        public CSystem()
        {            
        }

        public CSystem(IntPtr ptr)
            : base(ptr)
        {         
        }

        ~CSystem()
        {
        }        
    }
}

