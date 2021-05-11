# Adding a Controller to the NintendoExtensionCtrl Library
If you want to use an extension controller that is not currently supported by this library, you'll have to add it yourself. But fear not! This guide should walk you through how to quickly and easily add a new controller.

## Step #1: Creating The Class Framework
The first step in adding support for your controller is building it a class. The header (.h) and implementation (.cpp) files live within the `controllers` folder in the source directory. You'll need to create both of these files with the name of your controller.

Controller classes live inside the library namespace and inherit from the `ExtensionController` class, which contains methods for communicating with the controllers and manipulating the control surface data. This includes combining multi-byte data and extracting bits that correspond to button presses. To use this you'll need to include the "ExtensionController.h" header, which is in the `internal` source directory.

The class name for your controller is going to be the "Base" version of the class, which uses a reference to port and control data that exists elsewhere - thus it has a `Base` suffix. Here is what the start of the `ClassicControllerBase` class looks like:

```C++
#include "internal/ExtensionController.h"

namespace NintendoExtensionCtrl {
	class ClassicControllerBase : public ExtensionController {
	public:
		using ExtensionController::ExtensionController;
```

Note the `using` directive. This just means we're reusing the constructor from the base `ExtensionController` class. Most controller variants don't have their own data, and won't need to define their own constructors.

## Step #2: Initializing the Controller

Some controllers require a little extra handholding when they're first initialized. For example, the `DrawsomeTablet` requires two extra register writes before it will return data. The library has a dedicated virtual function for this called `specificInit`:

```C++
boolean DrawsomeTabletBase::specificInit() {
	return writeRegister(0xFB, 0x01) && writeRegister(0xF0, 0x55);
}
```

This function is called at the end of each `connect()` attempt, and returns 'true' if the controller is successfully initialized, and 'false' if it is not. For most controllers this function is not necessary and can be omitted entirely.

This is also where you would increase the data request size if needed. By default all controllers use the Wiimote 0x37 data reporting mode, which returns 6 bytes of control data starting at register 0x00. The library supports a request size of up to 21 bytes, which can be set using the `setRequestSize` function.

## Step #3: Building Your Data Maps
The next step is to add the data maps for your controller. These define where the data for each control input lies within in the data array.

Each map represents the size and position for all of the data of a control surface (button, joystick, etc.). The library has three data types for this, each with a different purpose:

### IndexMap

The simplest data format. This takes one value that represents the index in the array for your control data. Passing this to `getControlData` will return the full byte of data.

```C++
IndexMap JoyX = 1;  // Array[1] is the data for JoyX
```

### ByteMap
A `ByteMap` is a struct that takes four input values:

1. The index of the data in the control data array
2. The size of the data, in bits
3. The starting position of the data, in bits from the right
4. The offset, or amount of bits to shift the final data *to* the right

From the `size` and `position` values, the constructor will generate a bitmask to apply to the control data, saving cycles at runtime. Note that the 'offset' value cannot be negative. If you need to shift the data left you must do it yourself.

```C++
// Data is at Array[1]
// 5 bits wide, starting 3 bits from the right (0xF8 mask)
// Shift the final data 2 bits to the right
ByteMap JoyY = ByteMap(1, 5, 3, 2);
```

If the data is spread out over multiple bytes, you can use an array of `ByteMap` structs:

```C++
ByteMap TriggerL[2] = { ByteMap(2, 2, 5, 2), ByteMap(3, 3, 5, 5) };
```

### BitMap
A `BitMap` is a simpler struct that takes two input values:

1. The index of the data in the control data array
2. The position of the bit, in bits from the right

The resulting bit from the control data is extracted and inverted, as extension controller buttons are `0` if pressed.

```C++
BitMap  ButtonA = { 5, 4 };
```
Full definitions of these data types can be found in the [`NXC_DataMaps.h`](../src/internal/NXC_DataMaps.h) file. Methods for using them are defined in the `ExtensionController` class definition ([`ExtensionController.h`](../src/internal/ExtensionController.h)).

---

For each control input on your controller, you will need to reverse-engineer the data positions and format. You can use the `printDebugRaw` function of the `ExtensionController` class to help with this. I'd also recommend checking out the information available at [WiiBrew](http://wiibrew.org/wiki/Wiimote/Extension_Controllers), which was tremendously helpful to me in putting the rest of the library together.

Once you have your mappings you'll need to add them to the controller class. Within the class definition, create a `Maps` struct. This will be filled with the `constexpr static` data for each control surface mapping. It's important to use the `constexpr static` keywords so that these are evaluated as expressions and no memory is allocated.

Here is a sampling of some of the mappings for the Classic Controller:

```C++
struct Maps {
  constexpr static ByteMap LeftJoyX = ByteMap(0, 6, 0, 0);
  constexpr static ByteMap LeftJoyY = ByteMap(1, 6, 0, 0);

  constexpr static ByteMap RightJoyX[3] = { ByteMap(0, 2, 6, 3), ByteMap(1, 2, 6, 5), ByteMap(2, 1, 7, 7) };
  constexpr static ByteMap RightJoyY = ByteMap(2, 5, 0, 0);

  constexpr static BitMap  ButtonA = { 5, 4 };
  constexpr static BitMap  ButtonB = { 5, 6 };
};
```

## Step #4: Add Your "Get" Functions
With your control maps in place, you'll now need to add your 'get' functions. These functions are public, and will return their respective control data to the user.

Since the library is based around "getting" and working with this control data, I elected early-on to ditch the "get" prefix for simplicity. All functions are just the name of the control input itself. Here are the Classic Controller function declarations for the above maps:

```C++
uint8_t leftJoyX() const;  // 6 bits, 0-63
uint8_t leftJoyY() const;

uint8_t rightJoyX() const;  // 5 bits, 0-31
uint8_t rightJoyY() const;

boolean buttonA() const;
boolean buttonB() const;
```

Since you've already spent the time creating the data maps, the function definitions should be straight-forward. Either call `getControlBit()` passing a `BitMap`, or call `getControlData()` passing your `IndexMap` or `ByteMap` value(s). Here are the function definitions for the above controls:

```C++
uint8_t ClassicControllerBase::leftJoyX() const {
	return getControlData(Maps::LeftJoyX);
}

uint8_t ClassicControllerBase::leftJoyY() const {
	return getControlData(Maps::LeftJoyY);
}

uint8_t ClassicControllerBase::rightJoyX() const {
	return getControlData(Maps::RightJoyX);
}

uint8_t ClassicControllerBase::rightJoyY() const {
	return getControlData(Maps::RightJoyY);
}

boolean ClassicControllerBase::buttonA() const {
	return getControlBit(Maps::ButtonA);
}

boolean ClassicControllerBase::buttonB() const {
	return getControlBit(Maps::ButtonB);
}
```

***Note:** I decided to use two different function names for generic control data and bits because the bits are automatically inverted. I might decide to change this in the future, but for now it seems to work fine.*

## Step #5: Add a `printDebug` Function
This should be a fun step. Create a `printDebug` function that prints out the values for your controller! Since this should only ever be called when debugging, I say go crazy with the formatting. The other controllers use `sprintf`/`snprintf` to make things easy, in spite of the extra overhead.

Here's how the Classic Controller debug line looks:
```
<^v> | +H- | ABXY L:(32, 32) R:(16, 16) | LT:31X RT:31X Z:LR
```
This includes all of the possible control data: the directional pad, +/- and home buttons, ABXY buttons, left and right joysticks, left and right triggers, and ZL/ZR buttons. You can check [the code](../src/controllers/ClassicController.cpp) to see how it was put together.

## Step #6: Add Your Controller's Identity
Now that your controller definition is nearly done, it's time to add its identity to the list of available controllers!

Open up the [`NXC_Identity.h`](../src/internal/NXC_Identity.h) file and add your controller name to the `ExtensionType` enumeration. Then, modify the `decodeIdentity` function so that it will return your controller's ID if the identity bytes match. You can run the [`IdentifyController`](../examples/Any/IdentifyController/IdentifyController.ino) example to fetch the string of ID bytes.

Once that's done, head back to your controller's header file. You'll need to create a new function, `getExpectedType`, which returns the identity value you just created. This will limit connections to this specific type and report problems if the type doesn't match.

```C++
ExtensionType ClassicControllerBase::getExpectedType() const {
	return ExtensionType::ClassicController;
}
```

You will also need to edit the switch statement in the `IdentifyControllers` example to add your controller to the 'switch' statement.

## Step #7: Create the Combined Class
The last step to get your controller working is to create a combined class that inherits from your `Base` class and bundles it with a set of extension data to use. This creates an easy to use class for most users who are looking to get data from just one controller. Just copy this line, replacing all instances of `YourController` with your controller's name:

```C++
using YourController = NintendoExtensionCtrl::BuildControllerClass
	<NintendoExtensionCtrl::YourControllerBase>;
```

Be sure to place this *outside* of the namespace, in the header file. See other controller definitions for reference.

## Step #8: Add Examples
What use is a good controller definition if no one knows how to use it? You should add some examples showing off how the controller works. I usually like to add two:

* `DebugPrint`, which is barebones and only includes connection and the `printDebug` function. This is useful for testing that your controller and all of your data maps are functioning properly.
* `Demo`, which is longer and includes references to all of the different 'get' function types. This should explain to the user how to access your controller's data, including ranges and any quirks.

## Step #9: Library Housekeeping
At this point your controller is up and running, and everyone should know how to use it! There's just a little bit of housekeeping left to do to make the controller fit in nicely with the others in the library.

### Update the Keyword Map
The library includes a file called [`keywords.txt`](../keywords.txt), which includes the names of classes, functions, datatypes, and other useful keywords worth highlighting in the Arduino IDE. Please update this with the public version of your class name and any unique functions. See the [Arduino Library Specification](https://github.com/arduino/arduino/wiki/arduino-ide-1.5:-library-specification) for more information.

### Update Supported Controllers
Add your controller to the list of supported controllers! This must be done in **two** places:
* [README.md](../README.md)
* [library.properties](../library.properties)

This will let others know that the controller is supported and is available to use.

---

That's it! Once you've completed these steps and tested your controller, create a pull request and add your controller to the library!
