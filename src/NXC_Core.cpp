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

#include "NXC_Core.h"

namespace NintendoExtensionCtrl {
	boolean writePointer(NXC_I2C_TYPE& i2c, byte ptr) {
		i2c.beginTransmission(I2C_Addr);
		i2c.write(ptr);
		return i2c.endTransmission() == 0;  // 0 = No Error
	}

	boolean writeRegister(NXC_I2C_TYPE& i2c, byte reg, byte value) {
		i2c.beginTransmission(I2C_Addr);
		i2c.write(reg);
		i2c.write(value);
		return i2c.endTransmission() == 0;
	}

	boolean requestMultiple(NXC_I2C_TYPE& i2c, uint8_t requestSize, uint8_t * dataOut) {
		uint8_t nBytesRecv = i2c.readBytes(dataOut,
			i2c.requestFrom(I2C_Addr, requestSize));

		return (nBytesRecv == requestSize);  // Success if all bytes received
	}

	boolean readDataArray(NXC_I2C_TYPE& i2c, byte ptr, uint8_t requestSize, uint8_t * dataOut) {
		if (!writePointer(i2c, ptr)) { return false; }  // Set start for data read
		delayMicroseconds(I2C_ConversionDelay);  // Wait for data conversion
		return requestMultiple(i2c, requestSize, dataOut);
	}
}

