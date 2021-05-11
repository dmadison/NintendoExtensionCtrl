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
*  Example:      MultipleTypes
*  Description:  Connect to a controller and then switch between two controller
*                types depending on the identity of the connected controller.
*/

#include <NintendoExtensionCtrl.h>

ExtensionPort port;  // Port for communicating with extension controllers

ClassicController::Shared classic(port);
Nunchuk::Shared nchuk(port);

void setup() {
	Serial.begin(115200);
	port.begin();  // init I2C

	while (!port.connect()) {
		Serial.println("No controller found!");
		delay(1000);
	}
}

void loop() {
	boolean success = port.update();  // Get new data from the controller

	if (success == true) {  // We've got data!
		ExtensionType type = port.getControllerType();

		switch (type) {
			case(ExtensionType::Nunchuk):
				nchuk.printDebug();
				break;
			case(ExtensionType::ClassicController):
				classic.printDebug();
				break;
			default:
				break;
		}
	}
	else {  // Data is bad :(
		while (!port.connect()) {
			Serial.println("Controller Disconnected!");
			delay(1000);
		}
	}
}
