#include <EEPROM.h>
/*

1. Include EEPROM library:
   - #include <EEPROM.h> includes the EEPROM library for interacting with the Arduino's EEPROM.

2. Define array size and EEPROM address:
   - const int arraySize = 10; defines the number of elements in the array.
   - const int eepromAddress = 0; defines the starting address in EEPROM to store the array.

3. Declare array:
   - int myArray[arraySize]; declares an integer array myArray with the defined size.

4. setup() function:
   - Serial.begin(9600); initializes serial communication at 9600 baud.
   - The loop initializes the array with values (in this case, multiples of 10) and calls the writeArrayToEEPROM function to save the array to EEPROM.

5. loop() function:
   - Calls the readArrayFromEEPROM function to read the array from EEPROM.
   - Prints the array values in a table format using Serial.print and Serial.println.
   - Delays for 1 second before repeating.

6. writeArrayToEEPROM() function:
   - Iterates through each element of the array.
   - Writes the value of each element to the corresponding EEPROM address using EEPROM.write().

7. readArrayFromEEPROM() function:
   - Iterates through each element of the array.
   - Reads the value from the corresponding EEPROM address using EEPROM.read() and stores it in the array.

Notes:

- The EEPROM has a limited number of write cycles, so avoid writing to it too often.
- Consider using a different data storage method if you need to write large amounts of data frequently.
- You can modify the array size, EEPROM address, and initial array values in the code to suit your needs.
*/

// Define the size of the array
const int arraySize = 25;

// Total EEPROM size for Arduino UNO:
// const int eepromSize = 1024;
const int eepromSize = 512;

// Define the EEPROM address to start storing the array
const int eepromAddress = 0;

// Declare the array
// int myArray[arraySize];
int myArray[eepromSize];

void setup()
{
  Serial.begin(9600);

  // Use an unconnected analog pin (e.g., A0) to seed the random number generator
  randomSeed(analogRead(A0));

  // Initialize the array with some values
  for (int i = 0; i < arraySize; i++)
  {
    // myArray[i] = i * 10;
    myArray[i] = random(0, 100); // Generate random number between 0 and 99
  }

  // Write the array to EEPROM
  // writeArrayToEEPROM(myArray, arraySize, eepromAddress);
}

void loop()
{
  // Read the array from EEPROM
  readArrayFromEEPROM(myArray, eepromAddress);

  // Print the array as a table
  Serial.println("EEPROM Array Values:");
  Serial.println("-----------------");

  for (int i = 0; i < arraySize; i++)
  // for (int i = 0; i < eepromSize; i++)
  {
    Serial.print("Index ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(myArray[i]);
  }
  Serial.println();

  delay(60000);
}

// Function to write an array to EEPROM
void writeArrayToEEPROM(int array[], int arraySize, int address)
{
    for (int i = 0; i < arraySize; i++)
    {
        byte currentValue = EEPROM.read(address + i); // Read the current value from EEPROM

        if (currentValue != array[i])
        { // Check if the value is different
            EEPROM.write(address + i, array[i]); // Write only if the values are different
        }
        else
        {
         Serial.print("Current Value:");
         Serial.print(currentValue);
         Serial.println(" is equal to write value. No WRITE operation performed.");
        }
    }
}

// Function to write value to EEPROM only if it is different from the current value
void writeEEPROM(int value, int address)
{
  if (EEPROM.read(address) != value)
  {
    EEPROM.write(address, value);
  }
}


/*
void writeArrayToEEPROM(int array[], int address)
{
  for (int i = 0; i < arraySize; i++)
  {
    EEPROM.write(address + i, array[i]);
  }
}
*/

// Function to read an array from EEPROM
void readArrayFromEEPROM(int array[], int address)
{
  // for (int i = 0; i < arraySize; i++)
  for (int i = 0; i < eepromSize; i++)
  {
    array[i] = EEPROM.read(address + i);
  }
}
