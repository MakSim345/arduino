/*
To use interrupts in the slave device for receiving I2C messages, we need to detect changes on the SDA and SCL lines and handle communication in an Interrupt Service Routine (ISR). Since Arduino does not have native hardware I2C interrupts without using the Wire library, we will use Pin Change Interrupts to trigger when the I2C master sends data.


Steps to Implement I2C Slave with Interrupts:
    - Enable Pin Change Interrupts on the SDA (A4) and SCL (A5) pins.
    - Use an ISR to detect communication start and process data.
    - Store received bytes in a buffer for later use.
    - Keep the loop() empty, as everything is handled by interrupts.

Arduino’s ATmega328P microcontroller has three Pin Change Interrupt registers (PCINT):

    - PCINT0 (Port B) → Pins D8 to D13
    - PCINT1 (Port C) → Pins A0 to A5 (used for I2C SDA and SCL)
    - PCINT2 (Port D) → Pins D0 to D7

Since A4 (SDA) and A5 (SCL) belong to Port C (PCINT1), we use the PCICR (Pin Change Interrupt Control Register) and PCMSK1 (Pin Change Mask Register for Port C).
*/


// Slave code
#define SDA_PIN A4  // I2C Data
#define SCL_PIN A5  // I2C Clock

volatile bool i2c_message_received = false;
volatile uint8_t received_data = 0;

void i2c_delay()
{
    delayMicroseconds(100);  // Slow down for oscilloscope visibility
}

// Enable pin change interrupt on A4 (SDA) and A5 (SCL)
void enable_interrupts()
{
    /*
    Enables Pin Change Interrupts for the SDA (A4) and SCL (A5) pins so that the Arduino can detect I2C communication without constantly polling.
    Any change (HIGH to LOW or LOW to HIGH) on SDA (A4) triggers the ISR.*/

    PCICR |= (1 << PCIE1);     // Enable Pin Change Interrupt for PCINT[14:8] (A0-A5)

    /*
    -  PCICR (Pin Change Interrupt Control Register): Controls which port can trigger pin change interrupts.
    -  PCIE1 (Bit 1 of PCICR): Enables interrupts for PCINT[14:8] (which corresponds to Port C: A0 to A5).
    -  (1 << PCIE1): Sets bit 1 of PCICR, enabling interrupts for analog pins (A0–A5).
    */
    PCMSK1 |= (1 << PCINT12);  // Enable interrupt for A4 (SDA)

    PCMSK1 |= (1 << PCINT13);   // Enable interrupt for A5 (SCL)
}

// Interrupt Service Routine (ISR) for SDA changes
ISR(PCINT1_vect)
{
    /*
    Whenever SDA (A4) changes state, an interrupt is triggered, this one is calling

    This function will execute every time SDA changes from HIGH to LOW or LOW to HIGH.

    The slave device can now detect when the master starts a transmission.
    */
    static bool receiving = false;
    static uint8_t bit_count = 0;
    static uint8_t temp_byte = 0;

    // Detect START condition (SDA goes LOW while SCL is HIGH)
    if (!digitalRead(SDA_PIN) && digitalRead(SCL_PIN))
    {
        receiving = true;
        bit_count = 0;
        temp_byte = 0;
        return;
    }

    // If we are receiving a byte
    if (receiving)
    {
        // Read a bit when SCL is HIGH
        if (digitalRead(SCL_PIN))
        {
            temp_byte = (temp_byte << 1) | digitalRead(SDA_PIN);
            bit_count++;
        }

        // If a full byte (8 bits) is received
        if (bit_count == 8)
        {
            received_data = temp_byte;
            i2c_message_received = true;
            receiving = false;  // Stop receiving
        }
    }
}

void setup()
{
    Serial.begin(9600);

    pinMode(SDA_PIN, INPUT);
    pinMode(SCL_PIN, INPUT);

    enable_interrupts();
}

void loop()
{
    if (i2c_message_received)
    {
        Serial.print("Received: ");
        Serial.println(received_data, HEX);
        i2c_message_received = false;
    }
}
