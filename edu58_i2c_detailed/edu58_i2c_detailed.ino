/*
Explanation
1. Master Sends Data
    - Generates start condition.
    - Sends slave address (0x10) + write bit (0).
    - Sends data byte (0x55).
    - Generates stop condition.

2. Slave Receives Data

    - Waits for start condition.
    - Reads slave address and acknowledges.
    - Reads data byte, acknowledges, and prints it.

3. Master Requests Data

    - Generates start condition.
    - Sends slave address (0x10) + read bit (1).
    - Reads one byte (0xAA).
    - Sends NACK and generates stop condition.

4.Slave Sends Data

    - Waits for read request.
    - Sends predefined byte (0xAA).
*/

#define SDA_PIN 4  // Use digital pin 4 for SDA
#define SCL_PIN 5  // Use digital pin 5 for SCL

#define I2C_ADDRESS 0x10  // Slave address (7-bit format)

/***Define ONLY ONE of it:**/
#define MASTER
// #define SLAVE
/***************************/

//Disable if buttonPin01 is in use:
// #ifdef DEBUG_PRINT


void i2c_delay()
{
    delayMicroseconds(100);  // Adjust to slow down for oscilloscope visibility
}

#ifdef MASTER
// Master Code

// Set SDA and SCL as outputs
void i2c_init()
{
    pinMode(SDA_PIN, OUTPUT);
    pinMode(SCL_PIN, OUTPUT);
    digitalWrite(SDA_PIN, HIGH);
    digitalWrite(SCL_PIN, HIGH);
}

// Start condition: SDA goes LOW while SCL is HIGH
void i2c_start()
{
    digitalWrite(SDA_PIN, LOW);
    i2c_delay();
    digitalWrite(SCL_PIN, LOW);
}

// Stop condition: SDA goes HIGH while SCL is HIGH
void i2c_stop()
{
    digitalWrite(SCL_PIN, HIGH);
    i2c_delay();
    digitalWrite(SDA_PIN, HIGH);
    i2c_delay();
}

// Write one bit
void i2c_write_bit(bool bit)
{
    digitalWrite(SDA_PIN, bit);
    i2c_delay();
    digitalWrite(SCL_PIN, HIGH);
    i2c_delay();
    digitalWrite(SCL_PIN, LOW);
}

// Read one bit
bool i2c_read_bit()
{
    pinMode(SDA_PIN, INPUT);
    i2c_delay();
    digitalWrite(SCL_PIN, HIGH);
    i2c_delay();
    bool bit = digitalRead(SDA_PIN);
    digitalWrite(SCL_PIN, LOW);
    pinMode(SDA_PIN, OUTPUT);
    return bit;
}

// Write a byte (MSB first)
bool i2c_write_byte(uint8_t data)
{
    for (int i = 7; i >= 0; i--)
    {
        i2c_write_bit((data >> i) & 1);
    }
    return !i2c_read_bit(); // Read ACK (0 = acknowledged)
}

// Read a byte (MSB first)
uint8_t i2c_read_byte(bool ack)
{
    uint8_t data = 0;
    for (int i = 7; i >= 0; i--)
    {
        data |= i2c_read_bit() << i;
    }
    i2c_write_bit(!ack); // Send ACK/NACK
    return data;
}

void setup()
{
    Serial.begin(9600);
    i2c_init();

    i2c_start();
    if (i2c_write_byte(0x10 << 1))
    {
        // Slave address (0x10) + Write bit (0)
        i2c_write_byte(0x55);  // Data to send
    }
    i2c_stop();

    delay(500);

    i2c_start();
    if (i2c_write_byte((0x10 << 1) | 1))
    {
        // Slave address (0x10) + Read bit (1)
        uint8_t received = i2c_read_byte(false);  // Read and send NACK
        Serial.print("Received: ");
        Serial.println(received, HEX);
    }
    i2c_stop();
}

void loop()
{
    delay(1000);
}

# endif // MASTER


#ifdef SLAVE
// Slave Code

// Set SDA and SCL as inputs initially (high impedance)
void i2c_init()
{
    pinMode(SDA_PIN, INPUT);
    pinMode(SCL_PIN, INPUT);
}

// Wait for start condition
void i2c_wait_for_start()
{
    while (digitalRead(SDA_PIN) == HIGH || digitalRead(SCL_PIN) == LOW);
}

// Read a byte
uint8_t i2c_read_byte()
{
    uint8_t data = 0;
    pinMode(SDA_PIN, INPUT);

    for (int i = 7; i >= 0; i--)
    {
        while (digitalRead(SCL_PIN) == LOW);
        data |= digitalRead(SDA_PIN) << i;
        while (digitalRead(SCL_PIN) == HIGH);
    }
    return data;
}

// Write a byte
void i2c_write_byte(uint8_t data)
{
    pinMode(SDA_PIN, OUTPUT);

    for (int i = 7; i >= 0; i--)
    {
        digitalWrite(SDA_PIN, (data >> i) & 1);
        while (digitalRead(SCL_PIN) == LOW);
        while (digitalRead(SCL_PIN) == HIGH);
    }
    pinMode(SDA_PIN, INPUT);
}

// Send ACK
void i2c_send_ack()
{
    pinMode(SDA_PIN, OUTPUT);
    digitalWrite(SDA_PIN, LOW);
    while (digitalRead(SCL_PIN) == LOW);
    while (digitalRead(SCL_PIN) == HIGH);
    pinMode(SDA_PIN, INPUT);
}

void setup()
{
    Serial.begin(9600);
    i2c_init();
}

void loop()
{
    i2c_wait_for_start();

    uint8_t address = i2c_read_byte();
    i2c_send_ack();

    if ((address >> 1) == 0x10)
    {
        if ((address & 1) == 0)
        {
            // Write mode
            uint8_t received = i2c_read_byte();
            i2c_send_ack();
            Serial.print("Received: ");
            Serial.println(received, HEX);
        }
        else
        {
            // Read mode
            i2c_write_byte(0xAA);
        }
    }
}

# endif // SLAVE
