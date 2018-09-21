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

#ifndef NXC_Comms_h
#define NXC_Comms_h

#include "Arduino.h"
#include "NXC_Identity.h"

#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MKL26Z64__) || \
    defined(__MK64FX512__) || defined(__MK66FX1M0__) // Teensy 3.0/3.1-3.2/LC/3.5/3.6
#include <i2c_t3.h>
#define NXC_I2C_TYPE i2c_t3
#else
#include <Wire.h>  // All other platforms
#define NXC_I2C_TYPE TwoWire
#endif

#define NXC_I2C_DEFAULT Wire
#define NXC_SERIAL_DEFAULT Serial

namespace NintendoExtensionCtrl {
	const long I2C_ConversionDelay = 175;  // Microseconds, ~200 on AVR
	const uint8_t I2C_Addr = 0x52;  // Address for all extension controllers

	const uint8_t ID_Size = 6;

	// Generic I2C slave device control functions
	// ------------------------------------------
	inline boolean i2c_writePointer(NXC_I2C_TYPE &i2c, byte addr, byte ptr) {
		i2c.beginTransmission(addr);
		i2c.write(ptr);
		return i2c.endTransmission() == 0;  // 0 = No Error
	}

	inline boolean i2c_writeRegister(NXC_I2C_TYPE &i2c, byte addr, byte reg, byte value) {
		i2c.beginTransmission(addr);
		i2c.write(reg);
		i2c.write(value);
		return i2c.endTransmission() == 0;
	}

	inline boolean i2c_requestMultiple(NXC_I2C_TYPE &i2c, byte addr, uint8_t requestSize, uint8_t * dataOut) {
		uint8_t nBytesRecv = i2c.readBytes(dataOut,
			i2c.requestFrom(addr, requestSize));

		return (nBytesRecv == requestSize);  // Success if all bytes received
	}

	inline boolean i2c_readDataArray(NXC_I2C_TYPE &i2c, byte addr, byte ptr, uint8_t requestSize, uint8_t * dataOut) {
		if (!i2c_writePointer(i2c, addr, ptr)) { return false; }  // Set start for data read
		delayMicroseconds(I2C_ConversionDelay);  // Wait for data conversion
		return i2c_requestMultiple(i2c, addr, requestSize, dataOut);
	}

	// Extension controller specific I2C functions
	// -------------------------------------------
	// Control Data
	inline boolean initialize(NXC_I2C_TYPE &i2c) {
		/* Initialization for unencrypted communication.
		* *Should* work on all devices, genuine + 3rd party.
		* See http://wiibrew.org/wiki/Wiimote/Extension_Controllers
		*/
		if (!i2c_writeRegister(i2c, I2C_Addr, 0xF0, 0x55)) { return false; }
		delay(10);
		if (!i2c_writeRegister(i2c, I2C_Addr, 0xFB, 0x00)) { return false; }
		delay(20);
		return true;
	}

	inline boolean requestData(NXC_I2C_TYPE &i2c, uint8_t ptr, size_t size, uint8_t * data) {
		return i2c_readDataArray(i2c, I2C_Addr, ptr, size, data);
	}

	inline boolean requestControlData(NXC_I2C_TYPE &i2c, size_t size, uint8_t * controlData) {
		return i2c_readDataArray(i2c, I2C_Addr, 0x00, size, controlData);
	}

	// Identity
	inline boolean requestIdentity(NXC_I2C_TYPE &i2c, uint8_t * idData) {
		return i2c_readDataArray(i2c, I2C_Addr, 0xFA, ID_Size, idData);
	}

	inline ExtensionType identifyController(NXC_I2C_TYPE &i2c) {
		uint8_t idData[ID_Size];

		if (!requestIdentity(i2c, idData)) {
			return ExtensionType::NoController;  // Bad response from device
		}
		return identifyController(idData);
	}
}

#endif
