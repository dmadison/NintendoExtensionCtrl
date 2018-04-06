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

#include "DrumController.h"

DrumController::DrumController(NXC_I2C_TYPE& i2cBus) : ExtensionController(i2cBus, NXC_DrumController, 6) {}

uint8_t DrumController::joyX() {
	return getControlData(0) & 0x3F;
}

uint8_t DrumController::joyY() {
	return getControlData(1) & 0x3F;
}

boolean DrumController::drumRed() {
	return extractControlBit(5, 6);
}

boolean DrumController::drumBlue() {
	return extractControlBit(5, 3);
}

boolean DrumController::drumGreen() {
	return extractControlBit(5, 4);
}

boolean DrumController::cymbalYellow() {
	return extractControlBit(5, 5);
}

boolean DrumController::cymbalOrange() {
	return extractControlBit(5, 7);
}

boolean DrumController::bassPedal() {
	return extractControlBit(5, 2);
}

boolean DrumController::buttonPlus() {
	return extractControlBit(4, 2);
}

boolean DrumController::buttonMinus() {
	return extractControlBit(4, 4);
}

boolean DrumController::velocityAvailable() {
	return extractControlBit(2, 6);
}

NXC_DrumVelocityID DrumController::velocityID() {
	uint8_t velocityID = (getControlData(2) & 0x3E) >> 1;  // 5 bit identifier

	if (validVelocityID(velocityID)) {
		return (NXC_DrumVelocityID) velocityID;
	}

	return NXC_Drum_None;
}

boolean DrumController::validVelocityID(uint8_t idIn) {
	switch (idIn) {
		case(NXC_Drum_None):  // Intentionally fall through cases
		case(NXC_Drum_Red):
		case(NXC_Drum_Blue):
		case(NXC_Drum_Green):
		case(NXC_Drum_Yellow):
		case(NXC_Drum_Orange):
		case(NXC_Drum_Pedal):
			return true;  // One of the above, is a valid ID
			break;
		default:
			return false;  // Not one of the above, invalid
	}
}

uint8_t DrumController::velocity() {
	if (velocityAvailable()) {
		uint8_t velocityRaw = (getControlData(3) & 0xE0) >> 5;
		velocityRaw = 7 - velocityRaw;  // Invert so high = fast attack
		return velocityRaw;
	}
	return 0;  // Invalid data
}

uint8_t DrumController::velocity(NXC_DrumVelocityID idIn) {
	if (idIn == velocityID()) {
		return velocity();
	}
	return 0;  // ID mismatch
}

uint8_t DrumController::velocityRed() {
	return velocity(NXC_Drum_Red);
}

uint8_t DrumController::velocityBlue() {
	return velocity(NXC_Drum_Blue);
}

uint8_t DrumController::velocityGreen() {
	return velocity(NXC_Drum_Green);
}

uint8_t DrumController::velocityYellow() {
	return velocity(NXC_Drum_Yellow);
}

uint8_t DrumController::velocityOrange() {
	return velocity(NXC_Drum_Orange);
}

uint8_t DrumController::velocityPedal() {
	return velocity(NXC_Drum_Pedal);  // Fix this
}

void DrumController::printDebug(Stream& stream) {
	const char fillCharacter = '_';
	
	char buffer[45];
	
	stream.print("Drums: ");

	char redPrint = drumRed() ? 'R' : fillCharacter;
	char bluePrint = drumBlue() ? 'B' : fillCharacter;
	char greenPrint = drumGreen() ? 'G' : fillCharacter;

	char yellowPrint = cymbalYellow() ? 'Y' : fillCharacter;
	char orangePrint = cymbalOrange() ? 'O' : fillCharacter;

	char pedalPrint = bassPedal() ? 'P' : fillCharacter;

	uint8_t velocityPrint = 0;
	char velocityIDPrint = fillCharacter;

	if (velocityAvailable()) {
		velocityPrint = velocity();

		switch (velocityID()) {
			case (NXC_Drum_None):
				break;  // Keep as fillCharacter
			case(NXC_Drum_Red):
				velocityIDPrint = 'R';
				break;
			case(NXC_Drum_Blue):
				velocityIDPrint = 'B';
				break;
			case(NXC_Drum_Green):
				velocityIDPrint = 'G';
				break;
			case(NXC_Drum_Yellow):
				velocityIDPrint = 'Y';
				break;
			case(NXC_Drum_Orange):
				velocityIDPrint = 'O';
				break;
			case(NXC_Drum_Pedal):
				velocityIDPrint = 'P';
				break;
		}
	}

	char plusPrint = buttonPlus() ? '+' : fillCharacter;
	char minusPrint = buttonMinus() ? '-' : fillCharacter;

	sprintf(buffer,
		"%c\\%c%c%c/%c %c | V:%1u for %c | %c%c | Joy:(%2u, %2u)",
		yellowPrint, redPrint, bluePrint, greenPrint, orangePrint, pedalPrint,
		velocityPrint, velocityIDPrint,
		minusPrint, plusPrint,
		joyX(), joyY());
	stream.println(buffer);
}
