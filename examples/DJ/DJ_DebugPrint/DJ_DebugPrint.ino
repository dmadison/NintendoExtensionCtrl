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
	boolean success = dj.update();  // Get new data from the controller

	if (success == true) {  // We've got data!
		dj.printDebug();  // Print all of the values!
	}
	else {  // Data is bad :(
		Serial.println("Controller Disconnected!");
		dj.reconnect();
	}
}
