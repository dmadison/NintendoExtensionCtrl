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

ExtensionController::ExtensionController(NXC_I2C_TYPE& i2cBus, NXC_ControllerType conID, uint8_t datSize)
	: controllerID(conID), ControlDataSize(datSize), I2C_Bus(i2cBus), enforceControllerID(true) {}

boolean ExtensionController::begin() {
	I2C_Bus.begin();

	return connect();
}

boolean ExtensionController::connect() {
	initSuccess = initialize();
	if (initSuccess) {
		connectedID = requestIdentity();
		if (controllerIDMatches()) {
			return update();  // Seed with initial values
		}
	}
	else {
		connectedID = NXC_NoController;  // Bad init, nothing connected
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
	if (!NXC_Core::writeRegister(I2C_Bus, 0xF0, 0x55)) { return false; }
	delay(10);
	if (!NXC_Core::writeRegister(I2C_Bus, 0xFB, 0x00)) { return false; }
	delay(20);
	return true;
}

NXC_ControllerType ExtensionController::requestIdentity() {
	const uint8_t IDHeaderSize = 6;
	const uint8_t IDPointer = 0xFA;

	uint8_t idData[IDHeaderSize];

	if (!NXC_Core::readDataArray(I2C_Bus, IDPointer, IDHeaderSize, idData)) {
		return NXC_NoController;  // Bad response from device
	}

	if (idData[2] == 0xA4 && idData[3] == 0x20) {  // All valid IDs
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
			&& idData[4] <= 0x01 && idData[5] == 0x03) {

			// Guitar: 0x00
			if (idData[0] == 0x00) {
				return NXC_GuitarController;
			}
			// Drums: 0x01
			else if (idData[0] == 0x01) {
				return NXC_DrumController;
			}
			// DJ Turntable: 0x03
			else if (idData[0] == 0x03) {
				return NXC_DJTurntable;
			}
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

NXC_ControllerType ExtensionController::identifyController() {
	if (initialize()) {  // Must initialize before ID call will return proper data
		return requestIdentity();
	}

	return NXC_NoController;  // Bad init
}

NXC_ControllerType ExtensionController::getConnectedID() const {
	return connectedID;
}

void ExtensionController::setEnforceID(boolean enforce) {
	enforceControllerID = enforce;
}

boolean ExtensionController::update() {
	if (initSuccess && controllerIDMatches()){
		if (NXC_Core::readDataArray(I2C_Bus, 0x00, ControlDataSize, controlData)) {
			return verifyData();
		}
	}
	
	return initSuccess = false;  // Something went wrong. User must re-initialize
}

boolean ExtensionController::verifyData() {
	byte orCheck = 0x00;   // Check if data is zeroed (bad connection)
	byte andCheck = 0xFF;  // Check if data is maxed (bad init)

	for (int i = 0; i < ControlDataSize; i++) {
		orCheck |= controlData[i];
		andCheck &= controlData[i];
	}

	if (orCheck == 0x00 || andCheck == 0xFF) {
		return false;  // No data or bad data
	}
	
	return true;
}

uint8_t ExtensionController::getControlData(uint8_t controlIndex) const {
	if (controlIndex < ControlDataSize) {
		return controlData[controlIndex];
	}
	return 0;
}

boolean ExtensionController::extractControlBit(uint8_t arrIndex, uint8_t bitNum) const {
	return !(controlData[arrIndex] & (1 << bitNum));
}

void ExtensionController::printDebug(Stream& stream) {
	printDebugRaw(stream);
}

void ExtensionController::printDebugRaw(uint8_t baseFormat) {
	printDebugRaw(Serial, baseFormat);
}

void ExtensionController::printDebugRaw(Stream& stream, uint8_t baseFormat) {
	char padChar = ' ';
	if (baseFormat == BIN || baseFormat == HEX) {
		padChar = '0';
	}

	// Calculate max # of spaces for the base
	uint8_t maxInput = 0xFF;
	uint8_t maxNPlaces = 0;
	while (maxInput != 0) {
		maxInput /= baseFormat;
		maxNPlaces++;
	}

	for (int i = 0; i < ControlDataSize; i++) {
		uint8_t dataOut = getControlData(i);

		if (baseFormat == HEX) {
			stream.print("0x");  // Hex prefix
		}

		// Calculate # of spaces that will be printed. Max - n = # to pad.
		uint8_t nPlaces = 0;
		uint8_t tempOut = dataOut;
		do {
			tempOut /= baseFormat;
			nPlaces++;
		} while (tempOut != 0);


		// Print pad characters
		for (int padOut = 0; padOut < (maxNPlaces - nPlaces); padOut++) {
			stream.print(padChar);
		}

		stream.print(dataOut, baseFormat);

		if (i != ControlDataSize - 1) {  // Print separators
			stream.print(" | ");
		}
	}
	stream.println();
}
