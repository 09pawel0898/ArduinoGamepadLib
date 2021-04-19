# A minimalistic library for Arduino to communicate with PC via serial port

Simple library which i wrote for handilng basic user-interactive components like buttons, switches, joysticks
that can be connected to arduino. Data produced during interacion is being sent via serial port to the computer.
I use it to work with my [Gamepad-Emulator](https://github.com/09pawel0898/arudino-gamepad-emulator "Gamepad-Emulator")
program, which recognizes the incoming data and performs acions previously assigned to the components. 

Every message has 4 bytes. Each byte represents one character.

All methods are called try..() because the method is not sending data every call, but only when a change appears
in relative to the previous component state.

### Button messages :  
    
    A message describes a pin that the button is connected to and its current state.
    
    b050 - means button connected to pin nr 5 is not pressed
    b131 - means button connected to pin nr 13 is pressed
    
### Joystick message :
    
    Axis data can be equal to 0...8 which is divided to 3 sections
    0..3 -  swing in one direction
    4 -     neutral position
    5..8 -  swing in the other direction
    So the program detects 4 swing levels on every direction.
    
    j144 - means joystick nr 1 is in neutral state (4,4)
    j248 - means joystick nr 2 is leaned out on y axis
    
## Joystick example

```cpp
#include <ArduinoGamepad.h>

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
```

## Buttons example

```cpp
#include <ArduinoGamepad.h>

int PINB1 = 2;
int PINB2 = 3;

Button button1(PINB1,INPUT_PULLUP);
Button button2(PINB2,INPUT_PULLUP);

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
```
