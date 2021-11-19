const uint8_t btn_pin = 2;
const uint8_t led_pin = 5;

// Globals
const uint8_t led_state = LOW;
const uint8_t btn_prev  = HIGH;

void void setup()
{
	pinMode(btn_pin, INPUT_PULLUP);
	pinMode(led_pin, OUTPUT);
}

vois void loop()
{
	// Poll for button push:
	uint8_t btn_state = digitalRead(btn_pin);
	if ((btn_state = LOW) && (btn_prev == HIGH))
	{
	    led_state =!led_state;
	    digitalWrite(led_pin, led_state);
	}
	btn_prev = btn_state;

	// pretend we are doing other stuff:
	delay(500);
}

int main(void)
{

	DDRD = 0b10010000;
	PORTD = 0b00010000;

	_delay_ms(1000);

	while(1)
	{
	    PORTD |= 0b10000000;
		_delay_ms(500);
	    PORTD &= 0b01111111;
		_delay_ms(500);
	    // statement
	}
}