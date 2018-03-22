#include <NintendoExtensionCtrl.h>

ExtensionController controller;

void setup() {
	Serial.begin(115200);
	controller.begin();
}

void loop() {
	boolean success = controller.update();  // Get new data from the controller

	if (success == true) {  // We've got data!
		controller.printDebugRaw();  // Print all of the values!
	}
	else {  // Data is bad :(
		Serial.println("Controller Disconnected!");
		controller.reconnect();
	}
}