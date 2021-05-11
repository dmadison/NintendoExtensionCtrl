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
*  Example:      IdentifyController
*  Description:  Attempt to connect to an extension controller, then print
*                out the matching ID and ID data bytes.
*/

#include <NintendoExtensionCtrl.h>

ExtensionPort controller;

void setup() {
	Serial.begin(115200);
	while (!Serial);  // wait for connection

	controller.begin();
	controller.connect();

	ExtensionType conType = controller.getControllerType();

	switch (conType) {
		case(ExtensionType::NoController):
			Serial.println("No controller detected");
			break;
		case(ExtensionType::UnknownController):
			Serial.println("Unknown controller connected");
			break;
		case(ExtensionType::Nunchuk):
			Serial.println("Nunchuk connected!");
			break;
		case(ExtensionType::ClassicController):
			Serial.println("Classic Controller connected!");
			break;
		case(ExtensionType::GuitarController):
			Serial.println("Guitar controller connected!");
			break;
		case(ExtensionType::DrumController):
			Serial.println("Drum set controller connected!");
			break;
		case(ExtensionType::DJTurntableController):
			Serial.println("DJ turntable connected!");
			break;
		case(ExtensionType::uDrawTablet):
			Serial.println("uDraw Tablet connected!");
			break;
		case(ExtensionType::DrawsomeTablet):
			Serial.println("Drawsome Tablet connected!");
			break;
		default: break;
	}
	controller.printDebugID();
}

void loop() {

}
