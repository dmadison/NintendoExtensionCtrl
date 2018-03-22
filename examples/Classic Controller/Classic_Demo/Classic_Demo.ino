#include <NintendoExtensionCtrl.h>

ClassicController classic;

void setup() {
	Serial.begin(115200);
	classic.begin();
}

void loop() {
	Serial.println("----- Classic Controller Demo -----"); // Making things easier to read
	
	classic.update();  // Get new data from the controller

	// Read the DPAD (Up/Down/Left/Right)
	boolean padUp = classic.dpadUp();

	Serial.print("The DPAD up button is ");
	if (padUp == 1) {
		Serial.println("pressed");
	}
	else if (padUp == 0) {
		Serial.println("released");
	}

	// Read a button (on/off, ABXY, Minus, Home, Plus, LT, RT, ZL, ZR)
	boolean aButton = classic.buttonA();
	
	Serial.print("The A button is ");
	if (aButton == 1) {
		Serial.println("pressed");
	}
	else if (aButton == 0) {
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
