#include <NintendoExtensionCtrl.h>

ExtensionController extensionPort;  // Port for communicating with extension controllers

Nunchuk::DataMap nchuk(extensionPort);  // Read Nunchuk formatted data from the port
ClassicController::DataMap classic(extensionPort);  // Read Classic Controller formatted data from the port

void setup() {
	Serial.begin(115200);
	extensionPort.begin();

	while (!extensionPort.connect()) {
		Serial.println("No controller found!");
		delay(1000);
	}
}

void loop() {
	boolean success = extensionPort.update();  // Get new data from the controller

	if (success == true) {  // We've got data!
		ExtensionType conType = extensionPort.getConnectedID();

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
