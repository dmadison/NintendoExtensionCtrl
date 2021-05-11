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

#include "DJTurntable.h"

namespace NintendoExtensionCtrl {

constexpr ByteMap DJTurntableControllerBase::Maps::JoyX;
constexpr ByteMap DJTurntableControllerBase::Maps::JoyY;

constexpr BitMap  DJTurntableControllerBase::Maps::ButtonPlus;
constexpr BitMap  DJTurntableControllerBase::Maps::ButtonMinus;

constexpr ByteMap DJTurntableControllerBase::Maps::Left_Turntable;
constexpr ByteMap DJTurntableControllerBase::Maps::Left_TurntableSign;
constexpr BitMap  DJTurntableControllerBase::Maps::Left_ButtonGreen;
constexpr BitMap  DJTurntableControllerBase::Maps::Left_ButtonRed;
constexpr BitMap  DJTurntableControllerBase::Maps::Left_ButtonBlue;

constexpr ByteMap DJTurntableControllerBase::Maps::Right_Turntable[3];
constexpr ByteMap DJTurntableControllerBase::Maps::Right_TurntableSign;
constexpr BitMap  DJTurntableControllerBase::Maps::Right_ButtonGreen;
constexpr BitMap  DJTurntableControllerBase::Maps::Right_ButtonRed;
constexpr BitMap  DJTurntableControllerBase::Maps::Right_ButtonBlue;

constexpr ByteMap DJTurntableControllerBase::Maps::EffectDial[2];
constexpr ByteMap DJTurntableControllerBase::Maps::CrossfadeSlider;

constexpr BitMap  DJTurntableControllerBase::Maps::ButtonEuphoria;

ExtensionType DJTurntableControllerBase::getExpectedType() const {
	return ExtensionType::DJTurntableController;
}

// Combined Turntable
int8_t DJTurntableControllerBase::turntable() const {
	return left.turntable() + right.turntable();
}

boolean DJTurntableControllerBase::buttonGreen() const {
	return left.buttonGreen() | right.buttonGreen();
}

boolean DJTurntableControllerBase::buttonRed() const {
	return left.buttonRed() | right.buttonRed();
}

boolean DJTurntableControllerBase::buttonBlue() const {
	return left.buttonBlue() | right.buttonBlue();
}

// Main Board
uint8_t DJTurntableControllerBase::effectDial() const {
	return getControlData(Maps::EffectDial);
}

uint8_t DJTurntableControllerBase::crossfadeSlider() const {
	return getControlData(Maps::CrossfadeSlider);
}

boolean DJTurntableControllerBase::buttonEuphoria() const {
	return getControlBit(Maps::ButtonEuphoria);
}

uint8_t DJTurntableControllerBase::joyX() const {
	return getControlData(Maps::JoyX);
}

uint8_t DJTurntableControllerBase::joyY() const {
	return getControlData(Maps::JoyY);
}

boolean DJTurntableControllerBase::buttonPlus() const {
	return getControlBit(Maps::ButtonPlus);
}

boolean DJTurntableControllerBase::buttonMinus() const {
	return getControlBit(Maps::ButtonMinus);
}

DJTurntableControllerBase::TurntableConfig DJTurntableControllerBase::getTurntableConfig() {
	if (tableConfig == TurntableConfig::Both) {
		return tableConfig;  // Both are attached, no reason to check data
	}

	boolean leftState = left.connected();
	boolean rightState = right.connected();

	if (leftState && rightState) {
		return tableConfig = TurntableConfig::Both;
	}
	else if (leftState) {
		return tableConfig = TurntableConfig::Left;
	}
	else if (rightState) {
		return tableConfig = TurntableConfig::Right;
	}
	else {
		return tableConfig = TurntableConfig::BaseOnly;
	}
}

uint8_t DJTurntableControllerBase::getNumTurntables() {
	getTurntableConfig();  // Update config from data

	switch (tableConfig) {
		case TurntableConfig::BaseOnly:
			return 0;
			break;
		case TurntableConfig::Left:
		case TurntableConfig::Right:
			return 1;
			break;
		case TurntableConfig::Both:
			return 2;
			break;
	}
	return 0;  // Just in-case
}

void DJTurntableControllerBase::printDebug(Print& output) {
	const char fillCharacter = '_';

	char buffer[45];

	output.print("DJ:");

	if (getNumTurntables() == 0) {
		output.print(" No turntable found! |");
	}
	else if (left.connected()) {
		printTurntable(output, left);
		output.print(" |");
	}

	const char plusPrint = buttonPlus() ? '+' : fillCharacter;
	const char minusPrint = buttonMinus() ? '-' : fillCharacter;

	const char euphoriaPrint = buttonEuphoria() ? 'E' : fillCharacter;

	snprintf(buffer, sizeof(buffer),
		" Joy:(%2u, %2u) | %c | %c%c | FX: %2u | Fade: %2u",
		joyX(), joyY(),
		euphoriaPrint,
		minusPrint, plusPrint,
		effectDial(), crossfadeSlider());
	output.print(buffer);

	if (right.connected()) {
		output.print(" |");
		printTurntable(output, right);
	}
	output.println();
}

void DJTurntableControllerBase::printTurntable(Print& output, TurntableExpansion &table) const {
	const char fillCharacter = '_';

	char idPrint = 'X';
	if (table.side == TurntableConfig::Left) {
		idPrint = 'L';
	}
	else if (table.side == TurntableConfig::Right) {
		idPrint = 'R';
	}

	const char greenPrint = table.buttonGreen() ? 'G' : fillCharacter;
	const char redPrint = table.buttonRed() ? 'R' : fillCharacter;
	const char bluePrint = table.buttonBlue() ? 'B' : fillCharacter;

	char buffer[15];
	snprintf(buffer, sizeof(buffer),
		" T%c:%3d %c%c%c",
		idPrint,
		table.turntable(),
		greenPrint, redPrint, bluePrint);
	output.print(buffer);
}

// Turntable Expansion Base
boolean DJTurntableControllerBase::TurntableExpansion::connected() const {
	if (base.tableConfig == TurntableConfig::Both || base.tableConfig == side) {
		return true;  // Already checked
	}
	return turntable() != 0 || buttonGreen() || buttonRed() || buttonBlue();
}

// Left Turntable
int8_t DJTurntableControllerBase::TurntableLeft::turntable() const {
	uint8_t turnData = base.getControlData(Maps::Left_Turntable);
	boolean turnSign = base.getControlData(Maps::Left_TurntableSign);
	return getTurntableSpeed(turnData, turnSign);
}

boolean DJTurntableControllerBase::TurntableLeft::buttonGreen() const {
	return base.getControlBit(Maps::Left_ButtonGreen);
}

boolean DJTurntableControllerBase::TurntableLeft::buttonRed() const {
	return base.getControlBit(Maps::Left_ButtonRed);
}

boolean DJTurntableControllerBase::TurntableLeft::buttonBlue() const {
	return base.getControlBit(Maps::Left_ButtonBlue);
}

// Right Turntable
int8_t DJTurntableControllerBase::TurntableRight::turntable() const {
	uint8_t turnData = base.getControlData(Maps::Right_Turntable);
	boolean turnSign = base.getControlData(Maps::Right_TurntableSign);
	return getTurntableSpeed(turnData, turnSign);
}

boolean DJTurntableControllerBase::TurntableRight::buttonGreen() const {
	return base.getControlBit(Maps::Right_ButtonGreen);
}

boolean DJTurntableControllerBase::TurntableRight::buttonRed() const {
	return base.getControlBit(Maps::Right_ButtonRed);
}

boolean DJTurntableControllerBase::TurntableRight::buttonBlue() const {
	return base.getControlBit(Maps::Right_ButtonBlue);
}

// Effect Rollover
int8_t DJTurntableControllerBase::EffectRollover::getChange() {
	return RolloverChange::getChange(dj.effectDial());
}

}  // End "NintendoExtensionCtrl" namespace

