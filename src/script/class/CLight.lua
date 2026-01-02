-------------------------------------------------------------------------------------------------------
--    __                    ____            __
--   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
--  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
--  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
--      /___/                       /___/ 
--
-- 	class CLight
-------------------------------------------------------------------------------------------------------
function list_iter (t)
  local i = 0
  local n = table.getn(t)
  return function ()
            i = i + 1
            if i <= n then return t[i] end
         end
end

Light = CLight.new(1)
-------------------------------------------------------------------------------------------------------
--
--  »¡©ú:   
-------------------------------------------------------------------------------------------------------
Light.OnInit = function () 				
    Table = {1, 2, 3, "G", "HH"}
    for element in list_iter(Table) do
      print(element)
    end

	Light:SetName("Derived Script Light")		
	_CLight.m_Radius = LynxSDK.Math.QuickSin(45.0) -- 35.5
	_CLight.m_Radius = LynxSDK.Math.QuickCos(35.0)
	Light.m_Radius = LynxSDK.Math.QuickCos(35.0)
	--LynxSDK.Math.QuickSin(45.0)
	--Obj:Yaw(50, POST)			
	--Obj:Pitch(50, POST)			
	--Obj.UpdateTransform()
	--Obj:Forward(-10, POST)	
	--Test.InitTable.HP = Test.InitTable.HP+1 --Scene.GetVersion() + CDynamicObj.Add(19, 70)
	return 1;
end
