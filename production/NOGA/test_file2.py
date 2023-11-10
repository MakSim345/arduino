import serial

START_BYTE = 0xAA
END_BYTE = 0xBB

def send_command(action, parameter):
    command = bytearray([START_BYTE, action, parameter, 0, END_BYTE])
    command[3] = calculate_checksum(command)
    ser.write(command)

def calculate_checksum(command):
    checksum = 0
    for byte in command[:-1]:
        checksum ^= byte
    return checksum

# Configure serial communication with Arduino
ser = serial.Serial('COM23', 9600)

# Example usage: sending command with action 0x01 and parameter 0x42
# send_command(0x01, 0x42)
ser.write("ON\n".encode())

print("Read response from Arduino")
response = ser.read()
print("Read response from Arduino")

if len(response) == 5 and response[0] == START_BYTE and response[-1] == END_BYTE and response[3] == calculate_checksum(response):
    action = response[1]
    parameter = response[2]
    print(f"Received response: Action={action}, Parameter={parameter}")
else:
    print("Invalid response received")

# Close the serial connection
ser.close()


# Configure serial communication with the desired parameters
ser = serial.Serial('COM1', 9600)  # Replace 'COM1' with the appropriate serial port
ser.timeout = 1  # Set a timeout value (in seconds) for serial communication

# Main loop for user interaction
while True:
    user_input = input("Enter data to send (or 'q' to quit): ")
    
    if user_input.lower() == 'q':
        break

    # Send the user input data to the serial port
    ser.write(user_input.encode())

# Close the serial connection
ser.close()
