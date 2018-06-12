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

#ifndef NXC_ClassicController_h
#define NXC_ClassicController_h

#include "ExtensionController.h"

namespace NintendoExtensionCtrl {
	class ClassicController_Data : private ControlDataMap {
	public:
		using ControlDataMap::ControlDataMap;

		uint8_t leftJoyX() const;  // 6 bits, 0-63
		uint8_t leftJoyY() const;

		uint8_t rightJoyX() const;  // 5 bits, 0-31
		uint8_t rightJoyY() const;

		boolean dpadUp() const;
		boolean dpadDown() const;
		boolean dpadLeft() const;
		boolean dpadRight() const;

		boolean buttonA() const;
		boolean buttonB() const;
		boolean buttonX() const;
		boolean buttonY() const;

		uint8_t triggerL() const;  // 5 bits, 0-31
		uint8_t triggerR() const;

		boolean buttonL() const;
		boolean buttonR() const;

		boolean buttonZL() const;
		boolean buttonZR() const;

		boolean buttonStart() const;
		boolean buttonSelect() const;

		boolean buttonPlus() const;
		boolean buttonMinus() const;

		boolean buttonHome() const;

		void printDebug(Stream& stream = NXC_SERIAL_DEFAULT) const;
	};
}

typedef NintendoExtensionCtrl::BuildControllerClass
	<NintendoExtensionCtrl::ClassicController_Data, ExtensionType::ClassicController>
	ClassicController;

#endif
