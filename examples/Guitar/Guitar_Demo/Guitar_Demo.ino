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
*  Example:      Guitar_Demo
*  Description:  Connect to a Guitar Hero controller and demonstrate all
*                of the avaiable control data functions.
*/

#include <NintendoExtensionCtrl.h>

GuitarController guitar;

void setup() {
	Serial.begin(115200);
	guitar.begin();

	while (!guitar.connect()) {
		Serial.println("Guitar controller not detected!");
		delay(1000);
	}
}

void loop() {
	Serial.println("----- Guitar Controller Demo -----");  // Making things easier to read
	
	boolean success = guitar.update();  // Get new data from the controller

	if (!success) {  // Ruh roh
		Serial.println("Controller disconnected!");
		delay(1000);
	}
	else {
		// Read the strum bar (up/down, or neither)
		Serial.print("The strum bar is ");

		if (guitar.strumUp()) {
			Serial.println("pressed up");
		}
		else if (guitar.strumDown()) {
			Serial.println("pressed down");
		}
		else {
			Serial.println("not pressed");
		}

		// Read the fret buttons (Green, Red, Yellow, Blue, Orange)
		boolean green = guitar.fretGreen();

		Serial.print("The green fret button is ");
		if (green == true) {
			Serial.println("pressed");
		}
		else if (green == false) {
			Serial.println("released");
		}

		// Read the touchbar (if the controller has one)
		if (guitar.supportsTouchbar()) {

			// Touchbar raw value (0-31)
			uint8_t touchbarValue = guitar.touchbar();

			Serial.print("The touchbar is at ");
			Serial.println(touchbarValue);

			// Touchbar zones (Green, Red, Yellow, Blue, Orange)
			boolean touchGreen = guitar.touchGreen();

			Serial.print("The green touch zone is ");
			if (touchGreen == true) {
				Serial.println("pressed");
			}
			else if (touchGreen == false) {
				Serial.println("not pressed");
			}
		}

		// Read the whammy bar (0-31, starting around 15-16)
		uint8_t whammy = guitar.whammyBar();

		Serial.print("The whammy bar's position is ");
		Serial.println(whammy);

		// Read the joystick axis (0-63 XY)
		int joyVal = guitar.joyX();

		Serial.print("The joystick's X axis is at ");
		Serial.println(joyVal);

		// Read a button (Plus/Minus)
		boolean plusButton = guitar.buttonPlus();

		Serial.print("The plus button is ");
		if (plusButton == true) {
			Serial.println("pressed");
		}
		else if (plusButton == false) {
			Serial.println("released");
		}

		// Print all the values!
		guitar.printDebug();
	}
}
