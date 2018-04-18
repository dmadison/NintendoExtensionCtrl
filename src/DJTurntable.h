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

class DJTurntableController : public ExtensionController {
public:
	DJTurntableController(NXC_I2C_TYPE& i2cBus = NXC_I2C_DEFAULT);
	DJTurntableController(ExtensionData& busData);

	int8_t turntable();  // 6 bits, -30-29. Clockwise = positive, faster = larger.

	boolean buttonGreen();
	boolean buttonRed();
	boolean buttonBlue();

	uint8_t effectDial();  // 5 bits, 0-31. One rotation per rollover.
	int8_t crossfadeSlider();  // 4 bits, -8-7. Negative to the left.
	
	boolean buttonEuphoria();

	uint8_t joyX();  // 6 bits, 0-63
	uint8_t joyY();

	boolean buttonPlus();
	boolean buttonMinus();

	void printDebug(Stream& stream=NXC_SERIAL_DEFAULT);

	NXC_DJTurntable_Configuration getTurntableConfig();
	uint8_t getNumTurntables();

	class TurntableExpansion {
	public:
		TurntableExpansion(NXC_DJTurntable_Configuration conf, DJTurntableController &baseObj)
			: side(conf), base(baseObj) {}
		boolean connected();

		virtual int8_t turntable() = 0;

		virtual boolean buttonGreen() = 0;
		virtual boolean buttonRed() = 0;
		virtual boolean buttonBlue() = 0;

		const NXC_DJTurntable_Configuration side = NXC_DJTurntable_BaseOnly;
	protected:
		int8_t tableSignConversion(int8_t turnData);
		DJTurntableController & base;
	};

	class TurntableLeft : public TurntableExpansion {
	public:
		TurntableLeft(DJTurntableController &baseObj)
			: TurntableExpansion(NXC_DJTurntable_Left, baseObj) {}
		int8_t turntable();

		boolean buttonGreen();
		boolean buttonRed();
		boolean buttonBlue();
	} left;

	class TurntableRight: public TurntableExpansion {
	public:
		TurntableRight(DJTurntableController &baseObj)
			: TurntableExpansion(NXC_DJTurntable_Right, baseObj) {}
		int8_t turntable();

		boolean buttonGreen();
		boolean buttonRed();
		boolean buttonBlue();
	} right;

private:
	void printTurntable(Stream& stream, TurntableExpansion &table);

	NXC_DJTurntable_Configuration tableConfig = NXC_DJTurntable_BaseOnly;
};

#endif
