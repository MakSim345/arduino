#include <avr/pgmspace.h>
#include <LedControl.h>
#include <Wire.h>
#include <RTClib.h>
#include "Time.h"
#include "char_matrix.h"

RTC_DS1307 RTC;


//#define NANO_IN_USE
#define ARDUINO_IN_USE

#ifdef ARDUINO_IN_USE
  #define DATA_IN_PIN     5 // D5
  #define CLK_PIN         6 // D6
  #define LOAD_PIN        7 // D7 
  #define INTERRUPT_PIN   2 // A button between PIN-2 and GND
  #define LED_MONSTER_PIN 4 // D4
#endif
/*
  Arduino NANO, pin #XX
*/
#ifdef NANO_IN_USE
  #define DATA_IN_PIN     5  //
  #define CLK_PIN         12 //
  #define LOAD_PIN        11 //
  #define INTERRUPT_PIN   2  // A button between PIN and GND
  #define LED_MONSTER_PIN 7  //
#endif

#define LED_TOMATO_PIN  9  // D9
#define LED_BREAK_PIN   8  // D8

const int numDevices = 4;      // number of MAX7219s used
const long scrollDelay = 75;   // adjust scrolling speed
const int DIGIT_DELAY = 5; // 2ms optimal

// #define DEBUG_FAST_TIME
#ifdef DEBUG_FAST_TIME
    const int TOMATO_TIME = 1;
    const int BREAK_TIME = 1;
    const int MONSTERS_TIME = 5;
#else
    const int TOMATO_TIME = 25; // minutes
    const int BREAK_TIME = 5;   // minutes
    const int MONSTERS_TIME = 10; //counter
#endif

enum RUN_STATUS
{
    BREAK_IN_RUN = 0,
    TOMATO_IN_RUN,
    MONSTERS_IN_RUN,
    CLOCK_IN_RUN,
    AFTER_TOMATO,
    AFTER_BREAK
};

long nextChange;
unsigned long _sec_to_print = 0;
unsigned long _cur_sec = 0;

int delayTime = 200; // Delay between Frames

char v_str[8] = "       ";  //reserve the string space first
unsigned long bufferLong [14] = {0};

RUN_STATUS flag = CLOCK_IN_RUN;
int timer_min = TOMATO_TIME;
int timer_sec = 0;
int invider_show_ctr = MONSTERS_TIME;
bool is_timer_run = false;
bool is_time_adjusted_today = false;

// a variable can change inside an ISR, thus must be volatile:
volatile byte btn_pressed_state = false;

DateTime ADTnow;

void(* resetFunc) (void) = 0; //declare reset function @ address 0

LedControl lc = LedControl(DATA_IN_PIN, CLK_PIN, LOAD_PIN, numDevices);
byte*  nums[] = {number0, number1, number2, number3, number4, number5, number6, number7, number8, number9};

void setup_matrix()
{
  for (int x = 0; x < numDevices; x++)
  {
    lc.shutdown(x, false);      //The MAX72XX is in power-saving mode on startup
    lc.setIntensity(x, 4 );      // Set the brightness to default value
    lc.clearDisplay(x);         // and clear the display
  }
}


void ISR_Button_Press()
{
// uncomment it in case of usage a SCHMITT trigger:
// #define USE_TRIGGER
#ifdef USE_TRIGGER
    btn_pressed_state = true;
#else
    static unsigned long millis_prev;
    const int debounce_delay = 10;

    if (millis() - debounce_delay > millis_prev)
    {
        btn_pressed_state = true;
    }

    millis_prev = millis();
#endif

}

void setup()
{
    setup_matrix();
    Serial.begin(9600);
    Serial.println("APP START!");
    // Serial.println(__DATE__);// May 20 2021

    // set the digital pin as output:
    // pinMode(PB5, OUTPUT); // LED_BUILTIN = PB5 = 13 pin
    // pinMode(PB1, OUTPUT); //PB1 = 9 pin
    pinMode(LED_BREAK_PIN, OUTPUT);
    pinMode(LED_TOMATO_PIN, OUTPUT);
    pinMode(LED_MONSTER_PIN, OUTPUT);

    pinMode(INTERRUPT_PIN, INPUT); // use external resistor to pull-down (GND)
    attachInterrupt(0, ISR_Button_Press, FALLING); //raise ISR every time button pressed
    //attachInterrupt(0, ISR_Button_Press, RISING); //raise ISR every time button pressed

    _sec_to_print = now();

    Wire.begin(); // need for RTC work!
    RTC.begin();

    if (!RTC.isrunning())
    {
        Serial.println("RTC is NOT running! Time will be ");
        Serial.println(__DATE__);
        Serial.println(__TIME__);
        // RTC.adjust(DateTime(__DATE__, __TIME__));
    }
    ADTnow = RTC.now();
    digitalWrite(LED_TOMATO_PIN, LOW); // initial: LED OFF, the led connect to this port and GND.
    digitalWrite(LED_BREAK_PIN, LOW); // initial: LED OFF, the led connect to this port and GND.
    digitalWrite(LED_MONSTER_PIN, LOW);

    // following line sets the RTC to the date & time this sketch was compiled:
    // RTC.adjust(DateTime(__DATE__, __TIME__));
    //RTC.adjust(DateTime(__DATE__, "09:49:00"));
    Serial.print(ADTnow.day());
    Serial.print(ADTnow.month());
    Serial.println(ADTnow.year());
}

void loop()
{
    if (btn_pressed_state)
    {
        Serial.println("btn_pressed_state: set to TRUE.");
        btn_pressed_state = false; // reset back
        Serial.println("btn_pressed_state: Reset back to FALSE.");
        if (flag == CLOCK_IN_RUN)
        {
            Serial.println("btn_pressed_state: flag == CLOCK_IN_RUN. call changeState()");
            changeState();
        }
    }

    long curentMillis = millis();
    if (curentMillis >= nextChange)
    {
        nextChange = curentMillis + DIGIT_DELAY;
        _cur_sec = now(); // function from "Time.h"
        if (_sec_to_print < _cur_sec)
        {
            ADTnow = RTC.now();

            /*
            Serial.print("_cur_sec: ");
            Serial.print(_cur_sec);
            Serial.print(" ");
            Serial.print("_sec_to_print: ");
            Serial.println(_sec_to_print);
            */

            decrement_timer();
            _sec_to_print = now();

            // DEBUG: check the current time:
            Serial.print(ADTnow.hour());
            Serial.print(':');
            Serial.print(ADTnow.minute());
            Serial.print(':');
            Serial.println(ADTnow.second());

            Serial.print(ADTnow.day());
            Serial.print('-');
            Serial.print(ADTnow.month());
            Serial.print('-');
            Serial.println(ADTnow.year());

            const int SECONDS_TO_SUBTRACT= 3; // amount of seconds used for adjust time once per day.
            // once per day, at 12:32 time it is adjusted because RTC is not perfect:
            if ( (SECONDS_TO_SUBTRACT == ADTnow.second()) && (32 == ADTnow.minute()) && (12 == ADTnow.hour()) )
            {
                Serial.println("It is adjusted TIME!");
                if (!is_time_adjusted_today)
                {
                    // #TODO: __DATE__ will be always same, need to find how to get current data.
                    // char temp [] = __DATE__; // May 20 2021
                    RTC.adjust(DateTime(__DATE__, "12:32:00"));
                    is_time_adjusted_today = true;
                    Serial.print("Time adjusted to: ");
                    Serial.print(SECONDS_TO_SUBTRACT);
                    Serial.println(" seconds.");
                }
                else
                {
                    is_time_adjusted_today = false;
                    Serial.println("Time was adjusted few seconds ago. Wait next 24 hours.");
                }
            }
        }

    switch (flag)
    {
        case TOMATO_IN_RUN:
            show_min (timer_min);
            show_sec (timer_sec);
            break;
        case MONSTERS_IN_RUN:
            showMonsters();
            break;
        case BREAK_IN_RUN:
            show_min (timer_min);
            show_sec (timer_sec);
            break;
        case CLOCK_IN_RUN:
            timer_min = ADTnow.hour();
            timer_sec = ADTnow.minute();
            //Serial.print(timer_min);
            //Serial.print(':');
            //Serial.print(timer_sec);
            //Serial.println(':');
            show_min (timer_min);
            show_sec (timer_sec);
            break;
        default:
            Serial.println("case - DEFAULT");
            break;
        }
    }
}

void showMonsters()
{
    digitalWrite(LED_MONSTER_PIN, HIGH);
    // Put #1 frame on both Display
    sinvader1a();
    delay(delayTime);
    sinvader2a();
    delay(delayTime);

    // Put #2 frame on both Display
    sinvader1b();
    delay(delayTime);
    sinvader2b();
    delay(delayTime);

    digitalWrite(LED_MONSTER_PIN, LOW);

    invider_show_ctr = invider_show_ctr - 1;

    if (invider_show_ctr <= 0)
    {
        invider_show_ctr = MONSTERS_TIME;// set counter to initial
        changeState();
    }
}

void changeState()
{
    static RUN_STATUS prevStatus = TOMATO_IN_RUN;
    switch (flag)
    {
    case TOMATO_IN_RUN:
        flag = MONSTERS_IN_RUN;
        prevStatus = TOMATO_IN_RUN;
        timer_min = BREAK_TIME;
        timer_sec = 0;
        digitalWrite(LED_TOMATO_PIN, LOW); // LED - OFF
        Serial.write("CHANGE from TOMATO_IN_RUN to MONSTERS_IN_RUN \n");
        break;
    case MONSTERS_IN_RUN:
        if (prevStatus == TOMATO_IN_RUN)
        {
            flag = BREAK_IN_RUN;
            is_timer_run = true;
            digitalWrite(LED_BREAK_PIN, HIGH); // LED - ON, to show BREAK in RUN
            Serial.write("CHANGE from MONSTERS_IN_RUN to BREAK_IN_RUN \n");
        }
        else
        {
            flag = CLOCK_IN_RUN;
            digitalWrite(LED_BREAK_PIN, LOW); // LED - OFF, to show BREAK in OVER
            Serial.write("CHANGE from MONSTERS_IN_RUN to CLOCK_IN_RUN \n");
        }
        break;
    case BREAK_IN_RUN:
        flag = MONSTERS_IN_RUN;
        prevStatus = BREAK_IN_RUN;
        timer_min = TOMATO_TIME;
        timer_sec = 0;
        Serial.write("CHANGE from BREAK_IN_RUN to MONSTERS_IN_RUN \n");
        break;
    case CLOCK_IN_RUN:
        flag = TOMATO_IN_RUN;
        is_timer_run = true;
        timer_min = TOMATO_TIME;
        timer_sec = 0;
        digitalWrite(LED_TOMATO_PIN, HIGH); // LED - ON, to show TOMATO in RUN
        Serial.write("CHANGE from CLOCK_IN_RUN to TOMATO_IN_RUN \n");
        break;
    default:
        flag = TOMATO_IN_RUN;
        Serial.println("case - DEFAULT: TOMATO_IN_RUN");
        break;
    }
}

void decrement_timer()
{
    static bool isNewTimerStart = false;
    // Serial.println("enter decrement_timer()");

    if (!is_timer_run)
    {
        // Serial.println("return from decrement_timer(): is_timer_run is FALSE");
        return;
    }

    if (isNewTimerStart == true)
    {
        // Serial.write("Set isNewTimerStart to 0\n");
        isNewTimerStart = false;
        return;
    }

    if (0 == timer_sec)
    {
        timer_sec = 59;
        timer_min = timer_min - 1;
        if (timer_min < 0)
        {
            timer_min = 0;
            timer_sec = 0;
            isNewTimerStart = true;
            is_timer_run = false;
            Serial.println("is_timer_run set to FALSE");
            changeState();
        }
    }
    else
    {
        timer_sec = timer_sec - 1;
        // Serial.print("decrement timer_sec: ");
        // Serial.println(timer_sec);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Take values in Arrays and Display them
void show_sec(int seconds)
{
    byte decimal[8] = {0};
    decimal[1] = seconds / 10;
    decimal[0] = seconds % 10;

    for (int i = 0; i < 8; i++)
    {
        lc.setRow(0, i, nums[decimal[0]][i]);
    }

    for (int i = 0; i < 8; i++)
    {
        lc.setRow(1, i, nums[decimal[1]][i]);
    }

    //lc.setDigit(0, 0, decimal[0], false);
    //lc.setDigit(0, 1, decimal[1], false);
}

void show_min(int minutes)
{
    byte decimal[8] = {0};
    decimal[1] = minutes / 10;
    decimal[0] = minutes % 10;

    for (int i = 0; i < 8; i++)
    {
        lc.setRow(2, i, nums[decimal[0]][i]);
    }

    for (int i = 0; i < 8; i++)
    {
        lc.setRow(3, i, nums[decimal[1]][i]);
    }

    //lc.setDigit(0, 3, decimal[0], false);
    //lc.setDigit(0, 4, decimal[1], false);
}

void sinvader1a()
{
    for (int i = 0; i < 8; i++)
    {
        lc.setRow(0, i, invader1a[i]);
        lc.setRow(2, i, invader1a[i]);
    }
}

void sinvader1b()
{
    for (int i = 0; i < 8; i++)
    {
        lc.setRow(0, i, invader1b[i]);
        lc.setRow(2, i, invader1b[i]);
    }
}

void sinvader2a()
{
    for (int i = 0; i < 8; i++)
    {
        lc.setRow(1, i, invader2a[i]);
        lc.setRow(3, i, invader2a[i]);
    }
}

void sinvader2b()
{
    for (int i = 0; i < 8; i++)
    {
        lc.setRow(1, i, invader2b[i]);
        lc.setRow(3, i, invader2b[i]);
    }
}

void scrollFont()
{
    for (int counter = 0x20; counter < 0x80; counter++)
    {
        loadBufferLong(counter);
        delay(500);
    }
}

// Scroll Message
void scrollMessage(const prog_uchar * messageString)
{
    int counter = 0;
    int myChar = 0;
    do
    {
        // read back a char
        myChar =  pgm_read_byte_near(messageString + counter);
        if (myChar != 0)
        {
            loadBufferLong(myChar);
        }
        counter++;
    }
    while (myChar != 0);
}
// Load character into scroll buffer
void loadBufferLong(int ascii)
{
    if (ascii >= 0x20 && ascii <= 0x7f)
    {
        for (int a = 0; a < 7; a++)
        {
        // Loop 7 times for a 5x7 font
        unsigned long c = pgm_read_byte_near(font5x7 + ((ascii - 0x20) * 8) + a);     // Index into character table to get row data
        unsigned long x = bufferLong [a * 2];   // Load current scroll buffer
        x = x | c;                              // OR the new character onto end of current
        bufferLong [a * 2] = x;                 // Store in buffer
        }

        byte count = pgm_read_byte_near(font5x7 + ((ascii - 0x20) * 8) + 7);    // Index into character table for kerning data

        for (byte x = 0; x < count; x++)
        {
        rotateBufferLong();
        printBufferLong();
        delay(scrollDelay);
        }
    }
}
// Rotate the buffer
void rotateBufferLong()
{
    for (int a = 0; a < 7; a++)
    {   // Loop 7 times for a 5x7 font
        unsigned long x = bufferLong [a * 2];   // Get low buffer entry
        byte b = bitRead(x, 31);                // Copy high order bit that gets lost in rotation
        x = x << 1;                             // Rotate left one bit
        bufferLong [a * 2] = x;                 // Store new low buffer
        x = bufferLong [a * 2 + 1];             // Get high buffer entry
        x = x << 1;                             // Rotate left one bit
        bitWrite(x, 0, b);                      // Store saved bit
        bufferLong [a * 2 + 1] = x;             // Store new high buffer
    }
}
// Display Buffer on LED matrix
void printBufferLong()
{
    for (int a = 0; a < 7; a++)
    { // Loop 7 times for a 5x7 font
        unsigned long x = bufferLong [a * 2 + 1]; // Get high buffer entry
        byte y = x;                             // Mask off first character
        lc.setRow(3, a, y);                     // Send row to relevent MAX7219 chip
        x = bufferLong [a * 2];                 // Get low buffer entry
        y = (x >> 24);                          // Mask off second character
        lc.setRow(2, a, y);                     // Send row to relevent MAX7219 chip
        y = (x >> 16);                          // Mask off third character
        lc.setRow(1, a, y);                     // Send row to relevent MAX7219 chip
        y = (x >> 8);                           // Mask off forth character
        lc.setRow(0, a, y);                     // Send row to relevent MAX7219 chip
    }
}
