//Arduino Dice made by Nicholas Rivera 
//Instrucatbles at nicholas_rivera
const int One = 12;
const int Two = 11;
const int Three = 10;
const int Four = 9;
const int Five = 8;
const int Six = 7;
const int Seven = 6;
const int Button = 5;

const int LED = 4;
int val = 0;
int state = 0;
long randNumber; 
void setup(){
  pinMode(One, OUTPUT);
  pinMode(Two, OUTPUT);
  pinMode(Three, OUTPUT);
  pinMode(Four, OUTPUT);
  pinMode(Five, OUTPUT);
  pinMode(Six, OUTPUT);
  pinMode(Seven, OUTPUT);
  pinMode(Button, INPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  randomSeed(analogRead(0));
}
 void loop(){
   val = digitalRead(Button);
   if (val == HIGH){
     randNumber = random(1, 7);
     delay(100);
     Serial.println(randNumber);
     
     if (randNumber == 6){
       digitalWrite(One, HIGH);
       digitalWrite(Two, HIGH);
       digitalWrite(Three, HIGH);
       digitalWrite(Five, HIGH);
       digitalWrite(Six, HIGH);
       digitalWrite(Seven, HIGH);
       delay(3000);
       digitalWrite(One, LOW);
       digitalWrite(Two, LOW);
       digitalWrite(Three, LOW);
       digitalWrite(Five, LOW);
       digitalWrite(Six, LOW);
       digitalWrite(Seven, LOW);
     }
     if (randNumber == 5){
       digitalWrite(One, HIGH);
       digitalWrite(Three, HIGH);
       digitalWrite(Four, HIGH);
       digitalWrite(Five, HIGH);
       digitalWrite(Seven, HIGH);
       delay(3000);
       digitalWrite(One, LOW);
       digitalWrite(Three, LOW);
       digitalWrite(Four, LOW);
       digitalWrite(Five, LOW);
       digitalWrite(Seven, LOW);
     }
     if (randNumber == 4){
       digitalWrite(One, HIGH);
       digitalWrite(Three, HIGH);
       digitalWrite(Five, HIGH);
       digitalWrite(Seven, HIGH);
       delay(3000);
       digitalWrite(One, LOW);
       digitalWrite(Three, LOW);
       digitalWrite(Five, LOW);
       digitalWrite(Seven, LOW);
     }
     if (randNumber == 3){
       digitalWrite(One, HIGH);
       digitalWrite(Four, HIGH);
       digitalWrite(Seven, HIGH);
       delay(3000);
       digitalWrite(One, LOW);
       digitalWrite(Four, LOW);
       digitalWrite(Seven, LOW);
     }
     if (randNumber == 2){
       digitalWrite(One, HIGH);
       digitalWrite(Seven, HIGH);
       delay(3000);
       digitalWrite(One, LOW);
       digitalWrite(Seven, LOW);
     }
     if (randNumber == 1){
       digitalWrite(Four, HIGH);
       delay(3000);
       digitalWrite(Four, LOW);
     }
   }
 }


