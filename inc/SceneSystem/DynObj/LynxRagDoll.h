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

#ifndef __LYNXRAGDOLL_H__
#define __LYNXRAGDOLL_H__

namespace LynxEngine 
{
	namespace SceneSystem 
	{
		class CScene;
		class CDynamicObj;
		class LYNXENGCLASS CRagDoll : public CDynamicObj
		{	
			LYNX_DECLARE_CLASS(CRagDoll);
		private:						
		protected:			
		public:						
			CRagDoll();
			CRagDoll(CScene *lpscene);			
			~CRagDoll();

		friend class CScene;
		};
	}
}

#endif