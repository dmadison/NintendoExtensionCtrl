/*
*  Project     Nintendo Extension Controller Library
*  @author     Nullstalgia
*  @link       github.com/dmadison/NintendoExtensionCtrl
*  @license    LGPLv3 - Copyright (c) 2020 Nullstalgia
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

#ifndef NXC_Tatacon_h
#define NXC_Tatacon_h

#include "internal/ExtensionController.h"

namespace NintendoExtensionCtrl {
	class Tatacon_Shared : public ExtensionController {
	public:
		struct Maps {
			constexpr static BitMap   CenterLeft = { 5, 6 };
			constexpr static BitMap   CenterRight = { 5, 4 };
			constexpr static BitMap   RimLeft = { 5, 5 };
			constexpr static BitMap   RimRight = { 5, 3 };
		};
		
		Tatacon_Shared(ExtensionData &dataRef) :
			ExtensionController(dataRef, ExtensionType::Tatacon) {}

		Tatacon_Shared(ExtensionPort &port) :
			Tatacon_Shared(port.getExtensionData()) {}
			
		boolean specificInit();

		boolean CenterLeft() const; // ドン !!
		boolean CenterRight() const; // ヽ（゜∀゜○）ノ
	
		boolean RimLeft() const; // カッ !!
		boolean RimRight() const; // ヽ(。ゝω・)ノ☆

		void printDebug(Print& output = NXC_SERIAL_DEFAULT) const;
	};
}

using Tatacon = NintendoExtensionCtrl::BuildControllerClass
	<NintendoExtensionCtrl::Tatacon_Shared>;

#endif
