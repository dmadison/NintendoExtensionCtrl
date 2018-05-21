#include <NintendoExtensionCtrl.h>

NESMiniController nes;

void setup() {
	Serial.begin(115200);
	nes.begin();

	while (!nes.connect()) {
		Serial.println("NES Classic Controller not detected!");
		delay(1000);
	}
}

void loop() {
	Serial.println("----- NES Mini Controller Demo -----");  // Making things easier to read
	
	boolean success = nes.update();  // Get new data from the controller

	if (!success) {  // Ruh roh
		Serial.println("Controller disconnected!");
		delay(1000);
	}
	else {
		// Read the DPAD (Up/Down/Left/Right)
		boolean padUp = nes.dpadUp();

		Serial.print("The DPAD up button is ");
		if (padUp == true) {
			Serial.println("pressed");
		}
		else if (padUp == false) {
			Serial.println("released");
		}

		// Read a button (A/B/Start/Select)
		boolean aButton = nes.buttonA();

		Serial.print("The A button is ");
		if (aButton == true) {
			Serial.println("pressed");
		}
		else if (aButton == false) {
			Serial.println("released");
		}

		// Print all the values!
		nes.printDebug();
	}
}
