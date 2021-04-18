#include <ArduinoGamepad.h>

/*
 * Simple configuration
 * Two buttons, communication with ArduinoGamepadEmulator application via serial port.
 * A message example - b031 - means that button connected to pin (0)3 is pressed (1)
 */

 
int b1 = 2;
int b2 = 3;


Button button1(b1,INPUT_PULLUP);
Button button2(b2,INPUT_PULLUP);

void setup() 
{
  Serial.begin(9600);
}

void loop() 
{

if(button1.isClicked())
{
  button1.trySendPressedMsg();
}
else 
{
  button1.trySendReleasedMsg();
}

if(button2.isClicked())
{
  button2.trySendPressedMsg();
}
else 
{
  button2.trySendReleasedMsg();
}

delay (10);
}
