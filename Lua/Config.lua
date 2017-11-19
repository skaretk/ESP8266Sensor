-- Config.lua
-- Configuration specific for each ESP8266 module

local CFG = {}
-- Current DeviceIdx
-- 78 = LivingRoom - DHT22
-- 79 = Basement - DHT22
-- 80 = Workout - DHHT22

local function GetDeviceId()
	local DeviceIdx
	mac = wifi.ap.getmac()
	if mac == "1a:fe:34:e1:b4:2a" then DeviceIdx = 80
	elseif mac == "5e:cf:7f:02:f4:29" then DeviceIdx = 78
    elseif mac == "1a:fe:34:de:44:18" then DeviceIdx = 79
    else print("Cannot get DeviceIdx! Mac: "..mac)
        return nil -- Not Known MAC address, needs to be configured
    end
	print("DeviceId: "..DeviceIdx)
	return DeviceIdx
end
CFG.GetDeviceId = GetDeviceId

return CFG
