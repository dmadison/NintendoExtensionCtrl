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

#ifndef NXC_CLASSICCONTROLLER_H
#define NXC_CLASSICCONTROLLER_H

#include "internal/ExtensionController.h"

namespace NintendoExtensionCtrl {
	class ClassicControllerBase : public ExtensionController {
	public:
		struct Maps {
			/* Classic Controller "Standard" Mode
			 *     7   6   5   4   3   2   1   0
			 * 0   RX<4:3> LX <5:0>
			 * 1   RX<2:1> LY <5:0>
			 * 2   RX<0>   LT<4:3> RY<4:0>
			 * 3   LT<2:0> RT<4:0>
			 * 4   BDR BDD BLT B-  BH  B+  BRT 1
			 * 5   BZL BB  BY  BA  BX  BZR BDL BDU
			 */
			constexpr static ByteMap LeftJoyX = ByteMap(0, 6, 0, 0);
			constexpr static ByteMap LeftJoyY = ByteMap(1, 6, 0, 0);

			constexpr static ByteMap RightJoyX[3] = { ByteMap(0, 2, 6, 3), ByteMap(1, 2, 6, 5), ByteMap(2, 1, 7, 7) };
			constexpr static ByteMap RightJoyY = ByteMap(2, 5, 0, 0);

			constexpr static BitMap  DpadUp = { 5, 0 };
			constexpr static BitMap  DpadDown = { 4, 6 };
			constexpr static BitMap  DpadLeft = { 5, 1 };
			constexpr static BitMap  DpadRight = { 4, 7 };

			constexpr static BitMap  ButtonA = { 5, 4 };
			constexpr static BitMap  ButtonB = { 5, 6 };
			constexpr static BitMap  ButtonX = { 5, 3 };
			constexpr static BitMap  ButtonY = { 5, 5 };

			constexpr static ByteMap TriggerL[2] = { ByteMap(2, 2, 5, 2), ByteMap(3, 3, 5, 5) };
			constexpr static ByteMap TriggerR = ByteMap(3, 5, 0, 0);

			constexpr static BitMap  ButtonL = { 4, 5 };
			constexpr static BitMap  ButtonR = { 4, 1 };
			constexpr static BitMap  ButtonZL = { 5, 7 };
			constexpr static BitMap  ButtonZR = { 5, 2 };

			constexpr static BitMap  ButtonPlus = { 4, 2 };
			constexpr static BitMap  ButtonMinus = { 4, 4 };
			constexpr static BitMap  ButtonHome = { 4, 3 };
		};

		struct MapsHR {
			/* Classic Controller "High Resolution" Mode
			 *     7   6   5   4   3   2   1   0
			 * 0   LX<7:0>
			 * 1   RX<7:0>
			 * 2   LY<7:0>
			 * 3   RY<7:0>
			 * 4   LT<7:0>
			 * 5   RT<7:0>
			 * 6   BDR BDD BLT B-  BH  B+  BRT 1
			 * 7   BZL BB  BY  BA  BX  BZR BDL BDU
			 */
			constexpr static IndexMap LeftJoyX = 0;
			constexpr static IndexMap LeftJoyY = 2;

			constexpr static IndexMap RightJoyX = 1;
			constexpr static IndexMap RightJoyY = 3;

			constexpr static BitMap   DpadUp = { 7, 0 };
			constexpr static BitMap   DpadDown = { 6, 6 };
			constexpr static BitMap   DpadLeft = { 7, 1 };
			constexpr static BitMap   DpadRight = { 6, 7 };

			constexpr static BitMap   ButtonA = { 7, 4 };
			constexpr static BitMap   ButtonB = { 7, 6 };
			constexpr static BitMap   ButtonX = { 7, 3 };
			constexpr static BitMap   ButtonY = { 7, 5 };

			constexpr static IndexMap TriggerL = 4;
			constexpr static IndexMap TriggerR = 5;

			constexpr static BitMap   ButtonL = { 6, 5 };
			constexpr static BitMap   ButtonR = { 6, 1 };
			constexpr static BitMap   ButtonZL = { 7, 7 };
			constexpr static BitMap   ButtonZR = { 7, 2 };

			constexpr static BitMap   ButtonPlus = { 6, 2 };
			constexpr static BitMap   ButtonMinus = { 6, 4 };
			constexpr static BitMap   ButtonHome = { 6, 3 };
		};

		using ExtensionController::ExtensionController;

		boolean specificInit();

		ExtensionType getExpectedType() const;

		boolean setHighRes(boolean hr = true, boolean verify = true);
		boolean getHighRes() const;

		uint8_t leftJoyX() const;  // 8 bits, 6 shifted in std mode
		uint8_t leftJoyY() const;

		uint8_t rightJoyX() const;  // 8 bits, 5 shifted in std mode
		uint8_t rightJoyY() const;

		boolean dpadUp() const;
		boolean dpadDown() const;
		boolean dpadLeft() const;
		boolean dpadRight() const;

		boolean buttonA() const;
		boolean buttonB() const;
		boolean buttonX() const;
		boolean buttonY() const;

		uint8_t triggerL() const;  // 8 bits, 5 shifted in std mode
		uint8_t triggerR() const;

		boolean buttonL() const;
		boolean buttonR() const;

		boolean buttonZL() const;
		boolean buttonZR() const;

		boolean buttonStart() const;
		boolean buttonSelect() const;

		boolean buttonPlus() const;
		boolean buttonMinus() const;

		boolean buttonHome() const;

		void printDebug(Print& output = NXC_SERIAL_DEFAULT) const;

	protected:
		boolean highRes = false;  // 'high resolution' mode setting

		boolean checkDataMode(boolean *hr) const;
		boolean setDataMode(boolean hr, boolean verify = true);
	};


	/* Nintendo Mini Console Controllers */

	class NESMiniControllerBase : public ClassicControllerBase {
	public:
		using ClassicControllerBase::ClassicControllerBase;

		void printDebug(Print& output = NXC_SERIAL_DEFAULT) const;
	};

	class SNESMiniControllerBase : public ClassicControllerBase {
	public:
		using ClassicControllerBase::ClassicControllerBase;

		void printDebug(Print& output = NXC_SERIAL_DEFAULT) const;
	};
}

using ClassicController = NintendoExtensionCtrl::BuildControllerClass
	<NintendoExtensionCtrl::ClassicControllerBase>;

using NESMiniController = NintendoExtensionCtrl::BuildControllerClass
	<NintendoExtensionCtrl::NESMiniControllerBase>;

using SNESMiniController = NintendoExtensionCtrl::BuildControllerClass
	<NintendoExtensionCtrl::SNESMiniControllerBase>;

#endif
