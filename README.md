# Nintendo Extension Controller Library 
[![arduino-library-badge](https://www.ardu-badge.com/badge/Nintendo%20Extension%20Ctrl.svg?)](https://www.ardu-badge.com/Nintendo%20Extension%20Ctrl) [![Build Status](https://github.com/dmadison/NintendoExtensionCtrl/workflows/build/badge.svg?branch=master)](https://github.com/dmadison/NintendoExtensionCtrl/actions?query=workflow%3Abuild)

This is an Arduino library that makes it easy to read data from Nintendo extension controllers such as the Wii Nunchuk and Classic Controller.

"Extension controller" is the name given to devices that *extend* the functionality of the [Wiimote](https://en.wikipedia.org/wiki/Wii_Remote), plugging into the expansion port at the bottom of the device. While originally designed for Wiimote accessories, the port and communication protocol is now also being used for the controllers of Nintendo's new line of "Mini" consoles.

## Getting Started

![ClassicController Example in IDE](/extras/NXCtrl_ClassicExample.png)

### Make Your Connections

Before anything else, you need to connect your controller to the Arduino. The easist way to do this is with a breakout board, which slides in to the extension controller plug and doesn't require dismantaling your controller. I recommend the [Nunchucky](https://www.adafruit.com/product/345) breakout, which is inexpensive and available at Adafruit.

Controllers require 3.3V power. If you don't have 3.3V power available on your board, you will need to use an external regulator. Using 5V power can result in erratic data and risks damaging your controller. For this reason it's recommended *not* to use so-called "inline" adapters that attach to the Arduino Uno's analog pins.

The "data" and "clock" lines on the breakout go to the SDA and SCL pins (respectively) on the microcontroller. For the Arduino Uno, these are pins A4 and A5. If you're not using an Uno, look [here](https://www.arduino.cc/en/reference/wire) to find the I²C pins for your Arduino board.

### Run an Example

After [installing the library](https://www.arduino.cc/en/guide/libraries), load an example by going to `File -> Examples -> NintendoExtensionCtrl` in the Arduino IDE and selecting an example specific to your controller. I recommend the `DebugPrint` examples to start, as they give you a nice overview of what data is available for your controller.

Plug in your controller, upload the example to your board, and have fun!

## Supported Controllers

### Wii
* Nunchuk
* Classic Controller

### Wii Instruments
* Guitar Hero Guitar
* Guitar Hero World Tour Drums
* DJ Hero Turntable

### Wii Drawing Tablets
* uDraw Tablet
* Drawsome Tablet

### Mini Console
* NES Mini Controller
* SNES Mini Controller

Currently the library supports any extension controller using unencrypted communication. If you'd like to add support for another controller, I've written [a short guide](extras/AddingControllers.md) that should be helpful. 

## License
This library is licensed under the terms of the [GNU Lesser General Public License (LGPL)](https://www.gnu.org/licenses/lgpl.html), either version 3 of the License, or (at your option) any later version.
