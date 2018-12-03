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

ExtensionPort::ExtensionPort(ExtensionData& dataRef)
	: ExtensionPort(dataRef, ExtensionType::AnyController) {}

ExtensionPort::ExtensionPort(ExtensionData& dataRef, ExtensionType conID)
	: i2c(dataRef.i2c), id(conID), data(dataRef)  {}

void ExtensionPort::begin() {
	data.i2c.begin();  // Initialize the bus
}

boolean ExtensionPort::connect() {
	disconnect();  // Clear current data
	return reconnect();
}

boolean ExtensionPort::reconnect() {
	boolean success = false;

	if (initialize(data.i2c)) {
		identifyController();
		success = update();  // Seed with initial values
	}
	else {
		data.connectedType = ExtensionType::NoController;  // Bad init, nothing connected
	}

	return success;
}

void ExtensionPort::disconnect() {
	data.connectedType = ExtensionType::NoController;  // Nothing connected
	memset(&data.controlData, 0x00, ExtensionData::ControlDataSize);  // Clear control data
}

void ExtensionPort::reset() {
	disconnect();
	requestSize = MinRequestSize;  // Request size back to minimum
}

void ExtensionPort::identifyController() {
	data.connectedType = NintendoExtensionCtrl::identifyController(data.i2c);  // Polls the controller for its identity
}

boolean ExtensionPort::controllerIDMatches() const {
	if (data.connectedType == id) {
		return true;  // Match!
	}
	else if (id == ExtensionType::AnyController && data.connectedType != ExtensionType::NoController) {
		return true;  // No enforcing and some sort of controller connected
	}

	return false;  // Enforced types or no controller connected
}

ExtensionType ExtensionPort::getControllerType() const {
	return data.connectedType;
}

boolean ExtensionPort::update() {
	if (controllerIDMatches() && requestControlData(data.i2c, requestSize, data.controlData)) {
		return verifyData(data.controlData, requestSize);
	}
	
	return false;  // Something went wrong :(
}

uint8_t ExtensionPort::getControlData(uint8_t controlIndex) const {
	return data.controlData[controlIndex];
}

void ExtensionPort::setRequestSize(size_t r) {
	if (r >= MinRequestSize && r <= MaxRequestSize) {
		requestSize = (uint8_t) r;
	}
}

void ExtensionPort::printDebug(Print& output) const {
	printDebugRaw(output);
}

void ExtensionPort::printDebugID(Print& output) const {
	uint8_t idData[ID_Size];
	boolean success = requestIdentity(data.i2c, idData);

	if (success) {
		output.print("ID: ");
		printRaw(idData, NintendoExtensionCtrl::ID_Size, HEX, output);
	}
	else {
		output.println("Bad ID Read");
	}
}

void ExtensionPort::printDebugRaw(Print& output) const {
	printDebugRaw(HEX, output);
}

void ExtensionPort::printDebugRaw(uint8_t baseFormat, Print& output) const {
	output.print("Raw[");
	output.print(requestSize);
	output.print("]: ");
	printRaw(data.controlData, requestSize, baseFormat, output);
}
