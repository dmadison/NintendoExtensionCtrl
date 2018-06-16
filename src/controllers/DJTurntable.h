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

#ifndef NXC_DJTurntable_h
#define NXC_DJTurntable_h

#include "internal/ExtensionController.h"

namespace NintendoExtensionCtrl {
	class DJTurntableController_Data : private ControlDataMap {
	public:
		DJTurntableController_Data(ExtensionController & dataSource) : ControlDataMap(dataSource), left(*this), right(*this) {}

		enum class TurntableConfig {
			BaseOnly,
			Left,
			Right,
			Both,
		};

		int8_t turntable() const;  // 6 bits, -30-29. Clockwise = positive, faster = larger.

		boolean buttonGreen() const;
		boolean buttonRed() const;
		boolean buttonBlue() const;

		uint8_t effectDial() const;  // 5 bits, 0-31. One rotation per rollover.
		int8_t crossfadeSlider() const;  // 4 bits, -8-7. Negative to the left.

		boolean buttonEuphoria() const;

		uint8_t joyX() const;  // 6 bits, 0-63
		uint8_t joyY() const;

		boolean buttonPlus() const;
		boolean buttonMinus() const;

		void printDebug(Print& output = NXC_SERIAL_DEFAULT);

		TurntableConfig getTurntableConfig();
		uint8_t getNumTurntables();

		class TurntableExpansion {
		public:
			TurntableExpansion(TurntableConfig conf, DJTurntableController_Data &baseObj)
				: side(conf), base(baseObj) {}
			boolean connected() const;

			virtual int8_t turntable() const = 0;

			virtual boolean buttonGreen() const = 0;
			virtual boolean buttonRed() const = 0;
			virtual boolean buttonBlue() const = 0;

			const TurntableConfig side = TurntableConfig::BaseOnly;
		protected:
			int8_t tableSignConversion(int8_t turnData) const;
			const DJTurntableController_Data & base;
		};

		class TurntableLeft : public TurntableExpansion {
		public:
			TurntableLeft(DJTurntableController_Data &baseObj)
				: TurntableExpansion(TurntableConfig::Left, baseObj) {}
			int8_t turntable() const;

			boolean buttonGreen() const;
			boolean buttonRed() const;
			boolean buttonBlue()  const;
		} left;

		class TurntableRight : public TurntableExpansion {
		public:
			TurntableRight(DJTurntableController_Data &baseObj)
				: TurntableExpansion(TurntableConfig::Right, baseObj) {}
			int8_t turntable() const;

			boolean buttonGreen() const;
			boolean buttonRed() const;
			boolean buttonBlue() const;
		} right;

		class EffectRollover : private NintendoExtensionCtrl::RolloverChange {
		public:
			EffectRollover(DJTurntableController_Data & controller) : RolloverChange(0, 31), dj(controller) {}
			int8_t getChange();
		private:
			const DJTurntableController_Data & dj;
		};

	private:
		void printTurntable(Print& output, TurntableExpansion &table) const;

		TurntableConfig tableConfig = TurntableConfig::BaseOnly;
	};
}

typedef NintendoExtensionCtrl::BuildControllerClass
	<NintendoExtensionCtrl::DJTurntableController_Data, ExtensionType::DJTurntableController>
	DJTurntableController;

#endif
