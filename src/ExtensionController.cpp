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

ExtensionController::ExtensionController(uint8_t size, NXC_ControllerType conID) 
	: DataSize(size), controllerID(conID), enforceControllerID(true) {}

boolean ExtensionController::begin() {
	Wire.begin();

	return connect();
}

boolean ExtensionController::connect() {
	if (initialize() && identifyController() != NXC_NoController) {
		return update();  // Seed with initial values
	}
	return false;
}

boolean ExtensionController::reconnect() {
	delay(5);  // Breathe + clear the bus
	return connect();
}

boolean ExtensionController::initialize() {
	/* Initialization for unencrypted communication.
	 * *Should* work on all devices, genuine + 3rd party.
	 * See http://wiibrew.org/wiki/Wiimote/Extension_Controllers
	*/ 
	if (!writeRegister(0xF0, 0x55)) { return false; }
	delay(10);
	if (!writeRegister(0xFB, 0x00)) { return false; }
	delay(20);
	return true;
}

NXC_ControllerType ExtensionController::identifyController() {
	const uint8_t IDHeaderSize = 6;
	const uint8_t IDPointer = 0xFE;

	if (!readDataArray(IDPointer, IDHeaderSize, controlData)) {
		lastID = NXC_NoController;  // Bad response from device
		return lastID;
	}

	lastID = NXC_UnknownController;  // Default if no matches below

	// Nunchuk ID: All 0s
	if (controlData[0] == 0x00 && controlData[1] == 0x00 &&
		controlData[2] == 0x00 && controlData[3] == 0x00 &&
		controlData[4] == 0x00 && controlData[5] == 0x00) {
			lastID = NXC_Nunchuk;
	}

	// Classic Con. ID: 0x0101 followed by 4 0s
	else if (controlData[0] == 0x01 && controlData[1] == 0x01 &&
		controlData[2] == 0x00 && controlData[3] == 0x00 &&
		controlData[4] == 0x00 && controlData[5] == 0x00) {
			lastID = NXC_ClassicController;
	}

	return lastID;
}

boolean ExtensionController::update() {
	// Before getting new control data, check if we have the right controller
	if (enforceControllerID && lastID != controllerID) { return false; }

	if (readDataArray(0x00, DataSize, controlData)) {
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

boolean ExtensionController::readDataArray(byte pointer, uint8_t requestSize, uint8_t * dataOut) {
	if (!writePointer(pointer)) { return false; }  // Set start for data read
	delayMicroseconds(175);  // Wait for data conversion (~200 us)
	return requestMulti(requestSize, dataOut);
}

boolean ExtensionController::writePointer(byte pointer) {
	Wire.beginTransmission(I2C_Addr);
	Wire.write(pointer);
	return Wire.endTransmission() == 0;  // 0 = No Error
}

boolean ExtensionController::writeRegister(byte reg, byte value) {
	Wire.beginTransmission(I2C_Addr);
	Wire.write(reg);
	Wire.write(value);
	return Wire.endTransmission() == 0;
}

boolean ExtensionController::requestMulti(uint8_t requestSize, uint8_t * dataOut) {
	uint8_t nBytesRecv = Wire.readBytes(dataOut,
		Wire.requestFrom(I2C_Addr, requestSize));

	return (nBytesRecv == requestSize);  // Success if all bytes received
}

boolean ExtensionController::extractBit(uint8_t arrIndex, uint8_t bitNum) {
	return !(controlData[arrIndex] & (1 << bitNum));
}

void ExtensionController::printDebug(Stream& stream) {
	printDebugRaw(stream);
}

void ExtensionController::printDebugRaw(Stream& stream) {
	// 5 characters per byte, 5 for the prefix, 1 for the null terminator
	const uint8_t bufferSize = (sizeof(controlData) * 5) + 5 + 1;
	char buffer[bufferSize] = "RAW -";

	for (int i = 0; i < sizeof(controlData); i++){
		sprintf(buffer, "%s %02x |", buffer, controlData[i]);
	}
	stream.println(buffer);
}
