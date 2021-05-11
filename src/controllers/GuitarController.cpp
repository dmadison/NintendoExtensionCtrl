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

constexpr ByteMap GuitarControllerBase::Maps::JoyX;
constexpr ByteMap GuitarControllerBase::Maps::JoyY;

constexpr BitMap  GuitarControllerBase::Maps::ButtonPlus;
constexpr BitMap  GuitarControllerBase::Maps::ButtonMinus;

constexpr BitMap  GuitarControllerBase::Maps::StrumUp;
constexpr BitMap  GuitarControllerBase::Maps::StrumDown;

constexpr BitMap  GuitarControllerBase::Maps::FretGreen;
constexpr BitMap  GuitarControllerBase::Maps::FretRed;
constexpr BitMap  GuitarControllerBase::Maps::FretYellow;
constexpr BitMap  GuitarControllerBase::Maps::FretBlue;
constexpr BitMap  GuitarControllerBase::Maps::FretOrange;

constexpr ByteMap GuitarControllerBase::Maps::Whammy;
constexpr ByteMap GuitarControllerBase::Maps::Touchbar;

ExtensionType GuitarControllerBase::getExpectedType() const {
	return ExtensionType::GuitarController;
}

uint8_t GuitarControllerBase::joyX() const {
	return getControlData(Maps::JoyX);
}

uint8_t GuitarControllerBase::joyY() const {
	return getControlData(Maps::JoyY);
}

boolean GuitarControllerBase::strum() const {
	return strumUp() | strumDown();
}

boolean GuitarControllerBase::strumUp() const {
	return getControlBit(Maps::StrumUp);
}

boolean GuitarControllerBase::strumDown() const {
	return getControlBit(Maps::StrumDown);
}

boolean GuitarControllerBase::fretGreen() const {
	return getControlBit(Maps::FretGreen);
}

boolean GuitarControllerBase::fretRed() const {
	return getControlBit(Maps::FretRed);
}

boolean GuitarControllerBase::fretYellow() const {
	return getControlBit(Maps::FretYellow);
}

boolean GuitarControllerBase::fretBlue() const {
	return getControlBit(Maps::FretBlue);
}

boolean GuitarControllerBase::fretOrange() const {
	return getControlBit(Maps::FretOrange);
}

uint8_t GuitarControllerBase::whammyBar() const {
	return getControlData(Maps::Whammy);
}

uint8_t GuitarControllerBase::touchbar() const {
	return getControlData(Maps::Touchbar);
}

boolean GuitarControllerBase::touchGreen() const {
	return touchbar() != 0 && touchbar() <= 7;
}

boolean GuitarControllerBase::touchRed() const {
	return touchbar() >= 7 && touchbar() <= 13;
}

boolean GuitarControllerBase::touchYellow() const {
	return touchbar() >= 12 && touchbar() <= 21
		&& touchbar() != 15;	// The "not touched" value
}

boolean GuitarControllerBase::touchBlue() const {
	return touchbar() >= 20 && touchbar() <= 26;
}

boolean GuitarControllerBase::touchOrange() const {
	return touchbar() >= 26;
}

boolean GuitarControllerBase::buttonPlus() const {
	return getControlBit(Maps::ButtonPlus);
}

boolean GuitarControllerBase::buttonMinus() const {
	return getControlBit(Maps::ButtonMinus);
}

boolean GuitarControllerBase::supportsTouchbar() {
	if (touchbarData) {
		return true;
	}
	else if (touchbar() != 31) {  // '1' for all data bits
		touchbarData = true;
	}
	return false;
}

void GuitarControllerBase::printDebug(Print& output) {
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

	const char greenPrint = fretGreen() ? 'G' : fillCharacter;
	const char redPrint = fretRed() ? 'R' : fillCharacter;
	const char yellowPrint = fretYellow() ? 'Y' : fillCharacter;
	const char bluePrint = fretBlue() ? 'B' : fillCharacter;
	const char orangePrint = fretOrange() ? 'O' : fillCharacter;

	snprintf(buffer, sizeof(buffer),
		"%c | %c%c%c%c%c | W:%2u ",
		strumPrint,
		greenPrint, redPrint, yellowPrint, bluePrint, orangePrint,
		whammyBar());
	output.print(buffer);
	buffer[0] = 0;

	// Touchbar, if World Controller
	if (supportsTouchbar()) {
		const char greenPrint = touchGreen() ? 'G' : fillCharacter;
		const char redPrint = touchRed() ? 'R' : fillCharacter;
		const char yellowPrint = touchYellow() ? 'Y' : fillCharacter;
		const char bluePrint = touchBlue() ? 'B' : fillCharacter;
		const char orangePrint = touchOrange() ? 'O' : fillCharacter;

		snprintf(buffer, sizeof(buffer),
			"Touch:%2u - %c%c%c%c%c | ",
			touchbar(),
			greenPrint, redPrint, yellowPrint, bluePrint, orangePrint);
		output.print(buffer);
		buffer[0] = 0;
	}

	// Joy + Plus/Minus
	const char plusPrint = buttonPlus() ? '+' : fillCharacter;
	const char minusPrint = buttonMinus() ? '-' : fillCharacter;

	snprintf(buffer, sizeof(buffer),
		"%c%c | Joy:(%2u, %2u)",
		minusPrint, plusPrint,
		joyX(), joyY());
	output.println(buffer);
}

}  // End "NintendoExtensionCtrl" namespace
