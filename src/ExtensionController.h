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

enum NXC_ControllerType {
	NXC_NoController,
	NXC_UnknownController,
	NXC_Nunchuk,
	NXC_ClassicController,
};

class ExtensionController {
public:
	ExtensionController();

	boolean begin();

	boolean connect();
	boolean reconnect();

	boolean initialize();
	NXC_ControllerType identifyController();

	boolean update();

	void printDebug(Stream& stream = Serial);
	void printDebugRaw(Stream& stream = Serial);

	const NXC_ControllerType controllerID = NXC_UnknownController;

protected:
	ExtensionController(uint8_t size, NXC_ControllerType conID);

	boolean extractBit(uint8_t arrIndex, uint8_t bitNum);

	const uint8_t I2C_Addr = 0x52;
	const uint8_t DataSize = 6;  // Bytes per update
	uint8_t controlData[6];

private:
	boolean verifyData();

	boolean readDataArray(byte pointer, uint8_t requestSize, uint8_t * dataOut);

	boolean writePointer(byte pointer);
	boolean writeRegister(byte reg, byte value);
	boolean requestMulti(uint8_t requestSize, uint8_t * dataOut);

	const boolean enforceControllerID = false;  // Off for generic controllers
	NXC_ControllerType lastID = NXC_NoController;  // Controller from the last identify call
};

#endif
