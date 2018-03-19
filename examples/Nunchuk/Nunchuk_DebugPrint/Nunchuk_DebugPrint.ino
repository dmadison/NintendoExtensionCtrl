#include <NintendoExtensionCtrl.h>

Nunchuk nchuk;

void setup() {
	Serial.begin(115200);
	nchuk.begin();
}

void loop() {
	nchuk.update();  // Get new data from the controller
	nchuk.printDebug();  // Print all of the values!
}
