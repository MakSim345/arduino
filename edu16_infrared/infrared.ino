int DS_Pin = 11;   //pin 14 DS/SDI on the 75HC595 сдвиговый регистр
int ST_Pin = 8;    //pin 12 ST_CP/RCLK on the 75HC595 сдвиговый регистр
int SH_Pin = 12;   //pin 11 SH_CP/SRCLK on the 75HC595 сдвиговый регистр
const int PIR1 = 5;//    Первый ИК-датчик
const int PIR2 = 6;//    Второй ИК-датчик
int sensorPin = A0;//    Фототранзистор
//    ***ПАРМЕТРЫ***
int OverTime =15000; // Время отключения в случае сбоя (мс)
const int N=14;      //Кол-во ступенек
int Qck_tm = 300;    // Время включения одной ступеньки
int Wt_tm = 2000;    // Время ожидания до полного выключения
int L=30;            // Значение пороговой освещенности 

//    ***ЗНАЧЕНИЯ***
int a1,a2;   // Расстояние считанное ультразвуковыми датчиками
long pM=0;   //Предыдущее значение millis()
long pM1=0;  //Предыдущее значение millis()
long pM10=0; //Предыдущее значение millis()
unsigned int sensorValue = 0;  // цифровое значение фоторезистора
int registers[N+1]; // Массив ступенек
boolean b2=false;   //Отслеживание была ли включена лестница
boolean b3=false;   // Если было прерывание во время выключения


void setup()
{ 
    pinMode(PIR1, INPUT); 
    pinMode(PIR2, INPUT); 
    pinMode(DS_Pin, OUTPUT);
    pinMode(ST_Pin, OUTPUT);
    pinMode(SH_Pin, OUTPUT);
    Serial.begin(9600);  
    clearRegisters();
    writeRegisters();
}

 // Погасить все ступеньки
void clearRegisters()
{
    for(int i = N; i >=  0; i--)
    {
        registers[i] = LOW;
    }
}

//Включаем ступеньки
void writeRegisters()
{
    digitalWrite(ST_Pin, LOW);
    for(int i = N; i >=  0; i--)
    {
        digitalWrite(SH_Pin, LOW);
       
        int val = registers[i];
       
        digitalWrite(DS_Pin, val);
        digitalWrite(SH_Pin, HIGH);
    }
    digitalWrite(ST_Pin, HIGH);
}

// Выставляем индивидуальные значения ступенек
void setRegister(int index, int value)
{
    registers[index] = value;    
}

//Процедура включения лестницы
void LightON(boolean a)
{
    b2=true;
    if(a)
        for (int k=0;k<=N;)
            {  
                a1=digitalRead(PIR1);        
                if ( a1==HIGH && millis() - pM1 > 1000)
                 {                    
                     pM1 = millis();
                 }
                if (millis() - pM > Qck_tm)
                 {
                     pM = millis();
                     setRegister(k, HIGH);     
                     writeRegisters();
                     k++; 
                 }   
            }
    else
        for (int k=N;k>=0;)
            {
              a2=digitalRead(PIR2);
             if ( a2 == HIGH && millis() - pM1 > 1000)
             {         
               pM1 = millis();
              } 
                if (millis() - pM > Qck_tm)
                {
                  pM = millis();
                  setRegister(k, HIGH);     
                  writeRegisters();
                  k--; 
                }   
            }
}

//Процедура выключения лестницы 
void LightOFF(boolean a)
{
    b2=false;
    if(a)
    {
        int k=0;
        while(k<=N)
        { 
            a1=digitalRead(PIR1);
            if (a1== HIGH)
            {
                LightON(true);
                break;
            }
            if (millis() - pM > Qck_tm)
            {
                pM = millis();  
                setRegister(k, LOW);     
                writeRegisters();  
                k++;          
            }
        }
    }
  else 
       {
        int k=N;
        while(k>=0)
        { 
            a2=digitalRead(PIR2);
            if (a2 == HIGH)
            {
                LightON(false);
                break;
           }
            if (millis() - pM > Qck_tm)
            {
                pM = millis();  
                setRegister(k, LOW);     
                writeRegisters();  
                k--;          
            }
        }
        }
}

void loop()
{
    sensorValue = analogRead(sensorPin);  // считываем значение с фоторезистора
    Serial.println(sensorValue, DEC); 
    if (sensorValue < L)
    {
        a1=digitalRead(PIR1);
        if ( a1 == HIGH && a2== LOW)// Если задет первый датчик
        { 
            pM=millis();
            LightON(true);
            while (b2)
            { 
                a2=digitalRead(PIR2);        
                if (a2 == HIGH && millis() - pM10 > 1000 || (millis() - pM > OverTime))
                {   
                    pM10 = millis();
                    LightOFF(true);
                }
            }
        }

    a2=digitalRead(PIR2);  
    if (a2 == HIGH && a1 == LOW)
    { 
        pM=millis();
        LightON(false);        
        while (b2)
        {   
            a1=digitalRead(PIR1); 
            if (a1== HIGH && millis() - pM10 > 1000 || (millis() - pM > OverTime))
            {   
                pM10 = millis();
                LightOFF(false);                     
            }
        }
    }
 }
}

