import serial
import time

# Replace 'COM7' with your Arduino's port
arduino = serial.Serial('COM7', 115200, timeout=1)
time.sleep(2)  # Wait for the connection to initialize

def send_data(muxIndex1, muxIndex2, mode, r, g, b, CODE):
    # Send data to the Arduino in the format: muxIndex1 muxIndex2 mode color
    data = f"{muxIndex1} {muxIndex2} {mode} {r} {g} {b} {CODE}\n"

    # Debugging print to ensure correct data is being sent
    print("Sending data:", data.strip())
    
    arduino.write(data.encode())

    # Wait for a response from Arduino
    response = arduino.readline().decode('utf-8').strip()
    print("Arduino:", response)

if __name__ == "__main__":
    # Example: Send 1 2 1 FF5733 to Arduino
    muxIndex1 = 1
    muxIndex2 = 61
    mode = 1
    r = 255
    g = 0
    b = 255
    CODE = "CLR"

    send_data(muxIndex1, muxIndex2, mode, r, g, b, "NULL")

    time.sleep(5)
    
    send_data(muxIndex1, muxIndex2, mode, r, g, b, CODE)


# Close the connection
arduino.close()
