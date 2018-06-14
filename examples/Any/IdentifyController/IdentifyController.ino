#include <NintendoExtensionCtrl.h>

ExtensionController controller;

void setup() {
	Serial.begin(115200);
	controller.begin();
	controller.connect();

	ExtensionType conType = controller.getConnectedID();

	switch (conType) {
		case(ExtensionType::NoController):
			Serial.println("No controller detected");
			break;
		case(ExtensionType::UnknownController):
			Serial.println("Unknown controller detected");
			break;
		case(ExtensionType::Nunchuk):
			Serial.println("Nunchuk detected!");
			break;
		case(ExtensionType::ClassicController):
			Serial.println("Classic Controller detected!");
			break;
		case(ExtensionType::GuitarController):
			Serial.println("Guitar controller connected!");
			break;
		case(ExtensionType::DrumController):
			Serial.println("Drum set controller connected!");
			break;
		case(ExtensionType::DJTurntableController):
			Serial.println("DJ turntable connected!");
			break;
	}
	controller.printDebugID();
}

void loop() {

}
