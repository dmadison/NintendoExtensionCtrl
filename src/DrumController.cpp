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

namespace NintendoExtensionCtrl {

uint8_t DrumController_Data::joyX() const {
	return getControlData(0) & 0x3F;
}

uint8_t DrumController_Data::joyY() const {
	return getControlData(1) & 0x3F;
}

boolean DrumController_Data::drumRed() const {
	return getControlBit(5, 6);
}

boolean DrumController_Data::drumBlue() const {
	return getControlBit(5, 3);
}

boolean DrumController_Data::drumGreen() const {
	return getControlBit(5, 4);
}

boolean DrumController_Data::cymbalYellow() const {
	return getControlBit(5, 5);
}

boolean DrumController_Data::cymbalOrange() const {
	return getControlBit(5, 7);
}

boolean DrumController_Data::bassPedal() const {
	return getControlBit(5, 2);
}

boolean DrumController_Data::buttonPlus() const {
	return getControlBit(4, 2);
}

boolean DrumController_Data::buttonMinus() const {
	return getControlBit(4, 4);
}

boolean DrumController_Data::velocityAvailable() const {
	return getControlBit(2, 6);
}

NXC_DrumVelocityID DrumController_Data::velocityID() const {
	uint8_t velocityID = (getControlData(2) & 0x3E) >> 1;  // 5 bit identifier

	if (validVelocityID(velocityID)) {
		return (NXC_DrumVelocityID) velocityID;
	}

	return NXC_Drum_None;
}

boolean DrumController_Data::validVelocityID(uint8_t idIn) const {
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

uint8_t DrumController_Data::velocity() const {
	if (velocityAvailable()) {
		uint8_t velocityRaw = (getControlData(3) & 0xE0) >> 5;
		velocityRaw = 7 - velocityRaw;  // Invert so high = fast attack
		return velocityRaw;
	}
	return 0;  // Invalid data
}

uint8_t DrumController_Data::velocity(NXC_DrumVelocityID idIn) const {
	if (idIn == velocityID()) {
		return velocity();
	}
	return 0;  // ID mismatch
}

uint8_t DrumController_Data::velocityRed() const {
	return velocity(NXC_Drum_Red);
}

uint8_t DrumController_Data::velocityBlue() const {
	return velocity(NXC_Drum_Blue);
}

uint8_t DrumController_Data::velocityGreen() const {
	return velocity(NXC_Drum_Green);
}

uint8_t DrumController_Data::velocityYellow() const {
	return velocity(NXC_Drum_Yellow);
}

uint8_t DrumController_Data::velocityOrange() const {
	return velocity(NXC_Drum_Orange);
}

uint8_t DrumController_Data::velocityPedal() const {
	return velocity(NXC_Drum_Pedal);  // Fix this
}

void DrumController_Data::printDebug(Stream& stream) const {
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

}  // End "NintendoExtensionCtrl" namespace
