-- DHTInterface
-- Reads DHT sensor and returns temperature and humidity

local DhtInterface = {}
local pin = 1 --  data pin, GPIO2

-- Return Codes
-- dht.OK
-- dht.ERROR_CHECKSUM
-- dht.ERROR_TIMEOUT
local function readDht()
    status, temp, humi, temp_dec, humi_dec = dht.read(PIN)
    if status == dht.OK then
        return temp, temp_dec,humi, humi_dec
    elseif status == dht.ERROR_CHECKSUM then
        print( "DHT Checksum error." )
        return nil
    elseif status == dht.ERROR_TIMEOUT then
        print( "DHT timed out." )
        return nil
    end
end
DhtInterface.readDht = readDht

return DhtInterface


