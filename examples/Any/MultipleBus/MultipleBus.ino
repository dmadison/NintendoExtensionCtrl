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
*
*  Example:      MultipleBus
*  Description:  Communicate with two extension controllers, each on their own
*                I2C bus (e.g. Wire and Wire1). Requires a microcontroller
*                with two I2C buses (e.g. Arduino Due or Teensy).
*
*                This example uses Nunchuks, but this process works the same
*                with any controller in the library.
*/

#include <NintendoExtensionCtrl.h>

Nunchuk nchuk1(Wire);   // Controller on bus #1
Nunchuk nchuk2(Wire1);  // Controller on bus #2

void setup() {
	Serial.begin(115200);

	while (!Serial);  // Wait for serial for debug
	Serial.println("Attempting connection to controllers...");

	nchuk1.begin();
	while (!nchuk1.connect()) {
		Serial.println("Nunchuk on bus #1 not detected!");
		delay(1000);
	}

	nchuk2.begin();
	while (!nchuk2.connect()) {
		Serial.println("Nunchuk on bus #2 not detected!");
		delay(1000);
	}
}

void loop() {
	Serial.println("-------------");

	if (nchuk1.update()) {
		nchuk1.printDebug();
	}
	else {
		Serial.println("Bus #1 Disconnected");
	}

	if (nchuk2.update()) {
		nchuk2.printDebug();
	}
	else {
		Serial.println("Bus #2 Disconnected");
	}
}
