EEPROM (англ. Electrically Erasable Programmable Read-Only Memory) — электрически стираемое перепрограммируемое ПЗУ (ЭСППЗУ), один из видов энергонезависимой памяти.


На заметку:
Память EEPROM, по заявлениям производителя, обладает гарантированным жизненным циклом 100 000 операций записи/стирания и 100 лет хранения данных при температуре 25°С. Эти данные не распространяются на операции чтения данных из EEPROM — чтение данных не лимитировано. Исходя из этого, нужно проектировать свои скетчи максимально щадящими по отношению к EEPROM.

Время затраченное на одну операцию записи в EEPROM составляет примерно 3,3 мс.

Доступ к EEPROM организован таким образом, что пользователь имеет возможность манипулировать каждым отдельным байтом памяти.

Взаимодействие с памятью EEPROM обеспечивает штатная библиотека Arduino IDE — EEPROM.h, которую нужно импортировать в начале каждого скетча:


#include <EEPROM.h>
// ...

Каждый байт EEPROM по умолчанию (ни разу не записанный) имеет значение 255.
Библиотека EEPROM.h предоставляет доступ к основным функциям по работе с EEPROM:

    Функция	Описание
read()	EEPROM.read(address)
Считывает один байт из EEPROM по адресу address. Значение ячейки, по умолчанию, 255.
address — адрес считываемой ячейки (int), начинается с 0.
write()	EEPROM.write(address, value)
Записывает один байт со значением value в EEPROM по адресу address.
address — адрес ячейки для записи (int), начинается с 0.
value — значение записываемого байта (byte), от 0 до 255.
update()	EEPROM.update(address, value)
Аналог функции write(), за тем лишь исключением, что значение записывается только тогда, когда оно отличается от уже записанного. Поскольку EEPROM обладает жизненным циклом в 100 000 операций записи, использование данной функции предпочтительнее и позволяет продлить жизнь EEPROM.
get()	EEPROM.get(address, data)
Считывает из EEPROM любой тип данных по адресу address.
address — адрес ячейки для записи (int), начинается с 0.
data — данные для чтения, могут быть любым базовым типом (например, float), либо пользовательской структурой struct.
put()	EEPROM.put(address, data)
Записывает в EEPROM любой тип данных по адресу address. Поскольку функция реализована на функции update(), то и работает она схожим образом — записывает только отличающиеся данные.
address — адрес ячейки для записи (int), начинается с 0.
data — данные для записи, могут быть любым базовым типом (например, float), либо пользовательской структурой struct.
EEPROM[]	EEPROM[address]
Позволяет обращаться с байтами EEPROM как с массивом байтов. Байты можно считывать и записывать.
address — адрес ячейки, к которой происходит обращение (int), начинается с 0.


Для чтения всех байт EEPROM правильнее пользоваться универсальным решением, которое подходит для МК с разным объемом EEPROM — EEPROM.length(). Пример получения значения всех байт памяти EEPROM:

#include <EEPROM.h>                       // Импортируем бмблиотеку

int address = 0;                          // Переменная для хранения адреса
byte value;                               // Переменная для хранения значения

void setup() {
  Serial.begin(9600);

  while (address < EEPROM.length()) {     // Перебираем адреса, до тех пор, пока не перебирем все
    value = EEPROM.read(address);         // Считываем значение байта
    Serial.println("Address: "+String(address)+", value: "+String(value));  // Выводим полученное значение в монитор
    address++;                            // Наращиваем адрес
  }
}

void loop() {

}

13
Операции чтения и записи данных с типом byte (диапазон значений 0-255) очень просты:

#include <EEPROM.h>                       // Импортируем бмблиотеку

int address = 0;                          // Переменная для хранения адреса
byte value;                               // Переменная для хранения значения

void setup() {
  Serial.begin(9600);

  EEPROM.write(0,195);                    // Записать значение 195 в ячейку с адресом 0
  value=EEPROM.read(0);                   // Считать значение ячейки с адресом 0
  Serial.println(value, DEC);             // Вывести полученное значение
  // 195
}

void loop() {}

15
Поскольку память EEPROM имеет ограниченный жизненный цикл, вместо функции записи write() предпочтительнее использовать функцию обновления значения update() — она осуществляет запись, только в случае отличия нового значения от существующего:

#include <EEPROM.h>                       // Импортируем бмблиотеку

int address = 0;                          // Переменная для хранения адреса
byte value;                               // Переменная для хранения значения

void setup() {
  Serial.begin(9600);

  EEPROM.update(0,195);                   // Записать значение 195 в ячейку с адресом 0
  value=EEPROM.read(0);                   // Считать значение ячейки с адресом 0
  Serial.println(value, DEC);             // Вывести полученное значение
  // 195
}

void loop() {}

17
Для записи в EEPROM данных, размер которых превышает 1 байт, нужно использовать функцию put(). Для корректного расчета адресов, по которым будет производиться запись сложных значений, необходимо пользоваться функцией sizeof() для расчета объема занимаемой памяти. Для чтения данных, в качестве второго аргумента функции put(), необходимо передать переменную, в которую эти данные будут считаны из EEPROM. Пример, записи числа типа integer (int, занимает 2 байта):


#include <EEPROM.h>                       // Импортируем бмблиотеку

int address = 0;                          // Переменная для хранения адреса
int value1 = 4587;                        // Переменная для хранения значения 1
int value2 = -25214;                      // Переменная для хранения значения 2

void setup() {
  Serial.begin(9600);

  EEPROM.put(address, value1);            // Записать значение value1 в ячейку с адресом 0

  // Для вычисления следующего доступного адреса, необходимо получить объем памяти,
  // отведенной под предыдущее значение:
  address += sizeof(int);                 // Корректируем адрес на уже записанное значение - 2 байта занимает тип int
  Serial.println("Следующее значение будет записано по адресу: " + String(address));

  EEPROM.put(address, value2);            // Записать значение value2 в ячейку с адресом 2

  // Обнуляем переменные для чистоты экперимента
  address = 0;
  int value1 = 0;
  int value2 = 0;

  // Получаем данные из EEPROM
  EEPROM.get(address, value1);
  address += sizeof(int);                 // Корректируем адрес на уже записанное значение
  EEPROM.get(address, value2);
  Serial.println( value1, DEC);           // Вывести полученное значение 1
  Serial.println( value2, DEC);           // Вывести полученное значение 2
}

void loop() {}


20
Пример записи/чтения значения типа float:


#include <EEPROM.h>                 // Импортируем бмблиотеку

int address = 0;                    // Переменная для хранения адреса
float value = -14.048f;             // Переменная для хранения значения типа float

void setup() {
  Serial.begin(9600);

  EEPROM.put(address, value);       // Записать значение value1 в ячейку с адресом 0

  // Для вычисления следующего доступного адреса, необходимо получить объем памяти,
  // отведенной под предыдущее значение:
  address += sizeof(float);         // Корректируем адрес на уже записанное значение - 2 байта занимает тип int
  Serial.println("Следующее значение будет записано по адресу: " + String(address));

  address = 0;
  value = 0;

  // Получаем данные из EEPROM
  EEPROM.get(address, value);
  Serial.println(value);            // Вывести полученное значение float
}

void loop() {}

22 Важно:
При хранении в памяти EEPROM типов данных отличных от byte, нельзя забывать о ситуации «первого чтения» из EEPROM — когда происходит чтение данных, которые не были записаны ранее. Выходом из ситуации может служить использование CRC (контрольной хеш-суммы). Об этом ниже в статье, в разделе о контроле целостности данных.

23
При чтении данных типа пользовательской структуры, если данные не были записаны ранее, будут получены неприемлемые данные:

#include <EEPROM.h>                 // Импортируем бмблиотеку

int address = 0;                    // Переменная для хранения адреса

struct myStruct {                   // Создаем пользовательскую структуру
  float sum;
  byte age;
  char fullname[15];
};

void setup() {
  Serial.begin(9600);

  myStruct person = {};             // Создаем объект пользовательской структуры для записи в EEPROM

  EEPROM.get(address, person);      // Считываем данные из EEPROM в созданную переменную
  Serial.println("Чтение пользовательской структуры из EEPROM: ");
  Serial.println(person.sum);
  Serial.println(person.age);
  Serial.println(person.fullname);
}

void loop() {}

25 Результат:

Пример записи и чтения пользовательских структур:

#include <EEPROM.h>                 // Импортируем бмблиотеку

int address = 0;                    // Переменная для хранения адреса

struct myStruct {                   // Создаем пользовательскую структуру
  float sum;                        // 4 байта
  byte age;                         // 1 байт
  char fullname[15];                // 15 байт - итого 20 байт занимает объект в памяти
};

void setup() {
  Serial.begin(9600);

  myStruct persons[] = {           // Создаем массив объектов пользовательской структуры
    {
      110.20,
      42,
      "Ivanov Ivan"
    }, {
      72.45,
      24,
      "Petrova Elena"
    }, {
      22.98,
      32,
      "Popov Vasiliy"
    }
  };

  // Записываем все данные в EEPROM
  for (int i = 0; i < 3; i++) {
    EEPROM.put(address, persons[i]);    // Записываем значение переменной в EEPROM
    address += sizeof(myStruct); // Корректируем адрес следующей записи на объем записываемых данных
  }

  // Теперь данные можно считать
  address = 0;
  myStruct person; // В переменную person будем считывать данные из EEPROM
  for (int i = 0; i < 3; i++) {
    EEPROM.get(address, person);      // Считываем данные из EEPROM в созданную переменную
    Serial.println("Чтение пользовательской структуры из EEPROM по адресу: " + String(address));
    Serial.println( String(person.sum) + " " +
                    String(person.age) + " " +
                    String(person.fullname));
    address += sizeof(myStruct); // Корректируем адрес следующей записи на объем записываемых данных
  }
}

void loop() {}

29
Контроль целостности данных

Возможность использования объекта EEPROM[] как массива данных, бывает востребована для задач связанных с контролем целостности данных. Контроль целостности данных заключается в расчете контрольной суммы CRC — отпечатка состояния EEPROM, и в дальнейшем сравнении эталонного отпечатка с отпечатком, полученным из актуального состояния EEPROM. В случае несовпадения отпечатков можно однозначно делать вывод о нарушении целостности памяти и запускать заранее подготовленную реакцию на эту ситуацию.

CRC (англ. Cyclic redundancy check, циклический избыточный код) — алгоритм нахождения контрольной суммы, предназначенный для проверки целостности данных

30
Для ускорения вычисления CRC используется таблица с заранее просчитанными «магическими» числами:

const unsigned long crc_table[16] = {
  0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
  0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
  0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
  0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
};

Можно привести 2 основных примера применения. Во-первых, удобно заранее посчитать CRC нетронутой памяти, для того, чтобы при идентификации первого запуска устройства, EEPROM заполнялась значениями по умолчанию. При последующих включения, процедура предварительного заполнения будет пропущена.

Получение CRC EEPROM в «нетронутом», первоначальном состоянии выглядит так:

#include <EEPROM.h>                           // Импортируем бмблиотеку

const unsigned long crc_table[16] = {         // Таблица заранее просчитанных констант для ускорения расчета CRC
  0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
  0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
  0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
  0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
};

void setup()
{
  Serial.begin(9600);

  for (int i=0; i<EEPROM.length(); i++)
  {  // Обнуляем EEPROM - приводим в первоначальное состояние
     EEPROM.update(i,255);
  }

  Serial.println("Отпечаток EEPROM (CRC):");
  Serial.println(eeprom_crc(), HEX);          // В HEX-формате
  Serial.println(eeprom_crc(), DEC);          // В чиловом формате
}

unsigned long eeprom_crc(void) {              // Функция вычисления CRC EEPROM
  unsigned long crc = ~0L;

  for (int index = 0 ; index < EEPROM.length()  ; ++index) {
    crc = crc_table[(crc ^ EEPROM[index]) & 0x0f] ^ (crc >> 4);
    crc = crc_table[(crc ^ (EEPROM[index] >> 4)) & 0x0f] ^ (crc >> 4);
    crc = ~crc;
  }
  return crc;
}


void loop() {}


36
Теперь, зная CRC нетронутой EEPROM, нетрудно инициализировать её первоначальными значениями:

#include <EEPROM.h>                           // Импортируем бмблиотеку

const unsigned long crc_table[16] = {         // Таблица заранее просчитанных констант для ускорения расчета CRC
  0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
  0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
  0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
  0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
};

const unsigned long freeEEPROM = 4288708479;  // Значение CRC неинициализированной памяти

void setup() {
  Serial.begin(9600);

  long actualEEPROM = eeprom_crc();           // Получаем актуальное состояние EEPROM при каждом запуске

  while (actualEEPROM == freeEEPROM) {        // Пока отпечатки совпадают, память пуста и её нужно инициализировать
    Serial.println("EEPROM не инициализирована");
    initializeEEPROM();
    actualEEPROM = eeprom_crc();              // Перерасчитываем CRC
  }

  // Программа не продолжит выполнение, пока EEPROM не будет инициализирована
  Serial.println("EEPROM инициализирована успешно");

}

void initializeEEPROM() {                     // Процедура заполнения EEPROM начальными значениями
  Serial.println("Инициализируем EEPROM");
  EEPROM.update(0, 0);
}

unsigned long eeprom_crc(void) {              // Функция вычисления CRC EEPROM
  unsigned long crc = ~0L;

  for (int index = 0 ; index < EEPROM.length()  ; ++index) {
    crc = crc_table[(crc ^ EEPROM[index]) & 0x0f] ^ (crc >> 4);
    crc = crc_table[(crc ^ (EEPROM[index] >> 4)) & 0x0f] ^ (crc >> 4);
    crc = ~crc;
  }
  return crc;
}

void loop() {}

38
Все последующие будут информировать о том, что EEPROM уже инициализирован.
Во-вторых, при хранении в EEPROM статических неизменяемых параметров, зная отпечаток EEPROM, при сравнении с эталоном легко идентифицировать нарушение целостности хранимых в EEPROM данных. Получим CRC необходимых данных:


#include <EEPROM.h>                           // Импортируем бмблиотеку

const unsigned long crc_table[16] = {         // Таблица заранее просчитанных констант для ускорения расчета CRC
  0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
  0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
  0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
  0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
};

void setup()
{
  Serial.begin(9600);

  for (int i=0; i<EEPROM.length(); i++) {     // Обнуляем EEPROM - приводим в первоначальное состояние
    EEPROM.update(i,255);
    }

  EEPROM.update(0,100);                       // Устанавливаем первоначальные параметры, для расчета эталонной CRC
  EEPROM.update(1,50);                        // 2 параметр
  EEPROM.update(2,25);                        // 3 параметр
  EEPROM.update(3,0);                         // 4 параметр

  Serial.println("Отпечаток EEPROM (CRC):");
  Serial.println(eeprom_crc(), HEX);          // В HEX-формате
  Serial.println(eeprom_crc(), DEC);          // В чиловом формате
}

unsigned long eeprom_crc(void) {              // Функция вычисления CRC EEPROM
  unsigned long crc = ~0L;

  for (int index = 0 ; index < EEPROM.length()  ; ++index) {
    crc = crc_table[(crc ^ EEPROM[index]) & 0x0f] ^ (crc >> 4);
    crc = crc_table[(crc ^ (EEPROM[index] >> 4)) & 0x0f] ^ (crc >> 4);
    crc = ~crc;
  }
  return crc;
}

void loop() {}

44
Теперь остается сравнить актуальное состояние EEPROM с заданным и запустить сценарий — либо инициализация, либо авария:

#include <EEPROM.h>                           // Импорт

const unsigned long crc_table[16] = {         // Таблица заранее просчитанных констант для ускорения расчета CRC
  0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
  0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
  0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
  0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
};

const unsigned long needEEPROM = 1607697274;  // Значение CRC инициализированной памяти

void setup() {
  Serial.begin(9600);

  long actualEEPROM = eeprom_crc();           // Получаем актуальное состояние EEPROM при каждом запуске

  while (actualEEPROM != needEEPROM) {        // Если отпечатки не совпадают, нарушена целостность EEPROM
    Serial.println("EEPROM не соответствует заданному значению");
    // Здесь принимается решение, либо об инициализации...
    initializeEEPROM();

    // либо аварийное предупреждение
    // ТРЕВОГА!
    actualEEPROM = eeprom_crc();              // Перерасчитываем CRC
    Serial.println(actualEEPROM, DEC);        // В числовом формате
  }

  // Программа не продолжит выполнение, пока EEPROM не будет инициализирована
  Serial.println("Целостность EEPROM не нарушена");

}

void initializeEEPROM() {                     // Процедура заполнения EEPROM начальными значениями
  Serial.println("Инициализируем EEPROM");
  for (int i = 0; i < EEPROM.length(); i++) { // Обнуляем EEPROM - приводим в первоначальное состояние
    EEPROM.update(i, 255);
  }

  EEPROM.update(0, 100);                      // Устанавливаем первоначальные параметры, для расчета эталонной CRC
  EEPROM.update(1, 50);                       // 2 параметр
  EEPROM.update(2, 25);                       // 3 параметр
  EEPROM.update(3, 0);                        // 4 параметр
}

unsigned long eeprom_crc(void) {              // Функция вычисления CRC EEPROM
  unsigned long crc = ~0L;

  for (int index = 0 ; index < EEPROM.length()  ; ++index) {
    crc = crc_table[(crc ^ EEPROM[index]) & 0x0f] ^ (crc >> 4);
    crc = crc_table[(crc ^ (EEPROM[index] >> 4)) & 0x0f] ^ (crc >> 4);
    crc = ~crc;
  }
  return crc;
}


void loop() {}

46


47
Плюшки

При разработке устройств, для чтения и записи значений удобно использовать макросы (директива "#define"), в том числе и с параметрами. При таком подходе, при смене адреса ячейки памяти, будет удобно изменить её в скетче только в одном месте. Также удобно записать макросы для всех параметров, которые сохраняются в EEPROM, в одном месте.

В скетче демонстрируется создание именованных макросов, для осуществления чтения/записи значений из/в EEPROM наглядным способом, без указания адресов при каждом обращении. При чем показаны макросы как для простых однобайтовых значений типа byte, так и сложных типов, занимающих в EEPROM более 1 байта:


#include <EEPROM.h>                           // Импорт

// Макросы чтения параметром из EEPROM
#define LOAD_VALUE_PARAMNAME_1        EEPROM.read(0)          // Чтение значения типа byte (0-255)
#define LOAD_VALUE_PARAMNAME_2(value) EEPROM.get(1, value)    // Чтение значения типа long
#define LOAD_VALUE_PARAMNAME_3(value) EEPROM.get(5, value)    // Чтение значения типа String

// Макросы записи параметров в EEPROM
#define SAVE_VALUE_PARAMNAME_1(value) EEPROM.update(0, value) // Запись значения тип byte (0-255)
#define SAVE_VALUE_PARAMNAME_2(value) EEPROM.put(1, value)    // Запись значения типа long
#define SAVE_VALUE_PARAMNAME_3(value) EEPROM.put(5, value)    // Запись значения типа char[2]

byte paramname1 = 12;
long paramname2 = -213841524;
String paramname3 = "Hello";


void setup() {
  Serial.begin(9600);

  // Теперь для манипуляций с EEPROM не нужно знать адреса хранения
  // Намного проще пользоваться заранее определенными именами переменных
  saveParams();           // После изменения параметры можно сохранить
  loadParams();           // Загружаем данные из EEPROM
  showParams();           // Показываем параметры в мониторе порта

  paramname1 = 154;  // Изменяем параметры
  paramname2 = 4858 / 2;
  paramname3 = "New value";

  saveParams();           // После изменения параметры можно сохранить
  loadParams();           // Загружаем данные из EEPROM
  showParams();           // Показываем новые параметры в мониторе порта
}

void loadParams() {
  paramname1=LOAD_VALUE_PARAMNAME_1;
  LOAD_VALUE_PARAMNAME_2(paramname2);
  LOAD_VALUE_PARAMNAME_3(paramname3);
}

void saveParams() {
  SAVE_VALUE_PARAMNAME_1(paramname1);
  SAVE_VALUE_PARAMNAME_2(paramname2);
  SAVE_VALUE_PARAMNAME_3(paramname3);
}

void showParams() {
  Serial.println("param1:" + String(paramname1) +
                 ", param2:" + String(paramname2) +
                 ", param3:" + String(paramname3));
}

void loop() {}


51  На заметку:
Необходимо помнить, что тип данных String не имеет фиксированного объема и меняется в зависимости от присвоенного значения. Таким образом, чтобы не происходило затирания данных, следующих в адресном пространстве следом за переменной типа String, нужно в ручном режиме контролировать её размер.


