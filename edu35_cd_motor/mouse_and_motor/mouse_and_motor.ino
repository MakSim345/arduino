#include <AccelStepper.h>
#include <SoftwareSerial.h>

#define HALFSTEP 8

// Определение пинов для управления двигателем
#define motorPin1  2 // IN1 на 1-м драйвере ULN2003
#define motorPin2  3 // IN2 на 1-м драйвере ULN2003
#define motorPin3  4 // IN3 на 1-м драйвере ULN2003
#define motorPin4  5 // IN4 на 1-м драйвере ULN2003

// Инициализируемся с последовательностью выводов IN1-IN3-IN2-IN4
// для использования AccelStepper с 28BYJ-48
AccelStepper stepper1(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);

String str = "START";

const int buttonPin = 12;     // Номер пина кнопки

const int minMaxSpeed = 800;
const int maxMaxSpeed = 1300;
const int minAcceleration = 500;
const int maxAcceleration = 700;
const int minSpeed = 700;
const int maxSpeed = 1000;
const int move = 600; // примерная длина одного перемещения
const int minPause = 1; // Минимальная пауза между группами прокруток (секунды)
const int maxPause = 3*60; // Максимальная пауза между группами прокруток (секунды)

long pos = 0;
int smove = move;
int sMaxSpeed = maxMaxSpeed;
int sAcceleration = maxAcceleration;
int sSpeed = maxSpeed;
unsigned msec = 1000;
bool PREV = false;
int dir = 50;
int count = 5;


void setup()
{
  pinMode(LED_BUILTIN, OUTPUT); // Светодиод (красный) для сигнализации (светит - стоп, не светит - работа)

  Serial.begin(9600);
  Serial.println("start");

  stepper1.setMaxSpeed(sMaxSpeed);
  stepper1.setAcceleration(sAcceleration);
  stepper1.setSpeed(sSpeed);

  pinMode(buttonPin, INPUT_PULLUP); // Настройка кнопки
  stepper1.move(move);
}

int inputHandler()
{ // Чтение команд из порта и отслеживание нажатия кнопки
    int res = 0;
    if (Serial.available() > 0)
    {
      str=Serial.readString();
      str.trim();
      Serial.println(">>>>>>>>>>>>>>>>>");
      Serial.println(str);
      res = 1;
    }

    buttonState = digitalRead(buttonPin);
    if (buttonState == LOW && PREV == HIGH )
    {
        if (str=="START")
        {
          str = "STOP";
          Serial.println("Press button to 'STOP'");
          stepper1.move(0);
          res = 1;
        }
        else
        {
          str = "START";
          Serial.println("Press button to 'START'");
          res = 1;
        }
        delay(150);
    }
    if (str!="START")
    {
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else
    {
      digitalWrite(LED_BUILTIN, LOW);
    }
    PREV = buttonState;
    return res;
}


int delayWithRead(unsigned ms)
{
    // Пауза с обработкой ввода
    Serial.print("Delay = ");
    Serial.print(ms);
    Serial.println(" msec.");
    int time = 0;
    while (time<ms)
    {
        delay(100);
        if (inputHandler() == 1)
        {
            Serial.println("Press Key");
        delay(2000);
        return 1;
        }
        time += 100;
    }
    return 0;
}

void printData()
{
    // Вывод данных в порт
    Serial.print("sMaxSpeed=");
    Serial.println(sMaxSpeed);

    Serial.print("sAcceleration=");
    Serial.println(sAcceleration);

    Serial.print("sSpeed=");
    Serial.println(sSpeed);

    Serial.print("count=");
    Serial.println(count);

    Serial.print("pos=");
    Serial.println(pos);

    Serial.print("smove=");
    Serial.println(smove);
}

void goNewPosition()
{
    // Обработик перехода на следующую позицию
    sMaxSpeed = random(minMaxSpeed, maxMaxSpeed);
    sAcceleration = random(minAcceleration, maxAcceleration);
    sSpeed = random(minSpeed, maxSpeed);
    stepper1.setMaxSpeed(sMaxSpeed);
    stepper1.setAcceleration(sAcceleration);
    stepper1.setSpeed(sSpeed);
    msec = random(100,500); // Пауза между скроллами в одной группе
    delayWithRead(msec); // Задержка с обработкой ввода
    count--;
    if (count<0)
    {
      Serial.println("*************************************");// Разделитель групп скроллов
      msec = random(minPause*10,maxPause*10)*100; // Пауза между группами скроллов
      delayWithRead(msec);
      dir = random(0,100);
      count = random(1,5);
    }
    else
    {
      Serial.println("_________________________________________");// Разделитель скроллов в одной группе
    }

    if (dir<35)
    { // Скролл назад с вероятностью 35%
      smove = 0 - move + (random(move/8) - move/16);
    }
    else
    {
      smove = 0 + move + (random(move/8) - move/16);
    }
    pos = pos + smove; // Счетчик общего скролла

    printData();// Вывод данных в порт

    if (pos>100)
    { // Крутить только если чуть ниже стартовой позиции
      stepper1.move(smove); // Задать поворот относительно текущей позиции
    }
}

void loop()
{
    inputHandler(); // Обработка ввода

    if(stepper1.distanceToGo()==0)
    {
        // Если поворот закончили
        if (str=="START")
        {
            goNewPosition(); // Получаем следующую позицию
        }
    }

    if (str == "START")
    {
        // Если нет команды стоять то
        stepper1.run();  // Крутим движок
    }
}

