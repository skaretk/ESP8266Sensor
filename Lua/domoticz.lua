-- domoticz.lua
-- Compare and transmit data to Domoticz

--------------------
-- Config
--------------------
local deviceCfg = require("Config")
PIN = 1             		        -- data pin, GPIO2
DEVICEIDX = deviceCfg.GetDeviceId()	-- Device Idx in Domoticz
DOMOTICZIP = "192.168.1.35"         -- IP Address for Domoticz server
SLEEPTIME = 60000			        -- ms
FORCEUPDATETIME = 60 		        -- min
--------------------
-- Offset in rtc memory
--------------------
TEMP_OFFSET 		= 0
TEMP_DEC_OFFSET     = 1
HUMI_OFFSET 		= 2
HUMI_DEC_OFFSET     = 3
LASTUPDATE_OFFSET 	= 4

function sendData(data)
    conn=net.createConnection(net.TCP, 0)
    conn:on("receive", function(conn, payload) print(payload) end )
    conn:connect(8080, DOMOTICZIP)
    conn:on("connection", function(conn, payload)
        -- Wait for connection before sending.
        conn:send("GET /json.htm?type=command&param=udevice&idx="..DEVICEIDX.."&nvalue=0&svalue=" .. data .. " HTTP/1.1\r\nHost: www.local.lan\r\n"
        .."Connection: keep-alive\r\nAccept: */*\r\n\r\n")
        end)
    print("Idx "..DEVICEIDX.." data "..data)    
end

function readDht()
    status, temp, humi, temp_dec, humi_dec = dht.read(PIN)
    if status == dht.OK then
        if compare(temp,temp_dec,humi,humi_dec) ~= nil then
            return temp.."."..temp_dec..";"..humi.."."..humi_dec..";1"
        else
            return nil
        end
    elseif status == dht.ERROR_CHECKSUM then
        print( "DHT Checksum error." )
        return nil
    elseif status == dht.ERROR_TIMEOUT then
        print( "DHT timed out." )
        return nil
    end
end

tmr.alarm(0,500, tmr.ALARM_SINGLE, function()    
    tempandHum = readDht()
    if tempandHum ~= nil then
        dofile("wifi.lua")
        tmr.register(2, 7000, tmr.ALARM_SINGLE, function()
            sendData(tempandHum)
            tmr.register(3, 500, tmr.ALARM_SINGLE, function()
                enterDSleep()
            end)
            tmr.start(3) 
        end)
        tmr.start(2)
    else
        print("Same values, retry in "..(SLEEPTIME/60000).."min")
        enterDSleep()
    end       
end)

function enterDSleep()
    print("Enter deep sleep for "..SLEEPTIME.."ms")
    node.dsleep(SLEEPTIME * 1000, 4)
end

function compare(temp, tempDec, humi, humiDec)
    local lastTemp, lastTempDec, lastHumi, lastHumiDec = rtcmem.read32(TEMP_OFFSET, 4) -- Read both temperature and humidity
    local temperature = temp.."."..tempDec
    local humidity = humi.."."..humiDec
    print("temp: "..temperature.." humi: "..humidity)
    print("lastTemp: "..lastTemp.."."..lastTempDec.." lastHumi: "..lastHumi.."."..lastHumiDec)
    
    --print("temp: "..temp.."."..tempDec.." humi: "..humi.."."..humiDec)
    --print("lastTemp: "..lastTemp.."."..lastTempDec.." lastHumi: "..lastHumi.."."..lastHumiDec)
    
    if setLastUpdate() == nil then
        return true -- Force update
    end
    if (temp ~= lastTemp or tempDec ~= lastTempDec or
       humi ~= lastHumi or humiDec ~= lastHumiDec) then
        store(temp, tempDec, humi, humiDec) -- new values, update
        rtcmem.write32(LASTUPDATE_OFFSET, 0) -- Reset counter, force update
        return true       
    else
        return nil
    end
end

function store(temp, tempDec, humi, humiDec)
    rtcmem.write32(TEMP_OFFSET, temp, tempDec, humi, humiDec) -- Store both temperature and humidity
end

function setLastUpdate()
    lastUpdate = rtcmem.read32(LASTUPDATE_OFFSET)
    print("Last Update: "..lastUpdate.." min")
    if lastUpdate >= FORCEUPDATETIME or lastUpdate < 0 then
        rtcmem.write32(LASTUPDATE_OFFSET, 0) -- Reset counter, force update
        return nil
    else
        lastUpdate = lastUpdate + 1
        rtcmem.write32(LASTUPDATE_OFFSET, lastUpdate) -- Increment
        return true
    end        
end
