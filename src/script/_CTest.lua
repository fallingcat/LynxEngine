PRE = 0
POST = 1

CTest = {	
	InitTable = {
		HP = 633,
		MP = 3650,
	}
}

CTest = class(CDynamicObj)
-----------------------------------------	
--
-----------------------------------------	
function CTest:test(x)
	return x
end			

function OnInit() 				
	Obj = CTest.new(1)
	Obj:SetName("Derived Script Dynamic Obj")	
	Obj:Yaw(50, POST)			
	Obj:Pitch(50, POST)			
	--Obj.UpdateTransform()
	Obj:Forward(-10, POST)	
	--Test.InitTable.HP = Test.InitTable.HP+1 --CScene.GetVersion() + CDynamicObj.Add(19, 70)
	return 1;
end
