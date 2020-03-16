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
*  Example:      uDraw_MouseDemo
*  Description:  Connect an Arduino Micro or Leonardo to a uDraw Tablet
*                and try it out as a USB HID Mouse!
*/

#include <Mouse.h>
#include <NintendoExtensionCtrl.h>

uDrawTablet tablet;

int previousX;
int previousY;
bool previousLeftClick;
bool previousRightClick;
bool previousDetected;

void setup() {
	Serial.begin(115200);
	tablet.begin();
	Mouse.begin();

	//while (!Serial) {
	//    ; // wait for serial port to connect. Needed for native USB
	//}
    
	while (!tablet.connect()) {
		Serial.println("uDraw Tablet not detected!");
		delay(1000);
	}
}

void loop() {
	boolean success = tablet.update();  // Get new data from the tablet

	if (!success) {  // Ruh roh
		Serial.println("Tablet disconnected!");
		delay(1000);
		tablet.reconnect();
	}
	else {
		// Read the pen buttons for the mouse clicks
		boolean leftClick = tablet.button1() || tablet.buttonTip();
		boolean rightClick = tablet.button2();
		int x = tablet.penX();
		int y = tablet.penY();

		int deltaX;
		int deltaY;

		if(leftClick != previousLeftClick){
			if(leftClick == true){
				Mouse.press(MOUSE_LEFT);
			} else {
				Mouse.release(MOUSE_LEFT);
			}
		}

		if(rightClick != previousRightClick){
			if(rightClick == true){
				Mouse.press(MOUSE_RIGHT);
			} else {
				Mouse.release(MOUSE_RIGHT);
			}
		}

		previousLeftClick = leftClick;
		previousRightClick = rightClick;
		
		bool detected = tablet.penDetected();
		if(detected != previousDetected){
			if(detected){
				previousX = x;
				previousY = y;
			}
		}
		if(detected){
			deltaX = x - previousX;
			if(deltaX < -127){
				deltaX = -127;
			} else if (deltaX > 127){
				deltaX = 127;
			}
			
			deltaY = previousY - y;
			if(deltaY < -127){
				deltaY = -127;
			} else if (deltaY > 127){
				deltaY = 127;
			}
			
			Mouse.move(deltaX, deltaY);
		}
		previousX = x;
		previousY = y;
		previousDetected = detected;

		// Print all the values!
		tablet.printDebug();
	}
}
