#include <Arduino.h>

#define SLAVE_ADDRESS 0x08 // Slave I2C address
#define SDA_PIN 4          // Data line pin (PD4, PCINT20)
#define SCL_PIN 5          // Clock line pin (PD5, PCINT21)


/***Define ONLY ONE of it:**/
#define MASTER
// #define SLAVE
/***************************/


#ifdef MASTER
/*

Pins:
SDA is on pin 4, SCL on pin 5. External pull-up resistors (e.g., 4.7kΩ to 5V) are assumed on both lines.

Behavior:
The master sends "Hello" every 500ms and then requests 6 bytes, printing the response (expecting "World!").

*/

// Function prototypes
void i2c_start();
void i2c_stop();
bool i2c_write_byte(uint8_t byte);
uint8_t i2c_read_byte(bool ack);

void setup()
{
  pinMode(SDA_PIN, INPUT); // SDA high (assuming external pull-ups)
  pinMode(SCL_PIN, INPUT); // SCL high (assuming external pull-ups)
  Serial.begin(9600);      // Start serial communication
}

void loop()
{
  // Send "Hello" to the slave
  i2c_start();
  if (i2c_write_byte(SLAVE_ADDRESS << 1))
  { // Address + write bit (0)
    i2c_write_byte('H');
    i2c_write_byte('e');
    i2c_write_byte('l');
    i2c_write_byte('l');
    i2c_write_byte('o');
  }
  i2c_stop();
  delay(500); // Wait before reading

  // Request 6 bytes from the slave
  i2c_start();
  if (i2c_write_byte((SLAVE_ADDRESS << 1) | 1))
  { // Address + read bit (1)
    for (int i = 0; i < 6; i++)
    {
      // Send ACK for all bytes except the last one
      uint8_t c = i2c_read_byte(i < 5);
      Serial.print((char)c);
    }
    Serial.println();
  }
  i2c_stop();
  delay(500); // Wait before next cycle
}

/** Generates I2C start condition: SDA goes low while SCL is high */
void i2c_start()
{
  pinMode(SDA_PIN, OUTPUT);
  digitalWrite(SDA_PIN, LOW);  // SDA low
  delayMicroseconds(5);        // Timing delay
  pinMode(SCL_PIN, OUTPUT);
  digitalWrite(SCL_PIN, LOW);  // SCL low
}

/** Generates I2C stop condition: SDA goes high while SCL is high */
void i2c_stop()
{
  pinMode(SCL_PIN, INPUT);     // SCL high
  delayMicroseconds(5);        // Timing delay
  pinMode(SDA_PIN, INPUT);     // SDA high
}

/** Writes a byte to the slave and returns true if acknowledged */
bool i2c_write_byte(uint8_t byte)
{
  for (int i = 7; i >= 0; i--)
  {
    // Set SDA based on the current bit
    if (byte & (1 << i))
    {
      pinMode(SDA_PIN, INPUT);        // SDA high
    }
    else
    {
      pinMode(SDA_PIN, OUTPUT);
      digitalWrite(SDA_PIN, LOW);     // SDA low
    }
    pinMode(SCL_PIN, INPUT);          // SCL high
    delayMicroseconds(5);
    pinMode(SCL_PIN, OUTPUT);
    digitalWrite(SCL_PIN, LOW);       // SCL low
    delayMicroseconds(5);
  }
  // Check for acknowledgment
  pinMode(SDA_PIN, INPUT);            // Release SDA
  pinMode(SCL_PIN, INPUT);            // SCL high
  delayMicroseconds(5);
  bool ack = (digitalRead(SDA_PIN) == LOW); // ACK if SDA is low
  pinMode(SCL_PIN, OUTPUT);
  digitalWrite(SCL_PIN, LOW);         // SCL low
  return ack;
}

/** Reads a byte from the slave and sends ACK or NACK */
uint8_t i2c_read_byte(bool ack)
{
  uint8_t byte = 0;
  pinMode(SDA_PIN, INPUT);            // Release SDA for slave to control
  for (int i = 7; i >= 0; i--)
  {
    pinMode(SCL_PIN, INPUT);          // SCL high
    delayMicroseconds(5);
    byte |= (digitalRead(SDA_PIN) << i); // Read bit
    pinMode(SCL_PIN, OUTPUT);
    digitalWrite(SCL_PIN, LOW);       // SCL low
    delayMicroseconds(5);
  }
  // Send ACK or NACK
  if (ack)
  {
    pinMode(SDA_PIN, OUTPUT);
    digitalWrite(SDA_PIN, LOW);       // ACK
  }
  else
  {
    pinMode(SDA_PIN, INPUT);          // NACK
  }
  pinMode(SCL_PIN, INPUT);            // SCL high
  delayMicroseconds(5);
  pinMode(SCL_PIN, OUTPUT);
  digitalWrite(SCL_PIN, LOW);         // SCL low
  pinMode(SDA_PIN, INPUT);            // Release SDA
  return byte;
}

#endif // MASTER

/***************************************************************************/
#ifdef SLAVE


/*
Uses PCINT2 vector to monitor both SDA and SCL changes.
*/

// State machine for I2C protocol
enum State
{
  IDLE,   // Waiting for start condition
  ADDR,   // Receiving address byte
  WRITE,  // Receiving data from master
  READ    // Sending data to master
};

// Volatile variables for interrupt handling
volatile State state = IDLE;
volatile uint8_t bit_count = 0;
volatile uint8_t current_byte = 0;
volatile bool address_match = false;
volatile char receive_buffer[32];     // Buffer for received data
volatile uint8_t receive_index = 0;
volatile char send_buffer[] = "World!"; // Data to send
volatile uint8_t send_index = 0;
volatile bool data_received = false;  // Flag for main loop
volatile uint8_t prev_pins = 0;       // Previous pin states

void setup()
{
  pinMode(SDA_PIN, INPUT); // SDA high (assuming external pull-ups)
  pinMode(SCL_PIN, INPUT); // SCL high (assuming external pull-ups)
  prev_pins = PIND;        // Capture initial pin states
  // Enable pin change interrupts for SDA (D4) and SCL (D5)
  PCICR |= (1 << PCIE2);   // Enable PCINT2 vector (Port D)
  PCMSK2 |= (1 << PCINT20) | (1 << PCINT21); // Enable PCINT for D4, D5
  Serial.begin(9600);
  Serial.println("Slave ready");
}

void loop()
{
  // Print received data when available
  if (data_received)
  {
    Serial.print("Received: ");
    for (int i = 0; i < receive_index; i++)
    {
      Serial.print(receive_buffer[i]);
    }
    Serial.println();
    receive_index = 0;
    data_received = false;
  }
}


/** Interrupt service routine for pin changes on SDA and SCL
 *  This ISR triggers whenever there’s a change on SDA (PD4) or SCL (PD5).
 *  It implements an I2C slave state machine to detect start/stop conditions,
 *  match the slave address, receive data from the master, and send data back.
 */
ISR(PCINT2_vect)
{
  // Read the current state of all pins on Port D (where SDA and SCL reside)
  uint8_t current_pins = PIND;

  // XOR current and previous pin states to detect which pins changed
  // 'changed' will have 1s where pins toggled (e.g., bit 5 for SCL, bit 4 for SDA)
  uint8_t changed = current_pins ^ prev_pins;

  // --- Handle SCL Changes ---
  // Check if SCL (pin 5) changed state
  if (changed & (1 << SCL_PIN))
  {
    // SCL Rising Edge (SCL goes from LOW to HIGH)
    if (current_pins & (1 << SCL_PIN))
    {
      // If in ADDR or WRITE state, we're receiving bits from the master
      if (state == ADDR || state == WRITE)
      {
        // Read the current SDA value (1 if HIGH, 0 if LOW) when SCL rises
        uint8_t sda = (current_pins & (1 << SDA_PIN)) ? 1 : 0;

        // Shift the current byte left and add the new SDA bit
        // This builds the byte bit-by-bit (MSB first)
        current_byte = (current_byte << 1) | sda;

        // Increment the bit counter (0 to 7 for an 8-bit byte)
        bit_count++;

        // When 8 bits are received, process the completed byte
        if (bit_count == 8)
        {
          // ADDR State: First byte after START is the address + R/W bit
          if (state == ADDR)
          {
            // Extract the 7-bit address (shift out the R/W bit)
            uint8_t address = current_byte >> 1;

            // Check if the address matches this slave’s address (0x08)
            if (address == SLAVE_ADDRESS)
            {
              // Address matches; set flag
              address_match = true;

              // Check the R/W bit (LSB of current_byte)
              if (current_byte & 1)
              {
                // R/W = 1: Master wants to read from slave
                state = READ;      // Switch to READ state
                send_index = 0;    // Start at beginning of send_buffer
                bit_count = 0;     // Reset bit counter for sending
              }
              else
              {
                // R/W = 0: Master wants to write to slave
                state = WRITE;     // Switch to WRITE state
                receive_index = 0; // Reset buffer for receiving data
              }
            }
            else
            {
              // Address doesn’t match; ignore this transaction
              state = IDLE;
            }
          }
          // WRITE State: Receiving data bytes from master
          else if (state == WRITE)
          {
            // Store the received byte in the buffer
            receive_buffer[receive_index++] = current_byte;

            // Prevent buffer overflow by wrapping around
            if (receive_index >= 32) receive_index = 0;
          }

          // Reset for the next byte
          bit_count = 0;
          current_byte = 0;
        }
      }
      // READ State: Master is reading from slave
      else if (state == READ)
      {
        // Master has clocked SCL to read a bit; we increment after each bit
        if (bit_count < 8)
        {
          bit_count++; // Count bits sent (0 to 7)
        }
        else
        {
          // After 8 bits, master sends ACK/NACK
          if (digitalRead(SDA_PIN) == LOW)
          {
            // ACK (LOW): Master wants more data
            send_index++;       // Move to next byte in send_buffer
            bit_count = 0;      // Reset for next byte
            if (send_index >= strlen(send_buffer))
            {
              // All data sent; return to IDLE
              state = IDLE;
            }
          }
          else
          {
            // NACK (HIGH): Master is done reading
            state = IDLE;
          }
        }
      }
    }
    // SCL Falling Edge (SCL goes from HIGH to LOW)
    else
    {
      // In READ state, set SDA before SCL falls for the next bit
      if (state == READ && send_index < strlen(send_buffer))
      {
        if (bit_count < 8)
        {
          // Get the current bit to send (MSB first)
          uint8_t bit = (send_buffer[send_index] >> (7 - bit_count)) & 1;

          // Set SDA based on the bit value
          if (bit)
          {
            pinMode(SDA_PIN, INPUT);       // SDA HIGH (via pull-up)
          }
          else
          {
            pinMode(SDA_PIN, OUTPUT);
            digitalWrite(SDA_PIN, LOW);    // SDA LOW
          }
        }
        else
        {
          // After 8 bits, release SDA for master’s ACK/NACK
          pinMode(SDA_PIN, INPUT);
        }
      }
    }
  }

  // --- Handle SDA Changes (Start/Stop Conditions) ---
  // Check if SDA (pin 4) changed state
  if (changed & (1 << SDA_PIN))
  {
    // SDA changes are only significant when SCL is HIGH
    if (current_pins & (1 << SCL_PIN))
    {
      // Stop Condition: SDA rises (LOW to HIGH) while SCL is HIGH
      if (!(prev_pins & (1 << SDA_PIN)) && (current_pins & (1 << SDA_PIN)))
      {
        // If in WRITE state, signal that data was fully received
        if (state == WRITE)
        {
          data_received = true; // Flag for loop() to print data
        }
        state = IDLE;           // Reset to IDLE state
      }
      // Start Condition: SDA falls (HIGH to LOW) while SCL is HIGH
      else if ((prev_pins & (1 << SDA_PIN)) && !(current_pins & (1 << SDA_PIN)))
      {
        state = ADDR;           // Expect address byte next
        bit_count = 0;          // Reset bit counter
        current_byte = 0;       // Clear current byte
        address_match = false;  // Reset address match flag
      }
    }
  }

  // Update previous pin states for the next interrupt
  prev_pins = current_pins;
}

#endif // SLAVE
