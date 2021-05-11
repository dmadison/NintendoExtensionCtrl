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

constexpr IndexMap uDrawTabletBase::Maps::PenX_LSB;
constexpr IndexMap uDrawTabletBase::Maps::PenY_LSB;
constexpr ByteMap  uDrawTabletBase::Maps::PenX_MSB;
constexpr ByteMap  uDrawTabletBase::Maps::PenY_MSB;

constexpr IndexMap uDrawTabletBase::Maps::Pressure_LSB;
constexpr BitMap   uDrawTabletBase::Maps::Pressure_MSB;

constexpr BitMap   uDrawTabletBase::Maps::ButtonLower;
constexpr BitMap   uDrawTabletBase::Maps::ButtonUpper;

ExtensionType uDrawTabletBase::getExpectedType() const {
	return ExtensionType::uDrawTablet;
}

uint16_t uDrawTabletBase::penX() const {
	return (getControlData(Maps::PenX_MSB) << 8) | getControlData(Maps::PenX_LSB);
}

uint16_t uDrawTabletBase::penY() const {
	return (getControlData(Maps::PenY_MSB) << 8) | getControlData(Maps::PenY_LSB);
}

uint16_t uDrawTabletBase::penPressure() const {
    return (!getControlBit(Maps::Pressure_MSB) << 8) | getControlData(Maps::Pressure_LSB);
}

boolean uDrawTabletBase::buttonLower() const {
	return getControlBit(Maps::ButtonLower);
}

boolean uDrawTabletBase::buttonUpper() const {
	return getControlBit(Maps::ButtonUpper);
}

boolean uDrawTabletBase::penDetected() const {
	return penX() < 4095 && penY() < 4095;
}

void uDrawTabletBase::printDebug(Print& output) const {
	// 59 characters, 1 terminating null, and 4 extra so the compiler stops
	// complaining about not having enough buffer space for the full 16 bit
	// values (5 characters each) that can fit in the type
	char buffer[64];
	
	const char penPrint = penDetected() ? 'Y' : 'N';
	const char lowerPrint = buttonLower() ? 'L' : '-';
	const char upperPrint = buttonUpper() ? 'U' : '-';

	output.print("uDrawTablet - ");
	snprintf(buffer, sizeof(buffer),
		"Pen:(%4u, %4u) | Pressure:%3u | Pen Detect:%c | Buttons:%c%c",
			penX(), penY(), penPressure(), penPrint, lowerPrint, upperPrint);

	output.println(buffer);
}

}  // End "NintendoExtensionCtrl" namespace
