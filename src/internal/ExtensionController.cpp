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

using namespace NintendoExtensionCtrl;

ExtensionController::ExtensionController(NXC_I2C_TYPE& i2cBus) 
	: ExtensionController(i2cBus, ExtensionType::AnyController) {}

ExtensionController::ExtensionController(NXC_I2C_TYPE& i2cBus, ExtensionType conID)
	: i2c(i2cBus), ID_Limit(conID) {}

void ExtensionController::begin() {
	i2c.begin();  // Initialize the bus
}

boolean ExtensionController::connect() {
	boolean success = false;

	if (initialize(i2c)) {
		identifyController();
		success = update();  // Seed with initial values
	}
	else {
		connectedID = ExtensionType::NoController;  // Bad init, nothing connected
	}

	return success;
}

boolean ExtensionController::reconnect() {
	return connect();
}

void ExtensionController::identifyController() {
	connectedID = NintendoExtensionCtrl::identifyController(i2c);  // Polls the controller for its identity
}

void ExtensionController::reset() {
	connectedID = ExtensionType::NoController;  // Nothing connected
	memset(controlData, 0x00, MaxRequestSize);  // No control data
	requestSize = MinRequestSize;  // Request size back to minimum
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

ExtensionType ExtensionController::getControllerType() const {
	return connectedID;
}

boolean ExtensionController::update() {
	if (controllerIDMatches() && requestControlData(i2c, requestSize, controlData)) {
		return verifyData(controlData, requestSize);
	}
	
	return false;  // Something went wrong :(
}

uint8_t ExtensionController::getControlData(uint8_t controlIndex) const {
	return controlData[controlIndex];
}

void ExtensionController::setRequestSize(size_t r) {
	if (r >= MinRequestSize && r <= MaxRequestSize) {
		requestSize = (uint8_t) r;
	}
}

void ExtensionController::printDebug(Print& output) const {
	printDebugRaw(output);
}

void ExtensionController::printDebugID(Print& output) const {
	uint8_t idData[ID_Size];
	boolean success = requestIdentity(i2c, idData);

	if (success) {
		output.print("ID: ");
		printRaw(idData, NintendoExtensionCtrl::ID_Size, HEX, output);
	}
	else {
		output.println("Bad ID Read");
	}
}

void ExtensionController::printDebugRaw(Print& output) const {
	printDebugRaw(HEX, output);
}

void ExtensionController::printDebugRaw(uint8_t baseFormat, Print& output) const {
	printRaw(controlData, requestSize, baseFormat, output);
}
