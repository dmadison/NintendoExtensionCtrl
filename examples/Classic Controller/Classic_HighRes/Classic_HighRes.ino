/*
*  Project     Nintendo Extension Controller Library
*  @author     David Madison
*  @link       github.com/dmadison/NintendoExtensionCtrl
*  @license    LGPLv3 - Copyright (c) 2020 David Madison
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
*  Example:      Classic_HighRes
*  Description:  Connect to a Classic Controller and demonstrate the "high res"
*                mode by switching back and forth between the standard and
*                high resolution maps.
*/

#include <NintendoExtensionCtrl.h>

const unsigned long SwitchTime = 6000;  // ms, time to switch between low and high res modes (6 seconds)
unsigned long lastSwitch = 0;  // timestamp, when the last switch was made

ClassicController classic;

void setup() {
	Serial.begin(115200);
	classic.begin();

	while (!classic.connect()) {
		Serial.println("Classic Controller not detected!");
		delay(1000);
	}
	classic.setHighRes(true);  // let's start in high res mode
}

void loop() {
	boolean success = classic.update();  // Get new data from the controller

	if (success == true) {  // We've got data!
		classic.printDebug();  // Print all of the values!

		if (millis() - lastSwitch >= SwitchTime) {  // wait until it's time to switch
			lastSwitch = millis();  // save the timestamp
			boolean hr = classic.getHighRes();  // are we in high res or not?
			hr = !hr;  // flip it, let's go to the other one
			classic.setHighRes(hr);  // set the new mode
		}
	}
	else {  // Data is bad :(
		Serial.println("Controller Disconnected!");
		delay(1000);
		classic.connect();
	}
}
