/*
Here is the Arduino code - a modification of the Ping ultrasonic sensor sample.

Adjusting the variable "numReadings" to a higher value will result in less noisy
 readings, but also a larger lag in reaction time.

The ultrasonic module name: HC-SR04
*/

#include <Servo.h>
Servo myservo;                  // create servo object to control a servo
int val;                        // range-mapped value for servo control

const int servoPin = 7;         // The servo pin

#define trigPin 13
#define pingPin 12

const int numReadings = 5;      // set higher to smooth more, also causes more latency
int readings[numReadings];      // the readings from the analog input
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
int lastValue = 0;

void setup()
{
  // initialize serial communication:
  Serial.begin(9600);

  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object

  //clear the smoothing value buffer:
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
  {
    readings[thisReading] = 0;
  }

  pinMode(trigPin, OUTPUT);
  pinMode(pingPin, INPUT);
}

long get_distance()
{
    long duration;
    long distance;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2); //
    digitalWrite(trigPin, HIGH);
    //  delayMicroseconds(1000); - Removed this line
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(pingPin, HIGH);
    distance = (duration/2) / 29.1;

    return distance;
    /*
    if (distance < 4)
    {
      // This is where the LED On/Off happens digitalWrite(led,HIGH);
      // When the Red condition is met, the Green LED should turn off digitalWrite(led2,LOW); }
      // else { digitalWrite(led,LOW); digitalWrite(led2,HIGH); } if (distance >= 200 || distance <= 0){
      Serial.println("Out of range");
    }
    else
    {
      Serial.print(distance);
      Serial.println(" cm");
    }
    */
    // delay(500);
}


void loop()
{
  // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  long duration, inches, cm;
  int fadeValue = 0;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:

  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);

  // This pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  // cm = get_distance();

  //smoothing code starts here
  // subtract the last reading:
  total= total - readings[index];
  // read from the sensor:
  readings[index] = cm;//analogRead(inputPin);
  // add the reading to the total:
  total= total + readings[index];
  // advance to the next position in the array:
  index = index + 1;

  // if we're at the end of the array...
  if (index >= numReadings)
  {
    // ...wrap around to the beginning:
    index = 0;
  }

  // calculate the average:
  average = total / numReadings;
  //smoothing code ends here

  //Serial.print(cm);
  //Serial.println(" cm\n");

  //remap value range and move the servo
  val = average;
  val = map(val, 10, 40, 0, 179);     // scale value to use it with the Tower Pro half turn analog servo (value between 0 and 180)

  if(average < 25)
    {
      myservo.write(val);
      // delay(10);  //let the servo cool down, or something
      //Serial.print(cm);
      //Serial.println(" cm");
    }
  else
    {
      myservo.write(100);
      //Serial.print(cm);
      //Serial.println(" cm");
    }
      // sets the servo position according to the scaled value if within a certain distance
  delay(10);  //let the servo cool down, or something
}

long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
