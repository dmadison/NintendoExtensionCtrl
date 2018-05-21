#include <NintendoExtensionCtrl.h>

DrumController drums;

void setup() {
	Serial.begin(115200);
	drums.begin();

	while (!drums.connect()) {
		Serial.println("Drum controller not detected!");
		delay(1000);
	}
}

void loop() {
	boolean success = drums.update();  // Get new data from the controller

	if (success == true) {  // We've got data!
		drums.printDebug();  // Print all of the values!
	}
	else {  // Data is bad :(
		Serial.println("Controller Disconnected!");
		drums.reconnect();
	}
}
