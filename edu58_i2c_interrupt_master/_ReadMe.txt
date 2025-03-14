To use interrupts in the slave device for receiving I2C messages, we need to detect changes on the SDA and SCL lines and handle communication in an Interrupt Service Routine (ISR).

Since Arduino does not have native hardware I2C interrupts without using the Wire library, we will use Pin Change Interrupts to trigger when the I2C master sends data.


Detailed Explanation of "enable_interrupts()"

The function enable_interrupts() enables Pin Change Interrupts for the SDA (A4) and SCL (A5) pins so that the Arduino can detect I2C communication without constantly polling.


How Pin Change Interrupts Work on Arduino

Arduino "ATmega328P" microcontroller has three "Pin Change Interrupt" registers (PCINT):

    1. PCINT0 (Port B) → Pins D8 to D13
    2. PCINT1 (Port C) → Pins A0 to A5 (used for I2C SDA and SCL)
    3. PCINT2 (Port D) → Pins D0 to D7

Since A4 (SDA) and A5 (SCL) belong to Port C (PCINT1), we use the PCICR ("Pin
Change Interrupt Control Register") and PCMSK1 ("Pin Change Mask Register" for Port C).


Step-by-Step Breakdown of "enable_interrupts()"

void enable_interrupts()
{
    PCICR |= (1 << PCIE1);     // Enable Pin Change Interrupt for PCINT[14:8] (A0-A5)
    PCMSK1 |= (1 << PCINT12);  // Enable interrupt for A4 (SDA)
}

1. Enabling Pin Change Interrupts for Port C (A0-A5)
    - PCICR (Pin Change Interrupt Control Register): Controls which port can trigger pin change interrupts.
    - PCIE1 (Bit 1 of PCICR): Enables interrupts for PCINT[14:8] (which corresponds to Port C: A0 to A5).
    - (1 << PCIE1): Sets bit 1 of PCICR, enabling interrupts for analog pins (A0–A5).

2. Enabling Interrupts for A4 (SDA)

PCMSK1 |= (1 << PCINT12);

    - PCMSK1 (Pin Change Mask Register for Port C): Controls which specific pin in Port C triggers an interrupt.
    - PCINT12 (Bit 4 of PCMSK1): Represents A4 (SDA).
    - (1 << PCINT12): Sets bit 4 of PCMSK1, enabling interrupts only for A4 (SDA).


How It Works

    - Interrupts are enabled for Port C (A0–A5) by setting PCIE1.
    - Only A4 (SDA) triggers the interrupt because we set PCINT12 in PCMSK1.
    - Whenever SDA (A4) changes state, an interrupt is triggered, calling:

ISR(PCINT1_vect)
{
    // Handle I2C communication
}

    - This function will execute every time SDA changes from HIGH to LOW or LOW to HIGH.
    - The slave device can now detect when the master starts a transmission.
