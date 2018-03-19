/*
*  Project     Nintendo Extension Controller Library
*  @author     David Madison
*  @link       github.com/dmadison/NintendoExtensionCtrl
*  @license    Copyright (c) 2018 David Madison
*/

#ifndef ExtensionController_h
#define ExtensionController_h

#include "Arduino.h"
#include "Wire.h"

class ExtensionController {
public:
	ExtensionController();

	void begin();
	void initialize(boolean blocking=true);

	boolean update();

protected:
	const uint8_t I2C_Addr = 0x52;
	uint8_t controlData[6];

private:
	boolean verifyData();

	void writeRegister(byte reg, byte value);
};

#endif
