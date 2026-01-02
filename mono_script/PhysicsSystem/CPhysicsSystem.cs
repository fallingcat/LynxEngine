using System;
using System.Runtime.CompilerServices;
using LynxEngine;

namespace LynxEngine
{
    public class CPhysicsSystem : CSystem
    {
        public enum SHAPETYPE{
            NULL_SHAPE = -1,
			BOX,
			SPHERE,	
			CAPSULE,	
			CYLINDER,
			PLANE,
			HULL,
			TRIMESH,
			HEIGHTFIELD,
			NUM_SHAPETYPE,	
		};

        public CPhysicsSystem()
        {
            GlobalVar.PhysicsSystem = this;
        }

        public CPhysicsSystem(IntPtr ptr)
            : base(ptr)
        {
            GlobalVar.PhysicsSystem = this;
        }

        ~CPhysicsSystem()
        {
        }       
    }
}
