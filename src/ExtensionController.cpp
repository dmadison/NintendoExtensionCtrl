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

#include "ExtensionController.h"

ExtensionController::ExtensionController() {}

ExtensionController::ExtensionController(uint8_t size) : DataSize(size) {}

void ExtensionController::begin() {
	Wire.begin();

	initialize();
	update();  // Seed with initial values
}

void ExtensionController::initialize(boolean blocking) {
	/* Initialization for unencrypted communication.
	 * *Should* work on all devices, genuine + 3rd party.
	 * See http://wiibrew.org/wiki/Wiimote/Extension_Controllers
	*/ 
	writeRegister(0xF0, 0x55);
	writeRegister(0xFB, 0x00);

	if (blocking) {
		delay(100);  // Wait after init for device startup
	}
}

void ExtensionController::reconnect() {
	delay(5);  // Breathe + clear the bus
	initialize();
	update();
}

boolean ExtensionController::update() {
	Wire.beginTransmission(I2C_Addr);
	Wire.write(0x00);  // Start at first register
	Wire.endTransmission();

	delayMicroseconds(175);  // Wait for data conversion (~200 us)

	uint8_t nBytesRecv = Wire.readBytes(controlData,
		Wire.requestFrom(I2C_Addr, DataSize));

	if (nBytesRecv == DataSize) {
		return verifyData();
	}

	return false;
}

boolean ExtensionController::verifyData() {
	byte orCheck = 0x00;   // Check if data is zeroed (bad connection)
	byte andCheck = 0xFF;  // Check if data is maxed (bad init)

	for (int i = 0; i < DataSize; i++) {
		orCheck |= controlData[i];
		andCheck &= controlData[i];
	}

	if (orCheck == 0x00 || andCheck == 0xFF) {
		return false;  // No data or bad data
	}
	
	return true;
}

void ExtensionController::writeRegister(byte reg, byte value) {
	Wire.beginTransmission(I2C_Addr);
	Wire.write(reg);
	Wire.write(value);
	Wire.endTransmission();
}

boolean ExtensionController::extractBit(uint8_t arrIndex, uint8_t bitNum) {
	return !(controlData[arrIndex] & (1 << bitNum));
}

void ExtensionController::printDebug(Stream& stream) {
	char buffer[48] = "ExtCtrl -";

	for (int i = 0; i < DataSize; i++){
		sprintf(buffer, "%s %3u |", buffer, controlData[i]);
	}
	stream.println(buffer);
}
