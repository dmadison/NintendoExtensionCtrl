#include <NintendoExtensionCtrl.h>

ClassicController classic;

void setup() {
	Serial.begin(115200);
	classic.begin();

	while (!classic.connect()) {
		Serial.println("Classic Controller not detected!");
		delay(1000);
	}
}

void loop() {
	Serial.println("----- Classic Controller Demo -----"); // Making things easier to read
	
	boolean success = classic.update();  // Get new data from the controller

	if (!success) {  // Ruh roh
		Serial.println("Controller disconnected!");
		delay(1000);
	}
	else {
		// Read the DPAD (Up/Down/Left/Right)
		boolean padUp = classic.dpadUp();

		Serial.print("The DPAD up button is ");
		if (padUp == true) {
			Serial.println("pressed");
		}
		else if (padUp == false) {
			Serial.println("released");
		}

		// Read a button (on/off, ABXY, Minus, Home, Plus, L, R, ZL, ZR)
		boolean aButton = classic.buttonA();

		Serial.print("The A button is ");
		if (aButton == true) {
			Serial.println("pressed");
		}
		else if (aButton == false) {
			Serial.println("released");
		}

		// Read a joystick axis (0-63 Left XY, 0-31 Right XY)
		int joyLX = classic.leftJoyX();

		Serial.print("The left joystick's X axis is at ");
		Serial.println(joyLX);

		// Read a trigger (0-31, L/R)
		int triggerL = classic.triggerL();

		Serial.print("The left trigger is at ");
		Serial.println(triggerL);

		// Print all the values!
		classic.printDebug();
	}
}
