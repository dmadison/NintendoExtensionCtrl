#include <NintendoExtensionCtrl.h>

ExtensionController controller;  // Port for communicating with extension controllers

Nunchuk::Data nchuk(controller);  // Read Nunchuk formatted data from the port
ClassicController::Data classic(controller);  // Read Classic Controller formatted data from the port

void setup() {
	Serial.begin(115200);
	controller.begin();

	while (!controller.connect()) {
		Serial.println("No controller found!");
		delay(1000);
	}
}

void loop() {
	boolean success = controller.update();  // Get new data from the controller

	if (success == true) {  // We've got data!
		ExtensionType conType = controller.getConnectedID();

		switch (conType) {
			case(ExtensionType::Nunchuk):
				nchuk.printDebug();
				break;
			case(ExtensionType::ClassicController):
				classic.printDebug();
				break;
			default:
				Serial.println("Other controller connected!");
				quit();
		}
	}
	else {  // Data is bad :(
		Serial.println("Controller Disconnected!");
		quit();
	}
}

void quit() {
	while (true) {}  // Infinite loop
}
