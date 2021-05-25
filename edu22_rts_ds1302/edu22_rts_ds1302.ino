#include "virtuabotixRTC.h"

#define NANO_IN_USE
#ifdef NANO_IN_USE
    #define CE_PIN   4  // use D4 for RST
    #define IO_PIN   3  // use D3 for DAT
    #define SCLK_PIN 2  // use D2 for CLK
#endif

// #define UNO_IN_USE
#ifdef UNO_IN_USE
    #define CE_PIN   13  // RST?
    #define IO_PIN   12  // DAT?
    #define SCLK_PIN 11  // CLK
#endif

// Creation of the Real Time Clock Object
// virtuabotixRTC(uint8_t inSCLK, uint8_t inIO, uint8_t inC_E);
virtuabotixRTC myRTC(SCLK_PIN, IO_PIN, CE_PIN);

char *getDayOfWeek(int dayP);

const char *months[] = {"Jan", "Feb", "Mar",
                        "Apr", "May", "Jun",
                        "Jul", "Aug", "Sep",
                        "Oct", "Nov", "Dec" };

void getdate(int& yearP, int& monthP, int& dayP)
{
    char temp [] = __DATE__; // "Jan  8 2021"

    const int MILLENIUM = 2000;
    unsigned char i;

    yearP = atoi(temp + 9) + MILLENIUM;
    *(temp + 6) = 0;

    dayP = atoi(temp + 4);
    *(temp + 3) = 0;

    for (i = 0; i < 12; i++)
    {
        if (!strcmp(temp, months[i]))
        {
            monthP = i + 1;
            return;
        }
    }
}

void gettime(int& hourP, int& minutesP, int& secondsP)
{
    char temp [] = __TIME__; //i.e - 12:52:22

    secondsP = atoi(temp + 6);
    *(temp + 5) = 0;

    minutesP = atoi(temp + 3);
    *(temp + 2) = 0;

    hourP = atoi(temp);
}


void setup()
{
  Serial.begin(9600);

  // Set the current date, and time in the following format:
  // seconds, minutes, hours, day of the week, day of the month, month, year
  //                  SS SS  MM  HH DW DD  MM  YYYY
  // myRTC.setDS1302Time(00, 21, 9, 2, 13, 10, 2015);
  // myRTC.setDS1302Time(00, 55, 10, 8, 8, 1,  2021);
  //Set date/time variables:

// #define SET_TIME
#ifdef SET_TIME
    int _YEAR = 0;
    int _MONTH = 0;
    int _DAY_M = 0;
    getdate(_YEAR, _MONTH, _DAY_M);
    Serial.print("Year - ");
    Serial.println(_YEAR);
    Serial.print("Month - ");
    Serial.println(_MONTH);
    Serial.print("Day - ");
    Serial.println(_DAY_M);

    const int _DAY_W = 6; // SUNDAY first day!

    int _HOUR  = 0;
    int _MINUT = 0;
    int _SECON = 0;
    Serial.println(__TIME__);
    gettime(_HOUR, _MINUT, _SECON);
    Serial.print("Hour - ");
    Serial.println(_HOUR);
    Serial.print("Min - ");
    Serial.println(_MINUT);
    Serial.print("Sec - ");
    Serial.println(_SECON);
    Serial.println("Set Time for DS1302....");
    myRTC.setDS1302Time(_SECON, _MINUT, _HOUR, _DAY_W, _DAY_M, _MONTH, _YEAR);
    Serial.println("Done.");
    // myRTC.setDS1302Time(0, 15, 13, 8, 8, 1, 2021);
#endif

}

void loop()
{
// This allows for the update of variables for time or accessing the individual elements.
  myRTC.updateTime();

  // Start printing elements as individuals
  Serial.print("Current Date or Time: ");
  //Serial.print(getDayOfWeek(myRTC.dayofweek));
  //Serial.print(" - ");
  Serial.print(myRTC.dayofmonth);
  Serial.print("/");
  Serial.print(myRTC.month);
  Serial.print("/");
  Serial.print(myRTC.year);
  Serial.print("  ");
  Serial.print(myRTC.hours);
  Serial.print(":");
  Serial.print(myRTC.minutes);
  Serial.print(":");
  Serial.println(myRTC.seconds);

  // Delay
  delay(2000);
}

char *getDayOfWeek(int dayP)
{
    /* Create buffers */
    // char day[10];
    char* day = malloc(10);
    /* Name the day of the week */
    // memset(day, 0, sizeof(day));  // clear day buffer
    switch (dayP)
    {
    case 1:
        strcpy(day, "Sunday");
        break;
    case 2:
        strcpy(day, "Monday");
        break;
    case 3:
        strcpy(day, "Tuesday");
        break;
    case 4:
        strcpy(day, "Wednesday");
        break;
    case 5:
        strcpy(day, "Thursday");
        break;
    case 6:
        strcpy(day, "Friday");
        break;
    case 7:
        strcpy(day, "Saturday");
        break;
    }

    return day;
}
