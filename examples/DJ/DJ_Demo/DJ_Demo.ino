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
*  Example:      DJ_Demo
*  Description:  Connect to a DJ Hero turntable and demonstrate all of
*                the avaiable control data functions.
*/

#include <NintendoExtensionCtrl.h>

DJTurntableController dj;

void setup() {
	Serial.begin(115200);
	dj.begin();

	while (!dj.connect()) {
		Serial.println("DJ controller not detected!");
		delay(1000);
	}
}

void loop() {
	Serial.println("----- DJ Controller Demo -----");  // Making things easier to read
	
	boolean success = dj.update();  // Get new data from the controller

	if (!success) {  // Ruh roh
		Serial.println("Controller disconnected!");
		delay(1000);
	}
	else {
		// Read the turntable, basic (-30-29. Clockwise = positive, faster = larger). ~900 ticks per revolution.
		int turntable = dj.turntable();

		Serial.print("The turntable is ");
		if (turntable == 0) {
			Serial.println("not moving");
		}
		else {
			Serial.print("moving ");
			if (turntable > 0) {
				Serial.print("clockwise");
			}
			else if (turntable < 0) {
				Serial.print("counter-clockwise");
			}
			Serial.print(" at ");
			Serial.println(turntable);
		}

		// Read the turntable buttons (Green, Red, Blue)
		boolean green = dj.buttonGreen();

		Serial.print("The green button is ");
		if (green == true) {
			Serial.println("pressed");
		}
		else if (green == false) {
			Serial.println("not pressed");
		}

		// Read the turntable config
		DJTurntableController::TurntableConfig config = dj.getTurntableConfig();
		
		Serial.print("Connected turntables: ");

		switch (config) {
			case(DJTurntableController::TurntableConfig::BaseOnly):
				Serial.println("None");
				break;
			case(DJTurntableController::TurntableConfig::Left):
				Serial.println("Left");
				break;
			case(DJTurntableController::TurntableConfig::Right):
				Serial.println("Right");
				break;
			case(DJTurntableController::TurntableConfig::Both):
				Serial.println("Both");
				break;
		}

		// Read a specific turntable (left/right)
		int leftTurntable = dj.left.turntable();
		boolean rightGreen = dj.right.buttonGreen();
		
		// Read the effect dial (0-31. One rotation per rollover)
		int fx = dj.effectDial();

		Serial.print("The effect dial is at ");
		Serial.println(fx);

		// Read the crossfade slider (0-15, left to right)
		int cross = dj.crossfadeSlider();

		Serial.print("The crossfade slider is ");

		if (cross <= 6) {
			Serial.println("left");
		}
		else if (cross >= 9) {
			Serial.println("right");
		}
		else {
			Serial.println("centered");
		}

		// Read the joystick axis (0-63 XY)
		int joyVal = dj.joyX();

		Serial.print("The joystick's X axis is at ");
		Serial.println(joyVal);

		// Read a button (Euphoria, Plus, Minus)
		boolean euphoria = dj.buttonEuphoria();

		Serial.print("The euphoria button is ");
		if (euphoria == true) {
			Serial.println("pressed");
		}
		else if (euphoria == false) {
			Serial.println("released");
		}

		// Print all the values!
		dj.printDebug();
	}
}
