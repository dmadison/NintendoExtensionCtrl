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

#ifndef NXC_ExtensionController_h
#define NXC_ExtensionController_h

#include "NXC_Identity.h"
#include "NXC_Comms.h"
#include "NXC_Utils.h"

class ExtensionController {
public:
	ExtensionController(NXC_I2C_TYPE& i2cBus = NXC_I2C_DEFAULT);

	void begin();

	boolean connect();
	boolean reconnect();

	boolean update();
	ExtensionType identifyController();

	void reset();

	ExtensionType getConnectedID() const;
	uint8_t getControlData(uint8_t controlIndex) const;

	void setEnforceID(boolean enforce);

	void printDebug(Print& output = NXC_SERIAL_DEFAULT) const;
	void printDebugID(Print& output = NXC_SERIAL_DEFAULT) const;
	void printDebugRaw(Print& output = NXC_SERIAL_DEFAULT) const;
	void printDebugRaw(uint8_t baseFormat, Print& output = NXC_SERIAL_DEFAULT) const;

	static const uint8_t ControlDataSize = 6;  // Enough for standard request size

protected:
	ExtensionController(NXC_I2C_TYPE& i2cBus, ExtensionType conID);

private:
	boolean controllerIDMatches() const;

	const ExtensionType ID_Limit = ExtensionType::AnyController;
	ExtensionType connectedID = ExtensionType::NoController;
	uint8_t controlData[ControlDataSize];

	NintendoExtensionCtrl::ExtensionComms comms;
};

#include "NXC_DataMaps.h"

#endif
