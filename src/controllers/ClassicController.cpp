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

// Standard Maps
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


// High Resolution Maps
constexpr IndexMap ClassicController_Shared::MapsHR::LeftJoyX;
constexpr IndexMap ClassicController_Shared::MapsHR::LeftJoyY;

constexpr IndexMap ClassicController_Shared::MapsHR::RightJoyX;
constexpr IndexMap ClassicController_Shared::MapsHR::RightJoyY;

constexpr BitMap   ClassicController_Shared::MapsHR::DpadUp;
constexpr BitMap   ClassicController_Shared::MapsHR::DpadDown;
constexpr BitMap   ClassicController_Shared::MapsHR::DpadLeft;
constexpr BitMap   ClassicController_Shared::MapsHR::DpadRight;

constexpr BitMap   ClassicController_Shared::MapsHR::ButtonA;
constexpr BitMap   ClassicController_Shared::MapsHR::ButtonB;
constexpr BitMap   ClassicController_Shared::MapsHR::ButtonX;
constexpr BitMap   ClassicController_Shared::MapsHR::ButtonY;

constexpr IndexMap ClassicController_Shared::MapsHR::TriggerL;
constexpr IndexMap ClassicController_Shared::MapsHR::TriggerR;

constexpr BitMap   ClassicController_Shared::MapsHR::ButtonL;
constexpr BitMap   ClassicController_Shared::MapsHR::ButtonR;
constexpr BitMap   ClassicController_Shared::MapsHR::ButtonZL;
constexpr BitMap   ClassicController_Shared::MapsHR::ButtonZR;

constexpr BitMap   ClassicController_Shared::MapsHR::ButtonPlus;
constexpr BitMap   ClassicController_Shared::MapsHR::ButtonMinus;
constexpr BitMap   ClassicController_Shared::MapsHR::ButtonHome;


/* Making use of the preprocessor here to repeat these conditionals for every
 * data retrieving function in the class. I'd rather trust the preprocessor
 * here than pass the constexpr objects through a nested function and hope
 * the compiler properly optimizes it. Plus I don't have to trust myself
 * to copy the mapping name twice.
 *
 * If the controller is not in "high resolution" mode (according to the class
 * data), return the controlData or controlBit function for the specified map
 * in the "standard" maps (Maps::). If the controller *is* in "high resolution"
 * mode, return the controlData or controlBit function for the specified map
 * of the same name in the "high resolution" maps (MapsHR::).
 */
#define HRDATA(map) !highRes ? getControlData(Maps::map) : getControlData(MapsHR::map)
#define HRBIT(map)  !highRes ? getControlBit(Maps::map) : getControlBit(MapsHR::map)


boolean ClassicController_Shared::setHighRes(boolean hr) {
	const uint8_t Register = 0xFE;  // address of the register we're altering
	const uint8_t Setting_Std = 0x01;  // the register's value for 'standard' mode
	const uint8_t Setting_HR  = 0x03;  // the register's value for 'high res' mode

	const uint8_t RegVal = hr ? Setting_HR : Setting_Std;

	if (writeRegister(Register, RegVal)) {  // write to controller
		uint8_t r = readRegister(Register);  // read back the saved value

		if (r == RegVal) highRes = hr;  // success! saved value matches the sent one
		else highRes = !hr;  // fail, sent value was not retained. Must be the other one.

		if (highRes == true && getRequestSize() < 8) setRequestSize(8);  // 8 bytes needed for hr mode
		else if (highRes == false) setRequestSize(MinRequestSize);  // if not in HR, set back to min
	}

	// success if the value we're using is the one we tried to set
	return hr == getHighRes();
}

boolean ClassicController_Shared::getHighRes() const {
	return highRes;
}

uint8_t ClassicController_Shared::leftJoyX() const {
	return HRDATA(LeftJoyX);
}

uint8_t ClassicController_Shared::leftJoyY() const {
	return HRDATA(LeftJoyY);
}

uint8_t ClassicController_Shared::rightJoyX() const {
	return HRDATA(RightJoyX);
}

uint8_t ClassicController_Shared::rightJoyY() const {
	return HRDATA(RightJoyY);
}

boolean ClassicController_Shared::dpadUp() const {
	return HRBIT(DpadUp);
}

boolean ClassicController_Shared::dpadDown() const {
	return HRBIT(DpadDown);
}

boolean ClassicController_Shared::dpadLeft() const {
	return HRBIT(DpadLeft);
}

boolean ClassicController_Shared::dpadRight() const {
	return HRBIT(DpadRight);
}

boolean ClassicController_Shared::buttonA() const {
	return HRBIT(ButtonA);
}

boolean ClassicController_Shared::buttonB() const {
	return HRBIT(ButtonB);
}

boolean ClassicController_Shared::buttonX() const {
	return HRBIT(ButtonX);
}

boolean ClassicController_Shared::buttonY() const {
	return HRBIT(ButtonY);
}

uint8_t ClassicController_Shared::triggerL() const {
	return HRDATA(TriggerL);
}

uint8_t ClassicController_Shared::triggerR() const {
	return HRDATA(TriggerR);
}

boolean ClassicController_Shared::buttonL() const {
	return HRBIT(ButtonL);
}

boolean ClassicController_Shared::buttonR() const {
	return HRBIT(ButtonR);
}

boolean ClassicController_Shared::buttonZL() const {
	return HRBIT(ButtonZL);
}

boolean ClassicController_Shared::buttonZR() const {
	return HRBIT(ButtonZR);
}

boolean ClassicController_Shared::buttonStart() const {
	return buttonPlus();
}

boolean ClassicController_Shared::buttonSelect() const {
	return buttonMinus();
}

boolean ClassicController_Shared::buttonPlus() const {
	return HRBIT(ButtonPlus);
}

boolean ClassicController_Shared::buttonMinus() const {
	return HRBIT(ButtonMinus);
}

boolean ClassicController_Shared::buttonHome() const {
	return HRBIT(ButtonHome);
}

void ClassicController_Shared::printDebug(Print& output) const {
	const char fillCharacter = '_';

	char buffer[68];

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
		"%c%c%c%c | %c%c%c | %c%c%c%c L:(%3u, %3u) R:(%3u, %3u) | LT:%3u%c RT:%3u%c Z:%c%c",
		dpadLPrint, dpadUPrint, dpadDPrint, dpadRPrint,
		minusPrint, homePrint, plusPrint,
		aButtonPrint, bButtonPrint, xButtonPrint, yButtonPrint,
		leftJoyX(), leftJoyY(), rightJoyX(), rightJoyY(),
		triggerL(), ltButtonPrint, triggerR(), rtButtonPrint,
		zlButtonPrint, zrButtonPrint);
	
	output.print(buffer);
	if (getHighRes()) output.print(" (High Res)");

	output.println();
}


// ######### Mini Controller Support #########

boolean MiniControllerBase::specificInit() {
	// all mini controllers use high res format, and some of the cheaper third
	// party ones will not work without it. So we're going to set this on
	// connection for all of them
	return setHighRes(true);
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
