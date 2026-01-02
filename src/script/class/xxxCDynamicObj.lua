-------------------------------------------
--    __                    ____            __
--   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
--  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
--  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
--      /___/                       /___/ 
--
-- 	class CDynamicObj
-------------------------------------------
CDynamicObj = {
	-----------------------------------------	
	-- 
	SetName = function(name) 
		_CDynamicObj:SetName(name) 
	end,
	-----------------------------------------	
	-- 
	Forward = function(f) 
		_CDynamicObj:Forward(f) 
	end,		
	-----------------------------------------	
	-- 
	Add = function(x, y)
		return x+y
	end,			
}

InitTable = {
	HP = 33,
	MP = 25,
}

function OnInit() 				
	CDynamicObj.SetName("Script Dynamic Obj")
	CDynamicObj.Forward(100)			
	InitTable.HP = CScene.GetVersion() + CDynamicObj.Add(13, 14)
	return 1;
end