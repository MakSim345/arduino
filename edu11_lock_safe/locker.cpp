/* @file Combination_Lock.pde
|| @version 1.0
|| @author Ade Vickers
|| @contact javickers@solutionengineers.com
||
|| @description
|| | Based on keypad.h amongst others. Drive a combination lock
|| | using a 16-key keypad. Secret codes are hard-coded below.
|| |
|| | Thanks to Alexander Brevig/Mark Stanley for the Keypad library
|| #
*/
#include <Keypad.h>
#include <Servo.h>

// Define keyboard
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = 
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {6,7,8,9};                 //connect to the row pinouts of the keypad
byte colPins[COLS] = {2,3,4,5};                 //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// Pins used for LEDs and the servo
byte attnPin = 12;
byte errorPin = 13;
byte lockOperatePin = 10;
byte lockIndicatePin = 11;

// Status and timer variables
boolean haveAttention = false;                  // True after "A" is pressed to get the machine's attention
byte loseAttentionIn = 0;                       // Used as a counter
const byte ATTENTION_SPAN = 50;                 // Approx 5 seconds
boolean haveError;                              // Problem with the combination (if true, red light is on)
byte loseErrorIn = 0;                           // Clear error timer
boolean lockOperate;                            // Lock is open (active)
byte lockDeOperateIn = 0;                       // Used as a counter, ala loseAttentionIn

char myCode[5] = "0000";                        // Starting point for the code
char* myComp[] = { "1234", "5678", "9012" };    // The secret codes the machine will recognise
const byte NUM_CODES = 3;                       // How many are there [I shouldn't need this...]

// Lock servo
Servo lockServo;
const int LOCK_CLOSED = 25;             // Servo sits at 25 degrees when "inactive"
const int LOCK_OPENED = 71;             // 71 degrees happens to be the sweet spot for the lock to be open

void setup()
{
  
  Serial.begin(9600);                   // Debugging via serial port
  
  pinMode(attnPin, OUTPUT);             // Prepare the output pins (these all connect to the LEDs)
  pinMode(errorPin, OUTPUT);
  pinMode(lockIndicatePin, OUTPUT);  
  
  digitalWrite(attnPin, HIGH);          // Flash all LEDs on & off in sequence, tells me the lock is initialized.
  delay(50);
  digitalWrite(errorPin, HIGH);
  delay(50);
  digitalWrite(lockIndicatePin, HIGH);
  
  delay(250);

  digitalWrite(attnPin, LOW);           // Initialize to waiting state
  delay(50);
  digitalWrite(errorPin, LOW);
  delay(50);
  digitalWrite(lockIndicatePin, LOW);

  // Attach the servo and move to position 0
  lockServo.attach(lockOperatePin);
  lockServo.write(LOCK_CLOSED);
  
  // Not waiting for a code yet
  haveAttention = false;
  
  keypad.addEventListener(keypadEvent); //add an event listener for this keypad
}
  
void loop()
{  
  char key = keypad.getKey();           // Read the keypad
  int i;
  
  if (key != NO_KEY) 
  {// Assuming a key is pressed...
    switch (key) 
    {
      case 'A':
            Serial.print("A ");         // For debugging purposes, output the button pressed.
        break;       
      case 'B':
            Serial.print("B ");
        break;       
      case 'C':
        // Handled in "press"
            Serial.print("C ");
        break;       
      case 'D':
            Serial.print("D ");
        
        if (haveAttention) {            // If we have attention, then check for codes
          boolean match = false;
          for(i=0;i<NUM_CODES;i++) {
            Serial.print(myComp[i]);    // Debugging - I was having trouble with strcmp
            Serial.print(" ");
            Serial.println(myCode);
            if (strcmp(myComp[i], myCode) == 0)
              match = true;             // One of the codes matched; set match = true
          }
          if (match) {
             // Unlock code
             setStatus(0,0,255);        // Operate the lock
          } else {
             // Error code
             setStatus(0,255,0);        // Display an error
          }
        }
        break;
      default:
        // Any other key, add it to the code. We roll the code forwards and add the char at the end...
        // The code could include # and * characters, although none of them do
        if (haveAttention) {
          loseAttentionIn = ATTENTION_SPAN; // Reset the attention counter
          for(i=1;i<4;i++)                  // Scroll the code in the code buffer; 
            myCode[i-1] = myCode[i];        //   i.e. "1234" becomes "2344"
        
          // Append the new code            // Replace the 4th char with the new char 
          myCode[3] = key;                  //   i.e. if 5 was pressed, "2344" becomes "2345"
          
          // Blink the attention light to register the keypress
          digitalWrite(attnPin,!haveAttention);
          delay(30);
          digitalWrite(attnPin,haveAttention);
        }

    }
  }
  
  // Wait 100ms, then do additional processing
  delay(100);
  
  // Attention timer
  if (loseAttentionIn > 0) {
    loseAttentionIn--;
    if (loseAttentionIn == 0) {
      setStatus(0,1,1);             // Change attention status to "don't have attention"
    }
  }
  
  // Error timer
  if (loseErrorIn > 0) {
    loseErrorIn--;
    if (loseErrorIn == 0) {
      setStatus(1,0,1);             // Put error lamp out, if it's on.
    }
  }
  
  // Lock operate timer
  if (lockDeOperateIn > 0) {        // If the lock is currently operating...
    lockDeOperateIn--;
    if (lockDeOperateIn == 0) {
      setStatus(1,1,0);             // Switch lock operator off
    }
  }
  
}
void setStatus(byte attention, byte error, byte lock) {
  // Depending on the flags, set/change accordingly
  // Flags can have 3 values: 0 (reset), 255 (set) or 1 (ignore)
  int i;
  
  // Set the attention lamp
  switch (attention) {
    case 0:
      haveAttention = false;
      break;
    case 255:
      haveAttention = true;
      loseAttentionIn = ATTENTION_SPAN;     // Approx 5 seconds
      for(i=0;i<4;i++)                      // Clear the current code
        myCode[i] = '0';
      break;
    default:
      break;
  }
  digitalWrite(attnPin,haveAttention);      // Set the Attention LED as applicable
  
  // Set the error lamp
  switch (error) {
    case 0:
      haveError = false;
      break;
    case 255:
      haveError = true;
      loseErrorIn = 75;                     // That should be a constant...
      break;
    default:
      break;
  }
  digitalWrite(errorPin,haveError);         // This code is the same as the above
  
  // Operate/deoperate the locking system
  switch (lock) {
    case 0:
      lockOperate = false;                    // Deallocate lock operator
      lockDeOperateIn = 0;                    // Cancel the timer
      break;
    case 255:
      lockOperate = true;                     // Indicates the lock is active
      lockDeOperateIn = 50;                   // 5 seconds, then de-operate automatically
      break;
    default:
      break;
  }
  if (lockOperate)                              // This code moves the servo
    lockServo.write(LOCK_OPENED);
  else 
    lockServo.write(LOCK_CLOSED);
  digitalWrite(lockIndicatePin,lockOperate);    // Cancel the lock indicator
}

//take care of some special events
// This is pretty similar to the Keypad.h example code.

void keypadEvent(KeypadEvent key){
  switch (keypad.getState()){
    case PRESSED:
      switch (key){
        case 'B':                               // Doorbell; drive the bellpin
                                                // At this time, I don't have a spare pin to drive a doorbell. I will have
                                                // to lose either serial tx/rx capability, or one of the LEDs, to drive it.                                                
          // Sacrifice attention if we hit the bell push
          if (!lockOperate) {   // Can't do it if the lock is operating (will leave lock in an indeterminate position)
            setStatus(0,0,0);
            digitalWrite(lockIndicatePin,true); // Use the lock indicator to show the "buzzer" is "active"
          }          
          break;
        case 'A':  // Get attention, clear the code
          if (!lockOperate)    // Can't do it if the lock is operating (will leave lock in an indeterminate position)
            setStatus(255,0,0);
          break;
        case 'C':  // Clear - lose attention, basically, and clear any error. Ideal if you muff the code half way through.
          if (!lockOperate)    // Can't do it if the lock is operating (will leave lock in an indeterminate position)
            setStatus(0,0,0);
          break;
      }
    break;
    case RELEASED:
      switch (key){
        case 'B':                               // When the "doorbell" is released, stop "ringing" it...          
          if (!lockOperate) // Can't do it if the lock is operating (will leave lock in an indeterminate position)
            digitalWrite(lockIndicatePin,false);  // doorbell released
          break;
      }
    break;
  }
}

