include <Ethernet.h>
// Библиотеки ниже нет в стандартной поставке среды разработки Arduino.
// придётся её скопировать.
include <DallasTemperature.h>

// MAC-адрес нашего устройства
byte mac[] = { 0x00, 0x3A, 0xF1, 0x19, 0x69, 0xFC };
// ip-адрес устройства
byte ip[] = { 192, 168, 1, 156 };
// ip-адрес удалённого сервера
byte server[] = { 79, 140, 28, 20 }; // измените на свой
char temp[6];
byte isdata=0;

Client client(server, 80); // 80-порт.
DallasTemperature tempSensor;

/*
Сборка устройства:

первую «ногу» датчика цепляем на «минус» GND
вторую «ногу» (DQ) на 7-й пин
третью на «плюс»
вторую и третью нужно соединить резистором на ~ 4,7 К. Но я заменил резистор на светодиод и получил индикатор обращения к шине датчика (ВНИМАНИЕ! Без резистора или светодиода работать ничего не будет. Не забудьте!)

*/

void setup()
{
 Ethernet.begin(mac, ip); // Инициализируем Ethernet Shield
 tempSensor.begin(7); // Датчик температуры на 7-й пин
 Serial.begin(9600); // Скорость консольного порта 9600 (пригодится для отладки)
}

void loop()
{
  delay(3000); // задержка в 3 сек.
  // Соединяемся
  if (client.connect()) {
  Serial.println("connecting..."); // Serial.println для отладки. Лучше его оставить, на всякий случай, потом будет легче понять, в чём проблема.
  // Обработчик ошибок датчика
  switch(tempSensor.isValid())
   {
     case 1:
       Serial.println("Invalid CRC"); // ошибка контрольной суммы
       tempSensor.reset(); // сбросить девайс
       return;
     case 2:
       Serial.println("Invalid device"); // какой-то "левый" датчик :)
       tempSensor.reset(); // сбросить девайс
       return;
   }
 
   Serial.println("connected");
   char buf[80];
   float f=tempSensor.getTemperature(); // получаем температуру

   Serial.println(tempSensor.getTemperature());
  
   // Ниже извращения с отделением дробной части и целой. Почему-то Arduino не хочет работать с float.
   // Вместо числа вставляет вопросик. Наверное, виной тому отсутствие аппаратной поддержки работы с
   // числами с плавающей запятой в Arduino. Буду рад увидеть более красивое решение в комментариях.
   int temp1 = (f - (int)f) * 100; // выделяем дробную часть
   // Составляем GET запрос. Переменная code нужна для того, чтобы вражеский термометр не слал какие попало значения.
   // проверяется на стороне Web-сервера.
   sprintf(buf, "GET /class/backend/meteo.php?temp=%0d.%d&code=123456 HTTP/1.0", (int)f, abs(temp1));
  
   Serial.println(buf);
   client.println(buf); // Отправляем GET запрос
   client.println("Host: opck.info"); // Указываем, какой конкретно host на данном ip нас интересует.
   client.println();
  
  } else {
   Serial.println("connection failed");
  }

 while (client.available()) {
  isdata=1;
  char c = client.read(); // Читаем, что нам ответил Web-сервер
  Serial.print(c);
  
 }
 
 if (!client.connected()) {
  isdata=0;
  Serial.println();
  Serial.println("disconnecting.");
  client.stop(); // Завершаем соединение
 }
}

