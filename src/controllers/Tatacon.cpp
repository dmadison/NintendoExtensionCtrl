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

#include "Tatacon.h"

namespace NintendoExtensionCtrl {

constexpr BitMap   Tatacon_Shared::Maps::CenterLeft;
constexpr BitMap   Tatacon_Shared::Maps::CenterRight;

constexpr BitMap   Tatacon_Shared::Maps::RimLeft;
constexpr BitMap   Tatacon_Shared::Maps::RimRight;

boolean Tatacon_Shared::specificInit() {
	/* It uses only 4 bits in the first byte,
	 * so this is to limit wasted time reading the 
	 * remaining empty, unchanging, bytes.
	 */
	setRequestSize(1);
	return true;
}

boolean Tatacon_Shared::CenterLeft() const {
	return getControlBit(Maps::CenterLeft);
}

boolean Tatacon_Shared::CenterRight() const {
	return getControlBit(Maps::CenterRight);
}

boolean Tatacon_Shared::RimLeft() const {
	return getControlBit(Maps::RimLeft);
}

boolean Tatacon_Shared::RimRight() const {
	return getControlBit(Maps::RimRight);
}

void Tatacon_Shared::printDebug(Print& output) const {
	char buffer[60];

	char donPrintL = CenterLeft() ? 'D' : '-';
	char donPrintR = CenterRight() ? 'D' : '-';
	
	char kaPrintL = RimLeft() ? 'K' : '-';
	char kaPrintR = RimRight() ? 'K' : '-';

	output.print("Nunchuk - ");
	sprintf(buffer,
            "Center: (Left: %c  Right: %C) | Rim: (Left: %c  Right: %c)",
			donPrintL, donPrintR, kaPrintL, kaPrintR);

	output.println(buffer);
}

}  // End "NintendoExtensionCtrl" namespace
