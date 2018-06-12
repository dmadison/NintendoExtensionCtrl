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

ExtensionController::ExtensionController(NXC_I2C_TYPE& i2cBus) : comms(i2cBus) {}

ExtensionController::ExtensionController(NXC_I2C_TYPE& i2cBus, ExtensionType conID)
	: ID_Limit(conID), comms(i2cBus) {}

void ExtensionController::begin() {
	comms.startBus();
}

boolean ExtensionController::connect() {
	if (comms.initialize()) {
		identifyController();
		if (controllerIDMatches()) {
			return update();  // Seed with initial values
		}
	}
	else {
		connectedID = ExtensionType::NoController;  // Bad init, nothing connected
	}

	return false;
}

boolean ExtensionController::reconnect() {
	reset();
	return connect();
}

ExtensionType ExtensionController::identifyController() {
	return connectedID = comms.identifyController();
}

void ExtensionController::reset() {
	connectedID = ExtensionType::NoController;
	for (int i = 0; i < ControlDataSize; i++) {
		controlData[i] = 0;
	}
}

boolean ExtensionController::controllerIDMatches() const {
	if (connectedID == ID_Limit) {
		return true;  // Match!
	}
	else if (ID_Limit == ExtensionType::AnyController && connectedID != ExtensionType::NoController) {
		return true;  // No enforcing and some sort of controller connected
	}

	return false;  // Enforced types or no controller connected
}

ExtensionType ExtensionController::getConnectedID() const {
	return connectedID;
}

boolean ExtensionController::update() {
	if (controllerIDMatches() && comms.requestControlData(ControlDataSize, controlData)) {
		return NintendoExtensionCtrl::verifyData(controlData, ControlDataSize);
	}
	
	return false;  // Something went wrong :(
}

uint8_t ExtensionController::getControlData(uint8_t controlIndex) const {
	return controlData[controlIndex];
}

boolean ExtensionController::getControlBit(uint8_t arrIndex, uint8_t bitNum) const {
	return !(controlData[arrIndex] & (1 << bitNum));
}

void ExtensionController::printDebug(Stream& stream) const {
	printDebugRaw(stream);
}

void ExtensionController::printDebugID(Stream& stream) const {
	uint8_t idData[comms.IDSize];
	boolean success = comms.requestIdentity(idData);

	if (success) {
		stream.print("ID: ");
		NintendoExtensionCtrl::printRaw(idData, comms.IDSize, HEX, stream);
	}
	else {
		stream.println("Bad ID Read");
	}
}

void ExtensionController::printDebugRaw(Stream& stream) const {
	printDebugRaw(HEX, stream);
}

void ExtensionController::printDebugRaw(uint8_t baseFormat, Stream& stream) const {
	NintendoExtensionCtrl::printRaw(controlData, ControlDataSize, baseFormat, stream);
}
