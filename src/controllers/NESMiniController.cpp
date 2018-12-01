/*
*  Project     Nintendo Extension Controller Library
*  @author     David Madison
*  @link       github.com/dmadison/NintendoExtensionCtrl
*  @license    LGPLv3 - Copyright (c) 2018 David Madison
*
*  This file is part of the Nintendo Extension Controller Library.
*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "NESMiniController.h"

namespace NintendoExtensionCtrl {

boolean NESMiniController_Data::dpadUp() const {
	return knockoffButton<&ClassicController_Data::dpadUp>(Maps::Knockoff_DpadUp);
}

boolean NESMiniController_Data::dpadDown() const {
	return knockoffButton<&ClassicController_Data::dpadDown>(Maps::Knockoff_DpadDown);
}

boolean NESMiniController_Data::dpadLeft() const {
	return knockoffButton<&ClassicController_Data::dpadLeft>(Maps::Knockoff_DpadLeft);
}

boolean NESMiniController_Data::dpadRight() const {
	return knockoffButton<&ClassicController_Data::dpadRight>(Maps::Knockoff_DpadRight);
}

boolean NESMiniController_Data::buttonA() const {
	return knockoffButton<&ClassicController_Data::buttonA>(Maps::Knockoff_ButtonA);
}

boolean NESMiniController_Data::buttonB() const {
	return knockoffButton<&ClassicController_Data::buttonB>(Maps::Knockoff_ButtonB);
}

boolean NESMiniController_Data::buttonStart() const {
	return knockoffButton<&ClassicController_Data::buttonStart>(Maps::Knockoff_ButtonStart);
}

boolean NESMiniController_Data::buttonSelect() const {
	return knockoffButton<&ClassicController_Data::buttonSelect>(Maps::Knockoff_ButtonSelect);
}

void NESMiniController_Data::printDebug(Print& output) const {
	const char fillCharacter = '_';
	
	output.print("NES ");

	output.print(dpadLeft() ? '<' : fillCharacter);
	output.print(dpadUp() ? '^' : fillCharacter);
	output.print(dpadDown() ? 'v' : fillCharacter);
	output.print(dpadRight() ? '>' : fillCharacter);
	output.print(" | ");

	buttonSelect() ? (void)output.print("SEL") : NintendoExtensionCtrl::printRepeat(fillCharacter, 3, output);
	output.print(' ');

	buttonStart() ? (void)output.print("STR") : NintendoExtensionCtrl::printRepeat(fillCharacter, 3, output);
	output.print(" | ");

	output.print(buttonB() ? 'B' : fillCharacter);
	output.print(buttonA() ? 'A' : fillCharacter);

	output.println();
}

boolean NESMiniController_Data::isKnockoff() const {
	// The NES knockoffs I've come across seem to display the same unchanging pattern
	// for the first six control bytes:
	//
	//     0x81, 0x81, 0x81, 0x81, 0x00, 0x00
	//
	// This is mostly garbage data that doesn't line up with the Classic Controller
	// at all. Using the library's debug output, here is what it translates to:
	//
	//    <^v> | -H+ | ABXY L : (1, 1) R : (21, 1) | LT : 4X RT : 1X Z : LR
	//
	// You'll notice a few things. ALL possible buttons are pressed. The left analog 
	// stick is completely down and to the left, while the right analog stick is down
	// and to the right. On the back, both trigger "fully depressed" buttons are
	// down, and yet the analog trigger values are very low. In short, this is a
	// difficult if not impossible state for a normal Classic Controller to be in.
	// Because of that, we can reasonably assume that if the bytes match this then the
	// connected controller is an NES Knockoff, and can be treated accordingly. 

	return getControlData(0) == 0x81 &&  // RX 4:3, LX
	       getControlData(1) == 0x81 &&  // RX 2:1, LY
	       getControlData(2) == 0x81 &&  // RX 0, LT 4:3, RY
	       getControlData(3) == 0x81 &&  // LT 2:0, RT
	       getControlData(4) == 0x00 &&  // Button packet 1 (all pressed)
	       getControlData(5) == 0x00;    // Button packet 2 (all pressed)
}

}  // End "NintendoExtensionCtrl" namespace
