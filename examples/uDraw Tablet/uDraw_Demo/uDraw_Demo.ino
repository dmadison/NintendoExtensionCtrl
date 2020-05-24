/*
*  Project     Nintendo Extension Controller Library
*  @author     nullstalgia
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
*  Example:      uDraw_Demo
*  Description:  Connect to a uDraw Tablet and demonstrate all of
*                the avaiable control data functions.
*/

#include <NintendoExtensionCtrl.h>

uDrawTablet tablet;

void setup() {
	Serial.begin(115200);
	tablet.begin();

	while (!tablet.connect()) {
		Serial.println("uDraw Tablet not detected!");
		delay(1000);
	}
}

void loop() {
	Serial.println("----- uDraw Tablet Demo -----");  // Making things easier to read
	
	boolean success = tablet.update();  // Get new data from the tablet

	if (!success) {  // Ruh roh
		Serial.println("Controller disconnected!");
		delay(1000);
		tablet.connect();
	}
	else {
		// Is the pen near the drawing surface?
		boolean detected = tablet.penDetected();

		if (detected == true) {
			Serial.print("The pen is detected! It's currently at ");

			// Read the X/Y coordinates (0-4095)
			int xCoordinate = tablet.penX();
			Serial.print("X: ");
			Serial.print(xCoordinate);

			int yCoordinate = tablet.penY();
			Serial.print(" Y: ");
			Serial.print(yCoordinate);

			Serial.println();
		}
		else if (detected == false) {
			Serial.println("The pen is too far away!");
		}

		// Read the Pressure (0-511)
		int pressure = tablet.penPressure();

		Serial.print("The current pressure is ");
		Serial.println(pressure);

		// Read a button (Lower, Upper)
		boolean buttonLower = tablet.buttonLower();

		Serial.print("The lower button is ");
		if (buttonLower == true) {
			Serial.println("pressed");
		}
		else if (buttonLower == false) {
			Serial.println("released");
		}

		// Print all the values!
		tablet.printDebug();
		delay(100);
	}
}
