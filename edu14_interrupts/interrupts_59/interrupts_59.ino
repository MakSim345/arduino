/*
  Прерывание с подавлением дребезга кнопки

  Схема:
  - LED, подключенный от пин-13 светодиод L на плате ARDUINO
  - кнопки, подключенные от пин-2 и пин-3 на GND

  Код для функции обработки прерываня пишем минимально минимальный.
    
  14.08.2021
*/

#define BUTTON_0 2 // объвляем пин для первой кнопки (прерывание INT0)
#define BUTTON_1 3 // объвляем пин для второй кнопки (прерывание INT1)
volatile uint32_t lastDebounce_INTERRUPTING;  // последнее время сигнала с кнопки
volatile word debounceDelay_INTERRUPTING = 5; //100? время на дребезг контактов кнопки 
volatile word counter_INTERRUPTING;           // переменная для счетчика
// ----- для кода(действий) по прерыванию
#define LED 13 // пин для светодиода (он же L на плате ARDUINO)

const int debounce_delay = 10; //100

volatile byte state = LOW;

void setup() 
{
  // pinMode(BUTTON_0, INPUT_PULLUP); // настраиваем пин кнопки как вход с подтяжкой
  // pinMode(BUTTON_1, INPUT_PULLUP); // настраиваем пин кнопки как вход с подтяжкой

  pinMode(BUTTON_0, INPUT); //
  pinMode(BUTTON_1, INPUT); //

  // attachInterrupt(0, ButtonINTERRUPTING, LOW); // активируем прерывание на кнопку
  // attachInterrupt(1, ButtonINTERRUPTING, LOW); // активируем прерывание на кнопку
                                              // для кода(действий) по прерыванию

  attachInterrupt(1, myISR_Blink, LOW);


  pinMode(LED_BUILTIN, OUTPUT); // настраиваем пин-13, как выход
  Serial.begin(9600);
  while (!Serial); //: wait for serial monitor
  Serial.println("\nTRACE output: START");

}
void loop() 
{
  // ----- main loop, ruled by interrupt, state volatile variable:
  digitalWrite(LED_BUILTIN, state); // LED on-off from ISR
  // -----
}


void myISR_Blink()
{
    static unsigned long millis_prev;
    if (millis() - debounce_delay > millis_prev)
    {
        state = !state;   // reverse
    }

    millis_prev = millis();
}

void ButtonINTERRUPTING()
{  

  // функция обработки прерывания кнопок (c фильтрацией дребезга)
  
  if ( lastDebounce_INTERRUPTING < millis() )
  { 
    // если это новое прерывание(дребезг)...
    lastDebounce_INTERRUPTING = millis();     // засекаем время
    counter_INTERRUPTING = 0;                // обнуляем счетчик
  }

  counter_INTERRUPTING++; // считаем цикл
  Serial.println("Button INTERRUPT - On");
  Serial.println(counter_INTERRUPTING);

  // if (true)
  if ( counter_INTERRUPTING > debounceDelay_INTERRUPTING )
  { 
    Serial.println("TRUE!");
    // если это не дребезг и...
    if ( digitalRead(BUTTON_0) ) 
    { 
      // если нажата первая кнопка...
      // ----- код(действие) по прерыванию от первой кнопки
      // digitalWrite(LED_BUILTIN, HIGH); // включаем LED
    }
    if ( digitalRead(BUTTON_1) )  // Pin "D3" on NANO
    { 
      // если нажата вторая кнопка...
      // ----- код(действие) по прерыванию от второй кнопки
      // digitalWrite(LED_BUILTIN, HIGH); // выключаем LED
      state = !state;
    }

    // digitalWrite(LED_BUILTIN, state); // moved to main loop.
  }
}
