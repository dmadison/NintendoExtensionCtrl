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

#include "ClassicController.h"

namespace NintendoExtensionCtrl {

uint8_t ClassicController_Data::leftJoyX() const {
	return getControlData(0) & 0x3F;
}

uint8_t ClassicController_Data::leftJoyY() const {
	return getControlData(1) & 0x3F;
}

uint8_t ClassicController_Data::rightJoyX() const {
	return ((getControlData(0) >> 3) & 0x18) | ((getControlData(1) >> 5) &  0x06 ) | (getControlData(2) >> 7);
}

uint8_t ClassicController_Data::rightJoyY() const {
	return getControlData(2) & 0x1F;
}

boolean ClassicController_Data::dpadUp() const {
	return getControlBit(5, 0);
}

boolean ClassicController_Data::dpadDown() const {
	return getControlBit(4, 6);
}

boolean ClassicController_Data::dpadLeft() const {
	return getControlBit(5, 1);
}

boolean ClassicController_Data::dpadRight() const {
	return getControlBit(4, 7);
}

boolean ClassicController_Data::buttonA() const {
	return getControlBit(5, 4);
}

boolean ClassicController_Data::buttonB() const {
	return getControlBit(5, 6);
}

boolean ClassicController_Data::buttonX() const {
	return getControlBit(5, 3);
}

boolean ClassicController_Data::buttonY() const {
	return getControlBit(5, 5);
}

uint8_t ClassicController_Data::triggerL() const {
	return ((getControlData(2) & 0x60) >> 2) | getControlData(3) >> 5;
}

uint8_t ClassicController_Data::triggerR() const {
	return getControlData(3) & 0x1F;
}

boolean ClassicController_Data::buttonL() const {
	return getControlBit(4, 5);
}

boolean ClassicController_Data::buttonR() const {
	return getControlBit(4, 1);
}

boolean ClassicController_Data::buttonZL() const {
	return getControlBit(5, 7);
}

boolean ClassicController_Data::buttonZR() const {
	return getControlBit(5, 2);
}

boolean ClassicController_Data::buttonStart() const {
	return buttonPlus();
}

boolean ClassicController_Data::buttonSelect() const {
	return buttonMinus();
}

boolean ClassicController_Data::buttonPlus() const {
	return getControlBit(4, 2);
}

boolean ClassicController_Data::buttonMinus() const {
	return getControlBit(4, 4);
}

boolean ClassicController_Data::buttonHome() const {
	return getControlBit(4, 3);
}

void ClassicController_Data::printDebug(Print& output) const {
	const char fillCharacter = '_';

	char buffer[62];

	char dpadLPrint = dpadLeft() ? '<' : fillCharacter;
	char dpadUPrint = dpadUp() ? '^' : fillCharacter;
	char dpadDPrint = dpadDown() ? 'v' : fillCharacter;
	char dpadRPrint = dpadRight() ? '>' : fillCharacter;

	char aButtonPrint = buttonA() ? 'A' : fillCharacter;
	char bButtonPrint = buttonB() ? 'B' : fillCharacter;
	char xButtonPrint = buttonX() ? 'X' : fillCharacter;
	char yButtonPrint = buttonY() ? 'Y' : fillCharacter;

	char plusPrint = buttonPlus() ? '+' : fillCharacter;
	char minusPrint = buttonMinus() ? '-' : fillCharacter;
	char homePrint = buttonHome() ? 'H' : fillCharacter;

	char ltButtonPrint = buttonL() ? 'X' : fillCharacter;
	char rtButtonPrint = buttonR() ? 'X' : fillCharacter;

	char zlButtonPrint = buttonZL() ? 'L' : fillCharacter;
	char zrButtonPrint = buttonZR() ? 'R' : fillCharacter;

	output.print("Classic ");
	sprintf(buffer,
		"%c%c%c%c | %c%c%c | %c%c%c%c L:(%2u, %2u) R:(%2u, %2u) | LT:%2u%c RT:%2u%c Z:%c%c",
		dpadLPrint, dpadUPrint, dpadDPrint, dpadRPrint,
		minusPrint, homePrint, plusPrint,
		aButtonPrint, bButtonPrint, xButtonPrint, yButtonPrint,
		leftJoyX(), leftJoyY(), rightJoyX(), rightJoyY(),
		triggerL(), ltButtonPrint, triggerR(), rtButtonPrint,
		zlButtonPrint, zrButtonPrint);
	output.println(buffer);
}

}  // End "NintendoExtensionCtrl" namespace
