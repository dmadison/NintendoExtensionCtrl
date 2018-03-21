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

	uint8_t getLeftJoyX();
	uint8_t getLeftJoyY();

	uint8_t getRightJoyX();
	uint8_t getRightJoyY();

	boolean getPadUp();
	boolean getPadDown();
	boolean getPadLeft();
	boolean getPadRight();

	boolean getButtonA();
	boolean getButtonB();
	boolean getButtonX();
	boolean getButtonY();

	uint8_t getTriggerL();
	uint8_t getTriggerR();
	boolean getButtonLT();
	boolean getButtonRT();

	boolean getButtonZL();
	boolean getButtonZR();

	boolean getButtonPlus();
	boolean getButtonMinus();
	boolean getButtonHome();
	
	void printDebug(Stream& stream=Serial);
};

#endif
