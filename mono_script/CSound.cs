using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    public class CSound : CObj
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _SetLoops(IntPtr _this, Int32 loop);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _SetVolume(IntPtr _this, float vol);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern bool _Play(IntPtr _this);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _Pause(IntPtr _this);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _Stop(IntPtr _this);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern bool _IsPlaying(IntPtr _this);

        public CSound()
        {
        }

        public CSound(IntPtr ptr)
            : base(ptr)
        {
        }

        ~CSound()
        {
        }

        public void SetLoops(Int32 loop)
        {
            _SetLoops(_this, loop);
        }

        public void SetVolume(float vol)
        {
            _SetVolume(_this, vol);
        }

        public bool Play()
        {
            return _Play(_this);
        }

        public void Pause()
        {
            _Pause(_this);
        }

        public void Stop()
        {
            _Stop(_this);
        }

        public bool IsPlaying()
        {
            return _IsPlaying(_this);
        }
    }
}