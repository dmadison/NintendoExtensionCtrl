#include <NintendoExtensionCtrl.h>

NESMiniController snes;

void setup() {
	Serial.begin(115200);

	while (!snes.begin()) {
		Serial.println("SNES Classic Controller not detected!");
		delay(1000);
	}
}

void loop() {
	Serial.println("----- SNES Mini Controller Demo -----");  // Making things easier to read
	
	boolean success = snes.update();  // Get new data from the controller

	if (!success) {  // Ruh roh
		Serial.println("Controller disconnected!");
		delay(1000);
	}
	else {
		// Read the DPAD (Up/Down/Left/Right)
		boolean padUp = snes.dpadUp();

		Serial.print("The DPAD up button is ");
		if (padUp == true) {
			Serial.println("pressed");
		}
		else if (padUp == false) {
			Serial.println("released");
		}

		// Read a button (A/B/X/Y, Start/Select, L/R)
		boolean aButton = snes.buttonA();

		Serial.print("The A button is ");
		if (aButton == true) {
			Serial.println("pressed");
		}
		else if (aButton == false) {
			Serial.println("released");
		}

		// Print all the values!
		snes.printDebug();
	}
}
