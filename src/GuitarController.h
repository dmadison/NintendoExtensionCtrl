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

#ifndef GuitarController_h
#define GuitarController_h

#include "ExtensionController.h"

class GuitarController : public ExtensionController {
public:
	GuitarController();

	uint8_t joyX();  // 6 bits, 0-63
	uint8_t joyY();

	boolean strum();
	boolean strumUp();
	boolean strumDown();

	boolean fretGreen();
	boolean fretRed();
	boolean fretYellow();
	boolean fretBlue();
	boolean fretOrange();

	uint8_t whammyBar();  // 5 bits, 0-31

	uint8_t touchbar();  // 5 bits, 0-31
	boolean touchGreen();
	boolean touchRed();
	boolean touchYellow();
	boolean touchBlue();
	boolean touchOrange();

	boolean buttonPlus();
	boolean buttonMinus();

	void printDebug(Stream& stream=Serial);

	boolean supportsTouchbar();

private:
	boolean touchbarData = false;  // Flag for touchbar data found
};

#endif
