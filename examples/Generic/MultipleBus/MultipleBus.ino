#include <NintendoExtensionCtrl.h>

// Requries a microcontroller with two I2C buses, e.g. Arduino Due or Teensy
Nunchuk nchuk1(Wire);   // Controller on bus #1
Nunchuk nchuk2(Wire1);  // Controller on bus #2

void setup() {
	Serial.begin(115200);

	while (!Serial);  // Wait for serial for debug
	Serial.println("Attempting connection to controllers...");

	while (!nchuk1.begin()) {
		Serial.println("Nunchuk on bus #1 not detected!");
		delay(1000);
	}

	while (!nchuk2.begin()) {
		Serial.println("Nunchuk on bus #2 not detected!");
		delay(1000);
	}
}

void loop() {
	Serial.println("-------------");

	if (nchuk1.update()) {
		nchuk1.printDebug();
	}
	else {
		Serial.println("Bus #1 Disconnected");
	}

	if (nchuk2.update()) {
		nchuk2.printDebug();
	}
	else {
		Serial.println("Bus #2 Disconnected");
	}
}
