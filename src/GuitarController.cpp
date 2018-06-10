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

#include "GuitarController.h"

namespace NintendoExtensionCtrl {

uint8_t GuitarController_Data::joyX() const {
	return getControlData(0) & 0x3F;
}

uint8_t GuitarController_Data::joyY() const {
	return getControlData(1) & 0x3F;
}

boolean GuitarController_Data::strum() const {
	return strumUp() | strumDown();
}

boolean GuitarController_Data::strumUp() const {
	return getControlBit(5, 0);
}

boolean GuitarController_Data::strumDown() const {
	return getControlBit(4, 6);
}

boolean GuitarController_Data::fretGreen() const {
	return getControlBit(5, 4);
}

boolean GuitarController_Data::fretRed() const {
	return getControlBit(5, 6);
}

boolean GuitarController_Data::fretYellow() const {
	return getControlBit(5, 3);
}

boolean GuitarController_Data::fretBlue() const {
	return getControlBit(5, 5);
}

boolean GuitarController_Data::fretOrange() const {
	return getControlBit(5, 7);
}

uint8_t GuitarController_Data::whammyBar() const {
	return getControlData(3) & 0x1F;
}

uint8_t GuitarController_Data::touchbar() const {
	return getControlData(2) & 0x1F;
}

boolean GuitarController_Data::touchGreen() const {
	return touchbar() != 0 && touchbar() <= 7;
}

boolean GuitarController_Data::touchRed() const {
	return touchbar() >= 7 && touchbar() <= 13;
}

boolean GuitarController_Data::touchYellow() const {
	return touchbar() >= 12 && touchbar() <= 21
		&& touchbar() != 15;	// The "not touched" value
}

boolean GuitarController_Data::touchBlue() const {
	return touchbar() >= 20 && touchbar() <= 26;
}

boolean GuitarController_Data::touchOrange() const {
	return touchbar() >= 26;
}

boolean GuitarController_Data::buttonPlus() const {
	return getControlBit(4, 2);
}

boolean GuitarController_Data::buttonMinus() const {
	return getControlBit(4, 4);
}

boolean GuitarController_Data::supportsTouchbar() {
	if (touchbarData) {
		return true;
	}
	else if (touchbar() != 31) {  // '1' for all data bits
		touchbarData = true;
	}
	return false;
}

void GuitarController_Data::printDebug(Stream& stream) {
	const char fillCharacter = '_';

	char buffer[25];

	stream.print("Guitar: ");

	// Strum + Fret Buttons
	char strumPrint = fillCharacter;
	if (strumUp()) {
		strumPrint = '^';
	}
	else if (strumDown()) {  // Mutually exclusive
		strumPrint = 'v';
	}

	char greenPrint = fretGreen() ? 'G' : fillCharacter;
	char redPrint = fretRed() ? 'R' : fillCharacter;
	char yellowPrint = fretYellow() ? 'Y' : fillCharacter;
	char bluePrint = fretBlue() ? 'B' : fillCharacter;
	char orangePrint = fretOrange() ? 'O' : fillCharacter;

	sprintf(buffer,
		"%c | %c%c%c%c%c | W:%2u ",
		strumPrint,
		greenPrint, redPrint, yellowPrint, bluePrint, orangePrint,
		whammyBar());
	stream.print(buffer);
	buffer[0] = 0;

	// Touchbar, if World Controller
	if (supportsTouchbar()) {
		greenPrint = touchGreen() ? 'G' : fillCharacter;
		redPrint = touchRed() ? 'R' : fillCharacter;
		yellowPrint = touchYellow() ? 'Y' : fillCharacter;
		bluePrint = touchBlue() ? 'B' : fillCharacter;
		orangePrint = touchOrange() ? 'O' : fillCharacter;

		sprintf(buffer,
			"Touch:%2u - %c%c%c%c%c | ",
			touchbar(),
			greenPrint, redPrint, yellowPrint, bluePrint, orangePrint);
		stream.print(buffer);
		buffer[0] = 0;
	}

	// Joy + Plus/Minus
	char plusPrint = buttonPlus() ? '+' : fillCharacter;
	char minusPrint = buttonMinus() ? '-' : fillCharacter;

	sprintf(buffer,
		"%c%c | Joy:(%2u, %2u)",
		minusPrint, plusPrint,
		joyX(), joyY());
	stream.println(buffer);
}

}  // End "NintendoExtensionCtrl" namespace
