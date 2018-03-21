#include <NintendoExtensionCtrl.h>

ClassicController classic;

void setup() {
	Serial.begin(115200);
	classic.begin();
}

void loop() {
	classic.update();  // Get new data from the controller
	classic.printDebug();  // Print all of the values!
}
