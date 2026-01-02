function Add(x, y) 
	return x + y;
end

function Mul(x, y) 
	return x * y;
end

function SetName(name) 
	CDynamicObj:SetName(name);	
end

function Forward(dist) 
	CDynamicObj:Forward(dist);	
end

Table = {
	Func0 = SetName,
	Func1 = Forward,
};

function OnInit()	
	Table.Func0("Script change name");
	Table.Func1(100);
	return 0;
end