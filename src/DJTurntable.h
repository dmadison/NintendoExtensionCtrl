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

#include "ExtensionController.h"

enum NXC_DJTurntable_Configuration {
	NXC_DJTurntable_BaseOnly,
	NXC_DJTurntable_Left,
	NXC_DJTurntable_Right,
	NXC_DJTurntable_Both,
};

namespace NintendoExtensionCtrl {
	namespace DataMaps {
		class DJTurntableController : private ControlDataMap {
		public:
			DJTurntableController(ExtensionController & dataSource) : ControlDataMap(dataSource), left(*this), right(*this) {}

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

			void printDebug(Stream& stream = NXC_SERIAL_DEFAULT);

			NXC_DJTurntable_Configuration getTurntableConfig();
			uint8_t getNumTurntables();

			class TurntableExpansion {
			public:
				TurntableExpansion(NXC_DJTurntable_Configuration conf, DJTurntableController &baseObj)
					: side(conf), base(baseObj) {}
				boolean connected() const;

				virtual int8_t turntable() const = 0;

				virtual boolean buttonGreen() const = 0;
				virtual boolean buttonRed() const = 0;
				virtual boolean buttonBlue() const = 0;

				const NXC_DJTurntable_Configuration side = NXC_DJTurntable_BaseOnly;
			protected:
				int8_t tableSignConversion(int8_t turnData) const;
				const DJTurntableController & base;
			};

			class TurntableLeft : public TurntableExpansion {
			public:
				TurntableLeft(DJTurntableController &baseObj)
					: TurntableExpansion(NXC_DJTurntable_Left, baseObj) {}
				int8_t turntable() const;

				boolean buttonGreen() const;
				boolean buttonRed() const;
				boolean buttonBlue()  const;
			} left;

			class TurntableRight : public TurntableExpansion {
			public:
				TurntableRight(DJTurntableController &baseObj)
					: TurntableExpansion(NXC_DJTurntable_Right, baseObj) {}
				int8_t turntable() const;

				boolean buttonGreen() const;
				boolean buttonRed() const;
				boolean buttonBlue() const;
			} right;

			class EffectRollover : private NXCtrl::RolloverChange {
			public:
				EffectRollover(DJTurntableController & controller) : RolloverChange(0, 31), dj(controller) {}
				int8_t getChange();
			private:
				const DJTurntableController & dj;
			};

		private:
			void printTurntable(Stream& stream, TurntableExpansion &table) const;

			NXC_DJTurntable_Configuration tableConfig = NXC_DJTurntable_BaseOnly;
		};
	}
}

class DJTurntableController : public ExtensionController, public NintendoExtensionCtrl::DataMaps::DJTurntableController {
public:
	typedef NintendoExtensionCtrl::DataMaps::DJTurntableController DataMap;

	DJTurntableController(NXC_I2C_TYPE& i2cBus = NXC_I2C_DEFAULT) :
		ExtensionController(i2cBus, NXC_DJTurntable, 6),
		DataMap(*(static_cast<ExtensionController*>(this))) {}

	using DataMap::printDebug;
};

#endif
