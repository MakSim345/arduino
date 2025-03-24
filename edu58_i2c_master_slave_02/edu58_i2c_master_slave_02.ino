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

/** Interrupt service routine for pin changes on SDA and SCL */
ISR(PCINT2_vect)
{
  uint8_t current_pins = PIND;           // Read current pin states
  uint8_t changed = current_pins ^ prev_pins; // Detect changes

  // Handle SCL changes
  if (changed & (1 << SCL_PIN))
  {
    if (current_pins & (1 << SCL_PIN))
    { // SCL rising edge
      if (state == ADDR || state == WRITE)
      {
        // Read SDA bit
        uint8_t sda = (current_pins & (1 << SDA_PIN)) ? 1 : 0;
        current_byte = (current_byte << 1) | sda;
        bit_count++;
        if (bit_count == 8)
        {
          if (state == ADDR)
          {
            uint8_t address = current_byte >> 1;
            if (address == SLAVE_ADDRESS)
            {
              address_match = true;
              if (current_byte & 1)
              { // R/W = 1 (read)
                state = READ;
                send_index = 0;
                bit_count = 0;
              }
              else
              {                // R/W = 0 (write)
                state = WRITE;
                receive_index = 0;
              }
            }
            else
            {
              state = IDLE;         // Address mismatch
            }
          }
          else if (state == WRITE)
          {
            receive_buffer[receive_index++] = current_byte;
            if (receive_index >= 32) receive_index = 0; // Prevent overflow
          }
          bit_count = 0;
          current_byte = 0;
        }
      }
      else if (state == READ)
      {
        // Master is reading; increment bit_count after bit is read
        if (bit_count < 8)
        {
          bit_count++;
        }
        else
        {
          // After 8 bits, check ACK from master
          if (digitalRead(SDA_PIN) == LOW)
          { // ACK received
            send_index++;
            bit_count = 0;
            if (send_index >= strlen(send_buffer))
            {
              state = IDLE;
            }
          }
          else
          {                         // NACK received
            state = IDLE;
          }
        }
      }
    }
    else
    { // SCL falling edge
      if (state == READ && send_index < strlen(send_buffer))
      {
        if (bit_count < 8)
        {
          // Set SDA for the next bit
          uint8_t bit = (send_buffer[send_index] >> (7 - bit_count)) & 1;
          if (bit)
          {
            pinMode(SDA_PIN, INPUT);       // SDA high
          }
          else
          {
            pinMode(SDA_PIN, OUTPUT);
            digitalWrite(SDA_PIN, LOW);    // SDA low
          }
        }
        else
        {
          pinMode(SDA_PIN, INPUT);         // Release SDA for ACK
        }
      }
    }
  }

  // Handle SDA changes (start/stop conditions)
  if (changed & (1 << SDA_PIN))
  {
    if (current_pins & (1 << SCL_PIN))
    { // SCL is high
      if (!(prev_pins & (1 << SDA_PIN)) && (current_pins & (1 << SDA_PIN)))
      {
        // Stop condition: SDA rises while SCL is high
        if (state == WRITE)
        {
          data_received = true;          // Data received to print
        }
        state = IDLE;
      }
      else if ((prev_pins & (1 << SDA_PIN)) && !(current_pins & (1 << SDA_PIN)))
      {
        // Start condition: SDA falls while SCL is high
        state = ADDR;
        bit_count = 0;
        current_byte = 0;
        address_match = false;
      }
    }
  }

  prev_pins = current_pins; // Update previous pin states
}

#endif // SLAVE
