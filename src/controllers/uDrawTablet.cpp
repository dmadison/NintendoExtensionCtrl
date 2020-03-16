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

constexpr CtrlIndex uDrawTablet_Shared::Maps::PenX_LSB;
constexpr CtrlIndex uDrawTablet_Shared::Maps::PenY_LSB;
constexpr ByteMap   uDrawTablet_Shared::Maps::PenX_MSB;
constexpr ByteMap   uDrawTablet_Shared::Maps::PenY_MSB;

constexpr CtrlIndex uDrawTablet_Shared::Maps::Pressure;

constexpr BitMap    uDrawTablet_Shared::Maps::ButtonTip;
constexpr BitMap    uDrawTablet_Shared::Maps::Button1;
constexpr BitMap    uDrawTablet_Shared::Maps::Button2;

uint16_t uDrawTablet_Shared::penX() const {
	return (getControlData(Maps::PenX_MSB) << 8) | getControlData(Maps::PenX_LSB);
}

uint16_t uDrawTablet_Shared::penY() const {
	return (getControlData(Maps::PenY_MSB) << 8) | getControlData(Maps::PenY_LSB);
}

uint8_t uDrawTablet_Shared::penPressure() const {
	return getControlData(Maps::Pressure);
}

boolean uDrawTablet_Shared::buttonTip() const {
	return !getControlBit(Maps::ButtonTip);
}

boolean uDrawTablet_Shared::button1() const {
	return getControlBit(Maps::Button1);
}

boolean uDrawTablet_Shared::button2() const {
	return getControlBit(Maps::Button2);
}

boolean uDrawTablet_Shared::penDetected() const {
	//return !(4095*2) == (penX()+penY());
	if(penX() != 4095 || penY() != 4095){
		return true;
	}
	return false;
}

void uDrawTablet_Shared::printDebug(Print& output) const {
	char buffer[70];
	
	char penPrint = penDetected() ? 'Y' : 'N';
	char tipPrint = buttonTip() ? 'T' : '-';
	char b1Print = button1() ? '1' : '-';
	char b2Print = button2() ? '2' : '-';

	output.print("uDrawTablet - ");
	sprintf(buffer,
		"Pen:(%4u, %4u) | Pressure:%4u | Pen Detect: %c | Buttons: %c%c%c",
		penX(), penY(), penPressure(), penPrint, tipPrint, b1Print, b2Print);

	output.println(buffer);
}

}  // End "NintendoExtensionCtrl" namespace
