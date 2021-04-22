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

constexpr ByteMap GuitarController_Shared::Maps::JoyX;
constexpr ByteMap GuitarController_Shared::Maps::JoyY;

constexpr BitMap  GuitarController_Shared::Maps::ButtonPlus;
constexpr BitMap  GuitarController_Shared::Maps::ButtonMinus;

constexpr BitMap  GuitarController_Shared::Maps::StrumUp;
constexpr BitMap  GuitarController_Shared::Maps::StrumDown;

constexpr BitMap  GuitarController_Shared::Maps::FretGreen;
constexpr BitMap  GuitarController_Shared::Maps::FretRed;
constexpr BitMap  GuitarController_Shared::Maps::FretYellow;
constexpr BitMap  GuitarController_Shared::Maps::FretBlue;
constexpr BitMap  GuitarController_Shared::Maps::FretOrange;

constexpr ByteMap GuitarController_Shared::Maps::Whammy;
constexpr ByteMap GuitarController_Shared::Maps::Touchbar;

ExtensionType GuitarController_Shared::getControllerType() const {
	return ExtensionType::GuitarController;
}

uint8_t GuitarController_Shared::joyX() const {
	return getControlData(Maps::JoyX);
}

uint8_t GuitarController_Shared::joyY() const {
	return getControlData(Maps::JoyY);
}

boolean GuitarController_Shared::strum() const {
	return strumUp() | strumDown();
}

boolean GuitarController_Shared::strumUp() const {
	return getControlBit(Maps::StrumUp);
}

boolean GuitarController_Shared::strumDown() const {
	return getControlBit(Maps::StrumDown);
}

boolean GuitarController_Shared::fretGreen() const {
	return getControlBit(Maps::FretGreen);
}

boolean GuitarController_Shared::fretRed() const {
	return getControlBit(Maps::FretRed);
}

boolean GuitarController_Shared::fretYellow() const {
	return getControlBit(Maps::FretYellow);
}

boolean GuitarController_Shared::fretBlue() const {
	return getControlBit(Maps::FretBlue);
}

boolean GuitarController_Shared::fretOrange() const {
	return getControlBit(Maps::FretOrange);
}

uint8_t GuitarController_Shared::whammyBar() const {
	return getControlData(Maps::Whammy);
}

uint8_t GuitarController_Shared::touchbar() const {
	return getControlData(Maps::Touchbar);
}

boolean GuitarController_Shared::touchGreen() const {
	return touchbar() != 0 && touchbar() <= 7;
}

boolean GuitarController_Shared::touchRed() const {
	return touchbar() >= 7 && touchbar() <= 13;
}

boolean GuitarController_Shared::touchYellow() const {
	return touchbar() >= 12 && touchbar() <= 21
		&& touchbar() != 15;	// The "not touched" value
}

boolean GuitarController_Shared::touchBlue() const {
	return touchbar() >= 20 && touchbar() <= 26;
}

boolean GuitarController_Shared::touchOrange() const {
	return touchbar() >= 26;
}

boolean GuitarController_Shared::buttonPlus() const {
	return getControlBit(Maps::ButtonPlus);
}

boolean GuitarController_Shared::buttonMinus() const {
	return getControlBit(Maps::ButtonMinus);
}

boolean GuitarController_Shared::supportsTouchbar() {
	if (touchbarData) {
		return true;
	}
	else if (touchbar() != 31) {  // '1' for all data bits
		touchbarData = true;
	}
	return false;
}

void GuitarController_Shared::printDebug(Print& output) {
	const char fillCharacter = '_';

	char buffer[25];

	output.print("Guitar: ");

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
	output.print(buffer);
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
		output.print(buffer);
		buffer[0] = 0;
	}

	// Joy + Plus/Minus
	char plusPrint = buttonPlus() ? '+' : fillCharacter;
	char minusPrint = buttonMinus() ? '-' : fillCharacter;

	sprintf(buffer,
		"%c%c | Joy:(%2u, %2u)",
		minusPrint, plusPrint,
		joyX(), joyY());
	output.println(buffer);
}

}  // End "NintendoExtensionCtrl" namespace
