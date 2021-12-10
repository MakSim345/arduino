

const int cs  = 0x07;
const int sdi = 0x08;
const int clk = 0x09;
#define POTENTIOMETER_DELAY 500

void setup()
{
	pinMode(cs, OUTPUT);
	pinMode(sdi, OUTPUT);
	pinMode(clk, OUTPUT);
  	Serial.begin(9600);           // init serial
}

void setRes(int resistanceP)
{
	// prepare to transmit:
	digitalWrite(cs, HIGH);
	digitalWrite(sdi, LOW);
	digitalWrite(clk, LOW);

	digitalWrite(cs, LOW);
	
	// 2 zero bits for address:
	digitalWrite(sdi, LOW);
	for(int i=0; i<2; i++)
	{
    	digitalWrite(clk, LOW);
    	digitalWrite(clk, HIGH);
	}

	//send a value to device:
	for(int i=7; i>=0; i--)
	{
    	digitalWrite(clk, LOW);

    	int bt = bitRead(resistanceP, i);
    	if(bt == 1)
    	{
        	digitalWrite(sdi, HIGH);
    	}
    	else
    	{
        	digitalWrite(sdi, LOW);
    	}

    	digitalWrite(clk, HIGH);
    }
}

void loop()
{
	Serial.write("Loop started\n"); // 
  	
	Serial.write("Count up\n"); // 
/*
	int i = 10;
	setRes(i);
  	Serial.println(i); //
	delay(5000);
	int i = 50;
	setRes(i);
  	Serial.println(i); //
	delay(5000);
	setRes(150);
  	Serial.println(i); //
	delay(5000);
	setRes(2000);
  	Serial.println(i); //
	delay(5000);
*/
	
	for(int i=0; i<255; i+=10)
	{
		setRes(i);	
		delay(POTENTIOMETER_DELAY);    
  		Serial.println(i); //
	}

	Serial.write("Count down\n"); // 
	for(int i=255; i>1; i-=10)
	{
		setRes(i);	
		delay(POTENTIOMETER_DELAY);    
  		Serial.println(i); 
	}
}