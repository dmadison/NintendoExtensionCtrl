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

#ifndef NXC_DataMaps_h
#define NXC_DataMaps_h

namespace NintendoExtensionCtrl {
	class ControlDataMap {
	public:
		ControlDataMap(ExtensionController & dataSource) : ControlData(dataSource) {}

		uint8_t getControlData(uint8_t index) const {
			return ControlData.getControlData(index);
		}

		boolean getControlBit(uint8_t index, uint8_t pos) const {
			return ControlData.getControlBit(index, pos);
		}

	private:
		const ExtensionController & ControlData;
	};

	template <class ControllerMap, ExtensionType controllerID>
	class BuildControllerClass : public ExtensionController, public ControllerMap {
	public:
		typedef ControllerMap DataMap;  // Make controller data class easily accessible

		BuildControllerClass(NXC_I2C_TYPE& i2cBus = NXC_I2C_DEFAULT) :
			ExtensionController(i2cBus, controllerID),
			DataMap(*(static_cast<ExtensionController*>(this))) {}

		using DataMap::printDebug;  // Use the controller-specific print
	};
}

#endif
