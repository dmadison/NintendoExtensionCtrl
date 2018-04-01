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

#ifndef NXC_DrumController_h
#define NXC_DrumController_h

#include "ExtensionController.h"

enum NXC_DrumVelocityID {
	NXC_Drum_None = 0x1F,
	NXC_Drum_Red = 0x19,
	NXC_Drum_Blue = 0x0F,
	NXC_Drum_Green = 0x12,
	NXC_Drum_Yellow = 0x11,
	NXC_Drum_Orange = 0x0E,
	NXC_Drum_Pedal = 0x1B,
};

class DrumController : public ExtensionController {
public:
	DrumController();

	uint8_t joyX();  // 6 bits, 0-63
	uint8_t joyY();

	boolean drumRed();
	boolean drumBlue();
	boolean drumGreen();

	boolean cymbalYellow();
	boolean cymbalOrange();
	
	boolean bassPedal();

	boolean buttonPlus();
	boolean buttonMinus();

	boolean velocityAvailable();
	NXC_DrumVelocityID velocityID();

	uint8_t velocity();  // 3 bits, 0-7. 7 is fast/hard, 1 is slow/soft, 0 is not hit.
	uint8_t velocity(NXC_DrumVelocityID idIn);  // velocity by ID

	uint8_t velocityRed();
	uint8_t velocityBlue();
	uint8_t velocityGreen();
	uint8_t velocityYellow();
	uint8_t velocityOrange();
	uint8_t velocityPedal();

	void printDebug(Stream& stream = Serial);
private:
	boolean validVelocityID(uint8_t idIn);
};

#endif
