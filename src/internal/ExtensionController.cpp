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

namespace NintendoExtensionCtrl {

ExtensionController::ExtensionController(ExtensionData& dataRef)
	: data(dataRef)  {}

void ExtensionController::begin() {
	data.i2c.begin();  // Initialize the bus
}

boolean ExtensionController::connect() {
	boolean success = false;  // assume no connection

	if (initialize()) {
		identifyController();  // poll controller for its identity

		if (controllerTypeMatches()) {  // if the right controller is connected...
			memset(&data.controlData, 0x00, ExtensionData::ControlDataSize);  // clear control data
			data.requestSize = MinRequestSize;  // request size back to minimum
			success = specificInit();  // connect success dependent on controller-specific init
		}
	}
	else {
		data.connectedType = ExtensionType::NoController;
	}

	return success;
}

boolean ExtensionController::specificInit() {
	return true;  // default 'success' (no controller-specific init) for generic controllers
}

void ExtensionController::reset() {
	data.connectedType = ExtensionType::NoController;  // Nothing connected
	memset(&data.controlData, 0x00, ExtensionData::ControlDataSize);  // Clear control data
	data.requestSize = MinRequestSize;  // Request size back to minimum
}

boolean ExtensionController::controllerTypeMatches() const {
	const ExtensionType id = getExpectedType();

	if (data.connectedType == id) {
		return true;  // Match!
	}
	else if (id == ExtensionType::AnyController && data.connectedType != ExtensionType::NoController) {
		return true;  // No enforcing and some sort of controller connected
	}

	return false;  // Enforced types or no controller connected
}

ExtensionType ExtensionController::getExpectedType() const {
	return ExtensionType::AnyController;
}

ExtensionType ExtensionController::getControllerType() const {
	return data.connectedType;
}

boolean ExtensionController::update() {
	if (controllerTypeMatches() && requestControlData(data.requestSize, data.controlData)) {
		return verifyData(data.controlData, data.requestSize);
	}
	
	return false;  // Something went wrong :(
}

uint8_t ExtensionController::getControlData(uint8_t controlIndex) const {
	return data.controlData[controlIndex];
}

void ExtensionController::setControlData(uint8_t index, uint8_t val) {
	data.controlData[index] = val;
}

ExtensionController::ExtensionData & ExtensionController::getExtensionData() const {
	return data;
}

size_t ExtensionController::getRequestSize() const {
	return data.requestSize;
}

void ExtensionController::setRequestSize(size_t r) {
	if (r >= MinRequestSize && r <= MaxRequestSize) {
		data.requestSize = (uint8_t) r;
	}
}

NXC_I2C_TYPE & ExtensionController::i2c() const {
	return data.i2c;
}

void ExtensionController::printDebug(Print& output) const {
	printDebugRaw(output);
}

void ExtensionController::printDebugID(Print& output) const {
	uint8_t idData[ID_Size];
	boolean success = requestIdentity(idData);

	if (success) {
		output.print("ID: ");
		printRaw(idData, ID_Size, HEX, output);
	}
	else {
		output.println("Bad ID Read");
	}
}

void ExtensionController::printDebugRaw(Print& output) const {
	printDebugRaw(HEX, output);
}

void ExtensionController::printDebugRaw(uint8_t baseFormat, Print& output) const {
	output.print("Raw[");
	output.print(data.requestSize);
	output.print("]: ");
	printRaw(data.controlData, data.requestSize, baseFormat, output);
}


boolean ExtensionController::initialize(NXC_I2C_TYPE& i2c) {
	/* Initialization for unencrypted communication.
	 * This *should* work on all devices, genuine + 3rd party.
	 * See http://wiibrew.org/wiki/Wiimote/Extension_Controllers
	 */
	if (!i2c_writeRegister(i2c, I2C_Addr, 0xF0, 0x55, false)) return false;
	delay(10);
	if (!i2c_writeRegister(i2c, I2C_Addr, 0xFB, 0x00, false)) return false;
	delay(20);
	return true;
}

boolean ExtensionController::writeRegister(NXC_I2C_TYPE& i2c, byte reg, byte value) {
	return i2c_writeRegister(i2c, I2C_Addr, reg, value);
}

boolean ExtensionController::readRegister(NXC_I2C_TYPE& i2c, byte reg, uint8_t* dataOut) {
	return i2c_readRegister(i2c, I2C_Addr, reg, dataOut);
}

uint8_t ExtensionController::readRegister(NXC_I2C_TYPE& i2c, byte reg) {
	uint8_t regOut = 0x00;
	i2c_readRegister(i2c, I2C_Addr, reg, &regOut);
	return regOut;  // return the value read whether it's valid or not
}

boolean ExtensionController::requestData(NXC_I2C_TYPE& i2c, uint8_t ptr, size_t size, uint8_t* dataOut) {
	return i2c_readDataArray(i2c, I2C_Addr, ptr, size, dataOut);
}

boolean ExtensionController::requestControlData(NXC_I2C_TYPE& i2c, size_t size, uint8_t* controlData) {
	return i2c_readDataArray(i2c, I2C_Addr, 0x00, size, controlData);
}

boolean ExtensionController::requestIdentity(NXC_I2C_TYPE& i2c, uint8_t* idData) {
	return i2c_readDataArray(i2c, I2C_Addr, 0xFA, ID_Size, idData);
}

ExtensionType ExtensionController::identifyController(NXC_I2C_TYPE& i2c) {
	uint8_t idData[ID_Size];

	if (!requestIdentity(i2c, idData)) {
		return ExtensionType::NoController;  // Bad response from device
	}
	return decodeIdentity(idData);
}

// port-specific connect function that utilizes the linked list to evaluate
// each attached controller variant automatically
boolean ExtensionPort::connect() {
	// start by running the default 'connect' function, looking for any controller
	boolean success = ExtensionController::connect();

	if (!success) return false;  // no connection, no reason to iterate controllers

	// We have a connection, that means we need to go through the linked list of
	// controller variants attached to this port and look for a type match. If
	// we've found one, then we need to run the controller-specific initialization
	// functions before telling the user that we're successfully connected.

	ExtensionList::Node* ptr = list.getHead();

	while (ptr != nullptr) {
		// Because of how the linked list is separated from the controller base
		// class to keep the single-controller instance lightweight, we need to fetch
		// the controller object from the node pointer before we do anything with it
		ExtensionController& controller = ptr->getController();

		if (controller.controllerTypeMatches()) {
			success = controller.specificInit();
			if (success) break;
		}
		ptr = ptr->getNext();
	}

	return success;
}

}  // End NintendoExtensionCtrl namespace
