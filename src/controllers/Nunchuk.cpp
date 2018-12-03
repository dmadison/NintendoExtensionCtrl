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

#include "Nunchuk.h"

namespace NintendoExtensionCtrl {

constexpr CtrlIndex Nunchuk_Data::Maps::JoyX;
constexpr CtrlIndex Nunchuk_Data::Maps::JoyY;

constexpr CtrlIndex Nunchuk_Data::Maps::AccelX_MSB;
constexpr ByteMap   Nunchuk_Data::Maps::AccelX_LSB;

constexpr CtrlIndex Nunchuk_Data::Maps::AccelY_MSB;
constexpr ByteMap   Nunchuk_Data::Maps::AccelY_LSB;

constexpr CtrlIndex Nunchuk_Data::Maps::AccelZ_MSB;
constexpr ByteMap   Nunchuk_Data::Maps::AccelZ_LSB;

constexpr BitMap    Nunchuk_Data::Maps::ButtonC;
constexpr BitMap    Nunchuk_Data::Maps::ButtonZ;

uint8_t Nunchuk_Data::joyX() const {
	return getControlData(Maps::JoyX);
}

uint8_t Nunchuk_Data::joyY() const {
	return getControlData(Maps::JoyY);
}

uint16_t Nunchuk_Data::accelX() const {
	return (getControlData(Maps::AccelX_MSB) << 2) | getControlData(Maps::AccelX_LSB);
}

uint16_t Nunchuk_Data::accelY() const {
	return (getControlData(Maps::AccelY_MSB) << 2) | getControlData(Maps::AccelY_LSB);
}

uint16_t Nunchuk_Data::accelZ() const {
	return (getControlData(Maps::AccelZ_MSB) << 2) | getControlData(Maps::AccelZ_LSB);
}

boolean Nunchuk_Data::buttonC() const {
	return getControlBit(Maps::ButtonC);
}

boolean Nunchuk_Data::buttonZ() const {
	return getControlBit(Maps::ButtonZ);
}

float Nunchuk_Data::rollAngle() const {
	return atan2((float)accelX() - 511.0, (float)accelZ() - 511.0) * 180.0 / PI;
}

float Nunchuk_Data::pitchAngle() const {
	// Inverted so pulling back is a positive pitch
	return -atan2((float)accelY() - 511.0, (float)accelZ() - 511.0) * 180.0 / PI;
}

void Nunchuk_Data::printDebug(Print& output) const {
	char buffer[60];

	char cPrint = buttonC() ? 'C' : '-';
	char zPrint = buttonZ() ? 'Z' : '-';

	output.print("Nunchuk - ");
	sprintf(buffer,
		"Joy:(%3u, %3u) | Accel XYZ:(%4u, %4u, %4u) | Buttons: %c%c",
		joyX(), joyY(), accelX(), accelY(), accelZ(), cPrint, zPrint);

	output.println(buffer);
}

}  // End "NintendoExtensionCtrl" namespace
