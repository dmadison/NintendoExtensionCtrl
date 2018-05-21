#include <NintendoExtensionCtrl.h>

ExtensionData busData(Wire);  // Shared data for the controller on the 'Wire' bus
ExtensionController controller(busData);  // Generic type: works with any ID, and requests the max # of bytes possible

Nunchuk nchuk(busData);
ClassicController classic(busData);

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
		NXC_ControllerType conType = controller.getConnectedID();

		switch (conType) {
			case(NXC_Nunchuk):
				nchuk.printDebug();
				break;
			case(NXC_ClassicController):
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
