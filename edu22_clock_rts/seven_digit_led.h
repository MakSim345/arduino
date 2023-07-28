#ifndef seven_digit_led_h
#define seven_digit_led_h
/*
  seven_digit_led - Library for seven segments LED indicators.
*/
#include <Arduino.h>
#define DISPLAY_BRIGHTNESS  800
/* ATTN: This code works with a common cathode display. 
*  If you want to use a common anode display, 
*  change DIGIT_ON to "high" and DIGIT_OFF to "low" in the code.
**/
#define DIGIT_ON  LOW
#define DIGIT_OFF HIGH
#define ERASE_SEGMENTS 10

/*  Connection
 *
    Arduino pin     7 segment
        2             digit 0
        3             digit 1
        4             digit 1
        5             digit 1
        
        6              a
        7              b
        8              c
        9              d
        10             e
        11             f
        12             g
        13             dp(dot pin)

//           a
//        =======
//      ||       ||
//    f ||       || b
//      ||   g   ||
//        =======
//      ||       ||
//    e ||       || c
//      ||   d   ||
//        =======

*/

class seven_digit_led
{
  public:
    seven_digit_led(int pin);
    void show_number(int number_to_show);

  private:
    void writeNumber(int num);
    void init_segments();
    void init_digits();
    int current_pin;
    int digitONE;
    int digitTWO;
    int digitTHREE;
    int digitFOUR;
    
    int A, B, C, D, E, F, G;
    int H; // not in use
    void initDig_0();
    void initDig_1();
    void initDig_2();
    void initDig_3();
    void initDig_4();
    void initDig_5();
    void initDig_6();
    void initDig_7();
    void initDig_8();
    void initDig_9();

    int anodos[8];
    byte DIGITS[10][8];
/*
    const byte DIGITS[10][8] = {
    //A  B  C  D  E  F  G  H
    { 1, 1, 1, 1, 1, 1, 0, 0 }, // 0
    { 0, 1, 1, 0, 0, 0, 0, 0 }, // 1
    { 1, 1, 0, 1, 1, 0, 1, 0 }, // 2
    { 1, 1, 1, 1, 0, 0, 1, 0 }, // 3 
    { 0, 1, 1, 0, 0, 1, 1, 0 }, // 4
    { 1, 0, 1, 1, 0, 1, 1, 0 }, // 5
    { 1, 0, 1, 1, 1, 1, 1, 0 }, // 6
    { 1, 1, 1, 0, 0, 0, 0, 0 }, // 7
    { 1, 1, 1, 1, 1, 1, 1, 0 }, // 8
    { 1, 1, 1, 1, 0, 1, 1, 0 }  // 9
    };
*/
   
};

#endif
