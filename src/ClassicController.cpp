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

ClassicController::ClassicController() : ExtensionController(6, ID_ClassicCon) {}

uint8_t ClassicController::leftJoyX() {
	return controlData[0] & 0x3F;
}

uint8_t ClassicController::leftJoyY() {
	return controlData[1] & 0x3F;
}

uint8_t ClassicController::rightJoyX() {
	return ((controlData[0] >> 3) & 0x18) | ((controlData[1] >> 5) &  0x06 ) | (controlData[2] >> 7);
}

uint8_t ClassicController::rightJoyY() {
	return controlData[2] & 0x1F;
}

boolean ClassicController::dpadUp() {
	return extractBit(5, 0);
}

boolean ClassicController::dpadDown() {
	return extractBit(4, 6);
}

boolean ClassicController::dpadLeft() {
	return extractBit(5, 1);
}

boolean ClassicController::dpadRight() {
	return extractBit(4, 7);
}

boolean ClassicController::buttonA() {
	return extractBit(5, 4);
}

boolean ClassicController::buttonB() {
	return extractBit(5, 6);
}

boolean ClassicController::buttonX() {
	return extractBit(5, 3);
}

boolean ClassicController::buttonY() {
	return extractBit(5, 5);
}

uint8_t ClassicController::triggerL() {
	return ((controlData[2] & 0x60) >> 2) | controlData[3] >> 5;
}

uint8_t ClassicController::triggerR() {
	return controlData[3] & 0x1F;
}

boolean ClassicController::buttonLT() {
	return extractBit(4, 5);
}

boolean ClassicController::buttonRT() {
	return extractBit(4, 1);
}

boolean ClassicController::buttonZL() {
	return extractBit(5, 7);
}

boolean ClassicController::buttonZR() {
	return extractBit(5, 2);
}

boolean ClassicController::buttonPlus() {
	return extractBit(4, 2);
}

boolean ClassicController::buttonMinus() {
	return extractBit(4, 4);
}

boolean ClassicController::buttonHome() {
	return extractBit(4, 3);
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
