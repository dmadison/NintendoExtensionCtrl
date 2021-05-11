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
constexpr ByteMap ClassicControllerBase::Maps::LeftJoyX;
constexpr ByteMap ClassicControllerBase::Maps::LeftJoyY;

constexpr ByteMap ClassicControllerBase::Maps::RightJoyX[3];
constexpr ByteMap ClassicControllerBase::Maps::RightJoyY;

constexpr BitMap  ClassicControllerBase::Maps::DpadUp;
constexpr BitMap  ClassicControllerBase::Maps::DpadDown;
constexpr BitMap  ClassicControllerBase::Maps::DpadLeft;
constexpr BitMap  ClassicControllerBase::Maps::DpadRight;

constexpr BitMap  ClassicControllerBase::Maps::ButtonA;
constexpr BitMap  ClassicControllerBase::Maps::ButtonB;
constexpr BitMap  ClassicControllerBase::Maps::ButtonX;
constexpr BitMap  ClassicControllerBase::Maps::ButtonY;

constexpr ByteMap ClassicControllerBase::Maps::TriggerL[2];
constexpr ByteMap ClassicControllerBase::Maps::TriggerR;

constexpr BitMap  ClassicControllerBase::Maps::ButtonL;
constexpr BitMap  ClassicControllerBase::Maps::ButtonR;
constexpr BitMap  ClassicControllerBase::Maps::ButtonZL;
constexpr BitMap  ClassicControllerBase::Maps::ButtonZR;

constexpr BitMap  ClassicControllerBase::Maps::ButtonPlus;
constexpr BitMap  ClassicControllerBase::Maps::ButtonMinus;
constexpr BitMap  ClassicControllerBase::Maps::ButtonHome;


// High Resolution Maps
constexpr IndexMap ClassicControllerBase::MapsHR::LeftJoyX;
constexpr IndexMap ClassicControllerBase::MapsHR::LeftJoyY;

constexpr IndexMap ClassicControllerBase::MapsHR::RightJoyX;
constexpr IndexMap ClassicControllerBase::MapsHR::RightJoyY;

constexpr BitMap   ClassicControllerBase::MapsHR::DpadUp;
constexpr BitMap   ClassicControllerBase::MapsHR::DpadDown;
constexpr BitMap   ClassicControllerBase::MapsHR::DpadLeft;
constexpr BitMap   ClassicControllerBase::MapsHR::DpadRight;

constexpr BitMap   ClassicControllerBase::MapsHR::ButtonA;
constexpr BitMap   ClassicControllerBase::MapsHR::ButtonB;
constexpr BitMap   ClassicControllerBase::MapsHR::ButtonX;
constexpr BitMap   ClassicControllerBase::MapsHR::ButtonY;

constexpr IndexMap ClassicControllerBase::MapsHR::TriggerL;
constexpr IndexMap ClassicControllerBase::MapsHR::TriggerR;

constexpr BitMap   ClassicControllerBase::MapsHR::ButtonL;
constexpr BitMap   ClassicControllerBase::MapsHR::ButtonR;
constexpr BitMap   ClassicControllerBase::MapsHR::ButtonZL;
constexpr BitMap   ClassicControllerBase::MapsHR::ButtonZR;

constexpr BitMap   ClassicControllerBase::MapsHR::ButtonPlus;
constexpr BitMap   ClassicControllerBase::MapsHR::ButtonMinus;
constexpr BitMap   ClassicControllerBase::MapsHR::ButtonHome;


/* Making use of the preprocessor here to repeat these conditionals for every
 * data retrieving function in the class. I'd rather trust the preprocessor
 * here than pass the constexpr objects through a nested function and hope
 * the compiler properly optimizes it. Plus I don't have to trust myself
 * to copy the mapping name twice.
 *
 * If the controller is not in "high resolution" mode (according to the class
 * data), fetch the controlData or controlBit function for the specified map
 * in the "standard" maps (Maps::) and bit shift to the left in order to fit
 * the full width of the "high res" data range. If the controller *is* in
 * "high resolution" mode, fetch the controlData or controlBit function for
 * the specified mapof the same name in the "high resolution" maps (MapsHR::).
 */
#define HRDATA(map, shift) !highRes ? (getControlData(Maps::map) << shift) & 0xFF : getControlData(MapsHR::map)
#define HRBIT(map)  !highRes ? getControlBit(Maps::map) : getControlBit(MapsHR::map)


boolean ClassicControllerBase::specificInit() {
	/* On init, try to set the controller to work in "high resolution" mode so
	 * we get a full byte of data for each analog input. Then read the current
	 * "data mode" from the controller so that the control surface functions
	 * use the correct mappings. This way, the class flexes to support
	 * all controllers regardless of their available data mode.
	 *
	 * This function will only return false if there is a *communciation error*
	 * on the I2C bus, meaning that the controller did not respond to a write
	 * or did not provide the right amount of data for a request. It will *not*
	 * return false if the "high resolution" mode is not successfully set.
	 */
	delayMicroseconds(I2C_ConversionDelay);  // wait after ID read before writing register
	return setDataMode(true);  // try to set 'high res' mode. 'success' if no comms errors
}

ExtensionType ClassicControllerBase::getExpectedType() const {
	return ExtensionType::ClassicController;
}

boolean ClassicControllerBase::checkDataMode(boolean *hr) const {
	/* Programmator Emptor: vvv This is where all of the headaches stem from vvv */

	/* Okay, so here's the deal. The Wii Classic Controller reports its data
	 * as six bytes with bit-packing for the analog values. When the NES and
	 * SNES mini consoles were released it turned out that Nintendo had
	 * included a "high resolution" mode for the Classic Controller. Writing
	 * '0x03' to the register '0xFE' will make the controller output 8 bytes,
	 * with each analog control surface using a full byte for its output.
	 *
	 * So here's the rub:
	 *   * Bad knockoff Classic Controllers only support "normal" mode
	 *   * Bad knockoff NES Controllers only support "high resolution" mode
	 *   * Genuine controllers support both
	 *
	 * Some knockoffs will behave properly and switch between the modes as
	 * requested, but many will only report their data in one mode and ignore
	 * the host if it asks otherwise. This results in control data that is 
	 * misinterpreted and users that are unhappy. So not only do we have to
	 * switch between modes, but we need to come up with a robust method
	 * to figure out *what mode we're in*.
	 *
	 * Here's my idea: in "standard" mode, the controller outputs 6 bytes of
	 * control data, leaving bytes 7-8 blank (0x00). If we read these two bytes
	 * and they have data in them, the controller must be in high resolution
	 * mode! In theory, at least.
	 *
	 * This is complicated by the fact that the data from the I2C bus has no
	 * error checking and is open drain, so if the pull-ups are too weak or
	 * there is noise on the bus some of these bits may flip 'high' and then
	 * the check is no good.
	 *
	 * To mitigate this, the same data set is requested twice and compared
	 * against itself. If there is a data mismatch, the requests are repeated
	 * until the two arrays agree. Not perfect, but better than nothing.
	 *
	 * Note that this read starts at 0x00. I tried starting at where the data
	 * *actually starts* (bytes 7 and 8, i.e. ptr 0x06), but the knockoff
	 * controllers apparently don't understand how to act as a proper
	 * register-based I2C device and just return junk. So instead we're starting
	 * at the beginning of the data block.
	 */
	static const uint8_t CheckSize  = 8;     // 8 bytes to cover both std and high res
	static const uint8_t DataOffset = 0x06;  // start of the data we're interested in (7 / 8)
	uint8_t checkData[CheckSize] = { 0x00 }, verifyData[CheckSize] = { 0x00 };
	do {
		if (!requestControlData(CheckSize, checkData)) return false;
		delayMicroseconds(I2C_ConversionDelay);  // need a brief delay between reads
		if (!requestControlData(CheckSize, verifyData)) return false;

		boolean equal = true;
		for (uint8_t i = 0; i < CheckSize - DataOffset; i++) {
			if (checkData[i] != verifyData[i]) {
				equal = false;  // one byte does not match! quit
				break;
			}
		}

		if (equal) break;  // if data matches, continue
		delayMicroseconds(I2C_ConversionDelay);  // if we're doing another loop, wait between reads again
	} while (true);

	*hr = !(checkData[DataOffset] == 0x00 && checkData[DataOffset+1] == 0x00);  // if both are '0', high res is false
	return true;  // successfully read state
}

boolean ClassicControllerBase::setDataMode(boolean hr, boolean verify) {
	const uint8_t regVal = hr ? 0x03 : 0x01;  // 0x03 for high res, 0x01 for standard

	// Attempt to write 'high res' mode to controller register.
	const bool writeSuccess = writeRegister(0xFE, regVal);

	/* If there's no success on the register write there are two possibilities:
	 *
	 *   #1: The controller is disconnected and no I2C data can get through
	 *   #2: The controller is silly and not processing the command properly, 
	 *       as some clone controllers may
	 *
	 * To determine which state we're in we perform a bog-standard data read,
	 * which is guaranteed to be supported by all controllers. If the controller
	 * returns data (*any* data) then we're still connected and the controller
	 * NACK'd the register write. If the controller does *not* return data the I2C
	 * bus is presumably disconnected and we can return 'false' for a communication
	 * error.
	 */
	if (!writeSuccess) {
		uint8_t buffer[MinRequestSize];  // we don't care about this data, we just need someplace to dump it
		if (!requestControlData(MinRequestSize, buffer)) return false;  // bad read, we must be disconnected

		// if we're going to perfom more reads below, the controller needs a short delay to catch its breath
		if(verify == true) delayMicroseconds(I2C_ConversionDelay);
	}

	if (verify == true) {
		boolean currentMode;  // buffer for controller's deduced HR setting, set in the 'check' function
		if (!checkDataMode(&currentMode)) return false;  // error: could not read mode
		highRes = currentMode;  // save current mode to class
	}
	else {
		highRes = hr;  // save mode we're attempting to set (no verification)
	}

	if (getHighRes() == true && getRequestSize() < 8) {
		setRequestSize(8);  // 8 bytes needed for hr mode
	}
	else if (getHighRes() == false && hr == false) {
		setRequestSize(MinRequestSize);  // if not in HR and *trying* not to be, set back to min
	}

	return true;  // 'success' if no communication errors, regardless of setting
}

boolean ClassicControllerBase::setHighRes(boolean hr, boolean verify) {
	// 'success' if the mode is changed to the one we're trying to set
	return setDataMode(hr, verify) && (getHighRes() == hr);
}

boolean ClassicControllerBase::getHighRes() const {
	return highRes;
}

uint8_t ClassicControllerBase::leftJoyX() const {
	return HRDATA(LeftJoyX, 2);  // 6 bits for standard range, so shift left (8-6)
}

uint8_t ClassicControllerBase::leftJoyY() const {
	return HRDATA(LeftJoyY, 2);
}

uint8_t ClassicControllerBase::rightJoyX() const {
	return HRDATA(RightJoyX, 3);  // 5 bits for standard range, so shift left (8-5)
}

uint8_t ClassicControllerBase::rightJoyY() const {
	return HRDATA(RightJoyY, 3);
}

boolean ClassicControllerBase::dpadUp() const {
	return HRBIT(DpadUp);
}

boolean ClassicControllerBase::dpadDown() const {
	return HRBIT(DpadDown);
}

boolean ClassicControllerBase::dpadLeft() const {
	return HRBIT(DpadLeft);
}

boolean ClassicControllerBase::dpadRight() const {
	return HRBIT(DpadRight);
}

boolean ClassicControllerBase::buttonA() const {
	return HRBIT(ButtonA);
}

boolean ClassicControllerBase::buttonB() const {
	return HRBIT(ButtonB);
}

boolean ClassicControllerBase::buttonX() const {
	return HRBIT(ButtonX);
}

boolean ClassicControllerBase::buttonY() const {
	return HRBIT(ButtonY);
}

uint8_t ClassicControllerBase::triggerL() const {
	return HRDATA(TriggerL, 3);  // 5 bits for standard range, so shift left (8-5)
}

uint8_t ClassicControllerBase::triggerR() const {
	return HRDATA(TriggerR, 3);
}

boolean ClassicControllerBase::buttonL() const {
	return HRBIT(ButtonL);
}

boolean ClassicControllerBase::buttonR() const {
	return HRBIT(ButtonR);
}

boolean ClassicControllerBase::buttonZL() const {
	return HRBIT(ButtonZL);
}

boolean ClassicControllerBase::buttonZR() const {
	return HRBIT(ButtonZR);
}

boolean ClassicControllerBase::buttonStart() const {
	return buttonPlus();
}

boolean ClassicControllerBase::buttonSelect() const {
	return buttonMinus();
}

boolean ClassicControllerBase::buttonPlus() const {
	return HRBIT(ButtonPlus);
}

boolean ClassicControllerBase::buttonMinus() const {
	return HRBIT(ButtonMinus);
}

boolean ClassicControllerBase::buttonHome() const {
	return HRBIT(ButtonHome);
}

void ClassicControllerBase::printDebug(Print& output) const {
	const char fillCharacter = '_';

	char buffer[68];

	const char dpadLPrint = dpadLeft() ? '<' : fillCharacter;
	const char dpadUPrint = dpadUp() ? '^' : fillCharacter;
	const char dpadDPrint = dpadDown() ? 'v' : fillCharacter;
	const char dpadRPrint = dpadRight() ? '>' : fillCharacter;

	const char aButtonPrint = buttonA() ? 'A' : fillCharacter;
	const char bButtonPrint = buttonB() ? 'B' : fillCharacter;
	const char xButtonPrint = buttonX() ? 'X' : fillCharacter;
	const char yButtonPrint = buttonY() ? 'Y' : fillCharacter;

	const char plusPrint = buttonPlus() ? '+' : fillCharacter;
	const char minusPrint = buttonMinus() ? '-' : fillCharacter;
	const char homePrint = buttonHome() ? 'H' : fillCharacter;

	const char ltButtonPrint = buttonL() ? 'X' : fillCharacter;
	const char rtButtonPrint = buttonR() ? 'X' : fillCharacter;

	const char zlButtonPrint = buttonZL() ? 'L' : fillCharacter;
	const char zrButtonPrint = buttonZR() ? 'R' : fillCharacter;

	output.print("Classic ");

	snprintf(buffer, sizeof(buffer),
		"%c%c%c%c | %c%c%c | %c%c%c%c L:(%3u, %3u) R:(%3u, %3u) | LT:%3u%c RT:%3u%c Z:%c%c",
		dpadLPrint, dpadUPrint, dpadDPrint, dpadRPrint,
		minusPrint, homePrint, plusPrint,
		aButtonPrint, bButtonPrint, xButtonPrint, yButtonPrint,
		leftJoyX(), leftJoyY(), rightJoyX(), rightJoyY(),
		triggerL(), ltButtonPrint, triggerR(), rtButtonPrint,
		zlButtonPrint, zrButtonPrint);
	
	output.print(buffer);
	if (getHighRes()) output.print(" | (HR)");

	output.println();
}


// ######### Mini Controller Support #########

void NESMiniControllerBase::printDebug(Print& output) const {
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

void SNESMiniControllerBase::printDebug(Print& output) const {
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
