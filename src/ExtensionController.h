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
