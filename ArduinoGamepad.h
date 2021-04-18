#ifndef ARDUINO_GAMEPAD
#define ARDUINO_GAMEPAD

//#include "Arduino.h"
//#include "stdint.h"
//#include <stdio.h> 

class Button
{
private:
	enum class state { PRESSED, RELEASED };	
	char* signature;					// signature that is being sent via serial port to console application
	bool pullupConnect;					// is button connected as INPUT_PULLUP
	uint8_t pin;						// pin that the button is connected to
	state bstate;						// button state

public:
	/* constructor / destructor */
	explicit Button(uint8_t _pin, uint8_t _mode);
	~Button();

	/* basic utility functions */
	inline bool isClicked(void) const;	// true if button is pressed, false otherwise [assumes connecting to the pin as INPUT_PULLUP]
	
	/* serial port communication */
	size_t trySendPressedMsg(void);		// sends msg via serial port once after pressing button	[ changing state from released to pressed ]
	size_t trySendReleasedMsg(void);	// sends msg via serial port once after releasing button [ changing state from pressed to released ]
};


struct joypos
{
	// 4,4 represents default joystick position
	uint8_t x = 4;
	uint8_t y = 4;

	bool operator == (const joypos& rhs)
	{
		return (this->x == rhs.x && this->y == rhs.y) ? true : false;
	}
};

class Joystick
{
private:
	/*
	*	Position is mapped to values 0..8 on both axes , values 0..3 means joystick swing in one direction,
	*	and values 5..8 means swing in the other direction. Value equal to 4 represents neutral joystick position on a given axis.
	*/
	joypos position;	// current joystick position
	uint8_t xAxisPin;	// x axis analog pin
	uint8_t yAxisPin;	// y axis analog pin
	char* signature;	// signature that is being sent via serial port to console application/

public:
	explicit Joystick(uint8_t _xAxisPin, uint8_t _yAxisPin);
	~Joystick();

	/* basic utility functions */
	bool updatePosition(void);				// returns true if new joystick position is detected, false otherwise
	inline joypos getPosition(void) const;	// returns actual joystick position

	/* serial communication */
	size_t trySendJoystickMsg(void);		// sends msg via serial port after every joystick swing [ only when a position change appears ]
};

// Button -----------------------------------------------------------------------------

Button::Button(uint8_t _pin, uint8_t _mode)
	: pin(_pin),
	bstate(state::RELEASED)
{
	signature = new char[4];
	strcpy(signature, "b000");

	if (pin >= 0 && pin <= 9)
	{
		snprintf(signature + 2, 2, "%d", (int)pin);
	}
	else
	{
		snprintf(signature + 1, 3, "%d", (int)pin);
	}
	if (_mode == INPUT_PULLUP)
	{
		pinMode(pin, INPUT_PULLUP);
		pullupConnect = true;
	}
	else if (_mode == INPUT)
	{
		pinMode(pin, INPUT);
		pullupConnect = false;
	}
	else
	{ 
		Serial.println("Error, undefined pin mode!");
	}
}

Button::~Button()
{
	delete[] signature;
}

bool Button::isClicked(void) const
{
	if(pullupConnect)
		return (digitalRead(pin) == LOW) ? true : false;
	else
		return (digitalRead(pin) == HIGH) ? true : false;
}

size_t Button::trySendPressedMsg(void)
{
	if (bstate != state::PRESSED)
	{
		signature[3] = '1';
		size_t bytes = Serial.write(signature, 4 * sizeof(char));
		bstate = state::PRESSED;
		return bytes;
	}
	return 0;
}

size_t Button::trySendReleasedMsg(void)
{
	if (bstate != state::RELEASED)
	{
		signature[3] = '0';
		size_t bytes = Serial.write(signature, 4 * sizeof(char));
		bstate = state::RELEASED;
		return bytes;
	}
	return 0;
}

// Joystick -----------------------------------------------------------------------------

Joystick::Joystick(uint8_t _xAxisPin, uint8_t _yAxisPin)
	:	xAxisPin(_xAxisPin),
		yAxisPin(_yAxisPin)
{
	static int ID = 1;
	signature = new char[4];
	strcpy(signature, "j044");
	snprintf(signature + 1, 2, "%d", ID);
	pinMode(_xAxisPin, INPUT);
	pinMode(_yAxisPin, INPUT);
	ID++;
}
Joystick::~Joystick()
{
	delete[] signature;
}

bool Joystick::updatePosition(void)
{
	joypos tempPrevPosition = position;
	position.x = map(analogRead(xAxisPin) + 10, 0, 1023, 0, 8);
	position.y = map(analogRead(yAxisPin) + 10, 0, 1023, 0, 8);
	return (position == tempPrevPosition) ? false : true;
}

joypos Joystick::getPosition(void) const
{
	return position;
}

size_t Joystick::trySendJoystickMsg(void)
{
	if (updatePosition())
	{
		signature[2] = position.x + '0';
		signature[3] = position.y + '0';
		size_t bytes = Serial.write(signature, 4 * sizeof(char));
		return bytes;
	}
	return 0;
}

#endif