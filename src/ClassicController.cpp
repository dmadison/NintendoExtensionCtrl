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

ClassicController::ClassicController() {}

uint8_t ClassicController::getLeftJoyX() {
	return controlData[0] & 0x3F;
}

uint8_t ClassicController::getLeftJoyY() {
	return controlData[1] & 0x3F;
}

uint8_t ClassicController::getRightJoyX() {
	return (controlData[1] >> 5) | (controlData[0] >> 3) | (controlData[2] >> 7);
}

uint8_t ClassicController::getRightJoyY() {
	return controlData[2] & 0x1F;
}

boolean ClassicController::getPadUp() {
	return extractBit(5, 0);
}

boolean ClassicController::getPadDown() {
	return extractBit(4, 6);
}

boolean ClassicController::getPadLeft() {
	return extractBit(5, 1);
}

boolean ClassicController::getPadRight() {
	return extractBit(4, 7);
}

boolean ClassicController::getButtonA() {
	return extractBit(5, 4);
}

boolean ClassicController::getButtonB() {
	return extractBit(5, 6);
}

boolean ClassicController::getButtonX() {
	return extractBit(5, 3);
}

boolean ClassicController::getButtonY() {
	return extractBit(5, 5);
}

uint8_t ClassicController::getTriggerL() {
	return ((controlData[2] & 0x60) >> 2) | controlData[3] >> 5;
}

uint8_t ClassicController::getTriggerR() {
	return controlData[3] & 0x1F;
}

boolean ClassicController::getButtonLT() {
	return extractBit(4, 5);
}

boolean ClassicController::getButtonRT() {
	return extractBit(4, 1);
}

boolean ClassicController::getButtonZL() {
	return extractBit(5, 7);
}

boolean ClassicController::getButtonZR() {
	return extractBit(5, 2);
}

boolean ClassicController::getButtonPlus() {
	return extractBit(4, 2);
}

boolean ClassicController::getButtonMinus() {
	return extractBit(4, 4);
}

boolean ClassicController::getButtonHome() {
	return extractBit(4, 3);
}

void ClassicController::printDebug(Stream& stream) {
	const char fillCharacter = '_';

	char buffer[62];

	char dpadLPrint = getPadLeft() ? '<' : fillCharacter;
	char dpadUPrint = getPadUp() ? '^' : fillCharacter;
	char dpadDPrint = getPadDown() ? 'v' : fillCharacter;
	char dpadRPrint = getPadRight() ? '>' : fillCharacter;

	char aButtonPrint = getButtonA() ? 'A' : fillCharacter;
	char bButtonPrint = getButtonB() ? 'B' : fillCharacter;
	char xButtonPrint = getButtonX() ? 'X' : fillCharacter;
	char yButtonPrint = getButtonY() ? 'Y' : fillCharacter;

	char plusPrint = getButtonPlus() ? '+' : fillCharacter;
	char minusPrint = getButtonMinus() ? '-' : fillCharacter;
	char homePrint = getButtonHome() ? 'H' : fillCharacter;

	char ltButtonPrint = getButtonLT() ? 'X' : fillCharacter;
	char rtButtonPrint = getButtonRT() ? 'X' : fillCharacter;

	char zlButtonPrint = getButtonZL() ? 'L' : fillCharacter;
	char zrButtonPrint = getButtonZR() ? 'R' : fillCharacter;

	stream.print("Classic ");
	sprintf(buffer,
		"%c%c%c%c | %c%c%c | %c%c%c%c L:(%2u, %2u) R:(%2u, %2u) | LT:%2u%c RT:%2u%c Z:%c%c",
		dpadLPrint, dpadUPrint, dpadDPrint, dpadRPrint,
		minusPrint, homePrint, plusPrint,
		aButtonPrint, bButtonPrint, xButtonPrint, yButtonPrint,
		getLeftJoyX(), getLeftJoyY(), getRightJoyX(), getRightJoyY(),
		getTriggerL(), ltButtonPrint, getTriggerR(), rtButtonPrint,
		zlButtonPrint, zrButtonPrint);
	stream.println(buffer);
}
