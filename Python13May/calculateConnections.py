import json

class colors:
    RED = '\033[91m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    BLUE = '\033[94m'
    PURPLE = '\033[95m'
    CYAN = '\033[96m'
    WHITE = '\033[97m'
    RESET = '\033[0m'



def find_key_by_value(data, target_value, mode):
    # Search in the 'Inputs' dictionary for the target value
    if mode == 'Inputs':
        for key, value in data['Inputs'].items():
            if value == target_value:
                return key
    elif mode == 'Outputs':
        for key, value in data['Outputs'].items():
            if value == target_value:
                return key
    return None

def load_multiplexer_config(filepath):
    with open(filepath, 'r') as file:
        data = json.load(file)
    return data

def export_connections(config, MCUpin, MAINpin, mode, usedMUX1Pins, usedMUX2Pins): 
    

    mux1 = config['Multiplexers'][0]
    mux2 = config['Multiplexers'][1]

    currentMCUBreadboardPin = "MCUBreadboard" + str(MCUpin)
    currentMainBreadboardPin = "MainBreadboard" + str(MAINpin)
    
    YOfMCUBreadboard = find_key_by_value(mux1, currentMCUBreadboardPin, "Inputs")
    YOfMCUBreadboard = "MUX1_" + YOfMCUBreadboard

    XOfMainBreadboard = find_key_by_value(mux1, currentMainBreadboardPin, "Outputs")
    if XOfMainBreadboard is None:
        XOfMainBreadboard = find_key_by_value(mux2, currentMainBreadboardPin, "Outputs")
        XOfMainBreadboard = "MUX2_" + XOfMainBreadboard
    else:
        XOfMainBreadboard = "MUX1_" + XOfMainBreadboard

    # print(str(currentMCUBreadboardPin) + ": " + str(YOfMCUBreadboard) + " | " + str(currentMainBreadboardPin) + ": " + str(XOfMainBreadboard))

    splitYOfMCUBreadboard = YOfMCUBreadboard.split("_")
    splitXOfMainBreadboard = XOfMainBreadboard.split("_")

    if splitYOfMCUBreadboard[0] == splitXOfMainBreadboard[0]:
        print(f"SetConnection(1000, {splitYOfMCUBreadboard[1]}, {splitXOfMainBreadboard[1]}, {mode});")
    else:
        currentkey = None
        if mode == "false":
            usedMUX1Pins.pop()
            usedMUX2Pins.pop()
        # the break bellw doenst work, WHY?
        currentkey = None
        for key in mux1['Outputs'].items():
            if key[0] not in usedMUX1Pins:
                if key[0] not in ["X8", "X9", "X10", "X11", "X12", "X13", "X14", "X15"]:
                    if mode == "true":
                        usedMUX1Pins.append(key[0])
                        # print(f"usedMUX1Pins: {usedMUX1Pins}")
                        currentkey = key[0]
                        # print(f"currentkey: {currentkey}")
                        # print(f"BREAK NOW")
                        break
                    elif mode == "false":
                        currentkey = key[0]
                        # print(f"currentkey: {currentkey}")
                        break


            
        valueOfCurrentKey = mux1['Outputs'][currentkey]
        splitValueOfCurrentKey = valueOfCurrentKey.split("_")
        # print (splitValueOfCurrentKey[1])

        for key in mux2['Inputs'].items():
            # print(f"key: {key}")
            if key not in usedMUX2Pins:
                if key[0] == splitValueOfCurrentKey[1]:
                    if mode == "true":
                        usedMUX2Pins.append(key[0])
                        # print(f"usedMUX2Pins: {usedMUX2Pins}")
                        # print(f"SetConnection(1000, {splitYOfMCUBreadboard[1]}, {currentkey}, {mode});")
                        # print(f"SetConnection(1001, {key}, {splitXOfMainBreadboard[1]}, {mode});")
                    
                        return "1000;" + str(splitYOfMCUBreadboard[1]).lower()  + ";" + str(currentkey.lower() ) + ";" + str(mode).lower() + "\n" + \
                        "1001;" + str(key[0]).lower()  + ";" + str(splitXOfMainBreadboard[1]).lower()  + ";" + str(mode).lower()
                    elif mode == "false":
                        # print(f"SetConnection(1000, {splitYOfMCUBreadboard[1]}, {currentkey}, {mode});")
                        # print(f"SetConnection(1001, {key}, {splitXOfMainBreadboard[1]}, {mode});")
                        return "1000;" + str(splitYOfMCUBreadboard[1]).lower()  + ";" + str(currentkey.lower() ) + ";" + str(mode).lower() + "\n" + \
                        "1001;" + str(key[0]).lower()  + ";" + str(splitXOfMainBreadboard[1]).lower()  + ";" + str(mode).lower()
                                        
            
if __name__ == "__main__":
    usedMUX1Pins = []
    usedMUX2Pins = []
    while True:
        print(export_connections(load_multiplexer_config('rules.json'), input("MCU: "), input("MAIN: "), input("true or false: "), usedMUX1Pins, usedMUX2Pins))