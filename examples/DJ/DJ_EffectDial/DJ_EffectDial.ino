#include <NintendoExtensionCtrl.h>

DJTurntableController dj;
DJTurntableController::EffectRollover fx(dj);  // Pass the object to the effect tracker

int totalFx = 0;

void setup() {
	Serial.begin(115200);
	dj.begin();

	while (!dj.connect()) {
		Serial.println("DJ controller not detected!");
		delay(1000);
	}
	fx.getChange();  // Setup with initial values

	Serial.println("Starting! Move the effects dial to see the change");
}

void loop() {
	boolean success = dj.update();  // Get new data from the controller

	if (success == true) {  // We've got data!
		int8_t change = fx.getChange();  // Record change in the FX dial since last call
		totalFx += change;  // Add to total

		if (change != 0) {  // Only print if there's a change
			Serial.print("FX: ");

			// Print the raw value first
			Serial.print("Raw");
			printPadding(dj.effectDial());
			Serial.print(dj.effectDial());

			// Print the change
			Serial.print(" | ");
			Serial.print("+/-");
			printPadding(change);
			Serial.print(change);

			// Print the total since the program started
			Serial.print(" | ");
			Serial.print("Total ");
			printPadding(totalFx);
			Serial.print(totalFx);

			Serial.println();
		}
		delay(10);  // Rate limit
	}
	else {  // Data is bad :(
		Serial.println("Controller Disconnected!");
		dj.reconnect();
	}
}

// Little helper function to print padding characters, just to make things look good
void printPadding(int16_t printVal) {
	if (abs(printVal) < 10) {
		Serial.print(" ");  // One space, to take up for tens digit
	}
	if (printVal >= 0) {
		Serial.print(" ");  // One space, to take up for '-' sign
	}
}
