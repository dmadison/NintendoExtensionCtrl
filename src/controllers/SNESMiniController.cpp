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

void SNESMiniController::printDebug(Print& output) const {
	const char fillCharacter = '_';
	
	output.print("SNES ");

	output.print(dpadLeft() ? '<' : fillCharacter);
	output.print(dpadUp() ? '^' : fillCharacter);
	output.print(dpadDown() ? 'v' : fillCharacter);
	output.print(dpadRight() ? '>' : fillCharacter);
	output.print(" | ");

	buttonSelect() ? (void) output.print("SEL") : NintendoExtensionCtrl::printRepeat(fillCharacter, 3, output);
	output.print(' ');

	buttonStart() ? (void) output.print("STR") : NintendoExtensionCtrl::printRepeat(fillCharacter, 3, output);
	output.print(" | ");

	output.print(buttonA() ? 'A' : fillCharacter);
	output.print(buttonB() ? 'B' : fillCharacter);
	output.print(buttonX() ? 'X' : fillCharacter);
	output.print(buttonY() ? 'Y' : fillCharacter);
	output.print(" | ");

	output.print(buttonL() ? 'L' : fillCharacter);
	output.print(buttonR() ? 'R' : fillCharacter);

	output.println();
}
