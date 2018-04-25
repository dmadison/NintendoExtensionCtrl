#include <NintendoExtensionCtrl.h>

SNESMiniController snes;

void setup() {
	Serial.begin(115200);

	while (!snes.begin()) {
		Serial.println("Classic Controller not detected!");
		delay(1000);
	}
}

void loop() {
	boolean success = snes.update();  // Get new data from the controller

	if (success == true) {  // We've got data!
		snes.printDebug();  // Print all of the values!
	}
	else {  // Data is bad :(
		Serial.println("Controller Disconnected!");
		snes.reconnect();
	}
}
