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

// Combined Turntable
int8_t DJTurntableController_Data::turntable() const {
	return left.turntable() + right.turntable();
}

boolean DJTurntableController_Data::buttonGreen() const {
	return left.buttonGreen() | right.buttonGreen();
}

boolean DJTurntableController_Data::buttonRed() const {
	return left.buttonRed() | right.buttonRed();
}

boolean DJTurntableController_Data::buttonBlue() const {
	return left.buttonBlue() | right.buttonBlue();
}

// Main Board
uint8_t DJTurntableController_Data::effectDial() const {
	return ((getControlData(2) & 0x60) >> 2) | ((getControlData(3) & 0xE0) >> 5);
}

int8_t DJTurntableController_Data::crossfadeSlider() const {
	return ((getControlData(2) & 0x1E) >> 1) - 8;  // Shifted to signed int
}

boolean DJTurntableController_Data::buttonEuphoria() const {
	return getControlBit(5, 4);
}

uint8_t DJTurntableController_Data::joyX() const {
	return getControlData(0) & 0x3F;
}

uint8_t DJTurntableController_Data::joyY() const {
	return getControlData(1) & 0x3F;
}

boolean DJTurntableController_Data::buttonPlus() const {
	return getControlBit(4, 2);
}

boolean DJTurntableController_Data::buttonMinus() const {
	return getControlBit(4, 4);
}

DJTurntableController_Data::TurntableConfig DJTurntableController_Data::getTurntableConfig() {
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

uint8_t DJTurntableController_Data::getNumTurntables() {
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

void DJTurntableController_Data::printDebug(Print& output) {
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

	char plusPrint = buttonPlus() ? '+' : fillCharacter;
	char minusPrint = buttonMinus() ? '-' : fillCharacter;

	char euphoriaPrint = buttonEuphoria() ? 'E' : fillCharacter;

	sprintf(buffer,
		" Joy:(%2u, %2u) | %c | %c%c | FX: %2u | Fade: %2d",
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

void DJTurntableController_Data::printTurntable(Print& output, TurntableExpansion &table) const {
	const char fillCharacter = '_';

	char idPrint = 'X';
	if (table.side == TurntableConfig::Left) {
		idPrint = 'L';
	}
	else if (table.side == TurntableConfig::Right) {
		idPrint = 'R';
	}

	char greenPrint = table.buttonGreen() ? 'G' : fillCharacter;
	char redPrint = table.buttonRed() ? 'R' : fillCharacter;
	char bluePrint = table.buttonBlue() ? 'B' : fillCharacter;

	char buffer[15];
	sprintf(buffer,
		" T%c:%3d %c%c%c",
		idPrint,
		table.turntable(),
		greenPrint, redPrint, bluePrint);
	output.print(buffer);
}

// Turntable Expansion Base
boolean DJTurntableController_Data::TurntableExpansion::connected() const {
	if (base.tableConfig == TurntableConfig::Both || base.tableConfig == side) {
		return true;  // Already checked
	}
	return turntable() != 0 || buttonGreen() || buttonRed() || buttonBlue();
}

int8_t DJTurntableController_Data::TurntableExpansion::tableSignConversion(int8_t turnData) const {
	if (turnData & 0x80) {  // If sign bit is 1...
		turnData |= 0x60;  // Flip missing bits to '1's
	}
	return turnData;
}

// Left Turntable
int8_t DJTurntableController_Data::TurntableLeft::turntable() const {
	int8_t turnData = base.getControlData(3) & 0x1F;
	turnData |= ((base.getControlData(4) & 0x01) << 7);  // Sign bit

	return tableSignConversion(turnData);
}

boolean DJTurntableController_Data::TurntableLeft::buttonGreen() const {
	return base.getControlBit(5, 3);
}

boolean DJTurntableController_Data::TurntableLeft::buttonRed() const {
	return base.getControlBit(4, 5);
}

boolean DJTurntableController_Data::TurntableLeft::buttonBlue() const {
	return base.getControlBit(5, 7);
}

// Right Turntable
int8_t DJTurntableController_Data::TurntableRight::turntable() const {
	int8_t turnData = ((base.getControlData(0) & 0xC0) >> 3) | ((base.getControlData(1) & 0xC0) >> 5) | ((base.getControlData(2) & 0x80) >> 7);
	turnData |= ((base.getControlData(2) & 0x01) << 7);  // Sign bit

	return tableSignConversion(turnData);
}

boolean DJTurntableController_Data::TurntableRight::buttonGreen() const {
	return base.getControlBit(5, 5);
}

boolean DJTurntableController_Data::TurntableRight::buttonRed() const {
	return base.getControlBit(4, 1);
}

boolean DJTurntableController_Data::TurntableRight::buttonBlue() const {
	return base.getControlBit(5, 2);
}

// Effect Rollover
int8_t DJTurntableController_Data::EffectRollover::getChange() {
	return RolloverChange::getChange(dj.effectDial());
}

}  // End "NintendoExtensionCtrl" namespace

