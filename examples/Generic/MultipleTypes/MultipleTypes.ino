#include <NintendoExtensionCtrl.h>

ExtensionData busData(Wire);  // Shared data for the controller on the 'Wire' bus

Nunchuk nchuk(busData);
ClassicController classic(busData);

// You can use any of the controllers for your begin / connect / update
// calls, so long as they share the same extension data
#define controller nchuk

void setup() {
	Serial.begin(115200);

	while (!controller.begin()) {
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
