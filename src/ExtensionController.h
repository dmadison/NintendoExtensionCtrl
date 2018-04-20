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

#include "NXC_Core.h"

#define NXC_CONTROL_DATA_MAX 6  // Max # of control data bytes

struct ExtensionData {
	friend class ExtensionController;
	ExtensionData(NXC_I2C_TYPE& i2cBus = NXC_I2C_DEFAULT);
private:
	NXC_I2C_TYPE& I2C_Bus = NXC_I2C_DEFAULT;
	NXC_ControllerType connectedID = NXC_NoController;
	uint8_t controlData[NXC_CONTROL_DATA_MAX];
};

class ExtensionController {
public:
	ExtensionController(NXC_I2C_TYPE& i2cBus = NXC_I2C_DEFAULT);
	ExtensionController(ExtensionData& busData);

	boolean begin();

	boolean connect();
	boolean reconnect();

	boolean update();
	NXC_ControllerType identifyController();

	NXC_ControllerType getConnectedID() const;
	uint8_t getControlData(uint8_t controlIndex) const;
	boolean getControlBit(uint8_t arrIndex, uint8_t bitNum) const;

	void setEnforceID(boolean enforce);

	void printDebug(Stream& stream = NXC_SERIAL_DEFAULT) const;
	void printDebugID(Stream& stream = NXC_SERIAL_DEFAULT) const;
	void printDebugRaw(uint8_t baseFormat) const;
	void printDebugRaw(Stream& stream = NXC_SERIAL_DEFAULT, uint8_t baseFormat = HEX) const;

	const NXC_ControllerType ControllerID = NXC_UnknownController;
	const uint8_t ControlDataSize = NXC_CONTROL_DATA_MAX;  // Bytes per update

protected:
	ExtensionController(NXC_I2C_TYPE& i2cBus, NXC_ControllerType conID, uint8_t datSize);
	ExtensionController(ExtensionData& busData, NXC_ControllerType conID, uint8_t datSize);

private:
	boolean controllerIDMatches() const;

	ExtensionData * busData;
	boolean enforceControllerID = false;  // Off for generic controllers
};

#endif
