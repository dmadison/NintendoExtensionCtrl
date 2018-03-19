/*
*  Project     Nintendo Extension Controller Library
*  @author     David Madison
*  @link       github.com/dmadison/NintendoExtensionCtrl
*  @license    Copyright (c) 2018 David Madison
*/

#ifndef Nunchuk_h
#define Nunchuk_h

#include "ExtensionController.h"

class Nunchuk : public ExtensionController {
public:
	Nunchuk();

	uint8_t getJoyX();
	uint8_t getJoyY();

	uint16_t getAccelX();
	uint16_t getAccelY();
	uint16_t getAccelZ();

	boolean getButtonC();
	boolean getButtonZ();

	float getRollAngle();
	float getPitchAngle();

	void printDebug(Stream& stream=Serial);
};

#endif
