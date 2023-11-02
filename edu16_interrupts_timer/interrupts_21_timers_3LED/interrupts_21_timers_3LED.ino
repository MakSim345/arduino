volatile bool timerLED1On;    //переменная вкл/выкл таймера
bool LED1On;        //переменная для хранения состояния светодиода
volatile unsigned long int timerLED1, timerLED1Loop; //переменные подсчета мс и
                                                     //хранения значения таймера
volatile bool timerLED2On;
bool LED2On;
volatile unsigned long int timerLED2, timerLED2Loop;

volatile bool timerLED3On;
bool LED3On;
volatile unsigned long int timerLED3, timerLED3Loop;

ISR (TIMER0_COMPA_vect) //функция, вызываемая таймером-счетчиком каждые 0,001 сек.
{
  if(timerLED1On)       //если включен миллисекудный таймер для LED1
    timerLED1++;        //инкремент переменной таймера (+1)

  if(timerLED2On)       //если включен миллисекудный таймер для LED2
    timerLED2++;        //инкремент переменной таймера (+1)

  if(timerLED3On)       //если включен миллисекудный таймер для LED3
    timerLED3++;        //инкремент переменной таймера (+1)
}

void setup()
{
  pinMode(7, OUTPUT);   //настроить 7-й порт как выход
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);

/**** настройка прерывания по таймеру каждую 0,001 сек (вызов функции ISR (TIMER0_COMPA_vect)) ****/
  TCCR0A |= (1 << WGM01);              //сброс при совпадении
  OCR0A = 0xF9;                        //начало отсчета до переполнения (249)
  TIMSK0 |= (1 << OCIE0A);             //разрешить прерывание при совпадении с регистром А
  TCCR0B |= (1 << CS01) | (1 << CS00); //установить делитель частоты на 64
  sei();                               //разрешить прерывания

}

void loop()
{
  LED1(7, 1000, 1);   //функция мигания первого светодида (№ пина, длительность интервала, вкл/выкл)
  LED2(6, 500);       //функция мигания второго светодида (№ пина, длительность интервала)
  LED3(5, 100);       //функция мигания третьего светодида (№ пина, длительность интервала)
}

void LED1(byte pin, long int interval, bool state)  //функция мигания первого светодиода (пин, инт-л, состояние)
{
  if (timerLED1On == 0 && state == 1) //если таймер не был запущен и разрешена работа таймера
  {
    timerLED1On = 1;    //запустить таймер
  }
  if(timerLED1On == 1 && state == 0)  //если таймер был запущен и запрещена работа таймера
  {
    cli();            //остановить прерывания
    timerLED1On = 0;  //запретить пополнение переменной таймера
    timerLED1 = 0;    //обнулить переменную таймера
    sei();            //разрешить прерывания
    digitalWrite(pin, LOW);   //выключить светодиод
    LED1On = 0;       //установить флаг выключения светодиода
  }

  if(state == 1)  //если разрешена работа таймера
  {
    cli();  //остановить прерывания
    timerLED1Loop = timerLED1;  //сохранить значение переменной таймера
    sei();  //разрешить прерывания
    if(timerLED1Loop >= interval) //сравнить значение таймера с заданным интервалом
    { //если значение превысило интервал
      cli();
      timerLED1 = 0;  //обнулить таймер
      sei();
      if(LED1On == 0) //если светодиод был выключен
      {
        LED1On = 1;   //поставить флаг включения
        digitalWrite(pin, LED1On);   //включить светодиод
      }
      else  //если светодиод был включен
      {
        LED1On = 0;   //поставить флаг выключения
        digitalWrite(pin, LED1On);  //выключить светодиод
      }
    }
  }
}

void LED2(byte pin, long int interval)
{
  if (timerLED2On == 0)
  {
    timerLED2On = 1;
  }

  cli();
  timerLED2Loop = timerLED2;
  sei();
  if(timerLED2Loop >= interval)
  {
    cli();
    timerLED2 = 0;
    sei();
    if(LED2On == 0)
    {
      LED2On = 1;
      digitalWrite(pin, LED2On);
    }
    else
    {
      LED2On = 0;
      digitalWrite(pin, LED2On);
    }
  }
}


void LED3(byte pin, long int interval)
{
  if (timerLED3On == 0)
  {
    timerLED3On = 1;
  }

  cli();
  timerLED3Loop = timerLED3;
  sei();
  if(timerLED3Loop >= interval)
  {
    cli();
    timerLED3 = 0;
    sei();
    if(LED3On == 0)
    {
      LED3On = 1;
      digitalWrite(pin, LED3On);
    }
    else
    {
      LED3On = 0;
      digitalWrite(pin, LED3On);
    }
  }
}
