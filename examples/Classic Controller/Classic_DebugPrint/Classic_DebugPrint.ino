#include <NintendoExtensionCtrl.h>

ClassicController classic;

void setup() {
	Serial.begin(115200);
	classic.begin();
}

void loop() {
	boolean success = classic.update();  // Get new data from the controller

	if (success == true) {  // We've got data!
		classic.printDebug();  // Print all of the values!
	}
	else {  // Data is bad :(
		Serial.println("Controller Disconnected!");
		classic.reconnect();
	}
}
