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
#include "NXC_DataMaps.h"

class ExtensionData {
	friend class ExtensionController;
public:
	ExtensionData(NXC_I2C_TYPE& i2cbus = NXC_I2C_DEFAULT) :
		i2c(i2cbus) {}

	uint8_t getControlData(uint8_t index) const {
		return controlData[index];
	}

	ExtensionType getConnectedType() const {
		return connectedType;
	}

	static const uint8_t ControlDataSize = 21;  // Largest reporting mode (0x3d)

protected:
	NXC_I2C_TYPE & i2c;  // Reference for the I2C (Wire) class
	ExtensionType connectedType = ExtensionType::NoController;
	uint8_t controlData[ControlDataSize];
};

class ExtensionController {
public:
	ExtensionController(ExtensionData& dataRef);

	void begin();

	boolean connect();
	boolean reconnect();

	boolean update();

	void reset();

	ExtensionType getControllerType() const;
	uint8_t getControlData(uint8_t controlIndex) const;

	void setRequestSize(size_t size = MinRequestSize);

	void printDebug(Print& output = NXC_SERIAL_DEFAULT) const;
	void printDebugID(Print& output = NXC_SERIAL_DEFAULT) const;
	void printDebugRaw(Print& output = NXC_SERIAL_DEFAULT) const;
	void printDebugRaw(uint8_t baseFormat, Print& output = NXC_SERIAL_DEFAULT) const;

	static const uint8_t MinRequestSize = 6;   // Smallest reporting mode (0x37)
	static const uint8_t MaxRequestSize = ExtensionData::ControlDataSize;

	NXC_I2C_TYPE & i2c;  // Easily accessible I2C reference
	const ExtensionType id = ExtensionType::AnyController;

protected:
	ExtensionController(ExtensionData& dataRef, ExtensionType conID);

	typedef NintendoExtensionCtrl::CtrlIndex CtrlIndex;
	typedef NintendoExtensionCtrl::ByteMap   ByteMap;
	typedef NintendoExtensionCtrl::BitMap    BitMap;

	uint8_t getControlData(const ByteMap map) const {
		return NintendoExtensionCtrl::getControlData(data.controlData, map);
	}

	template<size_t size>
	uint8_t getControlData(const ByteMap(&map)[size]) const {
		return NintendoExtensionCtrl::getControlData(data.controlData, map);
	}

	boolean getControlBit(const BitMap map) const {
		return NintendoExtensionCtrl::getControlBit(data.controlData, map);
	}

private:
	ExtensionData &data;  // I2C and control data storage

	void disconnect();
	void identifyController();
	boolean controllerIDMatches() const;

	uint8_t requestSize = MinRequestSize;
};

namespace NintendoExtensionCtrl {
	template <class ControllerMap>
	class BuildControllerClass : public ControllerMap {
	public:
		BuildControllerClass(NXC_I2C_TYPE& i2cBus = NXC_I2C_DEFAULT) :
			ControllerMap(portData),
			portData(i2cBus) {}

		typedef ControllerMap Data;  // Make controller class easily accessible

	protected:
		// Included data instance. Contains:
		//    * I2C library object reference
		//    * Connected controller identity (type)
		//    * Control data array
		// This data can be shared between controller instances using a single
		// logical endpoint to keep memory down.
		ExtensionData portData;
	};
}

#endif
