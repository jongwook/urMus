local mic=_G["FBMic"]
local tuner=FlowBox("object","Tuner",_G["FBTuner"])
local vis=_G["FBVis"]

mic:SetPushLink(0,tuner,0)
tuner:SetPushLink(0,vis,0)


notes={"A","A#","B","C","C#","D","D#","E","F","F#","G","G#"}

backdrop = Region('Region','backdrop',UIParent)
backdrop:SetLayer("TOOPTIP")
backdrop:SetWidth(ScreenWidth());
backdrop:SetHeight(100);
backdrop:SetAnchor("BOTTOMLEFT",0,0)
backdrop:EnableInput(true)
backdrop.texture=backdrop:Texture()
backdrop.texture:SetTexture(0x11,0x22,0x44,0xff)
backdrop:Show()

backdrop.textlabel=backdrop:TextLabel()
backdrop.textlabel:SetFont("Trebuchet MS")
backdrop.textlabel:SetHorizontalAlign("CENTER")
backdrop.textlabel:SetVerticalAlign("TOP")
backdrop.textlabel:SetLabel("Tuner")
backdrop.textlabel:SetLabelHeight(40)
backdrop.textlabel:SetColor(255,0,0,255)
backdrop.textlabel:SetShadowColor(255,190,190,190)
backdrop.textlabel:SetShadowOffset(2,-3)
backdrop.textlabel:SetShadowBlur(4.0)


function Update(self) 
    val=vis:Get()
    if val==-1 then
        backdrop.textlabel:SetLabel("urTuner")
        DPrint("")
    else
        pitch=math.floor(val*12+0.5)
        index=pitch%12+1
        per=(val*12-pitch)*100
        DPrint(per.."%")
        backdrop.textlabel:SetLabel(notes[index])
    end
end

backdrop:Handle("OnUpdate",Update)
