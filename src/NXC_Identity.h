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

#ifndef NXC_Identity_h
#define NXC_Identity_h

#include "Arduino.h"
#include "NXC_Comms.h"

enum NXC_ControllerType {
	NXC_NoController,
	NXC_UnknownController,
	NXC_Nunchuk,
	NXC_ClassicController,
	NXC_GuitarController,
	NXC_DrumController,
	NXC_DJTurntable,
};

namespace NintendoExtensionCtrl {
	inline NXC_ControllerType identifyController(const uint8_t * idData) {
		if (idData[2] == 0xA4 && idData[3] == 0x20) {  // All valid IDs
			// Nunchuk ID: 0x0000
			if (idData[4] == 0x00 && idData[5] == 0x00) {
				return NXC_Nunchuk;
			}

			// Classic Con. ID: 0x0101
			else if (idData[4] == 0x01 && idData[5] == 0x01) {
				return NXC_ClassicController;
			}

			// Guitar Hero Controllers: 0x##00, 0xA420, 0x0103
			else if (idData[1] == 0x00
				&& idData[4] == 0x01 && idData[5] == 0x03) {

				// Guitar: 0x00
				if (idData[0] == 0x00) {
					return NXC_GuitarController;
				}
				// Drums: 0x01
				else if (idData[0] == 0x01) {
					return NXC_DrumController;
				}
				// DJ Turntable: 0x03
				else if (idData[0] == 0x03) {
					return NXC_DJTurntable;
				}
			}
		}

		return NXC_UnknownController;  // No matches
	}

	class ExtensionIdentifier {
	public:
		ExtensionIdentifier(ExtensionComms &comms) : i2c(comms) {}

		boolean requestIdentity(uint8_t * idData) {
			return i2c.readDataArray(IDPointer, IDSize, idData);
		}

		NXC_ControllerType identifyController() {
			uint8_t idData[IDSize];

			if (!requestIdentity(idData)) {
				return NXC_NoController;  // Bad response from device
			}
			return identifyController(idData);
		}

		NXC_ControllerType identifyController(const uint8_t * idData) {
			return NintendoExtensionCtrl::identifyController(idData);
		}
		
		static const uint8_t IDPointer = 0xFA;
		static const uint8_t IDSize = 6;
	private:
		ExtensionComms & i2c;
	};
}

#endif
