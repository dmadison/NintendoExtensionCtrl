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
*  Example:      uDraw_SerialDemo
*  Description:  Connect to uDraw Tablet and demonstrate all of
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
		tablet.reconnect();
	}
	else {
		// Read the Pressure (0-255)
		// Has 2 stages for double the pressure range;
		// Pushing on the tip will go from 0-255 until buttonTip() goes True
		// At that point, pressure goes back to 0, and will go back up to 255
		// at the hardest you can push it.
		uint16_t pressure = tablet.penPressure();

		uint16_t pressurePercent;
		
		if(tablet.buttonTip() == true){
			pressure += 255;
		}
		
		pressurePercent = map(pressure, 0, 255*2, 0, 100);
		
		Serial.print("The current pressure is ");
		Serial.print(pressurePercent);
		Serial.println("%.");

		// Read a button (Tip, 1, 2)
		boolean button1 = tablet.button1();

		Serial.print("The bottom button is ");
		if (button1 == true) {
			Serial.println("pressed");
		}
		else if (button1 == false) {
			Serial.println("released");
		}
		
		// Is the pen near the drawing surface?
		boolean detected = tablet.penDetected();
		
		Serial.print("The pen is ");
		if (detected == true) {
			Serial.println("detected!");
		}
		else if (detected == false) {
			Serial.println("too far away!");
		}

		// Print all the values!
		tablet.printDebug();
		delay(100);
	}
}