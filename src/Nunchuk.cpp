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

Nunchuk::Nunchuk() : ExtensionController(NXC_Nunchuk, 6) {}

uint8_t Nunchuk::joyX() {
	return controlData[0];
}

uint8_t Nunchuk::joyY() {
	return controlData[1];
}

uint16_t Nunchuk::accelX() {
	return controlData[2] << 2 | ((controlData[5] >> 6) & 0x03) ;
}

uint16_t Nunchuk::accelY() {
	return controlData[3] << 2 | ((controlData[5] >> 4) & 0x03);
}

uint16_t Nunchuk::accelZ() {
	return controlData[4] << 2 | ((controlData[5] >> 2) & 0x03);
}

boolean Nunchuk::buttonC() {
	return extractControlBit(5, 1);
}

boolean Nunchuk::buttonZ() {
	return extractControlBit(5, 0);
}

float Nunchuk::rollAngle() {
	return atan2((float)accelX() - 511.0, (float)accelZ() - 511.0) * 180.0 / PI;
}

float Nunchuk::pitchAngle() {
	// Inverted so pulling back is a positive pitch
	return -atan2((float)accelY() - 511.0, (float)accelZ() - 511.0) * 180.0 / PI;
}

void Nunchuk::printDebug(Stream& stream) {
	char buffer[60];

	char cPrint = buttonC() ? 'C' : '-';
	char zPrint = buttonZ() ? 'Z' : '-';

	stream.print("Nunchuk - ");
	sprintf(buffer,
		"Joy:(%3u, %3u) | Accel XYZ:(%4u, %4u, %4u) | Buttons: %c%c",
		joyX(), joyY(), accelX(), accelY(), accelZ(), cPrint, zPrint);
	
	stream.println(buffer);
}
