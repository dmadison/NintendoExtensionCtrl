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

#ifndef NXC_GuitarController_h
#define NXC_GuitarController_h

#include "ExtensionController.h"

namespace NintendoExtensionCtrl {
	namespace DataMaps {
		class GuitarController : private ControlDataMap {
		public:
			using ControlDataMap::ControlDataMap;

			uint8_t joyX() const;  // 6 bits, 0-63
			uint8_t joyY() const;

			boolean strum() const;
			boolean strumUp() const;
			boolean strumDown() const;

			boolean fretGreen() const;
			boolean fretRed() const;
			boolean fretYellow() const;
			boolean fretBlue() const;
			boolean fretOrange() const;

			uint8_t whammyBar() const;  // 5 bits, 0-31 (starting at ~15-16)

			uint8_t touchbar() const;  // 5 bits, 0-31
			boolean touchGreen() const;
			boolean touchRed() const;
			boolean touchYellow() const;
			boolean touchBlue() const;
			boolean touchOrange() const;

			boolean buttonPlus() const;
			boolean buttonMinus() const;

			void printDebug(Stream& stream = NXC_SERIAL_DEFAULT);

			boolean supportsTouchbar();

		private:
			boolean touchbarData = false;  // Flag for touchbar data found
		};
	}
}

class GuitarController : public ExtensionController, public NintendoExtensionCtrl::DataMaps::GuitarController {
public:
	typedef NintendoExtensionCtrl::DataMaps::GuitarController DataMap;

	GuitarController(NXC_I2C_TYPE& i2cBus = NXC_I2C_DEFAULT) :
		ExtensionController(i2cBus, NXC_GuitarController, 6),
		DataMap(*(static_cast<ExtensionController*>(this))) {}

	GuitarController(ExtensionData& busData) :
		ExtensionController(busData, NXC_GuitarController, 6),
		DataMap(*(static_cast<ExtensionController*>(this))) {}

	using DataMap::printDebug;
};

#endif
