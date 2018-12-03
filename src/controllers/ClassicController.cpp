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

constexpr ByteMap ClassicController_Data::Maps::LeftJoyX;
constexpr ByteMap ClassicController_Data::Maps::LeftJoyY;

constexpr ByteMap ClassicController_Data::Maps::RightJoyX[3];
constexpr ByteMap ClassicController_Data::Maps::RightJoyY;

constexpr BitMap  ClassicController_Data::Maps::DpadUp;
constexpr BitMap  ClassicController_Data::Maps::DpadDown;
constexpr BitMap  ClassicController_Data::Maps::DpadLeft;
constexpr BitMap  ClassicController_Data::Maps::DpadRight;

constexpr BitMap  ClassicController_Data::Maps::ButtonA;
constexpr BitMap  ClassicController_Data::Maps::ButtonB;
constexpr BitMap  ClassicController_Data::Maps::ButtonX;
constexpr BitMap  ClassicController_Data::Maps::ButtonY;

constexpr ByteMap ClassicController_Data::Maps::TriggerL[2];
constexpr ByteMap ClassicController_Data::Maps::TriggerR;

constexpr BitMap  ClassicController_Data::Maps::ButtonL;
constexpr BitMap  ClassicController_Data::Maps::ButtonR;
constexpr BitMap  ClassicController_Data::Maps::ButtonZL;
constexpr BitMap  ClassicController_Data::Maps::ButtonZR;

constexpr BitMap  ClassicController_Data::Maps::ButtonPlus;
constexpr BitMap  ClassicController_Data::Maps::ButtonMinus;
constexpr BitMap  ClassicController_Data::Maps::ButtonHome;

uint8_t ClassicController_Data::leftJoyX() const {
	return getControlData(Maps::LeftJoyX);
}

uint8_t ClassicController_Data::leftJoyY() const {
	return getControlData(Maps::LeftJoyY);
}

uint8_t ClassicController_Data::rightJoyX() const {
	return getControlData(Maps::RightJoyX);
}

uint8_t ClassicController_Data::rightJoyY() const {
	return getControlData(Maps::RightJoyY);
}

boolean ClassicController_Data::dpadUp() const {
	return getControlBit(Maps::DpadUp);
}

boolean ClassicController_Data::dpadDown() const {
	return getControlBit(Maps::DpadDown);
}

boolean ClassicController_Data::dpadLeft() const {
	return getControlBit(Maps::DpadLeft);
}

boolean ClassicController_Data::dpadRight() const {
	return getControlBit(Maps::DpadRight);
}

boolean ClassicController_Data::buttonA() const {
	return getControlBit(Maps::ButtonA);
}

boolean ClassicController_Data::buttonB() const {
	return getControlBit(Maps::ButtonB);
}

boolean ClassicController_Data::buttonX() const {
	return getControlBit(Maps::ButtonX);
}

boolean ClassicController_Data::buttonY() const {
	return getControlBit(Maps::ButtonY);
}

uint8_t ClassicController_Data::triggerL() const {
	return getControlData(Maps::TriggerL);
}

uint8_t ClassicController_Data::triggerR() const {
	return getControlData(Maps::TriggerR);
}

boolean ClassicController_Data::buttonL() const {
	return getControlBit(Maps::ButtonL);
}

boolean ClassicController_Data::buttonR() const {
	return getControlBit(Maps::ButtonR);
}

boolean ClassicController_Data::buttonZL() const {
	return getControlBit(Maps::ButtonZL);
}

boolean ClassicController_Data::buttonZR() const {
	return getControlBit(Maps::ButtonZR);
}

boolean ClassicController_Data::buttonStart() const {
	return buttonPlus();
}

boolean ClassicController_Data::buttonSelect() const {
	return buttonMinus();
}

boolean ClassicController_Data::buttonPlus() const {
	return getControlBit(Maps::ButtonPlus);
}

boolean ClassicController_Data::buttonMinus() const {
	return getControlBit(Maps::ButtonMinus);
}

boolean ClassicController_Data::buttonHome() const {
	return getControlBit(Maps::ButtonHome);
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
