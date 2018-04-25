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

#include "SNESMiniController.h"

SNESMiniController::SNESMiniController(NXC_I2C_TYPE& i2cBus) : ExtensionController(i2cBus, NXC_ClassicController, 6) {}
SNESMiniController::SNESMiniController(ExtensionData& busData) : ExtensionController(busData, NXC_ClassicController, 6) {}

void SNESMiniController::printDebug(Stream& stream) const {
	const char fillCharacter = '_';
	
	stream.print("SNES ");

	stream.print(dpadLeft() ? '<' : fillCharacter);
	stream.print(dpadUp() ? '^' : fillCharacter);
	stream.print(dpadDown() ? 'v' : fillCharacter);
	stream.print(dpadRight() ? '>' : fillCharacter);
	stream.print(" | ");

	buttonSelect() ? (void) stream.print("SEL") : NXCtrl::printRepeat(fillCharacter, 3, stream);
	stream.print(' ');

	buttonStart() ? (void) stream.print("STR") : NXCtrl::printRepeat(fillCharacter, 3, stream);
	stream.print(" | ");

	stream.print(buttonA() ? 'A' : fillCharacter);
	stream.print(buttonB() ? 'B' : fillCharacter);
	stream.print(buttonX() ? 'X' : fillCharacter);
	stream.print(buttonY() ? 'Y' : fillCharacter);
	stream.print(" | ");

	stream.print(buttonL() ? 'L' : fillCharacter);
	stream.print(buttonR() ? 'R' : fillCharacter);

	stream.println();
}
