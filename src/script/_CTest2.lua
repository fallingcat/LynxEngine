CTest2 = class(CTest)

function CTest2:test2(x)
	return x*0.5
end	

function OnInit() 				
	Obj = CTest2.new(1)
	Obj:SetName("Derived Script Dynamic Obj 2")		
	Obj:Forward(Obj:test(-20))
	return 1;
end
