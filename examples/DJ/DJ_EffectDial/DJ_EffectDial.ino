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
*  Example:      DJ_EffectDial
*  Description:  Connect to a DJ Hero turntable and show how the
*                EffectRollover class works to track the change in
*                the effect dial, even between rollovers.
*/

#include <NintendoExtensionCtrl.h>

DJTurntableController dj;
DJTurntableController::EffectRollover fx(dj);  // Pass the object to the effect tracker

int totalFx = 0;

void setup() {
	Serial.begin(115200);
	dj.begin();

	while (!dj.connect()) {
		Serial.println("DJ controller not detected!");
		delay(1000);
	}
	fx.getChange();  // Setup with initial values

	Serial.println("Starting! Move the effects dial to see the change");
}

void loop() {
	boolean success = dj.update();  // Get new data from the controller

	if (success == true) {  // We've got data!
		int8_t change = fx.getChange();  // Record change in the FX dial since last call
		totalFx += change;  // Add to total

		if (change != 0) {  // Only print if there's a change
			Serial.print("FX: ");

			// Print the raw value first
			Serial.print("Raw");
			printPadding(dj.effectDial());
			Serial.print(dj.effectDial());

			// Print the change
			Serial.print(" | ");
			Serial.print("+/-");
			printPadding(change);
			Serial.print(change);

			// Print the total since the program started
			Serial.print(" | ");
			Serial.print("Total ");
			printPadding(totalFx);
			Serial.print(totalFx);

			Serial.println();
		}
		delay(10);  // Rate limit
	}
	else {  // Data is bad :(
		Serial.println("Controller Disconnected!");
		delay(1000);
		dj.connect();
	}
}

// Little helper function to print padding characters, just to make things look good
void printPadding(int16_t printVal) {
	if (abs(printVal) < 10) {
		Serial.print(" ");  // One space, to take up for tens digit
	}
	if (printVal >= 0) {
		Serial.print(" ");  // One space, to take up for '-' sign
	}
}
