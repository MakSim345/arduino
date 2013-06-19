int DS_Pin = 11;   //pin 14 DS/SDI on the 75HC595 ��������� �������
int ST_Pin = 8;    //pin 12 ST_CP/RCLK on the 75HC595 ��������� �������
int SH_Pin = 12;   //pin 11 SH_CP/SRCLK on the 75HC595 ��������� �������
const int PIR1 = 5;//    ������ ��-������
const int PIR2 = 6;//    ������ ��-������
int sensorPin = A0;//    ��������������
//    ***��������***
int OverTime =15000; // ����� ���������� � ������ ���� (��)
const int N=14;      //���-�� ��������
int Qck_tm = 300;    // ����� ��������� ����� ���������
int Wt_tm = 2000;    // ����� �������� �� ������� ����������
int L=30;            // �������� ��������� ������������ 

//    ***��������***
int a1,a2;   // ���������� ��������� ��������������� ���������
long pM=0;   //���������� �������� millis()
long pM1=0;  //���������� �������� millis()
long pM10=0; //���������� �������� millis()
unsigned int sensorValue = 0;  // �������� �������� �������������
int registers[N+1]; // ������ ��������
boolean b2=false;   //������������ ���� �� �������� ��������
boolean b3=false;   // ���� ���� ���������� �� ����� ����������


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

 // �������� ��� ���������
void clearRegisters()
{
    for(int i = N; i >=  0; i--)
    {
        registers[i] = LOW;
    }
}

//�������� ���������
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

// ���������� �������������� �������� ��������
void setRegister(int index, int value)
{
    registers[index] = value;    
}

//��������� ��������� ��������
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

//��������� ���������� �������� 
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
    sensorValue = analogRead(sensorPin);  // ��������� �������� � �������������
    Serial.println(sensorValue, DEC); 
    if (sensorValue < L)
    {
        a1=digitalRead(PIR1);
        if ( a1 == HIGH && a2== LOW)// ���� ����� ������ ������
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

