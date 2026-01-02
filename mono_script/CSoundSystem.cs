using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    public class CSoundSystem : CSystem
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern CSound _LoadSound(IntPtr _this, String name, String filename);

        public CSoundSystem()
        {
            GlobalVar.SoundSystem = this;
        }

        public CSoundSystem(IntPtr ptr)
            : base(ptr)
        {
            GlobalVar.SoundSystem = this;
        }

        ~CSoundSystem()
        {
        }

        public CSound LoadSound(String name, String filename)
        {
            return _LoadSound(_this, name, filename);
        }
    }
}