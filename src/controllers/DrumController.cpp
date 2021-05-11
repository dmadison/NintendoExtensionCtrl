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

constexpr ByteMap DrumControllerBase::Maps::JoyX;
constexpr ByteMap DrumControllerBase::Maps::JoyY;

constexpr BitMap  DrumControllerBase::Maps::ButtonPlus;
constexpr BitMap  DrumControllerBase::Maps::ButtonMinus;

constexpr BitMap  DrumControllerBase::Maps::DrumRed;
constexpr BitMap  DrumControllerBase::Maps::DrumBlue;
constexpr BitMap  DrumControllerBase::Maps::DrumGreen;

constexpr BitMap  DrumControllerBase::Maps::CymbalYellow;
constexpr BitMap  DrumControllerBase::Maps::CymbalOrange;

constexpr BitMap  DrumControllerBase::Maps::Pedal;

constexpr ByteMap DrumControllerBase::Maps::Velocity;
constexpr ByteMap DrumControllerBase::Maps::VelocityID;
constexpr BitMap  DrumControllerBase::Maps::VelocityAvailable;

ExtensionType DrumControllerBase::getExpectedType() const {
	return ExtensionType::DrumController;
}

uint8_t DrumControllerBase::joyX() const {
	return getControlData(Maps::JoyX);
}

uint8_t DrumControllerBase::joyY() const {
	return getControlData(Maps::JoyY);
}

boolean DrumControllerBase::drumRed() const {
	return getControlBit(Maps::DrumRed);
}

boolean DrumControllerBase::drumBlue() const {
	return getControlBit(Maps::DrumBlue);
}

boolean DrumControllerBase::drumGreen() const {
	return getControlBit(Maps::DrumGreen);
}

boolean DrumControllerBase::cymbalYellow() const {
	return getControlBit(Maps::CymbalYellow);
}

boolean DrumControllerBase::cymbalOrange() const {
	return getControlBit(Maps::CymbalOrange);
}

boolean DrumControllerBase::bassPedal() const {
	return getControlBit(Maps::Pedal);
}

boolean DrumControllerBase::buttonPlus() const {
	return getControlBit(Maps::ButtonPlus);
}

boolean DrumControllerBase::buttonMinus() const {
	return getControlBit(Maps::ButtonMinus);
}

boolean DrumControllerBase::velocityAvailable() const {
	return getControlBit(Maps::VelocityAvailable);
}

DrumControllerBase::VelocityID DrumControllerBase::velocityID() const {
	uint8_t id = getControlData(Maps::VelocityID);  // 5 bit identifier

	if (validVelocityID(id)) {
		return (VelocityID) id;
	}

	return VelocityID::None;
}

boolean DrumControllerBase::validVelocityID(uint8_t idIn) const {
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

uint8_t DrumControllerBase::velocity() const {
	if (velocityAvailable()) {
		uint8_t velocityRaw = getControlData(Maps::Velocity);
		velocityRaw = 7 - velocityRaw;  // Invert so high = fast attack
		return velocityRaw;
	}
	return 0;  // Invalid data
}

uint8_t DrumControllerBase::velocity(VelocityID idIn) const {
	if (idIn == velocityID()) {
		return velocity();
	}
	return 0;  // ID mismatch
}

uint8_t DrumControllerBase::velocityRed() const {
	return velocity(VelocityID::Red);
}

uint8_t DrumControllerBase::velocityBlue() const {
	return velocity(VelocityID::Blue);
}

uint8_t DrumControllerBase::velocityGreen() const {
	return velocity(VelocityID::Green);
}

uint8_t DrumControllerBase::velocityYellow() const {
	return velocity(VelocityID::Yellow);
}

uint8_t DrumControllerBase::velocityOrange() const {
	return velocity(VelocityID::Orange);
}

uint8_t DrumControllerBase::velocityPedal() const {
	return velocity(VelocityID::Pedal);
}

void DrumControllerBase::printDebug(Print& output) const {
	const char fillCharacter = '_';
	
	char buffer[45];
	
	output.print("Drums: ");

	const char redPrint = drumRed() ? 'R' : fillCharacter;
	const char bluePrint = drumBlue() ? 'B' : fillCharacter;
	const char greenPrint = drumGreen() ? 'G' : fillCharacter;

	const char yellowPrint = cymbalYellow() ? 'Y' : fillCharacter;
	const char orangePrint = cymbalOrange() ? 'O' : fillCharacter;

	const char pedalPrint = bassPedal() ? 'P' : fillCharacter;

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

	const char plusPrint = buttonPlus() ? '+' : fillCharacter;
	const char minusPrint = buttonMinus() ? '-' : fillCharacter;

	snprintf(buffer, sizeof(buffer),
		"%c\\%c%c%c/%c %c | V:%1u for %c | %c%c | Joy:(%2u, %2u)",
		yellowPrint, redPrint, bluePrint, greenPrint, orangePrint, pedalPrint,
		velocityPrint, velocityIDPrint,
		minusPrint, plusPrint,
		joyX(), joyY());
	output.println(buffer);
}

}  // End "NintendoExtensionCtrl" namespace
