#include <NintendoExtensionCtrl.h>

Nunchuk nchuk;

void setup() {
	Serial.begin(115200);
	nchuk.begin();

	while (!nchuk.connect()) {
		Serial.println("Nunchuk not detected!");
		delay(1000);
	}
}

void loop() {
	Serial.println("----- Nunchuk Demo -----"); // Making things easier to read
	
	boolean success = nchuk.update();  // Get new data from the controller

	if (!success) {  // Ruh roh
		Serial.println("Controller disconnected!");
		delay(1000);
	}
	else {
		// Read a button (on/off, C and Z)
		boolean zButton = nchuk.buttonZ();

		Serial.print("The Z button is ");
		if (zButton == true) {
			Serial.println("pressed");
		}
		else if (zButton == false) {
			Serial.println("released");
		}

		// Read a joystick axis (0-255, X and Y)
		int joyY = nchuk.joyY();

		Serial.print("The joystick's Y axis is at ");
		Serial.println(joyY);

		// Read an accelerometer and print values (0-1023, X, Y, and Z)
		int accelX = nchuk.accelX();

		Serial.print("The accelerometer's X-axis is at ");
		Serial.println(accelX);

		// Print all the values!
		nchuk.printDebug();
	}
}
