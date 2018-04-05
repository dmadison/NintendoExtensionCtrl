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

ClassicController::ClassicController() : ExtensionController(NXC_ClassicController, 6) {}

uint8_t ClassicController::leftJoyX() {
	return getControlData(0) & 0x3F;
}

uint8_t ClassicController::leftJoyY() {
	return getControlData(1) & 0x3F;
}

uint8_t ClassicController::rightJoyX() {
	return ((getControlData(0) >> 3) & 0x18) | ((getControlData(1) >> 5) &  0x06 ) | (getControlData(2) >> 7);
}

uint8_t ClassicController::rightJoyY() {
	return getControlData(2) & 0x1F;
}

boolean ClassicController::dpadUp() {
	return extractControlBit(5, 0);
}

boolean ClassicController::dpadDown() {
	return extractControlBit(4, 6);
}

boolean ClassicController::dpadLeft() {
	return extractControlBit(5, 1);
}

boolean ClassicController::dpadRight() {
	return extractControlBit(4, 7);
}

boolean ClassicController::buttonA() {
	return extractControlBit(5, 4);
}

boolean ClassicController::buttonB() {
	return extractControlBit(5, 6);
}

boolean ClassicController::buttonX() {
	return extractControlBit(5, 3);
}

boolean ClassicController::buttonY() {
	return extractControlBit(5, 5);
}

uint8_t ClassicController::triggerL() {
	return ((getControlData(2) & 0x60) >> 2) | getControlData(3) >> 5;
}

uint8_t ClassicController::triggerR() {
	return getControlData(3) & 0x1F;
}

boolean ClassicController::buttonLT() {
	return extractControlBit(4, 5);
}

boolean ClassicController::buttonRT() {
	return extractControlBit(4, 1);
}

boolean ClassicController::buttonZL() {
	return extractControlBit(5, 7);
}

boolean ClassicController::buttonZR() {
	return extractControlBit(5, 2);
}

boolean ClassicController::buttonPlus() {
	return extractControlBit(4, 2);
}

boolean ClassicController::buttonMinus() {
	return extractControlBit(4, 4);
}

boolean ClassicController::buttonHome() {
	return extractControlBit(4, 3);
}

void ClassicController::printDebug(Stream& stream) {
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

	char ltButtonPrint = buttonLT() ? 'X' : fillCharacter;
	char rtButtonPrint = buttonRT() ? 'X' : fillCharacter;

	char zlButtonPrint = buttonZL() ? 'L' : fillCharacter;
	char zrButtonPrint = buttonZR() ? 'R' : fillCharacter;

	stream.print("Classic ");
	sprintf(buffer,
		"%c%c%c%c | %c%c%c | %c%c%c%c L:(%2u, %2u) R:(%2u, %2u) | LT:%2u%c RT:%2u%c Z:%c%c",
		dpadLPrint, dpadUPrint, dpadDPrint, dpadRPrint,
		minusPrint, homePrint, plusPrint,
		aButtonPrint, bButtonPrint, xButtonPrint, yButtonPrint,
		leftJoyX(), leftJoyY(), rightJoyX(), rightJoyY(),
		triggerL(), ltButtonPrint, triggerR(), rtButtonPrint,
		zlButtonPrint, zrButtonPrint);
	stream.println(buffer);
}
