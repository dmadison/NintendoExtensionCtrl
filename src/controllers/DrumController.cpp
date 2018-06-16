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
	return getControlData(Maps::JoyX);
}

uint8_t DrumController_Data::joyY() const {
	return getControlData(Maps::JoyY);
}

boolean DrumController_Data::drumRed() const {
	return getControlBit(Maps::DrumRed);
}

boolean DrumController_Data::drumBlue() const {
	return getControlBit(Maps::DrumBlue);
}

boolean DrumController_Data::drumGreen() const {
	return getControlBit(Maps::DrumGreen);
}

boolean DrumController_Data::cymbalYellow() const {
	return getControlBit(Maps::CymbalYellow);
}

boolean DrumController_Data::cymbalOrange() const {
	return getControlBit(Maps::CymbalOrange);
}

boolean DrumController_Data::bassPedal() const {
	return getControlBit(Maps::Pedal);
}

boolean DrumController_Data::buttonPlus() const {
	return getControlBit(Maps::ButtonPlus);
}

boolean DrumController_Data::buttonMinus() const {
	return getControlBit(Maps::ButtonMinus);
}

boolean DrumController_Data::velocityAvailable() const {
	return getControlBit(Maps::VelocityAvailable);
}

DrumController_Data::VelocityID DrumController_Data::velocityID() const {
	uint8_t id = getControlData(Maps::VelocityID);  // 5 bit identifier

	if (validVelocityID(id)) {
		return (VelocityID) id;
	}

	return VelocityID::None;
}

boolean DrumController_Data::validVelocityID(uint8_t idIn) const {
	switch (idIn) {
		case(VelocityID::None):  // Intentionally fall through cases
		case(VelocityID::Red):
		case(VelocityID::Blue):
		case(VelocityID::Green):
		case(VelocityID::Yellow):
		case(VelocityID::Orange):
		case(VelocityID::Pedal):
			return true;  // One of the above, is a valid ID
			break;
		default:
			return false;  // Not one of the above, invalid
	}
}

uint8_t DrumController_Data::velocity() const {
	if (velocityAvailable()) {
		uint8_t velocityRaw = getControlData(Maps::Velocity);
		velocityRaw = 7 - velocityRaw;  // Invert so high = fast attack
		return velocityRaw;
	}
	return 0;  // Invalid data
}

uint8_t DrumController_Data::velocity(VelocityID idIn) const {
	if (idIn == velocityID()) {
		return velocity();
	}
	return 0;  // ID mismatch
}

uint8_t DrumController_Data::velocityRed() const {
	return velocity(VelocityID::Red);
}

uint8_t DrumController_Data::velocityBlue() const {
	return velocity(VelocityID::Blue);
}

uint8_t DrumController_Data::velocityGreen() const {
	return velocity(VelocityID::Green);
}

uint8_t DrumController_Data::velocityYellow() const {
	return velocity(VelocityID::Yellow);
}

uint8_t DrumController_Data::velocityOrange() const {
	return velocity(VelocityID::Orange);
}

uint8_t DrumController_Data::velocityPedal() const {
	return velocity(VelocityID::Pedal);
}

void DrumController_Data::printDebug(Print& output) const {
	const char fillCharacter = '_';
	
	char buffer[45];
	
	output.print("Drums: ");

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
			case (VelocityID::None):
				break;  // Keep as fillCharacter
			case(VelocityID::Red):
				velocityIDPrint = 'R';
				break;
			case(VelocityID::Blue):
				velocityIDPrint = 'B';
				break;
			case(VelocityID::Green):
				velocityIDPrint = 'G';
				break;
			case(VelocityID::Yellow):
				velocityIDPrint = 'Y';
				break;
			case(VelocityID::Orange):
				velocityIDPrint = 'O';
				break;
			case(VelocityID::Pedal):
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
	output.println(buffer);
}

}  // End "NintendoExtensionCtrl" namespace
