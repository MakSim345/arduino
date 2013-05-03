#include "seven_digit_led.h"

seven_digit_led::seven_digit_led(int pin)
{
    current_pin = 0;
    firdstDigit = -1;
    secondDigit = -1;

    init_segments();
    init_digits();
}

void seven_digit_led::init_segments() 
{
    A = 6; B = 7; C = 8; D = 9;
    E = 10; F = 11; G = 12;
    H = 13; // not in use

    // anodos[8] = { A, B, C, D, E, F, G, H };
    anodos[0] = A;
    anodos[1] = B;
    anodos[2] = C;
    anodos[3] = D;
    anodos[4] = E;
    anodos[5] = F;
    anodos[6] = G;
    anodos[7] = H;
    
    // set 7 pins for segments to output:
    for (int a = 0; a < 8; a++)
    {
        pinMode(anodos[a], OUTPUT);
    }
}

void seven_digit_led::init_digits() 
{
    // create arrays of binary values for perform a digit:
    initDig_0();
    initDig_1();
    initDig_2();
    initDig_3();
    initDig_4();
    initDig_5();
    initDig_6();
    initDig_7();
    initDig_8();
    initDig_9();
    
    // set 4 pins for digits to output:
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
}

void seven_digit_led::writeNumber(int num) 
{
   // if param is 10 or more, deactivate all segments, i. e. erase.
   if (num > 9)
    {
      for (int a = 0; a < 8; a++) 
       {
        digitalWrite(anodos[a], LOW);
       }
    }
    
   // activate segments according to 1 in array related to the digit:
    for (int a = 0; a < 8; a++) 
    {
        if (DIGITS[num][a] == 1)
            digitalWrite(anodos[a], HIGH);
        else
            digitalWrite(anodos[a], LOW);
    }
}

void seven_digit_led::show_number(int currentDigit)
{ 
    firdstDigit = (currentDigit - currentDigit % 10)/10;
    secondDigit = currentDigit % 10;
    for(int digit = 4; digit > 0 ; digit--) 
    {
    //Turn on a digit for a short amount of time
    switch(digit) 
        {
        case 1:
            current_pin = 2;
            digitalWrite(current_pin, DIGIT_ON);
            writeNumber(0);
            //writeNumber(firdstDigit);
            break;
        case 2:
            current_pin = 3;
            digitalWrite(current_pin, DIGIT_ON);
            writeNumber(0);
            //writeNumber(secondDigit);
            break;
        case 3:
            current_pin = 4;
            digitalWrite(current_pin, DIGIT_ON);
            writeNumber(firdstDigit);
            break;
        case 4:
            current_pin = 5;
            digitalWrite(current_pin, DIGIT_ON);
            writeNumber(secondDigit);
            break;
        }

        // wait for fix an image:
        delayMicroseconds(DISPLAY_BRIGHTNESS);
        // reset all digits for refresh:
        writeNumber(ERASE_SEGMENTS);
        digitalWrite(current_pin, DIGIT_OFF);
    }
}

void seven_digit_led::initDig_0()
{
    DIGITS[0][0] = 1; // A
    DIGITS[0][1] = 1; // B
    DIGITS[0][2] = 1; // C
    DIGITS[0][3] = 1; // D
    DIGITS[0][4] = 1; // E
    DIGITS[0][5] = 1; // F
    DIGITS[0][6] = 0; // G
    DIGITS[0][7] = 0; // H
}
void seven_digit_led::initDig_1()
{ 
    DIGITS[1][0] = 0; // A
    DIGITS[1][1] = 1; // B
    DIGITS[1][2] = 1; // C
    DIGITS[1][3] = 0; // D
    DIGITS[1][4] = 0; // E
    DIGITS[1][5] = 0; // F
    DIGITS[1][6] = 0; // G
    DIGITS[1][7] = 0; // H
}
void seven_digit_led::initDig_2()
{
    DIGITS[2][0] = 1; // A
    DIGITS[2][1] = 1; // B
    DIGITS[2][2] = 0; // C
    DIGITS[2][3] = 1; // D
    DIGITS[2][4] = 1; // E
    DIGITS[2][5] = 0; // F
    DIGITS[2][6] = 1; // G
    DIGITS[2][7] = 0; // H
}
void seven_digit_led::initDig_3()
{
    DIGITS[3][0] = 1; // A
    DIGITS[3][1] = 1; // B
    DIGITS[3][2] = 1; // C
    DIGITS[3][3] = 1; // D
    DIGITS[3][4] = 0; // E
    DIGITS[3][5] = 0; // F
    DIGITS[3][6] = 1; // G
    DIGITS[3][7] = 0; // H
}
void seven_digit_led::initDig_4()
{
    DIGITS[4][0] = 0; // A
    DIGITS[4][1] = 1; // B
    DIGITS[4][2] = 1; // C
    DIGITS[4][3] = 0; // D
    DIGITS[4][4] = 0; // E
    DIGITS[4][5] = 1; // F
    DIGITS[4][6] = 1; // G
    DIGITS[4][7] = 0; // H
}
void seven_digit_led::initDig_5()
{
    DIGITS[5][0] = 1; // A
    DIGITS[5][1] = 0; // B
    DIGITS[5][2] = 1; // C
    DIGITS[5][3] = 1; // D
    DIGITS[5][4] = 0; // E
    DIGITS[5][5] = 1; // F
    DIGITS[5][6] = 1; // G
    DIGITS[5][7] = 0; // H
}
void seven_digit_led::initDig_6()
{
    DIGITS[6][0] = 1; // A
    DIGITS[6][1] = 0; // B
    DIGITS[6][2] = 1; // C
    DIGITS[6][3] = 1; // D
    DIGITS[6][4] = 1; // E
    DIGITS[6][5] = 1; // F
    DIGITS[6][6] = 1; // G
    DIGITS[6][7] = 0; // H
}
void seven_digit_led::initDig_7()
{
    DIGITS[7][0] = 1; // A
    DIGITS[7][1] = 1; // B
    DIGITS[7][2] = 1; // C
    DIGITS[7][3] = 0; // D
    DIGITS[7][4] = 0; // E
    DIGITS[7][5] = 0; // F
    DIGITS[7][6] = 0; // G
    DIGITS[7][7] = 0; // H
}
void seven_digit_led::initDig_8()
{
    DIGITS[8][0] = 1; // A
    DIGITS[8][1] = 1; // B
    DIGITS[8][2] = 1; // C
    DIGITS[8][3] = 1; // D
    DIGITS[8][4] = 2; // E
    DIGITS[8][5] = 3; // F
    DIGITS[8][6] = 1; // G
    DIGITS[8][7] = 0; // H
}
void seven_digit_led::initDig_9()
{
    DIGITS[9][0] = 1; // A
    DIGITS[9][1] = 1; // B
    DIGITS[9][2] = 1; // C
    DIGITS[9][3] = 1; // D
    DIGITS[9][4] = 0; // E
    DIGITS[9][5] = 1; // F
    DIGITS[9][6] = 1; // G
    DIGITS[9][7] = 0; // H
}
