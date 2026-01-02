	//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.net
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Owen Wu : 2004/07/27
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXIOSZIPSTREAM_H__
#define __LYNXIOSZIPSTREAM_H__

#include <LynxEngineClasses.h>
#include <LynxiOSFileStream.h>
#include <LynxZipStream.h>

namespace LynxEngine 
{
	namespace iOS
	{		
		/**	
		*  @brief Base stream object class of the entire engine。
		* 
		*  The basic stream object class of the entire engine. 
		*	
		*/
		class LYNXENGCLASS CZipStream : public LynxEngine::CZipStream
		{	
			LYNX_DECLARE_CLASS(CZipStream);
		public:		
            static CFileStream              m_FileStream;
		protected:		
		public:	
			CZipStream();
			virtual ~CZipStream();
		};	
	}
}

#endif