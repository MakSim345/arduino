#include <Adafruit_NeoPixel.h>
#include <PinChangeInt.h>

#define   START_PIN            7
#define   LEDS_PIN             11
#define   VOLUME_1_PIN         A0
#define   VOLUME_2_PIN         A1
#define   VOLUME_3_PIN         A2
#define   VOLUME_4_PIN         A3
#define   TOTAL_VOLUME_PIN     A4
#define   ACT_1_PIN            3
#define   ACT_2_PIN            5
#define   ACT_3_PIN            6
#define   ACT_4_PIN            9
#define   DRINKS_NUM           4
#define   PIXEL_IN_STICK       8
#define   PIXEL_IN_DRINK       PIXEL_IN_STICK
#define   PIXEL_IN_VOLUME      (2 * PIXEL_IN_STICK)
#define   PIXEL_NUM            (DRINKS_NUM * PIXEL_IN_DRINK + PIXEL_IN_VOLUME)

#define   VOLUME_START_PIXEL        0
#define   DRINKS_START_PIXEL        (VOLUME_START_PIXEL + PIXEL_IN_VOLUME)
#define   DRINK_START_PIXEL(DRINK)  (DRINKS_START_PIXEL + DRINK * PIXEL_IN_DRINK)

#define   BACKGROUND_COLOUR      ((uint32_t) 0x000001)
#define   SHADOW_1_COLOUR        ((uint32_t) 0x000100)
#define   SHADOW_2_COLOUR        ((uint32_t) 0x000100)
#define   SHADOW_3_COLOUR        ((uint32_t) 0x000100)
#define   SHADOW_4_COLOUR        ((uint32_t) 0x000100)
#define   PROCESS_1_COLOUR       ((uint32_t) 0xFF0000)
#define   PROCESS_2_COLOUR       ((uint32_t) 0x0100ff)
#define   PROCESS_3_COLOUR       ((uint32_t) 0x111100)
#define   PROCESS_4_COLOUR       ((uint32_t) 0xFF00FF)
#define   VOLUME_PROCESS_COLOUR  ((uint32_t) 0x888888)



#define   DataThreshold        ((uint16_t) (1024/PIXEL_IN_DRINK))
#define   DataThresholdVol     ((uint16_t) (1024/PIXEL_IN_VOLUME))

#define   PROCESS              (1 << 0)

#define   mlToTimeCoef         10 //время на подачу 1 мл жидксти, мс
#define   MIN_VOLUME           1 //минимальный учитываемый объем, мл
#define   MAX_VOLUME           ((uint32_t) 750) //мл
#define   mlForLED             ((float)((float)MAX_VOLUME / (float)PIXEL_IN_VOLUME))

#define   PREPROCESS_DELAY     ((uint32_t) 2000) //мс
#define   PUMP_POWER           ((uint16_t) 255) //от 0 (выключен) до 255 (максимум)


#define   WaitShowDelay        ((uint16_t) 300) //2 * WaitShowDelay - период моргания ожидающей жидкости
#define   WaitCycle            3 //циклы ожидания время ожидания = WaitCycle * 2 * WaitShowDelay
#define   EndDelay            ((uint16_t) 2500) //пауза в конце

typedef struct
{
    uint8_t  VolPin;
    uint8_t  ActPin;
    uint16_t Volume; //показания АЦП
    uint32_t ProcColour;
    uint32_t ShadColour;
    float    mlVol; //мл в зависимости от общего объема и доли напитка
    uint8_t  LEDsNum;
    uint8_t  LEDsPos;

}  DrinkType;

Adafruit_NeoPixel LEDS = Adafruit_NeoPixel(PIXEL_NUM, LEDS_PIN, NEO_GRB + NEO_KHZ800);
uint8_t State = 0;
uint16_t TotalVolume = 0;
float TotalVolml = 0;
uint8_t TotVolActLEDs = 0;
bool ColourMix = false;
uint32_t NewColour = 0;
bool SystemChange = false;


DrinkType Drinks[DRINKS_NUM];

void setup()
{
    digitalWrite(START_PIN, HIGH);

    uint8_t VolPINs[DRINKS_NUM] = {VOLUME_1_PIN, VOLUME_2_PIN, VOLUME_3_PIN, VOLUME_4_PIN};
    uint8_t ActPINs[DRINKS_NUM] = {ACT_1_PIN, ACT_2_PIN, ACT_3_PIN, ACT_4_PIN};
    uint32_t ProcCOLOURs[DRINKS_NUM] = {PROCESS_1_COLOUR, PROCESS_2_COLOUR, PROCESS_3_COLOUR, PROCESS_4_COLOUR};
    uint32_t ShadCOLOURs[DRINKS_NUM] = {SHADOW_1_COLOUR, SHADOW_2_COLOUR, SHADOW_3_COLOUR, SHADOW_4_COLOUR};  

    for(uint8_t i = 0; i < DRINKS_NUM; i++)
    {
        Drinks[i].VolPin = VolPINs[i];
        Drinks[i].ActPin = ActPINs[i];
        pinMode(Drinks[i].ActPin, OUTPUT);
        digitalWrite(Drinks[i].ActPin, LOW);
        Drinks[i].Volume = 0;
        Drinks[i].ProcColour = ProcCOLOURs[i];
        Drinks[i].ShadColour = ShadCOLOURs[i];
        Drinks[i].LEDsPos = 0;
    }

    PCintPort::attachInterrupt(START_PIN, &START_ISR, FALLING);

    LEDS.begin();
    for(byte i=0; i < PIXEL_NUM; i++)
        LEDS.setPixelColor(i, BACKGROUND_COLOUR);
    LEDS.show();
}
//----------------------------------------
void loop()
{
    if ((State & PROCESS) != PROCESS)
    {
        uint16_t Data;

        for(uint8_t cup = 0; cup < DRINKS_NUM; cup++)
        {
            Data = analogRead(Drinks[cup].VolPin);

            if (abs(Data - Drinks[cup].Volume) >= DataThreshold)
            {
                Drinks[cup].Volume = Data;

                if (Drinks[cup].Volume > 975)
                    Drinks[cup].Volume = 1024;

                uint8_t StartPixel = DRINK_START_PIXEL(cup);

                for(byte i = StartPixel; i < (StartPixel + PIXEL_IN_DRINK); i++)
                    LEDS.setPixelColor(i, Drinks[cup].ShadColour);
                for(byte i = StartPixel; i < (StartPixel + (Drinks[cup].Volume / DataThreshold)); i++)
                    LEDS.setPixelColor(i, Drinks[cup].ProcColour);

                SystemChange = true;
            }
        }

        Data = analogRead(TOTAL_VOLUME_PIN);

        if (abs(Data - TotalVolume) >= DataThresholdVol)
        {
            TotalVolume = Data;

            if (TotalVolume > 975)
                TotalVolume = 1024;

            TotVolActLEDs = TotalVolume / DataThresholdVol;

            for(byte i = VOLUME_START_PIXEL; i < (VOLUME_START_PIXEL + PIXEL_IN_VOLUME); i++)
                LEDS.setPixelColor(i, BACKGROUND_COLOUR);

            SystemChange = true;
        }

        if (SystemChange)
        {
            TotalVolml = (float)((TotalVolume * MAX_VOLUME) / 1024);

            uint16_t MinVol = 1025;
            for(uint8_t cup = 0; cup < DRINKS_NUM; cup++)
            {
                if ((Drinks[cup].Volume < MinVol) && (Drinks[cup].Volume != 0))
                    MinVol = Drinks[cup].Volume;
            }

            float OnePartVol = 0;
            for(uint8_t cup = 0; cup < DRINKS_NUM; cup++)
            {
                Drinks[cup].mlVol = (float)Drinks[cup].Volume / (float)MinVol;
                OnePartVol += Drinks[cup].mlVol;
            }

            OnePartVol = TotalVolml / OnePartVol;
            for(uint8_t cup = 0; cup < DRINKS_NUM; cup++)
            {
                Drinks[cup].mlVol *= OnePartVol;
                Drinks[cup].LEDsNum = Drinks[cup].mlVol / mlForLED;
                if ((Drinks[cup].mlVol > 0) && (Drinks[cup].LEDsNum < 1))
                    Drinks[cup].LEDsNum = 1;
            }

            uint8_t LEDsSum = 0;
            for(uint8_t cup = 0; cup < DRINKS_NUM; cup++)
                LEDsSum += Drinks[cup].LEDsNum;

            if ((LEDsSum > 0) && (LEDsSum <= TotVolActLEDs))
            {
                uint8_t LedsNumMAX = Drinks[0].LEDsNum;
                uint8_t LedsNumMAXPos = 0;
                for(uint8_t cup = 1; cup < DRINKS_NUM; cup++)
                {
                    if (Drinks[cup].LEDsNum > LedsNumMAX)
                    {
                        LedsNumMAX = Drinks[cup].LEDsNum;
                        LedsNumMAXPos = cup;
                    }
                }

                Drinks[LedsNumMAXPos].LEDsNum += TotVolActLEDs - LEDsSum;

                Drinks[0].LEDsPos = VOLUME_START_PIXEL;
                for(uint8_t cup = 1; cup < DRINKS_NUM; cup++)
                    Drinks[cup].LEDsPos = Drinks[cup - 1].LEDsPos + Drinks[cup - 1].LEDsNum;

                ColourMix = false;
            }

            else if (LEDsSum > TotVolActLEDs)
            {
                for(uint8_t cup = 0; cup < DRINKS_NUM; cup++)
                {
                    Drinks[cup].LEDsNum = TotVolActLEDs;
                    Drinks[cup].LEDsPos = VOLUME_START_PIXEL;
                }

                ColourMix = true;

                NewColour = 0;
                for(uint8_t cup = 0; cup < DRINKS_NUM; cup++)
                    NewColour |= Drinks[cup].ProcColour;
            }

            bool EmptyCup = true;
            for(uint8_t cup = 0; cup < DRINKS_NUM; cup++)
            {
                if (Drinks[cup].LEDsNum != 0)
                {
                    EmptyCup = false;
                    break;
                } 
            }

            if (EmptyCup)
            {
                for(byte i = VOLUME_START_PIXEL; i < (VOLUME_START_PIXEL + TotVolActLEDs); i++)
                    LEDS.setPixelColor(i, VOLUME_PROCESS_COLOUR);
            }

            else
            {
                if (ColourMix)
                {
                    for(byte i = VOLUME_START_PIXEL; i < (VOLUME_START_PIXEL + TotVolActLEDs); i++)
                        LEDS.setPixelColor(i, NewColour);
                }

                else
                {
                    for(uint8_t cup = 0; cup < DRINKS_NUM; cup++)
                    {
                        if (Drinks[cup].LEDsNum != 0)
                        {
                            for(byte i = Drinks[cup].LEDsPos; i < (Drinks[cup].LEDsPos + Drinks[cup].LEDsNum); i++)
                                LEDS.setPixelColor(i, Drinks[cup].ProcColour);
                        }
                    }
                }
            }
            SystemChange = false;
        }

        LEDS.show();
    }

    else
    {
        uint8_t LEDSPos[DRINKS_NUM];
        uint8_t LEDSNum[DRINKS_NUM];

        for(uint8_t cup = 0; cup < DRINKS_NUM; cup++)
        {
            LEDSPos[cup] = Drinks[cup].LEDsPos;
            LEDSNum[cup] = Drinks[cup].LEDsNum;
        }

        for(uint8_t cup = 0; cup < DRINKS_NUM; cup++)
        {
            if (Drinks[cup].LEDsNum != 0)
            {
                float Volume = Drinks[cup].mlVol;
                uint16_t VolCnt = 0;
                uint16_t mlPerLEDCoef = Volume / LEDSNum[cup];

                analogWrite(Drinks[cup].ActPin, PUMP_POWER);//вкл
                delay(PREPROCESS_DELAY);

                while (Volume >= MIN_VOLUME)
                {
                    delay(mlToTimeCoef * MIN_VOLUME);
                    Volume -= MIN_VOLUME;

                    VolCnt += MIN_VOLUME;
                    {
                        if (VolCnt >= mlPerLEDCoef)
                        {

                            if (ColourMix)
                            {

                            }

                            else
                            {

                                if (LEDSNum[cup] != 0)
                                {

                                    for(byte i = VOLUME_START_PIXEL; i < (VOLUME_START_PIXEL + PIXEL_IN_VOLUME); i++)
                                        LEDS.setPixelColor(i, BACKGROUND_COLOUR);

                                    if (LEDSNum[cup] > 0)
                                        LEDSNum[cup]--;
                                    for(uint8_t i = 0; i < DRINKS_NUM; i++)
                                    {
                                        if (LEDSPos[i] > 0)
                                            LEDSPos[i]--;
                                    }

                                    for(uint8_t i = 0; i < DRINKS_NUM; i++)
                                    {
                                        if (Drinks[i].LEDsNum != 0)
                                        {
                                            for(byte j = LEDSPos[i]; j < (LEDSPos[i] + LEDSNum[i]); j++)
                                                LEDS.setPixelColor(j, Drinks[i].ProcColour);
                                        }
                                    }
                                }
                            }

                            LEDS.show();
                            VolCnt = 0;
                        }
                    }
                }

                analogWrite(Drinks[cup].ActPin, 0);//выкл

                if (cup < (DRINKS_NUM - 1))
                {
                    uint8_t StickNum = cup + 1;
                    uint8_t StartPixel = DRINK_START_PIXEL(StickNum);
                    for (uint8_t BlinkTime = 0; BlinkTime < WaitCycle; BlinkTime++)
                    {
                        for(byte i = StartPixel; i < (StartPixel + PIXEL_IN_DRINK); i++)
                            LEDS.setPixelColor(i, Drinks[cup+1].ShadColour);
                        LEDS.show();
                        delay(WaitShowDelay);

                        for(byte i = StartPixel; i < (StartPixel + (Drinks[cup+1].Volume / DataThreshold)); i++)
                            LEDS.setPixelColor(i, Drinks[cup+1].ProcColour);
                        LEDS.show();
                        delay(WaitShowDelay);
                    }
                }

            }
        }

        delay(EndDelay);

        if (ColourMix)
        {
            for(byte i = VOLUME_START_PIXEL; i < (VOLUME_START_PIXEL + TotVolActLEDs); i++)
                LEDS.setPixelColor(i, NewColour);
        }

        else
        {
            for(uint8_t cup = 0; cup < DRINKS_NUM; cup++)
            {
                if (Drinks[cup].LEDsNum != 0)
                {
                    for(byte i = Drinks[cup].LEDsPos; i < (Drinks[cup].LEDsPos + Drinks[cup].LEDsNum); i++)
                        LEDS.setPixelColor(i, Drinks[cup].ProcColour);
                }
            }
        }

        State &= ~PROCESS;
    }
}
//----------------------------------------
void START_ISR()
{
    State |= PROCESS;
}