// Simple bootloader help programm for programming the ESP32
// When pressed <RESET> on the Arduino the programm turns
// the programm pin <A0> low and the boot pin <BOOT> high
// for a while. The ESP32 is now for 60 seconds in upload mode.
// After that the programm pin <A0> goes high and the boot pin
// <BOOT> will be hit one more time to reset the ESP32.
//
// The bootpin is connected at pin 3 of the ESP32. This is
// right alligned to fit properly on a 0,1" board.
//
// Note: When feeding the ESP32 directly e.g a Nano via the USB
// and the ESP32 has a high currect to run his programm.
// It is possible you will demolish the avalanche diode
// in the USB supply line (this is the same as you run a
// normal Arduino project with external components). So
// for safety (place the Nano on an adapter board and) feed
// external.
//
// Have fun de PA0AG.


#define ProgrammPin       2
#define BootPin           3
#define Led              13

#define UploadTime    60000
#define LedFlashSlow   3000
#define LedFlashQuick   500

int          RunOnce = 0;
volatile int Toggle  = 0;

void setup()
{
     pinMode(Led,         OUTPUT);
     pinMode(ProgrammPin, OUTPUT);
     pinMode(BootPin,     OUTPUT);

     digitalWrite(ProgrammPin, HIGH);
     digitalWrite(BootPin,     HIGH);
}

void TimerSettingSlow()
{
     TCCR1A  = 0;
     TCCR1B  = 0;
     TCNT1   = 0;
     OCR1A   = LedFlashSlow;
     TCCR1B |= (1 << WGM12);
     TCCR1B |= (1 << CS12) | (1 << CS10);  
     TIMSK1 |= (1 << OCIE1A);
}

void TimerSettingFast()
{
     TCCR1A  = 0;
     TCCR1B  = 0;
     TCNT1   = 0;
     OCR1A   = LedFlashQuick;
     TCCR1B |= (1 << WGM12);
     TCCR1B |= (1 << CS12) | (1 << CS10);  
     TIMSK1 |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect)
{
    if (Toggle == 1)
    {
       digitalWrite(Led, LOW);
       Toggle = 0;
    }
    else
    {
       digitalWrite(Led, HIGH);
       Toggle = 1;
    }
}

void loop()
{
     if (RunOnce == 0)
     {
        digitalWrite(ProgrammPin,  LOW); 
        digitalWrite(BootPin,      LOW);
        delay(250);
        digitalWrite(BootPin,     HIGH);
        
        TimerSettingFast();
        delay(UploadTime);
   
        digitalWrite(ProgrammPin, HIGH);
        delay(10);

        digitalWrite(BootPin,      LOW);
        TimerSettingSlow();
        delay(250);

        digitalWrite(BootPin,     HIGH);
        RunOnce = 1;
     }
}
