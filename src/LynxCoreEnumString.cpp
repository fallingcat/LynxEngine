#include <LynxString.h>
#include <LynxCoreEnumString.h>

#ifdef __TOOL__

namespace LynxEngine
{	
	ENUMDATA	LYNXPLAYMODE_EnumData[] = { {_T("Once"),		LYNX_PM_ONCE },
				 						    {_T("Loop"),		LYNX_PM_LOOP },
											{_T("Bounce"),		LYNX_PM_BOUNCE } };
}

#endif