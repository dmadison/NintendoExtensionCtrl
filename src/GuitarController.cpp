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

GuitarController::GuitarController(NXC_I2C_TYPE& i2cBus) : ExtensionController(i2cBus, NXC_GuitarController, 6) {}

uint8_t GuitarController::joyX() {
	return getControlData(0) & 0x3F;
}

uint8_t GuitarController::joyY() {
	return getControlData(1) & 0x3F;
}

boolean GuitarController::strum() {
	return strumUp() | strumDown();
}

boolean GuitarController::strumUp() {
	return extractControlBit(5, 0);
}

boolean GuitarController::strumDown() {
	return extractControlBit(4, 6);
}

boolean GuitarController::fretGreen() {
	return extractControlBit(5, 4);
}

boolean GuitarController::fretRed() {
	return extractControlBit(5, 6);
}

boolean GuitarController::fretYellow() {
	return extractControlBit(5, 3);
}

boolean GuitarController::fretBlue() {
	return extractControlBit(5, 5);
}

boolean GuitarController::fretOrange() {
	return extractControlBit(5, 7);
}

uint8_t GuitarController::whammyBar() {
	return getControlData(3) & 0x1F;
}

uint8_t GuitarController::touchbar() {
	return getControlData(2) & 0x1F;
}

boolean GuitarController::touchGreen() {
	return touchbar() != 0 && touchbar() <= 7;
}

boolean GuitarController::touchRed() {
	return touchbar() >= 7 && touchbar() <= 13;
}

boolean GuitarController::touchYellow() {
	return touchbar() >= 12 && touchbar() <= 21
		&& touchbar() != 15;	// The "not touched" value
}

boolean GuitarController::touchBlue() {
	return touchbar() >= 20 && touchbar() <= 26;
}

boolean GuitarController::touchOrange() {
	return touchbar() >= 26;
}

boolean GuitarController::buttonPlus() {
	return extractControlBit(4, 2);
}

boolean GuitarController::buttonMinus() {
	return extractControlBit(4, 4);
}

boolean GuitarController::supportsTouchbar() {
	if (touchbarData) {
		return true;
	}
	else if (touchbar() != 31) {  // '1' for all data bits
		touchbarData = true;
	}
	return false;
}

void GuitarController::printDebug(Stream& stream) {
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
