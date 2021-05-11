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

#include "uDrawTablet.h"

namespace NintendoExtensionCtrl {

constexpr IndexMap uDrawTablet_Shared::Maps::PenX_LSB;
constexpr IndexMap uDrawTablet_Shared::Maps::PenY_LSB;
constexpr ByteMap  uDrawTablet_Shared::Maps::PenX_MSB;
constexpr ByteMap  uDrawTablet_Shared::Maps::PenY_MSB;

constexpr IndexMap uDrawTablet_Shared::Maps::Pressure_LSB;
constexpr BitMap   uDrawTablet_Shared::Maps::Pressure_MSB;

constexpr BitMap   uDrawTablet_Shared::Maps::buttonLower;
constexpr BitMap   uDrawTablet_Shared::Maps::buttonUpper;

uint16_t uDrawTablet_Shared::penX() const {
	return (getControlData(Maps::PenX_MSB) << 8) | getControlData(Maps::PenX_LSB);
}

uint16_t uDrawTablet_Shared::penY() const {
	return (getControlData(Maps::PenY_MSB) << 8) | getControlData(Maps::PenY_LSB);
}

uint16_t uDrawTablet_Shared::penPressure() const {
    return (!getControlBit(Maps::Pressure_MSB) << 8) | getControlData(Maps::Pressure_LSB);
}

boolean uDrawTablet_Shared::buttonLower() const {
	return getControlBit(Maps::buttonLower);
}

boolean uDrawTablet_Shared::buttonUpper() const {
	return getControlBit(Maps::buttonUpper);
}

boolean uDrawTablet_Shared::penDetected() const {
	return penX() < 4095 && penY() < 4095;
}

void uDrawTablet_Shared::printDebug(Print& output) const {
	char buffer[60];
	
	const char penPrint = penDetected() ? 'Y' : 'N';
	const char lowerPrint = buttonLower() ? 'L' : '-';
	const char upperPrint = buttonUpper() ? 'U' : '-';

	output.print("uDrawTablet - ");
	sprintf(buffer,
		"Pen:(%4u, %4u) | Pressure:%3u | Pen Detect:%c | Buttons:%c%c",
			penX(), penY(), penPressure(), penPrint, lowerPrint, upperPrint);

	output.println(buffer);
}

}  // End "NintendoExtensionCtrl" namespace
