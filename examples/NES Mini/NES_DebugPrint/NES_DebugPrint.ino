#include <NintendoExtensionCtrl.h>

NESMiniController nes;

void setup() {
	Serial.begin(115200);
	nes.begin();

	while (!nes.connect()) {
		Serial.println("Classic Controller not detected!");
		delay(1000);
	}
}

void loop() {
	boolean success = nes.update();  // Get new data from the controller

	if (success == true) {  // We've got data!
		nes.printDebug();  // Print all of the values!
	}
	else {  // Data is bad :(
		Serial.println("Controller Disconnected!");
		nes.reconnect();
	}
}
