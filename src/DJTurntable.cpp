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

DJTurntableController::DJTurntableController() : ExtensionController(NXC_DJTurntable, 6) {}

// Left Turntable
int8_t DJTurntableController::leftTurntable() {
	int8_t turnData = controlData[3] & 0x1F;
	turnData |= ((controlData[4] & 0x01) << 5);  // Sign bit

	if (turnData >= 32) {  // Convert to signed scale
		turnData = -32 + (turnData - 32);
	}
	return turnData;
}

boolean DJTurntableController::leftButtonGreen() {
	return extractControlBit(5, 3);
}

boolean DJTurntableController::leftButtonRed() {
	return extractControlBit(4, 5);
}

boolean DJTurntableController::leftButtonBlue() {
	return extractControlBit(5, 7);
}

boolean DJTurntableController::leftConnected() {
	if (tableConfig == NXC_DJTurntable_Both || tableConfig == NXC_DJTurntable_Left) {
		return true;  // Already checked
	}
	return leftTurntable() != 0 | leftButtonGreen() | leftButtonRed() | leftButtonBlue();  // Any data = connected
}

// Right Turntable
int8_t DJTurntableController::rightTurntable() {
	int8_t turnData = ((controlData[0] & 0xC0) >> 3) | ((controlData[1] & 0xC0) >> 5) | ((controlData[2] & 0x80) >> 7);
	turnData |= ((controlData[2] & 0x01) << 5);  // Sign bit

	if (turnData >= 32) {  // Convert to signed scale
		turnData = -32 + (turnData - 32);
	}
	return turnData;
}

boolean DJTurntableController::rightButtonGreen() {
	return extractControlBit(5, 5);
}

boolean DJTurntableController::rightButtonRed() {
	return extractControlBit(4, 1);
}

boolean DJTurntableController::rightButtonBlue() {
	return extractControlBit(5, 2);
}

boolean DJTurntableController::rightConnected() {
	if (tableConfig == NXC_DJTurntable_Both || tableConfig == NXC_DJTurntable_Right) {
		return true;  // Already checked
	}
	return rightTurntable() != 0 | rightButtonGreen() | rightButtonRed() | rightButtonBlue();  // Any data = connected
}

// Combined Turntable
int8_t DJTurntableController::turntable() {
	return leftTurntable() + rightTurntable();
}

boolean DJTurntableController::buttonGreen() {
	return leftButtonGreen() | rightButtonGreen();
}

boolean DJTurntableController::buttonRed() {
	return leftButtonRed() | rightButtonRed();
}

boolean DJTurntableController::buttonBlue() {
	return leftButtonBlue() | rightButtonBlue();
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

	boolean leftState = leftConnected();
	boolean rightState = rightConnected();

	if (leftState && rightState) {
		return tableConfig = NXC_DJTurntable_Both;
	}
	if (leftState) {
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
}

void DJTurntableController::printDebug(Stream& stream) {
	const char fillCharacter = '_';

	char buffer[45];

	stream.print("DJ:");

	if (getNumTurntables() == 0) {
		stream.print(" No turntable found! |");
	}
	else if (leftConnected()) {
		printTurntable(stream, NXC_DJTurntable_Left);
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

	if (rightConnected()) {
		stream.print(" |");
		printTurntable(stream, NXC_DJTurntable_Right);
	}
	stream.println();
}

void DJTurntableController::printTurntable(Stream& stream, NXC_DJTurntable_Configuration table) {
	const char fillCharacter = '_';
	char buffer[13];

	char idPrint;
	int8_t tablePrint;
	char greenPrint;
	char redPrint;
	char bluePrint;

	if (table == NXC_DJTurntable_Left) {
		idPrint = 'L';
		tablePrint = leftTurntable();
		greenPrint = leftButtonGreen() ? 'G' : fillCharacter;
		redPrint = leftButtonRed() ? 'R' : fillCharacter;
		bluePrint = leftButtonBlue() ? 'B' : fillCharacter;
	}
	else if (table == NXC_DJTurntable_Right) {
		idPrint = 'R';
		tablePrint = rightTurntable();
		greenPrint = rightButtonGreen() ? 'G' : fillCharacter;
		redPrint = rightButtonRed() ? 'R' : fillCharacter;
		bluePrint = rightButtonBlue() ? 'B' : fillCharacter;
	}
	else {
		return;  // Don't print anything
	}

	sprintf(buffer,
		" T%c:%3d %c%c%c",
		idPrint,
		tablePrint,
		greenPrint, redPrint, bluePrint);
	stream.print(buffer);
}
