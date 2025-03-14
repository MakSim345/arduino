
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
// Master Code
void setup()
{
  pinMode(SDA_PIN, OUTPUT);
  pinMode(SCL_PIN, OUTPUT);
  digitalWrite(SDA_PIN, HIGH);
  digitalWrite(SCL_PIN, HIGH);
  delay(100);  // Allow time for devices to stabilize
}

void loop()
{
  // Send a byte to the slave
  i2cStart();  // Send start condition
  i2cWrite((I2C_ADDRESS << 1) | 0);  // Send slave address with write bit (0)
  i2cWrite(0x55);  // Send data byte (0x55 in this example)
  i2cStop();  // Send stop condition

  delay(1000);  // Wait for 1 second before repeating
}

// Generate I2C start condition
void i2cStart()
{
  digitalWrite(SDA_PIN, HIGH);
  digitalWrite(SCL_PIN, HIGH);
  delayMicroseconds(10);  // Slow down for observation
  digitalWrite(SDA_PIN, LOW);
  delayMicroseconds(10);
  digitalWrite(SCL_PIN, LOW);
}

// Generate I2C stop condition
void i2cStop()
{
  digitalWrite(SDA_PIN, LOW);
  digitalWrite(SCL_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(SDA_PIN, HIGH);
  delayMicroseconds(10);
}

// Write a byte to the I2C bus
void i2cWrite(uint8_t data)
{
  for (int i = 7; i >= 0; i--)
  {
    digitalWrite(SDA_PIN, (data & (1 << i)) ? HIGH : LOW);
    digitalWrite(SCL_PIN, HIGH);
    delayMicroseconds(10);  // Slow down for observation
    digitalWrite(SCL_PIN, LOW);
    delayMicroseconds(10);
  }

  // Check for ACK (Acknowledge) from the slave
  pinMode(SDA_PIN, INPUT);
  digitalWrite(SCL_PIN, HIGH);
  delayMicroseconds(10);
  bool ack = digitalRead(SDA_PIN) == LOW;
  digitalWrite(SCL_PIN, LOW);
  pinMode(SDA_PIN, OUTPUT);

  if (!ack)
  {
    // Handle no ACK (optional)
  }
}
# endif // MASTER


#ifdef SLAVE
// Slave Code

void setup()
{
  pinMode(SDA_PIN, INPUT);
  pinMode(SCL_PIN, INPUT);
  digitalWrite(SDA_PIN, HIGH);
  digitalWrite(SCL_PIN, HIGH);
}

void loop()
{
  // Wait for start condition
  if (i2cStartDetected())
  {
    // Read the address byte
    uint8_t address = i2cReadByte();

    if ((address >> 1) == I2C_ADDRESS)
    {
      // Send ACK if addressed
      i2cSendAck();

      // Read the data byte
      uint8_t data = i2cReadByte();
      i2cSendAck();  // Acknowledge data byte

      // Do something with the data (e.g., print it)
      // For demonstration, we'll just toggle an LED
      digitalWrite(LED_BUILTIN, data & 0x01);
    }
  }
}

// Detect I2C start condition
bool i2cStartDetected()
{
  while (digitalRead(SCL_PIN) == HIGH)
  {
    if (digitalRead(SDA_PIN) == LOW)
    {
      return true;
    }
  }
  return false;
}

// Read a byte from the I2C bus
uint8_t i2cReadByte()
{
  uint8_t data = 0;
  pinMode(SDA_PIN, INPUT);

  for (int i = 7; i >= 0; i--)
  {
    digitalWrite(SCL_PIN, HIGH);
    delayMicroseconds(10);  // Slow down for observation
    data |= (digitalRead(SDA_PIN) << i);
    digitalWrite(SCL_PIN, LOW);
    delayMicroseconds(10);
  }

  pinMode(SDA_PIN, OUTPUT);
  return data;
}

// Send ACK (Acknowledge) to the master
void i2cSendAck()
{
  digitalWrite(SDA_PIN, LOW);
  digitalWrite(SCL_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(SCL_PIN, LOW);
  digitalWrite(SDA_PIN, HIGH);
}

# endif // SLAVE
