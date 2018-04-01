#include <NintendoExtensionCtrl.h>

ExtensionController controller;

void setup() {
	Serial.begin(115200);
	controller.begin();

	NXC_ControllerType conType = controller.getConnectedID();

	switch (conType) {
		case(NXC_NoController):
			Serial.println("No controller detected");
			break;
		case(NXC_UnknownController):
			Serial.println("Unknown controller detected");
			break;
		case(NXC_Nunchuk):
			Serial.println("Nunchuk detected!");
			break;
		case(NXC_ClassicController):
			Serial.println("Classic Controller detected!");
			break;
		case(NXC_GuitarController):
			Serial.println("Guitar controller connected!");
			break;
		case(NXC_DrumController):
			Serial.println("Drum set controller connected!");
			break;
	}
}

void loop() {

}
