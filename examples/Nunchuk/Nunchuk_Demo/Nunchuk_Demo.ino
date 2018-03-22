#include <NintendoExtensionCtrl.h>

Nunchuk nchuk;

void setup() {
	Serial.begin(115200);
	nchuk.begin();
}

void loop() {
	Serial.println("----- Nunchuk Demo -----"); // Making things easier to read
	
	nchuk.update();  // Get new data from the controller

	// Read a button (on/off, C and Z)
	boolean zButton = nchuk.buttonZ();
	
	Serial.print("The Z button is ");
	if (zButton == 1) {
		Serial.println("pressed");
	}
	else if (zButton == 0) {
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
