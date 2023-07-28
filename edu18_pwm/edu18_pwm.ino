int pwmPin = 9; // assigns pin 9 to variable pwm
int pot = A0; // assigns analog input A0 to variable pot
int c1 = 0;   // declares variable c1
int c2 = 0;   // declares variable c2

void setup()  // setup loop
{
  pinMode(pwmPin, OUTPUT);
  pinMode(pot, INPUT);
}


void loop()
{
  c2 = 200;// analogRead(pot);
  c1 = 1024-c2;         // subtracts c2 from 1000 ans saves the result in c1

  digitalWrite(pwmPin, HIGH);
  delayMicroseconds(c1);
  digitalWrite(pwmPin, LOW);
  delayMicroseconds(c2);

}
