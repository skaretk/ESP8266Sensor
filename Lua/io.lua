filename = "storage.db"

local M = {}

local function saveData(lastTemp, lastHum)
	file.open("storage.db", w)
	file.writeline(lastTemp)
	file.writeline(lastHum)
	file.close()
end

local function readData()
	local temp = nil
	local humidity = nil
	if file.exists("storage.db") then
		file.open("storage.db", r)
		temp = file.readLine()
		humidity = file.readLine()
		file.close()
        end
	return temp, humidity
end

M.saveData = saveData
M.readData = readData

return M
		
