--Metronome
--Created by Ryan Proch
--Updated 6/30/10

FreeAllRegions()

dofile(SystemPath("urHelpers.lua"))

FRAMERATE = 50

urMetro = {}
urMetro.__index = urMetro

function urMetro:create(tempo, beatsOn, beatsOff)
    local metronome = {}             -- our new object
    setmetatable(metronome, urMetro)
    
    metronome.tempo = tempo or 60      -- initialize our object
    metronome.beatsOn = beatsOn or 4
    metronome.beatsOff = beatsOff or 0
    metronome.count = 0
    metronome.beat = 1
    
    metronome.clicker = MakeRegion() -- dummy region just to get updated so metronome can click on updates
    metronome.clicker:Hide()
    
    --Set up audio and flowbox
    dac = _G["FBDac"]
    upSample = FlowBox("object","mysample", FBSample)
    upSample:AddFile("Plick.wav")
    
    upPush = FlowBox("object", "mypush", FBPush)
    
    upPush:SetPushLink(0,upSample, 4)  
    upPush:Push(-1.0); -- Turn looping off
    upPush:RemovePushLink(0,upSample, 4)  
    upPush:SetPushLink(0,upSample, 2)
    upPush:Push(1.0); -- Set position to end so nothing plays
    
    return metronome
end

function urMetro:reset()
    self.count = 0
    self.beat = 1
    self.sounding = not self.sounding
end


function urMetro:click()
    local self = self.metronome
    
    if self.count == 0 then
        self.updatesPerBeat = 60/self.tempo*FRAMERATE
    end
    self.count = self.count+1
    
    if self.count >= self.updatesPerBeat-1 then -- if we have waited long enough beat for a beat
        DPrint(self.beat)
        if (self.sounding or self.beatsOff == 0) and self.beatsOn > 0 then
            upPush:Push(0.0)
            
            if self.beat == self.beatsOn then
                self:reset()
                return
            end
        elseif self.beat == self.beatsOff then
            self:reset()
            return
        end
        --DPrint(Time())
        self.count = 0
        self.beat = self.beat+1
    end   
end

function urMetro:start()
    self.running = true
    self:reset()
    self.sounding = true
    self.clicker.metronome = self        
    self.clicker:Handle("OnUpdate",urMetro.click)
    ReInitAudio()
end

function urMetro:stop()
    self.running = nil
    self.clicker:Handle("OnUpdate",nil)
    dac:RemovePullLink(0, upSample, 0)
    ShutdownAudio()
end


function urMetro:increaseParam(paramName)
    self[paramName] = self[paramName]+1
    DPrint(self[paramName])
end

function urMetro:decreaseParam(paramName)
    if self[paramName] > 0 then --disallows negative numbers
        self[paramName] = self[paramName]-1
    end
    DPrint(self[paramName])
end

function urMetro:makePowerButton(x, y)
    local powerButton = MakeRegion({w=75, h=75, layer='TOOLTIP', x=x, y=y, color='blue', input=true})
    powerButton.metronome = self
    
    function powerButton:toggleOnOff()
        if self.metronome.running then
            self.metronome:stop()
            SetAttrs(self,{color = 'blue'})
        else
            self.metronome:start()
            SetAttrs(self,{color = 'lightskyblue'})
        end
    end
    powerButton:Handle("OnTouchDown", powerButton.toggleOnOff)
end


function urMetro:makeController(y, paramName)
    local controller = {}
    controller.metronome = self
    controller.paramName = paramName
    
    
    controller.valueDisplay = MakeRegion({w=100, h=80, layer='TOOLTIP', x=ScreenWidth()/2-100/2, y=y, 
            label={color={0,0,60,190}, size=48, align='CENTER', text=self[controller.paramName]}})
    
    controller.upButton = MakeRegion({w=80, h=80, layer='TOOLTIP', x=ScreenWidth()-80, y=y, color='red', input=true})
    controller.upButton.metronome = self
    controller.upButton.controller = controller
    --controller.upButton.display = controller.valueDisplay
    
    controller.downButton = MakeRegion({w=80, h=80, layer='TOOLTIP', x=0, y=y, color='yellow', input=true})
    controller.downButton.metronome = self
    controller.downButton.controller = controller
    --controller.downButton.display = controller.valueDisplay
    
    controller.label = MakeRegion({w=200, h=50, layer='TOOLTIP', x=ScreenWidth()/2-200/2, y=y+controller.valueDisplay:Height(),
            label={color={0,0,60,190}, size=28, align='CENTER', text=paramName}})
    
    function controller:updateDisplay() 
        self.valueDisplay.tl:SetLabel(self.metronome[self.paramName])
    end
    
    controller.upButton.actionFunc = urMetro.increaseParam
    controller.downButton.actionFunc = urMetro.decreaseParam
    
    function doAction(self)
        if self.holding then
            if Time()-self.startTime > 0.7 then
                self.actionFunc(self.metronome, self.controller.paramName)
                self.controller:updateDisplay()
            end
        end
    end
    
    function startAction(self)
		self.actionFunc(self.metronome, self.controller.paramName)
		self.controller:updateDisplay()
        self.holding = true
        self.startTime = Time()
        self:Handle("OnUpdate", doAction)
    end
    
    function stopAction(self)
        self.holding = nil
        self:Handle("OnUpdate", nil)
    end
    
    controller.upButton:Handle("OnTouchDown", startAction)
    controller.upButton:Handle("OnTouchUp", stopAction)
--    controller.upButton:Handle("OnDoubleTap", stopAction)            
--    controller.upButton:Handle("OnEnter", startAction)
    controller.upButton:Handle("OnLeave", stopAction)
    controller.downButton:Handle("OnTouchDown", startAction)
    controller.downButton:Handle("OnTouchUp", stopAction)
--    controller.downButton:Handle("OnDoubleTap", stopAction)           
--    controller.downButton:Handle("OnEnter", startAction)
    controller.downButton:Handle("OnLeave", stopAction)
    
    return controller
end

metro = urMetro:create(180,2,2)
metro:makePowerButton(ScreenWidth()/2-60/2, 270)
metro.controller1 = metro:makeController(350, "tempo")
metro.controller2 = metro:makeController(140, "beatsOn")
metro.controller3 = metro:makeController(10, "beatsOff")


function ShutdownAudio()
    dac:RemovePullLink(0, upSample, 0)
end
function ReInitAudio()
    dac:SetPullLink(0, upSample, 0)
end

-- Instantiating background
background = MakeRegion({
        w=ScreenWidth(), 
        h=ScreenHeight(), 
        layer='BACKGROUND', 
        x=0,y=0, img="PongBG.png"
    })
background:Handle("OnPageEntered", ReInitAudio)
background:Handle("OnPageLeft", ShutdownAudio)


pagebutton = MakeRegion({w=16,h=16,
        layer='TOOLTIP',
        x=ScreenWidth()-28, y=ScreenHeight()-28,
        img="circlebutton-16.png",
        input=true
    })
pagebutton:EnableClamping(true)
pagebutton:Handle("OnTouchDown", FlipPage)
