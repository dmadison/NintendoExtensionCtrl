/*
*  Project     Nintendo Extension Controller Library
*  @author     David Madison
*  @link       github.com/dmadison/NintendoExtensionCtrl
*  @license    LGPLv3 - Copyright (c) 2018 David Madison
*
*  This file is part of the Nintendo Extension Controller Library.
*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef NXC_Nunchuk_h
#define NXC_Nunchuk_h

#include "ExtensionController.h"

class Nunchuk : public ExtensionController {
public:
	Nunchuk(NXC_I2C_TYPE& i2cBus = NXC_I2C_DEFAULT);

	uint8_t joyX();  // 8 bits, 0-255
	uint8_t joyY();

	uint16_t accelX();  // 10 bits, 0-1023
	uint16_t accelY();
	uint16_t accelZ();

	boolean buttonC();
	boolean buttonZ();

	float rollAngle();  // -180.0 to 180.0
	float pitchAngle();

	void printDebug(Stream& stream=NXC_SERIAL_DEFAULT);
};

#endif
