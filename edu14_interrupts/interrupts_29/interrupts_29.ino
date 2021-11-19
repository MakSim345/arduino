//
// Кодировка русских букв UTF-8
//
// Скетч примера для статьи Использование прерываний Arduino.
// http://sites.google.com/site/vanyambauseslinux/electronics/ispolzovanie-preryvanij-arduino 
//
// (D) Developed by Andrey Sharoyko <vanyamboe@gmail.com>, 2011.

#define LEDPIN 13        // Вывод светодиода
#define BTNPIN 2         // Вывод кнопки

volatile int count = 0;  // Переменная счётчика (volatile означает указание компилятору не оптимизировать код её чтения,
                         // поскольку её значение изменяется внутри обработчика прерывания)

ISR(INT0_vect)
{
   count = 25;           // Инициализировать счётчик
}

// Режимы вызова прерывания INT0
#define INT0_SENSE_LOW_LEVEL     0        // Прерывание при низком уровне на выводе
#define INT0_SENSE_LEVEL_CHANGE  1        // Прерывание при изменении уровня
#define INT0_SENSE_FALLING_EDGE  2        // Прерывание по фронту на спад (когда 1 переходит в 0)
#define INT0_SENSE_RISING_EDGE   3        // Прерывание по фронту на подъём (когда 0 переходит в 1)

// Управляющая функция для прерывания INT0
//    mode    - режим вызова прерывания
//    enable  - разрешить/запретить прерывание
void int0Control (uint8_t mode, bool enable)
{
    EIMSK &= ~ (1 << INT0);                    // Запретить прерывание (так как следующая команда устанавливает режим INT0_SENSE_LOW_LEVEL)
    EICRA &= ~ (1 << ISC00) | (1 << ISC01);    // Обнуляем биты ISC00 и ISC01 в регистре EICRA
    EICRA |= mode;                             // Устанавливаем режим вызова прерывания INT0
    if (enable)
      EIMSK |= (1 << INT0);                    // Разрешить прерывание
}

void setup()
{
    pinMode(LEDPIN, OUTPUT);                      // Вывод светодиода в режим вывода
    pinMode(BTNPIN, INPUT);                       // Вывод кнопки в режим ввода
    int0Control(INT0_SENSE_RISING_EDGE, true);    // Разрешить прерывание по фронту на подъём (в данном случае при нажатии на кнопку)
    interrupts();                                 // Разрешить прерывания глобально
}

void loop()
{
  if(count==0) 
  {                             
    digitalWrite(LEDPIN, LOW);        // Выключить светодиод, если счётчик равен 0...
  }
  else 
  {
    digitalWrite(LEDPIN, HIGH);       // ... иначе включить светодиод,
     --count;                         // и уменьшить счётчик на 1.
  }
  delay(10);                          // Подумать 10 милисекунд.
}

