//###########################################################################
//  LynxEngine Ver 1.00 
//  Copyright (C) 2003 fallingCAT studios.  All Rights Reserved.
//
//
//  Created by Owen Wu : 2005/11/02
//  Last Update : 
//--------------------------------------------------------------------------
//  說明:
//
//###########################################################################

#ifndef __LYNXNXPHYSICSSYSTEM_H__
#define __LYNXNXPHYSICSSYSTEM_H__

#ifdef __PHYSX__

//#include <NxPhysicsSDK.h>
#include <NxPhysics.h>
#include <LynxEngineClasses.h>
#include <NxStream.h>
#include <PhysicsSystem/LynxPhysicsSystem.h>

namespace LynxPhysics 
{
	namespace Novodex 
	{
		class MemAllocator : public NxUserAllocator    
		{    
		public:        
			void * malloc(size_t size)
			{            
				return LYNX_MALLOC(size);        
			}
			void * mallocDEBUG(size_t size,const char *fileName, int line)        
			{            
				return lynxMemAlloc_d(size, fileName, __FUNCTION__, line);        
			}        
			void * realloc(void * memory, size_t size)        
			{            
				return LYNX_REALLOC(memory, size);        
			}        
			void free(void * memory)        
			{            
				LYNX_FREE(memory);        
			}   
		};

		class UserStream : public NxStream
		{
		public:
									UserStream(const LYNXCHAR* filename, bool load);
		virtual						~UserStream();

		virtual		NxU8			readByte()								const;
		virtual		NxU16			readWord()								const;
		virtual		NxU32			readDword()								const;
		virtual		float			readFloat()								const;
		virtual		double			readDouble()							const;
		virtual		void			readBuffer(void* buffer, NxU32 size)	const;

		virtual		NxStream&		storeByte(NxU8 b);
		virtual		NxStream&		storeWord(NxU16 w);
		virtual		NxStream&		storeDword(NxU32 d);
		virtual		NxStream&		storeFloat(NxReal f);
		virtual		NxStream&		storeDouble(NxF64 f);
		virtual		NxStream&		storeBuffer(const void* buffer, NxU32 size);

					LYNXFILE		File;
		};

		class MemoryWriteBuffer : public NxStream
		{
		public:
									MemoryWriteBuffer();
		virtual						~MemoryWriteBuffer();

		virtual		NxU8			readByte()								const	{ NX_ASSERT(0);	return 0;	}
		virtual		NxU16			readWord()								const	{ NX_ASSERT(0);	return 0;	}
		virtual		NxU32			readDword()								const	{ NX_ASSERT(0);	return 0;	}
		virtual		float			readFloat()								const	{ NX_ASSERT(0);	return 0.0f;}
		virtual		double			readDouble()							const	{ NX_ASSERT(0);	return 0.0;	}
		virtual		void			readBuffer(void* buffer, NxU32 size)	const	{ NX_ASSERT(0);				}

		virtual		NxStream&		storeByte(NxU8 b);
		virtual		NxStream&		storeWord(NxU16 w);
		virtual		NxStream&		storeDword(NxU32 d);
		virtual		NxStream&		storeFloat(NxReal f);
		virtual		NxStream&		storeDouble(NxF64 f);
		virtual		NxStream&		storeBuffer(const void* buffer, NxU32 size);

					NxU32			currentSize;
					NxU32			maxSize;
					NxU8*			data;
		};

		class MemoryReadBuffer : public NxStream
		{
		public:
									MemoryReadBuffer(const NxU8* data);
		virtual						~MemoryReadBuffer();

		virtual		NxU8			readByte()								const;
		virtual		NxU16			readWord()								const;
		virtual		NxU32			readDword()								const;
		virtual		float			readFloat()								const;
		virtual		double			readDouble()							const;
		virtual		void			readBuffer(void* buffer, NxU32 size)	const;

		virtual		NxStream&		storeByte(NxU8 b)							{ NX_ASSERT(0);	return *this;	}
		virtual		NxStream&		storeWord(NxU16 w)							{ NX_ASSERT(0);	return *this;	}
		virtual		NxStream&		storeDword(NxU32 d)							{ NX_ASSERT(0);	return *this;	}
		virtual		NxStream&		storeFloat(NxReal f)						{ NX_ASSERT(0);	return *this;	}
		virtual		NxStream&		storeDouble(NxF64 f)						{ NX_ASSERT(0);	return *this;	}
		virtual		NxStream&		storeBuffer(const void* buffer, NxU32 size)	{ NX_ASSERT(0);	return *this;	}

		mutable		const NxU8*		buffer;
		};

		class LYNXENGCLASS CPhysicsSystem : public LynxEngine::PhysicsSystem::CPhysicsSystem
		{	
			LYNX_DECLARE_CLASS(CPhysicsSystem);
		protected:	
			MemAllocator								m_MemAllocator;
			NxPhysicsSDK*								m_NxPhySDK;
		public:			
			CPhysicsSystem(LynxEngine::CEngine* const lpengine);
			~CPhysicsSystem();

			LYNXINLINE const LYNXCHAR*					vGetAPIString() {return _T("Novodex"); };
			LYNXBOOL									vCreate();						
			LynxEngine::PhysicsSystem::CPhysicsWorld* const	vCreateWorld();
			LYNXINLINE NxPhysicsSDK*					GetNxPhySDK() {return m_NxPhySDK; };

			LYNXBOOL									vInit();			
		};	
	}
}

extern LYNXENGCLASS LynxEngine::CMemAllocator*	glpMemAllocator;

#endif

#endif