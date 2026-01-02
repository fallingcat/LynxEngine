using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    public class CSpriteContainer : CContainer
    {
        public const uint H_ALIGN_LEFT      = 0x01;			
		public const uint H_ALIGN_CENTER    = 0x02;
		public const uint H_ALIGN_RIGHT	    = 0x04;
		public const uint V_ALIGN_TOP       = 0x10;			
		public const uint V_ALIGN_CENTER    = 0x20;
		public const uint V_ALIGN_BOTTOM    = 0x40;
		
		public enum ORIGINTYPE{
			OT_UPPER_LEFT,
			OT_CENTER,
		};

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _SetOriginType(IntPtr _this, ORIGINTYPE type);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void _SetAlignmentType(IntPtr _this, UInt32 type);

        public CSpriteContainer()
        {
        }

        public CSpriteContainer(IntPtr ptr)
            : base(ptr)
        {
        }

        ~CSpriteContainer()
        {
        }

        public void SetOriginType(ORIGINTYPE type)
        {
            _SetOriginType(_this, type);
        }

        public void SetAlignmentType(UInt32 type)
        {
            _SetAlignmentType(_this, type);
        }
    }
}
