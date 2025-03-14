
#define SDA_PIN 4  // Use digital pin 4 for SDA
#define SCL_PIN 5  // Use digital pin 5 for SCL

#define I2C_ADDRESS 0x08  // Slave address (7-bit format)

/***Define ONLY ONE of it:**/
#define MASTER
// #define SLAVE
/***************************/

//Disable if buttonPin01 is in use:
// #ifdef DEBUG_PRINT


#ifdef MASTER

volatile bool i2c_message_received = false;
volatile uint8_t received_data = 0;

void i2c_delay()
{
    delayMicroseconds(100);  // Slow down for oscilloscope visibility
}

// Enable pin change interrupt on A4 (SDA) and A5 (SCL)
void enable_interrupts()
{
    PCICR |= (1 << PCIE1);     // Enable Pin Change Interrupt for PCINT[14:8] (A0-A5)
    PCMSK1 |= (1 << PCINT12);  // Enable interrupt for A4 (SDA)
}

// Interrupt Service Routine (ISR) for SDA changes
ISR(PCINT1_vect)
{
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


# endif // MASTER


#ifdef SLAVE
// Slave Code

volatile bool i2c_message_received = false;
volatile uint8_t received_data = 0;

void i2c_delay()
{
    delayMicroseconds(100);  // Slow down for oscilloscope visibility
}

// Enable pin change interrupt on A4 (SDA) and A5 (SCL)
void enable_interrupts()
{
    PCICR |= (1 << PCIE1);     // Enable Pin Change Interrupt for PCINT[14:8] (A0-A5)
    PCMSK1 |= (1 << PCINT12);  // Enable interrupt for A4 (SDA)
}

// Interrupt Service Routine (ISR) for SDA changes
ISR(PCINT1_vect)
{
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

# endif // SLAVE
