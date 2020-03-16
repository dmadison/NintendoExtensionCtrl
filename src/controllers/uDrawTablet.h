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

#ifndef NXC_uDrawTablet_h
#define NXC_uDrawTablet_h

#include "internal/ExtensionController.h"

namespace NintendoExtensionCtrl {
	class uDrawTablet_Shared : public ExtensionController {
	public:
		struct Maps {
			constexpr static CtrlIndex PenX_LSB = 0;
			constexpr static CtrlIndex PenY_LSB = 1;
			constexpr static ByteMap   PenX_MSB = ByteMap(2, 4, 0, 0);
			constexpr static ByteMap   PenY_MSB = ByteMap(2, 4, 4, 4);

			constexpr static CtrlIndex Pressure = 3;

			constexpr static BitMap    ButtonTip = { 5, 2 };
			constexpr static BitMap    Button1 = { 5, 1 };
			constexpr static BitMap    Button2 = { 5, 0 };
		};
		
		uDrawTablet_Shared(ExtensionData &dataRef) :
			ExtensionController(dataRef, ExtensionType::uDrawTablet) {}

		uDrawTablet_Shared(ExtensionPort &port) :
			uDrawTablet_Shared(port.getExtensionData()) {}

		// 8 bits, 0-255
		// Resets back to 0 if buttonTip is set HIGH or LOW.
		// So you can gauge how far into a soft/hard press it's in.
		uint8_t  penPressure() const;

		// 12 bits, 0-4095
		uint16_t penX() const;
		uint16_t penY() const;

		boolean  buttonTip() const;
		boolean  button1() const;
		boolean  button2() const;
		
		boolean  penDetected() const;

		void printDebug(Print& output = NXC_SERIAL_DEFAULT) const;
	};
}

using uDrawTablet = NintendoExtensionCtrl::BuildControllerClass
	<NintendoExtensionCtrl::uDrawTablet_Shared>;

#endif