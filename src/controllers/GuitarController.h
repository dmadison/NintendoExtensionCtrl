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

#ifndef NXC_GUITARCONTROLLER_H
#define NXC_GUITARCONTROLLER_H

#include "internal/ExtensionController.h"

#include "ClassicController.h"  // For joystick and +/- control maps

namespace NintendoExtensionCtrl {
	class GuitarControllerBase : public ExtensionController {
	public:
		struct Maps {
			constexpr static ByteMap JoyX = ClassicControllerBase::Maps::LeftJoyX;
			constexpr static ByteMap JoyY = ClassicControllerBase::Maps::LeftJoyY;

			constexpr static BitMap  ButtonPlus = ClassicControllerBase::Maps::ButtonPlus;
			constexpr static BitMap  ButtonMinus = ClassicControllerBase::Maps::ButtonMinus;

			constexpr static BitMap  StrumUp = { 5, 0 };
			constexpr static BitMap  StrumDown = { 4, 6 };

			constexpr static BitMap  FretGreen = { 5, 4 };
			constexpr static BitMap  FretRed = { 5, 6 };
			constexpr static BitMap  FretYellow = { 5, 3 };
			constexpr static BitMap  FretBlue = { 5, 5 };
			constexpr static BitMap  FretOrange = { 5, 7 };

			constexpr static ByteMap Whammy = ByteMap(3, 5, 0, 0);
			constexpr static ByteMap Touchbar = ByteMap(2, 5, 0, 0);
		};

		using ExtensionController::ExtensionController;

		ExtensionType getExpectedType() const;

		uint8_t joyX() const;  // 6 bits, 0-63
		uint8_t joyY() const;

		boolean strum() const;
		boolean strumUp() const;
		boolean strumDown() const;

		boolean fretGreen() const;
		boolean fretRed() const;
		boolean fretYellow() const;
		boolean fretBlue() const;
		boolean fretOrange() const;

		uint8_t whammyBar() const;  // 5 bits, 0-31 (starting at ~15-16)

		uint8_t touchbar() const;  // 5 bits, 0-31
		boolean touchGreen() const;
		boolean touchRed() const;
		boolean touchYellow() const;
		boolean touchBlue() const;
		boolean touchOrange() const;

		boolean buttonPlus() const;
		boolean buttonMinus() const;

		void printDebug(Print& output = NXC_SERIAL_DEFAULT);

		boolean supportsTouchbar();

	private:
		boolean touchbarData = false;  // Flag for touchbar data found
	};
}

using GuitarController = NintendoExtensionCtrl::BuildControllerClass
	<NintendoExtensionCtrl::GuitarControllerBase>;

#endif
