#include <ArduinoGamepad.h>

/*
 * Handling joystick is even simpler than buttons :)
 * It needs only declaration, and calling a method to communicate with your PC.
 * Check if it works well by opening serial port monitor.
 * [ a message example - j144 (meaning that joystick nr 1 is in neutral position) ]
 */

Joystick joy1(A0,A1);

void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
  joy1.trySendJoystickMsg();
  delay (10);
}
