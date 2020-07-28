/*
*  Project     Nintendo Extension Controller Library
*  @author     Nullstalgia
*  @link       github.com/dmadison/NintendoExtensionCtrl
*  @license    LGPLv3 - Copyright (c) 2020 Nullstalgia
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
*  Example:      Tatacon_Demo
*  Description:  Connect to a Tatacon and demonstrate all of the avaiable
*                control data functions.
*/

#include <NintendoExtensionCtrl.h>

Tatacon drum;

unsigned long lastLeftDon;
unsigned long lastRightDon;

unsigned long lastLeftKa;
unsigned long lastRightKa;

#define LARGE_NOTE_TIMING 100

bool centerLeftPrevious = false;
bool centerRightPrevious = false;

bool rimLeftPrevious = false;
bool rimRightPrevious = false;

void setup() {
	Serial.begin(115200);
	drum.begin();

	while (!drum.connect()) {
		Serial.println("Tatacon not detected!");
		delay(1000);
	}
}

void loop() {
	//Serial.println("----- Tatacon Demo -----"); // Making things easier to read
	boolean success = drum.update();  // Get new data from the controller

	if (!success) {  // Ruh roh
		Serial.println("Controller disconnected!");
		delay(1000);
	}
	else {
		unsigned long currentMillis = millis();
		
		bool centerLeftActive = drum.centerLeft();
		bool centerRightActive = drum.centerRight();
		
		bool rimLeftActive = drum.rimLeft();
		bool rimRightActive = drum.rimRight();
		
		if (centerLeftPrevious != centerLeftActive && centerLeftActive == true) {
			lastLeftDon = currentMillis;
			if (currentMillis - lastRightDon <= LARGE_NOTE_TIMING) {
				Serial.println("DON!");
			} else {
				Serial.println("Don");
			}
		}
		
		if (centerRightPrevious != centerRightActive && centerRightActive == true) {
			lastRightDon = currentMillis;
			if (currentMillis - lastLeftDon <= LARGE_NOTE_TIMING) {
				Serial.println("DON!");
			} else {
				Serial.println("Don");
			}
		}
		
		if (rimLeftPrevious != rimLeftActive && rimLeftActive == true) {
			lastLeftKa = currentMillis;
			if (currentMillis - lastRightKa <= LARGE_NOTE_TIMING) {
				Serial.println("KA!");
			} else {
				Serial.println("Ka");
			}
		}
		
		if (rimRightPrevious != rimRightActive && rimRightActive == true) {
			lastRightKa = currentMillis;
			if (currentMillis - lastLeftKa <= LARGE_NOTE_TIMING) {
				Serial.println("KA!");
			} else {
				Serial.println("Ka");
			}
		}

		centerLeftPrevious = centerLeftActive;
		centerRightPrevious = centerRightActive;
		
		rimLeftPrevious = rimLeftActive;
		rimRightPrevious = rimRightActive;
		
		// Print all the values!
		// drum.printDebug();
	}
}
