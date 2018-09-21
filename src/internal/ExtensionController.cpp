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

ExtensionController::ExtensionController(NXC_I2C_TYPE& i2cBus) : i2c(i2cBus) {}

ExtensionController::ExtensionController(NXC_I2C_TYPE& i2cBus, ExtensionType conID)
	: i2c(i2cBus), ID_Limit(conID) {}

void ExtensionController::begin() {
	i2c.begin();  // Initialize the bus
}

boolean ExtensionController::connect() {
	if (NintendoExtensionCtrl::initialize(i2c)) {
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
	return connectedID = NintendoExtensionCtrl::identifyController(i2c);  // Polls the controller for its identity
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
	if (controllerIDMatches() && NintendoExtensionCtrl::requestControlData(i2c, ControlDataSize, controlData)) {
		return NintendoExtensionCtrl::verifyData(controlData, ControlDataSize);
	}
	
	return false;  // Something went wrong :(
}

uint8_t ExtensionController::getControlData(uint8_t controlIndex) const {
	return controlData[controlIndex];
}

void ExtensionController::printDebug(Print& output) const {
	printDebugRaw(output);
}

void ExtensionController::printDebugID(Print& output) const {
	uint8_t idData[NintendoExtensionCtrl::ID_Size];
	boolean success = NintendoExtensionCtrl::requestIdentity(i2c, idData);

	if (success) {
		output.print("ID: ");
		NintendoExtensionCtrl::printRaw(idData, NintendoExtensionCtrl::ID_Size, HEX, output);
	}
	else {
		output.println("Bad ID Read");
	}
}

void ExtensionController::printDebugRaw(Print& output) const {
	printDebugRaw(HEX, output);
}

void ExtensionController::printDebugRaw(uint8_t baseFormat, Print& output) const {
	NintendoExtensionCtrl::printRaw(controlData, ControlDataSize, baseFormat, output);
}
