import serial
import time

ser = serial.Serial('COM3', 9600) # Open serial port
time.sleep(2) # Wait for Arduino to initialize

# Wait for Arduino to signal that it's ready
while True:
    response = ser.readline().decode().strip()
    if response == "Ready":
        print("Arduino is ready.")
        break

# Send command to Arduino
ser.write(b'1000;y5;x10;true;MainBreadboard 15;MCUBreadboard 1')
response = ser.readline().decode().strip() # Read the response
print("Response from Arduino:", response)
response = ser.readline().decode().strip() # Read the response
print("Response from Arduino:", response)


ser.close() # Close the serial port
