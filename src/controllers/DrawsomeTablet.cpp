/*
*  Project     Nintendo Extension Controller Library
*  @author     nullstalgia
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

#include "DrawsomeTablet.h"

namespace NintendoExtensionCtrl {

constexpr IndexMap  DrawsomeTablet_Shared::Maps::PenX_LSB;
constexpr IndexMap  DrawsomeTablet_Shared::Maps::PenX_MSB;
constexpr IndexMap  DrawsomeTablet_Shared::Maps::PenY_LSB;
constexpr IndexMap  DrawsomeTablet_Shared::Maps::PenY_MSB;

constexpr IndexMap  DrawsomeTablet_Shared::Maps::Pressure_LSB;
constexpr ByteMap   DrawsomeTablet_Shared::Maps::Pressure_MSB;

constexpr BitMap    DrawsomeTablet_Shared::Maps::Pen_Detected;

boolean DrawsomeTablet_Shared::specificInit() {
	/* Two necessary register writes during initialization before the tablet
	 * will start sending data. See this for reference:
	 * https://www.raphnet.net/divers/wii_graphics_tablets/index_en.php
	 */
	return writeRegister(0xFB, 0x01) && writeRegister(0xF0, 0x55);
}

uint16_t DrawsomeTablet_Shared::penX() const {
	return (getControlData(Maps::PenX_MSB) << 8) | getControlData(Maps::PenX_LSB);
}

uint16_t DrawsomeTablet_Shared::penY() const {
	return (getControlData(Maps::PenY_MSB) << 8) | getControlData(Maps::PenY_LSB);
}

uint16_t DrawsomeTablet_Shared::penPressure() const {
	return (getControlData(Maps::Pressure_MSB) << 8) | getControlData(Maps::Pressure_LSB);
}

boolean DrawsomeTablet_Shared::penDetected() const {
	return getControlBit(Maps::Pen_Detected);
}

void DrawsomeTablet_Shared::printDebug(Print& output) const {
	char buffer[60];
	
	const char penPrint = penDetected() ? 'Y' : 'N';

	output.print("DrawsomeTablet - ");
	sprintf(buffer,
		"Pen:(%6u, %6u) | Pressure:%4u | Pen Detect:%c",
			penX(), penY(), penPressure(), penPrint);

	output.println(buffer);
}

}  // End "NintendoExtensionCtrl" namespace
