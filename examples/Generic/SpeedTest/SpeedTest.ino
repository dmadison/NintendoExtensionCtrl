#include <NintendoExtensionCtrl.h>

ExtensionController controller;  // Generic controller, 6 bytes

const long TestDuration = 1000;  // Length of each test, in milliseconds

void setup() {
	Serial.begin(115200);
	controller.begin();

	Serial.println("Starting Speed Test...");
}

void loop() {
	boolean validData = true;
	long numUpdates = 0;
	long millisStart = millis();

	while (millis() - millisStart <= TestDuration && validData) {
		validData = controller.update();
		numUpdates++;
	}
	
	if (!validData) {
		Serial.println("ERROR! Invalid data received!");
		Serial.println("Reinitializing...");
		delay(2000);

		controller.begin();
		return;
	}

	Serial.print("Success! Completed ");
	Serial.print(numUpdates);
	Serial.print(" updates in ");
	Serial.print(TestDuration);
	Serial.println(" milliseconds.");
}

