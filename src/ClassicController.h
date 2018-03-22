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

#ifndef ClassicController_h
#define ClassicController_h

#include "ExtensionController.h"

class ClassicController : public ExtensionController {
public:
	ClassicController();

	uint8_t leftJoyX();  // 6 bits, 0-63
	uint8_t leftJoyY();

	uint8_t rightJoyX();  // 5 bits, 0-31
	uint8_t rightJoyY();

	boolean dpadUp();
	boolean dpadDown();
	boolean dpadLeft();
	boolean dpadRight();

	boolean buttonA();
	boolean buttonB();
	boolean buttonX();
	boolean buttonY();

	uint8_t triggerL();  // 5 bits, 0-31
	uint8_t triggerR();

	boolean buttonLT();
	boolean buttonRT();

	boolean buttonZL();
	boolean buttonZR();

	boolean buttonPlus();
	boolean buttonMinus();
	boolean buttonHome();
	
	void printDebug(Stream& stream=Serial);
};

#endif
