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
*  Example:      Nunchuk_Demo
*  Description:  Connect to a Nunchuk and demonstrate all of the avaiable
*                control data functions.
*/

#include <NintendoExtensionCtrl.h>

Nunchuk nchuk;

void setup() {
	Serial.begin(115200);
	nchuk.begin();

	while (!nchuk.connect()) {
		Serial.println("Nunchuk not detected!");
		delay(1000);
	}
}

void loop() {
	Serial.println("----- Nunchuk Demo -----"); // Making things easier to read
	
	boolean success = nchuk.update();  // Get new data from the controller

	if (!success) {  // Ruh roh
		Serial.println("Controller disconnected!");
		delay(1000);
	}
	else {
		// Read a button (on/off, C and Z)
		boolean zButton = nchuk.buttonZ();

		Serial.print("The Z button is ");
		if (zButton == true) {
			Serial.println("pressed");
		}
		else if (zButton == false) {
			Serial.println("released");
		}

		// Read a joystick axis (0-255, X and Y)
		int joyY = nchuk.joyY();

		Serial.print("The joystick's Y axis is at ");
		Serial.println(joyY);

		// Read an accelerometer and print values (0-1023, X, Y, and Z)
		int accelX = nchuk.accelX();

		Serial.print("The accelerometer's X-axis is at ");
		Serial.println(accelX);

		// Print all the values!
		nchuk.printDebug();
	}
}
