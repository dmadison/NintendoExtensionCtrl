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
	initSuccess = NXCtrl::initialize();
	if (initSuccess) {
		identifyController();
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

NXC_ControllerType ExtensionController::identifyController() {
	return connectedID = NXCtrl::identifyController(I2C_Bus);
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

NXC_ControllerType ExtensionController::getConnectedID() const {
	return connectedID;
}

void ExtensionController::setEnforceID(boolean enforce) {
	enforceControllerID = enforce;
}

boolean ExtensionController::update() {
	if (initSuccess && controllerIDMatches()){
		if (NXCtrl::readDataArray(I2C_Bus, 0x00, ControlDataSize, controlData)) {
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

boolean ExtensionController::getControlBit(uint8_t arrIndex, uint8_t bitNum) const {
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
