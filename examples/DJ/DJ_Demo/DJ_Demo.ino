#include <NintendoExtensionCtrl.h>

DJTurntableController dj;

void setup() {
	Serial.begin(115200);

	while (!dj.begin()) {
		Serial.println("DJ controller not detected!");
		delay(1000);
	}
}

void loop() {
	Serial.println("----- DJ Controller Demo -----");  // Making things easier to read
	
	boolean success = dj.update();  // Get new data from the controller

	if (!success) {  // Ruh roh
		Serial.println("Controller disconnected!");
		delay(1000);
	}
	else {
		// Read the turntable, basic (-30-29. Clockwise = positive, faster = larger)
		int turntable = dj.turntable();

		Serial.print("The turntable is ");
		if (turntable == 0) {
			Serial.println("not moving");
		}
		else {
			Serial.print("moving ");
			if (turntable > 0) {
				Serial.print("clockwise");
			}
			else if (turntable < 0) {
				Serial.print("counter-clockwise");
			}
			Serial.print(" at ");
			Serial.println(turntable);
		}

		// Read the turntable buttons (Green, Red, Blue)
		boolean green = dj.buttonGreen();

		Serial.print("The green button is ");
		if (green == true) {
			Serial.println("pressed");
		}
		else if (green == false) {
			Serial.println("not pressed");
		}

		// Read the turntable config
		NXC_DJTurntable_Configuration config = dj.getTurntableConfig();
		
		Serial.print("Connected turntables: ");

		switch (config) {
			case(NXC_DJTurntable_BaseOnly):
				Serial.println("None");
				break;
			case(NXC_DJTurntable_Left):
				Serial.println("Left");
				break;
			case(NXC_DJTurntable_Right):
				Serial.println("Right");
				break;
			case(NXC_DJTurntable_Both):
				Serial.println("Both");
				break;
		}

		// Read a specific turntable (left/right)
		int leftTurntable = dj.left.turntable();
		boolean rightGreen = dj.right.buttonGreen();
		
		// Read the effect dial (0-31. One rotation per rollover)
		int fx = dj.effectDial();

		Serial.print("The effect dial is at ");
		Serial.println(fx);

		// Read the crossfade slider (-8-7, negative to the left)
		int cross = dj.crossfadeSlider();

		Serial.print("The crossfade slider is ");

		if (cross <= -2) {
			Serial.println("left");
		}
		else if (cross >= 1) {
			Serial.println("right");
		}
		else {
			Serial.println("center");
		}

		// Read the joystick axis (0-63 XY)
		int joyVal = dj.joyX();

		Serial.print("The joystick's X axis is at ");
		Serial.println(joyVal);

		// Read a button (Euphoria, Plus, Minus)
		boolean euphoria = dj.buttonEuphoria();

		Serial.print("The euphoria button is ");
		if (euphoria == true) {
			Serial.println("pressed");
		}
		else if (euphoria == false) {
			Serial.println("released");
		}

		// Print all the values!
		dj.printDebug();
	}
}
