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

constexpr ByteMap ClassicController_Shared::Maps::LeftJoyX;
constexpr ByteMap ClassicController_Shared::Maps::LeftJoyY;

constexpr ByteMap ClassicController_Shared::Maps::RightJoyX[3];
constexpr ByteMap ClassicController_Shared::Maps::RightJoyY;

constexpr BitMap  ClassicController_Shared::Maps::DpadUp;
constexpr BitMap  ClassicController_Shared::Maps::DpadDown;
constexpr BitMap  ClassicController_Shared::Maps::DpadLeft;
constexpr BitMap  ClassicController_Shared::Maps::DpadRight;

constexpr BitMap  ClassicController_Shared::Maps::ButtonA;
constexpr BitMap  ClassicController_Shared::Maps::ButtonB;
constexpr BitMap  ClassicController_Shared::Maps::ButtonX;
constexpr BitMap  ClassicController_Shared::Maps::ButtonY;

constexpr ByteMap ClassicController_Shared::Maps::TriggerL[2];
constexpr ByteMap ClassicController_Shared::Maps::TriggerR;

constexpr BitMap  ClassicController_Shared::Maps::ButtonL;
constexpr BitMap  ClassicController_Shared::Maps::ButtonR;
constexpr BitMap  ClassicController_Shared::Maps::ButtonZL;
constexpr BitMap  ClassicController_Shared::Maps::ButtonZR;

constexpr BitMap  ClassicController_Shared::Maps::ButtonPlus;
constexpr BitMap  ClassicController_Shared::Maps::ButtonMinus;
constexpr BitMap  ClassicController_Shared::Maps::ButtonHome;

uint8_t ClassicController_Shared::leftJoyX() const {
	return getControlData(Maps::LeftJoyX);
}

uint8_t ClassicController_Shared::leftJoyY() const {
	return getControlData(Maps::LeftJoyY);
}

uint8_t ClassicController_Shared::rightJoyX() const {
	return getControlData(Maps::RightJoyX);
}

uint8_t ClassicController_Shared::rightJoyY() const {
	return getControlData(Maps::RightJoyY);
}

boolean ClassicController_Shared::dpadUp() const {
	return getControlBit(Maps::DpadUp);
}

boolean ClassicController_Shared::dpadDown() const {
	return getControlBit(Maps::DpadDown);
}

boolean ClassicController_Shared::dpadLeft() const {
	return getControlBit(Maps::DpadLeft);
}

boolean ClassicController_Shared::dpadRight() const {
	return getControlBit(Maps::DpadRight);
}

boolean ClassicController_Shared::buttonA() const {
	return getControlBit(Maps::ButtonA);
}

boolean ClassicController_Shared::buttonB() const {
	return getControlBit(Maps::ButtonB);
}

boolean ClassicController_Shared::buttonX() const {
	return getControlBit(Maps::ButtonX);
}

boolean ClassicController_Shared::buttonY() const {
	return getControlBit(Maps::ButtonY);
}

uint8_t ClassicController_Shared::triggerL() const {
	return getControlData(Maps::TriggerL);
}

uint8_t ClassicController_Shared::triggerR() const {
	return getControlData(Maps::TriggerR);
}

boolean ClassicController_Shared::buttonL() const {
	return getControlBit(Maps::ButtonL);
}

boolean ClassicController_Shared::buttonR() const {
	return getControlBit(Maps::ButtonR);
}

boolean ClassicController_Shared::buttonZL() const {
	return getControlBit(Maps::ButtonZL);
}

boolean ClassicController_Shared::buttonZR() const {
	return getControlBit(Maps::ButtonZR);
}

boolean ClassicController_Shared::buttonStart() const {
	return buttonPlus();
}

boolean ClassicController_Shared::buttonSelect() const {
	return buttonMinus();
}

boolean ClassicController_Shared::buttonPlus() const {
	return getControlBit(Maps::ButtonPlus);
}

boolean ClassicController_Shared::buttonMinus() const {
	return getControlBit(Maps::ButtonMinus);
}

boolean ClassicController_Shared::buttonHome() const {
	return getControlBit(Maps::ButtonHome);
}

void ClassicController_Shared::printDebug(Print& output) const {
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

// ######### Mini Controller Support #########

boolean ClassicController_Shared::fixNESKnockoffData() {
	// Public-facing function to check and "correct" data if using a knockoff
	// Returns 'true' if data was modified
	if(isNESKnockoff()) { 
		manipulateKnockoffData();
		return true;
	}
	return false;
}

boolean ClassicController_Shared::isNESKnockoff() const {
	// The NES knockoffs I've come across seem to display the same unchanging pattern
	// for the first six control bytes:
	//
	//     0x81, 0x81, 0x81, 0x81, 0x00, 0x00
	//
	// This is mostly garbage data that doesn't line up with the Classic Controller
	// at all. Using the library's debug output, here is what it translates to:
	//
	//    <^v> | -H+ | ABXY L : (1, 1) R : (21, 1) | LT : 4X RT : 1X Z : LR
	//
	// You'll notice a few things. ALL possible buttons are pressed. The left analog 
	// stick is completely down and to the left, while the right analog stick is down
	// and to the right. On the back, both trigger "fully depressed" buttons are
	// down, and yet the analog trigger values are very low. In short, this is a
	// difficult if not impossible state for a normal Classic Controller to be in.
	// Because of that, we can reasonably assume that if the bytes match this then the
	// connected controller is an NES Knockoff, and can be treated accordingly. 

	return getControlData(0) == 0x81 &&  // RX 4:3, LX
	       getControlData(1) == 0x81 &&  // RX 2:1, LY
	       getControlData(2) == 0x81 &&  // RX 0, LT 4:3, RY
	       getControlData(3) == 0x81 &&  // LT 2:0, RT
	       getControlData(4) == 0x00 &&  // Button packet 1 (all pressed)
	       getControlData(5) == 0x00;    // Button packet 2 (all pressed)
}

void ClassicController_Shared::manipulateKnockoffData() {
	// The data returned by knockoff NES controllers for the missing control surfaces
	// (joysticks, triggers, etc.) is "corrupted", meaning that it doesn't align with
	// what you would expect a Classic Controller at rest to display.
	//
	// A friend of mine kindly tested his genuine NES controller, and here are the first
	// six bytes it reports by default:
	//
	//     0x5F 0xDF 0x8F 0x00 0xFF 0xFF
	//
	// Using the library's debug output, here is what it translates to:
	//
	//      ____ | ___ | ____ L : (31, 31) R : (15, 15) | LT : 0_ RT : 0_ Z : __
	//
	// The left joystick is centered at 31/31, the right joystick is centered at 15/15,
	// the analog trigger values are 0, and all buttons are released.
	//
	// For the NES Mini knockoffs, only the two data packets containing the button booleans
	// matter. Bytes 0, 1, 2, and 3 (joysticks and triggers) are replaced entirely. Bytes
	// 4 and 5 are overridden by the values in 6 and 7.

	setControlData(0, 0x5F);
	setControlData(1, 0xDF);
	setControlData(2, 0x8F);
	setControlData(3, 0x00);
	setControlData(4, getControlData(6));
	setControlData(5, getControlData(7));
}

void NESMiniController_Shared::printDebug(Print& output) const {
	const char fillCharacter = '_';

	output.print("NES ");

	output.print(dpadLeft() ? '<' : fillCharacter);
	output.print(dpadUp() ? '^' : fillCharacter);
	output.print(dpadDown() ? 'v' : fillCharacter);
	output.print(dpadRight() ? '>' : fillCharacter);
	output.print(" | ");

	buttonSelect() ? (void)output.print("SEL") : NintendoExtensionCtrl::printRepeat(fillCharacter, 3, output);
	output.print(' ');

	buttonStart() ? (void)output.print("STR") : NintendoExtensionCtrl::printRepeat(fillCharacter, 3, output);
	output.print(" | ");

	output.print(buttonB() ? 'B' : fillCharacter);
	output.print(buttonA() ? 'A' : fillCharacter);

	output.println();
}

void SNESMiniController_Shared::printDebug(Print& output) const {
	const char fillCharacter = '_';

	output.print("SNES ");

	output.print(dpadLeft() ? '<' : fillCharacter);
	output.print(dpadUp() ? '^' : fillCharacter);
	output.print(dpadDown() ? 'v' : fillCharacter);
	output.print(dpadRight() ? '>' : fillCharacter);
	output.print(" | ");

	buttonSelect() ? (void)output.print("SEL") : NintendoExtensionCtrl::printRepeat(fillCharacter, 3, output);
	output.print(' ');

	buttonStart() ? (void)output.print("STR") : NintendoExtensionCtrl::printRepeat(fillCharacter, 3, output);
	output.print(" | ");

	output.print(buttonA() ? 'A' : fillCharacter);
	output.print(buttonB() ? 'B' : fillCharacter);
	output.print(buttonX() ? 'X' : fillCharacter);
	output.print(buttonY() ? 'Y' : fillCharacter);
	output.print(" | ");

	output.print(buttonL() ? 'L' : fillCharacter);
	output.print(buttonR() ? 'R' : fillCharacter);

	output.println();
}

}  // End "NintendoExtensionCtrl" namespace
