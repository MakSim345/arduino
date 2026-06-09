JUN-2026

Train Simulator app.

Note:
    - This app using a I2C protocol to connect with Arduino NANO/MEGA to increase number of GPIO.
    - It is MASTER for I2C. Use Arduino MEGA as big number of GPIO available.
    - Code for I2C Slave located in:
        "C:\dev\arduino\edu63_train_sim_keypress\edu63_train_sim_keypress.ino"
        [old - NOT USE]"C:\dev\arduino\edu63_keypress\edu63_keypress.ino"

Hardware Connections

1. Master Arduino (with buttons):

    - Connect buttons between digital pins (e.g., pin 7) and GND. Use pull-up resistors or enable the internal pull-up resistors in the code.
    - Connect the SDA pin to the SDA pin of the slave Arduino.
    - Connect the SCL pin to the SCL pin of the slave Arduino.
    - Connect the GND pin to the GND pin of the slave Arduino.

2. Slave Arduino:

    - Connect the SDA and SCL pins to the corresponding pins on the master Arduino.
    - Connect the GND pin to the GND pin of the master Arduino.
