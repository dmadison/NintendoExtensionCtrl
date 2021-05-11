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

constexpr IndexMap NunchukBase::Maps::JoyX;
constexpr IndexMap NunchukBase::Maps::JoyY;

constexpr IndexMap NunchukBase::Maps::AccelX_MSB;
constexpr ByteMap  NunchukBase::Maps::AccelX_LSB;

constexpr IndexMap NunchukBase::Maps::AccelY_MSB;
constexpr ByteMap  NunchukBase::Maps::AccelY_LSB;

constexpr IndexMap NunchukBase::Maps::AccelZ_MSB;
constexpr ByteMap  NunchukBase::Maps::AccelZ_LSB;

constexpr BitMap   NunchukBase::Maps::ButtonC;
constexpr BitMap   NunchukBase::Maps::ButtonZ;

ExtensionType NunchukBase::getExpectedType() const {
	return ExtensionType::Nunchuk;
}

uint8_t NunchukBase::joyX() const {
	return getControlData(Maps::JoyX);
}

uint8_t NunchukBase::joyY() const {
	return getControlData(Maps::JoyY);
}

uint16_t NunchukBase::accelX() const {
	return (getControlData(Maps::AccelX_MSB) << 2) | getControlData(Maps::AccelX_LSB);
}

uint16_t NunchukBase::accelY() const {
	return (getControlData(Maps::AccelY_MSB) << 2) | getControlData(Maps::AccelY_LSB);
}

uint16_t NunchukBase::accelZ() const {
	return (getControlData(Maps::AccelZ_MSB) << 2) | getControlData(Maps::AccelZ_LSB);
}

boolean NunchukBase::buttonC() const {
	return getControlBit(Maps::ButtonC);
}

boolean NunchukBase::buttonZ() const {
	return getControlBit(Maps::ButtonZ);
}

float NunchukBase::rollAngle() const {
	return atan2((float)accelX() - 511.0, (float)accelZ() - 511.0) * 180.0 / PI;
}

float NunchukBase::pitchAngle() const {
	// Inverted so pulling back is a positive pitch
	return -atan2((float)accelY() - 511.0, (float)accelZ() - 511.0) * 180.0 / PI;
}

void NunchukBase::printDebug(Print& output) const {
	// 59 characters, 1 terminating null, and 7 extra so the compiler stops
	// complaining about not having enough buffer space for the full 16 bit
	// values (5 characters each) that can fit in the type
	char buffer[67];

	const char cPrint = buttonC() ? 'C' : '-';
	const char zPrint = buttonZ() ? 'Z' : '-';

	output.print("Nunchuk - ");
	snprintf(buffer, sizeof(buffer),
		"Joy:(%3u, %3u) | Accel XYZ:(%4u, %4u, %4u) | Buttons: %c%c",
		joyX(), joyY(), accelX(), accelY(), accelZ(), cPrint, zPrint);

	output.println(buffer);
}

}  // End "NintendoExtensionCtrl" namespace
