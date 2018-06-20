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
*  Example:      Drums_Demo
*  Description:  Connect to a Guitar Hero drum set and demonstrate all of
*                the avaiable control data functions.
*/

#include <NintendoExtensionCtrl.h>

DrumController drums;

void setup() {
	Serial.begin(115200);
	drums.begin();

	while (!drums.connect()) {
		Serial.println("Drum set controller not detected!");
		delay(1000);
	}
}

void loop() {
	Serial.println("----- Drum Controller Demo -----");  // Making things easier to read
	
	boolean success = drums.update();  // Get new data from the controller

	if (!success) {  // Ruh roh
		Serial.println("Controller disconnected!");
		delay(1000);
	}
	else {
		// Read a drum hit (Red, Blue, and Green)
		boolean red = drums.drumRed();

		Serial.print("The red drum has ");
		if (red == true) {
			Serial.println("been hit");
		}
		else if (red == false) {
			Serial.println("not been hit");
		}

		// Read a cymbal hit (Yellow and Orange)
		boolean yellow = drums.cymbalYellow();

		Serial.print("The yellow cymbol has ");
		if (yellow == true) {
			Serial.println("been hit");
		}
		else if (yellow == false) {
			Serial.println("not been hit");
		}

		// Read the bass pedal
		boolean pedal = drums.bassPedal();

		Serial.print("The bass pedal is ");
		if (pedal == true) {
			Serial.println("pressed");
		}
		else if (pedal == false) {
			Serial.println("not pressed");
		}

		// Read velocity data smartly. Only one drum's data is ever sent at a time,
		// so you need to check which drum the data is for
		Serial.print("Velocity data is ");
		if (drums.velocityAvailable()) {
			Serial.print("available! ");

			DrumController::VelocityID id = drums.velocityID();
			switch (id) {
				case(DrumController::None):
					Serial.print("No Drum"); // Should never happen, hopefully
					break;
				case(DrumController::Red):
					Serial.print("Red");
					break;
				case(DrumController::Blue):
					Serial.print("Blue");
					break;
				case(DrumController::Green):
					Serial.print("Green");
					break;
				case(DrumController::Yellow):
					Serial.print("Yellow");
					break;
				case(DrumController::Orange):
					Serial.print("Orange");
					break;
				case(DrumController::Pedal):
					Serial.print("Bass pedal");
					break;
			}
			Serial.print(" was hit with velocity ");

			uint8_t velocity = drums.velocity();  // 0-7, higher is harder
			Serial.println(velocity);
		}
		else {
			Serial.println("not available");
		}
		
		// Read velocity data directly (Red, Blue, Green, Yellow, Orange, Pedal)
		// Note: this data is NOT sent for every drum simultaneously, and is not
		//       sent every frame. Function will return '0' for no data / no hit
		uint8_t blueVelocity = drums.velocityBlue();

		// Read the joystick axis (0-63 XY)
		int joyVal = drums.joyX();

		Serial.print("The joystick's X axis is at ");
		Serial.println(joyVal);

		// Read a button (Plus/Minus)
		boolean plusButton = drums.buttonPlus();

		Serial.print("The plus button is ");
		if (plusButton == true) {
			Serial.println("pressed");
		}
		else if (plusButton == false) {
			Serial.println("released");
		}

		// Print all the values!
		drums.printDebug();
	}
}
