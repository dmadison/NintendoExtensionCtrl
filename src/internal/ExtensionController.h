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
#include "NXC_LinkedList.h"

class ExtensionController {
public:
	struct ExtensionData {
		friend class ExtensionController;

		ExtensionData(NXC_I2C_TYPE& i2cbus) :
			i2c(i2cbus) {}

		static const uint8_t ControlDataSize = 21;  // Largest reporting mode (0x3d)

	private:
		NXC_I2C_TYPE & i2c;  // Reference for the I2C (Wire) class
		ExtensionType connectedType = ExtensionType::NoController;
		uint8_t requestSize = MinRequestSize;
		uint8_t controlData[ControlDataSize];
	};

	ExtensionController(ExtensionData& dataRef);

	void begin();

	boolean connect();
	virtual boolean specificInit();

	boolean update();

	void reset();

	virtual ExtensionType getControllerType() const;
	ExtensionType getConnectedType() const;
	boolean controllerTypeMatches() const;

	uint8_t getControlData(uint8_t controlIndex) const;
	ExtensionData & getExtensionData() const;

	size_t getRequestSize() const;
	void setRequestSize(size_t size = MinRequestSize);

	void printDebug(Print& output = NXC_SERIAL_DEFAULT) const;
	void printDebugID(Print& output = NXC_SERIAL_DEFAULT) const;
	void printDebugRaw(Print& output = NXC_SERIAL_DEFAULT) const;
	void printDebugRaw(uint8_t baseFormat, Print& output = NXC_SERIAL_DEFAULT) const;

	static const uint8_t MinRequestSize = 6;   // Smallest reporting mode (0x37)
	static const uint8_t MaxRequestSize = ExtensionData::ControlDataSize;

	NXC_I2C_TYPE& i2c() const;  // Easily accessible I2C reference

	static const uint8_t I2C_Addr = 0x52;  // Address for all extension controllers
	static const uint8_t ID_Size = 6;  // Number of bytes for ID signature

public:
	/* I2C Communication Functions, Static & Shared */
	static boolean initialize(NXC_I2C_TYPE& i2c);

	static boolean writeRegister(NXC_I2C_TYPE& i2c, byte reg, byte value);
	static boolean readRegister(NXC_I2C_TYPE& i2c, byte reg, uint8_t* dataOut);
	static uint8_t readRegister(NXC_I2C_TYPE& i2c, byte reg);  // no error reporting

	static boolean requestData(NXC_I2C_TYPE& i2c, uint8_t ptr, size_t size, uint8_t* dataOut);
	static boolean requestControlData(NXC_I2C_TYPE& i2c, size_t size, uint8_t* controlData);
	static boolean requestIdentity(NXC_I2C_TYPE& i2c, uint8_t* idData);

	static ExtensionType identifyController(NXC_I2C_TYPE& i2c);

	/* I2C Communication Functions, Inline Member */
	inline boolean initialize() const { return initialize(data.i2c); }

	inline boolean writeRegister(byte reg, byte value) const { return writeRegister(data.i2c, reg, value); }
	inline boolean readRegister(byte reg, uint8_t* dataOut) const { return readRegister(data.i2c, reg, dataOut); }
	inline uint8_t readRegister(byte reg) const { return readRegister(data.i2c, reg); }

	inline boolean requestData(uint8_t ptr, size_t size, uint8_t* dataOut) const { return requestData(data.i2c, ptr, size, dataOut); }
	inline boolean requestControlData(size_t size, uint8_t* controlData) const { return requestControlData(data.i2c, size, controlData); }
	inline boolean requestIdentity(uint8_t* idData) const { return requestIdentity(data.i2c, idData); }

	inline ExtensionType identifyController() const { return data.connectedType = identifyController(data.i2c); }

protected:
	typedef NintendoExtensionCtrl::IndexMap  IndexMap;
	typedef NintendoExtensionCtrl::ByteMap   ByteMap;
	typedef NintendoExtensionCtrl::BitMap    BitMap;

	uint8_t getControlData(const ByteMap map) const {
		return (data.controlData[map.index] & map.mask) >> map.offset;
	}

	template<size_t size>
	uint8_t getControlData(const ByteMap(&map)[size]) const {
		uint8_t dataOut = 0x00;
		for (size_t i = 0; i < size; i++) {
			/* Repeated line from the single-ByteMap function above. Apparently the
				constexpr stuff doesn't like being passed through nested functions. */
			dataOut |= (data.controlData[map[i].index] & map[i].mask) >> map[i].offset;
			//dataOut |= getControlData(map[i]);
		}
		return dataOut;
	}

	boolean getControlBit(const BitMap map) const {
		return !(data.controlData[map.index] & (1 << map.position));  // Inverted logic, '0' is pressed
	}

	void setControlData(uint8_t index, uint8_t val);

private:
	ExtensionData &data;  // I2C and shared connection data
};

namespace NintendoExtensionCtrl {
	template <class ControllerMap>
	class BuildControllerClass : private ExtensionController::ExtensionData, public ControllerMap {
	public:
		BuildControllerClass(NXC_I2C_TYPE& i2cBus = NXC_I2C_DEFAULT) :
			ExtensionData(i2cBus),  // data instance
			ControllerMap(static_cast<ExtensionData&>(*this))  // initialize ExtensionController instance with inherited ExtensionData
		{}

		using Shared = ControllerMap;  // Make controller class easily accessible
	};
}

// Public-facing version of the extension 'port' class that combines the 
// communication (ExtensionController) with a data instance (ExtensionData), but omits
// any controller-specific data maps.
using ExtensionPort = NintendoExtensionCtrl::BuildControllerClass<ExtensionController>;

#endif
