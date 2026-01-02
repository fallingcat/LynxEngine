//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
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

#ifndef __LYNXENTITY_H__
#define __LYNXENTITY_H__

namespace LynxEngine 
{
	namespace SceneSystem 
	{
		class CScene;
		class CDynamicObj;
		//#ifdef __TOOL__		
		//	class LYNXENGCLASS CEntity : virtual public CDynamicObj
		//#else
		//	class LYNXENGCLASS CEntity : public CDynamicObj
		//#endif
		class LYNXENGCLASS CEntity : public CDynamicObj
		{	
		private:						
		protected:			
		public:			
			LYNX_DECLARE_CLASS(CEntity);
			CEntity(CScene* const lpscene);			
			virtual ~CEntity();

		friend class CScene;
		};
	}
}

#endif