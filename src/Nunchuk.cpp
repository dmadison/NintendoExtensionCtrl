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

Nunchuk::Nunchuk() {}

uint8_t Nunchuk::getJoyX() {
	return controlData[0];
}

uint8_t Nunchuk::getJoyY() {
	return controlData[1];
}

uint16_t Nunchuk::getAccelX() {
	return controlData[2] << 2 | ((controlData[5] >> 6) & 0x03) ;
}

uint16_t Nunchuk::getAccelY() {
	return controlData[3] << 2 | ((controlData[5] >> 4) & 0x03);
}

uint16_t Nunchuk::getAccelZ() {
	return controlData[4] << 2 | ((controlData[5] >> 2) & 0x03);
}

boolean Nunchuk::getButtonC() {
	return !(controlData[5] & 0x02);
}

boolean Nunchuk::getButtonZ() {
	return !(controlData[5] & 0x01);
}

float Nunchuk::getRollAngle() {
	return atan2((float)getAccelX() - 511.0, (float)getAccelZ() - 511.0) * 180.0 / PI;
}

float Nunchuk::getPitchAngle() {
	// Inverted so pulling back is a positive pitch
	return -atan2((float)getAccelY() - 511.0, (float)getAccelZ() - 511.0) * 180.0 / PI;
}

void Nunchuk::printDebug(Stream& stream) {
	char buffer[75];

	char cPrint = getButtonC() ? 'C' : '-';
	char zPrint = getButtonZ() ? 'Z' : '-';

	stream.print("Nunchuk - ");
	sprintf(buffer,
		"JoyX: %3u | JoyY: %3u | Ax: %4u | Ay: %4u | Az: %4u | Buttons: %c%c",
		getJoyX(), getJoyY(), getAccelX(), getAccelY(), getAccelZ(), cPrint, zPrint);
	
	stream.println(buffer);
}
