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

// Classic Controller, Standard Data Map Storage
constexpr ByteMap ClassicDataMap_Std::LeftJoyX;
constexpr ByteMap ClassicDataMap_Std::LeftJoyY;

constexpr ByteMap ClassicDataMap_Std::RightJoyX[3];
constexpr ByteMap ClassicDataMap_Std::RightJoyY;

constexpr BitMap  ClassicDataMap_Std::DpadUp;
constexpr BitMap  ClassicDataMap_Std::DpadDown;
constexpr BitMap  ClassicDataMap_Std::DpadLeft;
constexpr BitMap  ClassicDataMap_Std::DpadRight;

constexpr BitMap  ClassicDataMap_Std::ButtonA;
constexpr BitMap  ClassicDataMap_Std::ButtonB;
constexpr BitMap  ClassicDataMap_Std::ButtonX;
constexpr BitMap  ClassicDataMap_Std::ButtonY;

constexpr ByteMap ClassicDataMap_Std::TriggerL[2];
constexpr ByteMap ClassicDataMap_Std::TriggerR;

constexpr BitMap  ClassicDataMap_Std::ButtonL;
constexpr BitMap  ClassicDataMap_Std::ButtonR;
constexpr BitMap  ClassicDataMap_Std::ButtonZL;
constexpr BitMap  ClassicDataMap_Std::ButtonZR;

constexpr BitMap  ClassicDataMap_Std::ButtonPlus;
constexpr BitMap  ClassicDataMap_Std::ButtonMinus;
constexpr BitMap  ClassicDataMap_Std::ButtonHome;


// Classic Controller, "High Resolution" Data Map Storage
constexpr IndexMap ClassicDataMap_HighRes::LeftJoyX;
constexpr IndexMap ClassicDataMap_HighRes::LeftJoyY;

constexpr IndexMap ClassicDataMap_HighRes::RightJoyX;
constexpr IndexMap ClassicDataMap_HighRes::RightJoyY;

constexpr BitMap   ClassicDataMap_HighRes::DpadUp;
constexpr BitMap   ClassicDataMap_HighRes::DpadDown;
constexpr BitMap   ClassicDataMap_HighRes::DpadLeft;
constexpr BitMap   ClassicDataMap_HighRes::DpadRight;

constexpr BitMap   ClassicDataMap_HighRes::ButtonA;
constexpr BitMap   ClassicDataMap_HighRes::ButtonB;
constexpr BitMap   ClassicDataMap_HighRes::ButtonX;
constexpr BitMap   ClassicDataMap_HighRes::ButtonY;

constexpr IndexMap ClassicDataMap_HighRes::TriggerL;
constexpr IndexMap ClassicDataMap_HighRes::TriggerR;

constexpr BitMap   ClassicDataMap_HighRes::ButtonL;
constexpr BitMap   ClassicDataMap_HighRes::ButtonR;
constexpr BitMap   ClassicDataMap_HighRes::ButtonZL;
constexpr BitMap   ClassicDataMap_HighRes::ButtonZR;

constexpr BitMap   ClassicDataMap_HighRes::ButtonPlus;
constexpr BitMap   ClassicDataMap_HighRes::ButtonMinus;
constexpr BitMap   ClassicDataMap_HighRes::ButtonHome;


template<class DataMaps>
uint8_t ClassicControllerCore<DataMaps>::leftJoyX() const {
	return getControlData(Maps::LeftJoyX);
}

template<class DataMaps>
uint8_t ClassicControllerCore<DataMaps>::leftJoyY() const {
	return getControlData(Maps::LeftJoyY);
}

template<class DataMaps>
uint8_t ClassicControllerCore<DataMaps>::rightJoyX() const {
	return getControlData(Maps::RightJoyX);
}

template<class DataMaps>
uint8_t ClassicControllerCore<DataMaps>::rightJoyY() const {
	return getControlData(Maps::RightJoyY);
}

template<class DataMaps>
boolean ClassicControllerCore<DataMaps>::dpadUp() const {
	return getControlBit(Maps::DpadUp);
}

template<class DataMaps>
boolean ClassicControllerCore<DataMaps>::dpadDown() const {
	return getControlBit(Maps::DpadDown);
}

template<class DataMaps>
boolean ClassicControllerCore<DataMaps>::dpadLeft() const {
	return getControlBit(Maps::DpadLeft);
}

template<class DataMaps>
boolean ClassicControllerCore<DataMaps>::dpadRight() const {
	return getControlBit(Maps::DpadRight);
}

template<class DataMaps>
boolean ClassicControllerCore<DataMaps>::buttonA() const {
	return getControlBit(Maps::ButtonA);
}

template<class DataMaps>
boolean ClassicControllerCore<DataMaps>::buttonB() const {
	return getControlBit(Maps::ButtonB);
}

template<class DataMaps>
boolean ClassicControllerCore<DataMaps>::buttonX() const {
	return getControlBit(Maps::ButtonX);
}

template<class DataMaps>
boolean ClassicControllerCore<DataMaps>::buttonY() const {
	return getControlBit(Maps::ButtonY);
}

template<class DataMaps>
uint8_t ClassicControllerCore<DataMaps>::triggerL() const {
	return getControlData(Maps::TriggerL);
}

template<class DataMaps>
uint8_t ClassicControllerCore<DataMaps>::triggerR() const {
	return getControlData(Maps::TriggerR);
}

template<class DataMaps>
boolean ClassicControllerCore<DataMaps>::buttonL() const {
	return getControlBit(Maps::ButtonL);
}

template<class DataMaps>
boolean ClassicControllerCore<DataMaps>::buttonR() const {
	return getControlBit(Maps::ButtonR);
}

template<class DataMaps>
boolean ClassicControllerCore<DataMaps>::buttonZL() const {
	return getControlBit(Maps::ButtonZL);
}

template<class DataMaps>
boolean ClassicControllerCore<DataMaps>::buttonZR() const {
	return getControlBit(Maps::ButtonZR);
}

template<class DataMaps>
boolean ClassicControllerCore<DataMaps>::buttonStart() const {
	return buttonPlus();
}

template<class DataMaps>
boolean ClassicControllerCore<DataMaps>::buttonSelect() const {
	return buttonMinus();
}

template<class DataMaps>
boolean ClassicControllerCore<DataMaps>::buttonPlus() const {
	return getControlBit(Maps::ButtonPlus);
}

template<class DataMaps>
boolean ClassicControllerCore<DataMaps>::buttonMinus() const {
	return getControlBit(Maps::ButtonMinus);
}

template<class DataMaps>
boolean ClassicControllerCore<DataMaps>::buttonHome() const {
	return getControlBit(Maps::ButtonHome);
}

template<class DataMaps>
void ClassicControllerCore<DataMaps>::printDebug(Print& output) const {
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

boolean ClassicControllerHR_Shared::specificInit() {
	boolean success = writeRegister(0xFE, 0x03);  // set "high res" mode
	if (success && getRequestSize() < 8) setRequestSize(8);  // 8 or more bytes of control data needed for this mode
	return success;
}

template<class DataMaps>
boolean ClassicControllerCore<DataMaps>::fixNESThirdPartyData(boolean force) {
	// Public-facing function to check and "correct" data if using a third party controller
	// Returns 'true' if data was modified
	if((isNESThirdParty() && getRequestSize() >= 8) || force == true) {  // 8 bytes is the minimum for valid data
		manipulateThirdPartyData();
		return true;
	}
	return false;
}

template<class DataMaps>
boolean ClassicControllerCore<DataMaps>::isNESThirdParty() const {
	// The third party NES controllers I've come across seem to display the same
	// unchanging pattern for the first six control bytes:
	//
	//     0x81, 0x81, 0x81, 0x81, 0x00, 0x00
	//
	// This is mostly garbage data that doesn't line up with the Classic Controller
	// at all. Using the library's debug output, here is what it translates to:
	//
	//    <^v> | -H+ | ABXY L:( 1,  1) R:(21,  1) | LT: 4X RT: 1X Z:LR
	//
	// You'll notice a few things. ALL possible buttons are pressed. The left analog 
	// stick is completely down and to the left, while the right analog stick is down
	// and to the right. On the back, both trigger "fully depressed" buttons are
	// down, and yet the analog trigger values are very low. In short, this is a
	// difficult if not impossible state for a normal Classic Controller to be in.
	// Because of that, we can reasonably assume that if the bytes match this then the
	// connected controller is a third party NES controller, and can be treated accordingly. 
	//
	// -------
	//
	// Issue #46 states that the 8BitDo Retro Receiver, another 3rd party controller,
	// has a different set of data for the first six bytes:
	//
	//    0x84, 0x86, 0x86, 0x86, 0x00, 0x00
	//
	// Which, again using the library's debug output, evaluates to:
	//
	//    <^v> | -H+ | ABXY L:( 4,  6) R:(21,  6) | LT: 4X RT: 6X Z:LR
	//
	// This has been added in as a second conditional check.
	//
	// If any other 3rd party NES controllers have a different signature than these two,
	// I'm going to modify this signature check to only check the last two bytes (4 & 5) as 0.

	       // 3rd Party Data Signature, Typical
	return (getControlData(0) == 0x81 &&  // RX 4:3, LX
	        getControlData(1) == 0x81 &&  // RX 2:1, LY
	        getControlData(2) == 0x81 &&  // RX 0, LT 4:3, RY
	        getControlData(3) == 0x81 &&  // LT 2:0, RT
	        getControlData(4) == 0x00 &&  // Button packet 1 (all pressed)
	        getControlData(5) == 0x00)    // Button packet 2 (all pressed)
	||
	       // 8BitDo Retro Receiver Signature
	       (getControlData(0) == 0x84 &&  // RX 4:3, LX
	        getControlData(1) == 0x86 &&  // RX 2:1, LY
	        getControlData(2) == 0x86 &&  // RX 0, LT 4:3, RY
	        getControlData(3) == 0x86 &&  // LT 2:0, RT
	        getControlData(4) == 0x00 &&  // Button packet 1 (all pressed)
	        getControlData(5) == 0x00);   // Button packet 2 (all pressed)
}

template<class DataMaps>
void ClassicControllerCore<DataMaps>::manipulateThirdPartyData() {
	// The data returned by third party NES controllers for the missing control surfaces
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
	// For the third party NES Mini controllers, only the two data packets containing the button
	// booleans matter. Bytes 0, 1, 2, and 3 (joysticks and triggers) are replaced entirely.
	// Bytes 4 and 5 are overridden by the values in 6 and 7.

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

template class ClassicControllerCore<ClassicDataMap_Std>;  // Standard Mappings
template class ClassicControllerCore<ClassicDataMap_HighRes>;  // "High Resolution" Mappings

}  // End "NintendoExtensionCtrl" namespace
