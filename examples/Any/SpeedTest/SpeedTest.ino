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
*  Example:      SpeedTest
*  Description:  Connect to an extension controller and record the max
*                number of updates per second.
*/

#include <NintendoExtensionCtrl.h>

ExtensionPort controller;  // Generic controller port, 6 bytes

const long TestDuration = 1000;  // Length of each test, in milliseconds

void setup() {
	Serial.begin(115200);
	controller.begin();

	// Uncomment to run at 400 kHz (default is 100 kHz)
	// controller.i2c.setClock(400000);  // 400 kHz "Fast" I2C

	while (!controller.connect()) {
		Serial.println("No controller detected!");
		delay(1000);
	};

	Serial.println("Starting Speed Test...");
}

void loop() {
	boolean validData = true;
	long numUpdates = 0;
	long millisStart = millis();

	while (millis() - millisStart <= TestDuration && validData) {
		validData = controller.update();  // Update and check if sucessful
		numUpdates++;
	}
	
	if (!validData) {
		Serial.println("ERROR! Invalid data received!");

		while (!controller.connect()) {
			Serial.println("Attempting to reconnect...");
			delay(1000);
		}
	}
	else {
		Serial.print("Success! Completed ");
		Serial.print(numUpdates);
		Serial.print(" updates in ");
		Serial.print(TestDuration);
		Serial.println(" milliseconds.");
	}
}

