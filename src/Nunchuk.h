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

#ifndef NXC_Nunchuk_h
#define NXC_Nunchuk_h

#include "ExtensionController.h"

namespace NintendoExtensionCtrl {
	class Nunchuk_Data : private ControlDataMap {
	public:
		using ControlDataMap::ControlDataMap;

		uint8_t joyX() const;  // 8 bits, 0-255
		uint8_t joyY() const;

		uint16_t accelX() const;  // 10 bits, 0-1023
		uint16_t accelY() const;
		uint16_t accelZ() const;

		boolean buttonC() const;
		boolean buttonZ() const;

		float rollAngle() const;  // -180.0 to 180.0
		float pitchAngle() const;

		void printDebug(Stream& stream = NXC_SERIAL_DEFAULT) const;
	};
}

typedef NintendoExtensionCtrl::BuildControllerClass
	<NintendoExtensionCtrl::Nunchuk_Data, NXC_Nunchuk, 6>
	Nunchuk;

#endif
