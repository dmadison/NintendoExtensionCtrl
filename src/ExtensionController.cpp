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

ExtensionController::ExtensionController(NXC_ControllerType conID, uint8_t datSize)
	: controllerID(conID), DataSize(datSize), enforceControllerID(true) {}

boolean ExtensionController::begin() {
	Wire.begin();

	return connect();
}

boolean ExtensionController::connect() {
	initSuccess = initialize();
	if (initSuccess) {
		connectedID = identifyController();
		if (controllerIDMatches()) {
			return update();  // Seed with initial values
		}
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
	const uint8_t IDPointer = 0xFA;

	uint8_t idData[IDHeaderSize];

	if (!readDataArray(IDPointer, IDHeaderSize, idData)) {
		return NXC_NoController;  // Bad response from device
	}

	// Nunchuk ID: 0x0000
	if (idData[4] == 0x00 && idData[5] == 0x00) {
			return NXC_Nunchuk;
	}

	// Classic Con. ID: 0x0101
	else if (idData[4] == 0x01 && idData[5] == 0x01) {
			return NXC_ClassicController;
	}

	// Guitar Hero Controllers: 0x##00, 0xA420, 0x0103
	else if (idData[1] == 0x00
		&& idData[2] == 0xA4 && idData[3] == 0x20
		&& idData[4] == 0x01 && idData[5] == 0x03) {

		// Guitar: 0x00
		if (idData[0] == 0x00) {
			return NXC_GuitarController;
		}
		// Drums: 0x01
		else if (idData[0] == 0x01) {
			return NXC_DrumController;
		}
	}

	return NXC_UnknownController;  // No matches
}

boolean ExtensionController::controllerIDMatches() {
	if (connectedID == controllerID) {
		return true;  // Match!
	}
	else if (enforceControllerID == false && connectedID != NXC_NoController) {
		return true;  // No enforcing and some sort of controller connected
	}

	return false;  // Enforced types or no controller connected
}

NXC_ControllerType ExtensionController::requestIdentity() {
	if (initialize()) {  // Must initialize before ID call will return proper data
		return identifyController();
	}

	return NXC_NoController;  // Bad init
}

NXC_ControllerType ExtensionController::getConnectedID() {
	return connectedID;
}

boolean ExtensionController::update() {
	if (initSuccess && controllerIDMatches()){
		if (readDataArray(0x00, DataSize, controlData)) {
			return verifyData();
		}
	}
	
	return initSuccess = false;  // Something went wrong. User must re-initialize
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

boolean ExtensionController::extractControlBit(uint8_t arrIndex, uint8_t bitNum) {
	return !(controlData[arrIndex] & (1 << bitNum));
}

void ExtensionController::printDebug(Stream& stream) {
	printDebugRaw(stream);
}

void ExtensionController::printDebugRaw(Stream& stream) {
	// 5 characters per byte, 5 for the prefix, 1 for the null terminator
	const uint8_t bufferSize = (sizeof(controlData) * 5) + 5 + 1;
	char buffer[bufferSize] = "RAW -";

	for (uint8_t i = 0; i < sizeof(controlData); i++){
		sprintf(buffer, "%s %02x |", buffer, controlData[i]);
	}
	stream.println(buffer);
}
