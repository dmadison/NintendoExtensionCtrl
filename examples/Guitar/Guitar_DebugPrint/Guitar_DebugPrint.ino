#include <NintendoExtensionCtrl.h>

GuitarController guitar;

void setup() {
	Serial.begin(115200);
	guitar.begin();

	while (!guitar.connect()) {
		Serial.println("Guitar controller not detected!");
		delay(1000);
	}
}

void loop() {
	boolean success = guitar.update();  // Get new data from the controller

	if (success == true) {  // We've got data!
		guitar.printDebug();  // Print all of the values!
	}
	else {  // Data is bad :(
		Serial.println("Controller Disconnected!");
		guitar.reconnect();
	}
}
