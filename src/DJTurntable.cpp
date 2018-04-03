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

DJTurntableController::DJTurntableController() : ExtensionController(NXC_DJTurntable, 6), left(*this), right(*this) {}

// Combined Turntable
int8_t DJTurntableController::turntable() {
	return left.turntable() + right.turntable();
}

boolean DJTurntableController::buttonGreen() {
	return left.buttonGreen() | right.buttonGreen();
}

boolean DJTurntableController::buttonRed() {
	return left.buttonRed() | right.buttonRed();
}

boolean DJTurntableController::buttonBlue() {
	return left.buttonBlue() | right.buttonBlue();
}

// Main Board
uint8_t DJTurntableController::effectDial() {
	return ((controlData[2] & 0x60) >> 2) | ((controlData[3] & 0xE0) >> 5);
}

int8_t DJTurntableController::crossfadeSlider() {
	return ((controlData[2] & 0x1E) >> 1) - 8;  // Shifted to signed int
}

boolean DJTurntableController::buttonEuphoria() {
	return extractControlBit(5, 4);
}

uint8_t DJTurntableController::joyX() {
	return controlData[0] & 0x3F;
}

uint8_t DJTurntableController::joyY() {
	return controlData[1] & 0x3F;
}

boolean DJTurntableController::buttonPlus() {
	return extractControlBit(4, 2);
}

boolean DJTurntableController::buttonMinus() {
	return extractControlBit(4, 4);
}

NXC_DJTurntable_Configuration DJTurntableController::getTurntableConfig() {
	if (tableConfig == NXC_DJTurntable_Both) {
		return tableConfig;  // Both are attached, no reason to check data
	}

	boolean leftState = left.connected();
	boolean rightState = right.connected();

	if (leftState && rightState) {
		return tableConfig = NXC_DJTurntable_Both;
	}
	else if (leftState) {
		return tableConfig = NXC_DJTurntable_Left;
	}
	else if (rightState) {
		return tableConfig = NXC_DJTurntable_Right;
	}
	else {
		return tableConfig = NXC_DJTurntable_BaseOnly;
	}
}

uint8_t DJTurntableController::getNumTurntables() {
	getTurntableConfig();  // Update config from data

	switch (tableConfig) {
		case NXC_DJTurntable_BaseOnly:
			return 0;
			break;
		case NXC_DJTurntable_Left:
		case NXC_DJTurntable_Right:
			return 1;
			break;
		case NXC_DJTurntable_Both:
			return 2;
			break;
	}
	return 0;  // Just in-case
}

void DJTurntableController::printDebug(Stream& stream) {
	const char fillCharacter = '_';

	char buffer[45];

	stream.print("DJ:");

	if (getNumTurntables() == 0) {
		stream.print(" No turntable found! |");
	}
	else if (left.connected()) {
		printTurntable(stream, left);
		stream.print(" |");
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
	stream.print(buffer);

	if (right.connected()) {
		stream.print(" |");
		printTurntable(stream, right);
	}
	stream.println();
}

void DJTurntableController::printTurntable(Stream& stream, TurntableExpansion &table) {
	const char fillCharacter = '_';

	char idPrint = 'X';
	if (table.side == NXC_DJTurntable_Left) {
		idPrint = 'L';
	}
	else if (table.side == NXC_DJTurntable_Right) {
		idPrint = 'R';
	}

	int8_t tablePrint = table.turntable();
	char greenPrint = table.buttonGreen() ? 'G' : fillCharacter;
	char redPrint = table.buttonRed() ? 'R' : fillCharacter;
	char bluePrint = table.buttonBlue() ? 'B' : fillCharacter;

	char buffer[15];
	sprintf(buffer,
		" T%c:%3d %c%c%c",
		idPrint,
		tablePrint,
		greenPrint, redPrint, bluePrint);
	stream.print(buffer);
}

// Turntable Expansion Base
boolean DJTurntableController::TurntableExpansion::connected() {
	if (base.tableConfig == NXC_DJTurntable_Both || base.tableConfig == side) {
		return true;  // Already checked
	}
	return turntable() != 0 || buttonGreen() || buttonRed() || buttonBlue();
}

int8_t DJTurntableController::TurntableExpansion::tableSignConversion(int8_t turnData) {
	if (turnData >= 32) {  // Scales the unsigned 0-63 as signed, symmetrical about 0
		turnData = -32 + (turnData - 32);
	}
	return turnData;
}

// Left Turntable
int8_t DJTurntableController::TurntableLeft::turntable() {
	int8_t turnData = base.controlData[3] & 0x1F;
	turnData |= ((base.controlData[4] & 0x01) << 5);  // Sign bit

	return tableSignConversion(turnData);
}

boolean DJTurntableController::TurntableLeft::buttonGreen() {
	return base.extractControlBit(5, 3);
}

boolean DJTurntableController::TurntableLeft::buttonRed() {
	return base.extractControlBit(4, 5);
}

boolean DJTurntableController::TurntableLeft::buttonBlue() {
	return base.extractControlBit(5, 7);
}

// Right Turntable
int8_t DJTurntableController::TurntableRight::turntable() {
	int8_t turnData = ((base.controlData[0] & 0xC0) >> 3) | ((base.controlData[1] & 0xC0) >> 5) | ((base.controlData[2] & 0x80) >> 7);
	turnData |= ((base.controlData[2] & 0x01) << 5);  // Sign bit

	return tableSignConversion(turnData);
}

boolean DJTurntableController::TurntableRight::buttonGreen() {
	return base.extractControlBit(5, 5);
}

boolean DJTurntableController::TurntableRight::buttonRed() {
	return base.extractControlBit(4, 1);
}

boolean DJTurntableController::TurntableRight::buttonBlue() {
	return base.extractControlBit(5, 2);
}
