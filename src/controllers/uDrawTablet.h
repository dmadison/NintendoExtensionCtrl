/*
*  Project     Nintendo Extension Controller Library
*  @author     nullstalgia	
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

#ifndef NXC_UDRAWTABLET_H
#define NXC_UDRAWTABLET_H

#include "internal/ExtensionController.h"

namespace NintendoExtensionCtrl {
	class uDrawTabletBase : public ExtensionController {
	public:
		struct Maps {
			constexpr static IndexMap PenX_LSB = 0;
			constexpr static IndexMap PenY_LSB = 1;
			constexpr static ByteMap  PenX_MSB = ByteMap(2, 4, 0, 0);
			constexpr static ByteMap  PenY_MSB = ByteMap(2, 4, 4, 4);

			constexpr static IndexMap Pressure_LSB = 3;

			constexpr static BitMap   Pressure_MSB = { 5, 2 };
			constexpr static BitMap   ButtonLower = { 5, 1 };
			constexpr static BitMap   ButtonUpper = { 5, 0 };
		};
		
		using ExtensionController::ExtensionController;

		ExtensionType getExpectedType() const;

		uint16_t penX() const;  // 12 bits, 0-4095
		uint16_t penY() const;

		uint16_t penPressure() const;  // 9 bits, 0-511

		boolean  buttonLower() const;
		boolean  buttonUpper() const;
		
		boolean  penDetected() const;

		void printDebug(Print& output = NXC_SERIAL_DEFAULT) const;
	};
}

using uDrawTablet = NintendoExtensionCtrl::BuildControllerClass
	<NintendoExtensionCtrl::uDrawTabletBase>;

#endif
