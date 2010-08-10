
local lastx = 320/2
local lasty = 480

local sqrt = math.sqrt
local random = math.random

local vis = _G["FBVis"]

local mx = {random(0,320),random(0,320)}
local my = {random(0,480),random(0,480)}
local maxset = 2
local setcount = 1

for i=1,maxset do
	local pushflowbox = _G["FBPush"]
	if pushflowbox.instances and pushflowbox.instances[3*i-2] and pushflowbox.instances[3*i-1] and pushflowbox.instances[3*i] then
		pushflowbox.instances[3*i-2]:Push(1.0)
		pushflowbox.instances[3*i-1]:Push((mx[i]-160.0)/160.0)
		pushflowbox.instances[3*i]:Push((my[i]-240.0)/240.0)
	end
end


function Paint(self)
	local x,y = InputPosition()

	self.texture:Clear(255,255,255)
	
	self.texture:SetBrushSize(3)
	
	local visout = vis:Get()

	if visout < 0.0 then
		self.texture:SetBrushColor(0,255,0,200)
	else
		self.texture:SetBrushColor(128,0,0,150)
	end
	local r = sqrt((mx[1]-x)*(mx[1]-x) + (my[1]-y)*(my[1]-y))
	self.texture:Ellipse( mx[1], my[1], r, r)

	if visout >= 0.0 then
		self.texture:SetBrushColor(0,255,0,200)
	else
		self.texture:SetBrushColor(128,0,0,150)
	end
	r = sqrt((mx[2]-x)*(mx[2]-x) + (my[2]-y)*(my[2]-y))
	self.texture:Ellipse( mx[2], my[2], r, r)
	DPrint(visout)
	local pushflowbox = _G["FBPush"]
	for i=1,maxset do
		if pushflowbox.instances and pushflowbox.instances[3*i-2] and pushflowbox.instances[3*i-1] and pushflowbox.instances[3*i] then
			pushflowbox.instances[3*i-2]:Push(-1.0)
			pushflowbox.instances[3*i-1]:Push((x-160.0)/160.0)
			pushflowbox.instances[3*i]:Push((y-240.0)/240.0)
		end
	end
end

function SetClass(self)
	local x,y = InputPosition()

	mx[setcount] = x
	my[setcount] = y
	
	local pushflowbox = _G["FBPush"]
	
	if pushflowbox.instances and pushflowbox.instances[3*setcount-2] and pushflowbox.instances[3*setcount-1] and pushflowbox.instances[3*setcount] then
		pushflowbox.instances[3*setcount-2]:Push(1.0)
		pushflowbox.instances[3*setcount-1]:Push((x-160.0)/160.0)
		pushflowbox.instances[3*setcount]:Push((y-240.0)/240.0)
		pushflowbox.instances[3*setcount-2]:Push(-1.0)
	end
	
	setcount = setcount + 1
	if setcount > maxset then
		setcount = 1
	end
	
end

visgraphbackdropregion=Region('region', 'visgraphbackdropregion', UIParent)
visgraphbackdropregion:SetWidth(ScreenWidth())
visgraphbackdropregion:SetHeight(ScreenHeight())
visgraphbackdropregion:SetLayer("BACKGROUND")
visgraphbackdropregion:SetAnchor('BOTTOMLEFT',0,0)
--visgraphbackdropregion:EnableClamping(true)
visgraphbackdropregion.texture = visgraphbackdropregion:Texture("Default.png")
visgraphbackdropregion.texture:SetGradientColor("TOP",255,255,255,255,255,255,255,255)
visgraphbackdropregion.texture:SetGradientColor("BOTTOM",255,255,255,255,255,255,255,255)
--visgraphbackdropregion.texture:SetBlendMode("BLEND")
visgraphbackdropregion.texture:SetTexCoord(0,0.63,0.94,0.0)
visgraphbackdropregion:Handle("OnUpdate", Paint)
visgraphbackdropregion:Handle("OnDoubleTap", SetClass)
visgraphbackdropregion:EnableInput(true)
visgraphbackdropregion:Show()
--visgraphbackdropregion.tl = visgraphbackdropregion:TextLabel()
--visgraphbackdropregion.tl:SetFont("Trebuchet MS")
--visgraphbackdropregion.tl:SetHorizontalAlign("LEFT")
--visgraphbackdropregion.tl:SetLabelHeight(30)
--visgraphbackdropregion.tl:SetColor(0,0,255,255)

visgraphbackdropregion.texture:Clear(255,255,255)
visgraphbackdropregion.texture:ClearBrush()

local pagebutton=Region('region', 'pagebutton', UIParent);
pagebutton:SetWidth(pagersize);
pagebutton:SetHeight(pagersize);
pagebutton:SetLayer("TOOLTIP");
pagebutton:SetAnchor('BOTTOMLEFT',ScreenWidth()-pagersize-4,ScreenHeight()-pagersize-4); 
pagebutton:EnableClamping(true)
--pagebutton:Handle("OnDoubleTap", FlipPage)
pagebutton:Handle("OnTouchDown", FlipPage)
pagebutton.texture = pagebutton:Texture("circlebutton-16.png");
pagebutton.texture:SetGradientColor("TOP",255,255,255,255,255,255,255,255);
pagebutton.texture:SetGradientColor("BOTTOM",255,255,255,255,255,255,255,255);
pagebutton.texture:SetBlendMode("BLEND")
pagebutton.texture:SetTexCoord(0,1.0,0,1.0);
pagebutton:EnableInput(true);
pagebutton:Show();
