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

#ifndef Nunchuk_h
#define Nunchuk_h

#include "ExtensionController.h"

class Nunchuk : public ExtensionController {
public:
	Nunchuk();

	uint8_t getJoyX();  // 8 bits, 0-255
	uint8_t getJoyY();

	uint16_t getAccelX();  // 10 bits, 0-1023
	uint16_t getAccelY();
	uint16_t getAccelZ();

	boolean getButtonC();
	boolean getButtonZ();

	float getRollAngle();  // -180.0 to 180.0
	float getPitchAngle();

	void printDebug(Stream& stream=Serial);
};

#endif
