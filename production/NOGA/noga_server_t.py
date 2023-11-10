import serial
import threading

GLOBAL_LOOP = True

# Function to read from the serial port in a separate thread
def read_serial(serial_obj):
    global GLOBAL_LOOP
    while GLOBAL_LOOP:
        if ser.in_waiting:
            received_data = serial_obj.readline().decode().strip()
            if received_data:
                print("\nReceived data:", received_data)

    print("read_serial: end.")

# Function to send user input to the serial port in a separate thread
def send_serial(serial_obj):
    global GLOBAL_LOOP
    while GLOBAL_LOOP:
        user_input = input("Enter data to send (or ':q' to quit): ")

        if user_input.lower() == ':q':
            print("Exit command received!")
            GLOBAL_LOOP = False
            break
        #end_if

        serial_obj.write(user_input.encode())
    print("send_serial: end.")

# main entrance point:
if __name__ == '__main__' or __name__ == sys.argv[0]:

    # Configure serial communication with the desired parameters
    ser = serial.Serial('COM23', 9600)  # select the appropriate serial port
    ser.timeout = 1  # Set a timeout value (in seconds) for serial communication

    # Create and start the threads
    read_thread = threading.Thread(target=read_serial, args=(ser,))
    send_thread = threading.Thread(target=send_serial, args=(ser,))

    read_thread.start()
    send_thread.start()

    # Wait for the threads to finish
    read_thread.join()
    send_thread.join()

    # Close the serial connection
    ser.close()

    # End of script
    print("Script has stopped")
