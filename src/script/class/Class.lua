-------------------------------------------------------------------------------------------------------
--    __                    ____            __
--   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
--  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
--  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
--      /___/                       /___/ 
--
-- 	class function
-------------------------------------------------------------------------------------------------------
local _class={}
-----------------------------------------	
--
-----------------------------------------	
function class(super)
    local class_type={}
	class_type.ctor=false
	class_type.super=super
	class_type.new=function(...) 
			local obj={}
			do
				local create
				create = function(c,...)
					if c.super then
						create(c.super,...)
					end
					if c.ctor then
						c.ctor(obj,...)
					end
				end 
				create(class_type,...)
			end
			setmetatable(obj,{ __index=_class[class_type] })
			return obj
		end
	local vtbl={}
	_class[class_type]=vtbl
 
	setmetatable(class_type,{__newindex=
		function(t,k,v)
			vtbl[k]=v
		end
	})
 
	if super then
		setmetatable(vtbl,{__index=
			function(t,k)
				local ret=_class[super][k]
				vtbl[k]=ret
				return ret
			end
		})
	end
 
	return class_type
end

-- forward declaration
CGameObj = class()
    CScene = class(CGameObj)
		CRenderableObj = class(CGameObj)
			CStaticObj = class(CRenderableObj)			
			CDynamicObj = class(CRenderableObj)
				CLight = class(CDynamicObj)
					CPointLight = class(CLight)
