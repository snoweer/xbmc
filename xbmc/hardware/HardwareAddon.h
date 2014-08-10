/*
 *      Copyright (C) 2012-2014 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/*
 * Adding new functions and callbacks to the XBMC Hardware API
 *
 * The Hardware API is spread out across various files. Adding new functions and
 * callbacks is an adventure that spans many layers of abstraction. Necessary
 * steps can easily be omitted, so the process of adding functions and
 * callbacks is documented here.
 *
 * The Hardware API is layed out in three files:
 *   - xbmc_hardware_dll.h        (API function declarations)
 *   - xbmc_hardware_callbacks.h  (API callback pointers)
 *   - xbmc_hardware_types.h      (API enums and structs)
 *
 * To add a new API function:
 *   1.  Declare the function in xbmc_hardware_dll.h with some helpful documentation
 *   2.  Assign the function pointer in get_addon() of the same file. get_addon()
 *       (aliased to GetAddon()) is called in AddonDll.h immediately after
 *       loading the shared library.
 *   3.  Add the function to the HardwareAddon struct in xbmc_hardware_types.h.
 *       This struct contains pointers to all the API functions. It is populated
 *       in get_addon(). CHardwareAddon invokes API functions through this struct.
 *   4.  Define the function in the cpp file of the hardware add-on project
 *
 * To add a new API callback:
 *   1.  Declare the callback as a function pointer in the CB_HardwareLib struct
 *       of xbmc_hardware_callbacks.h with some helpful documentation. The first
 *       parameter, addonData, is the CAddonCallbacksHardware object associated
 *       with the hardware add-on instance.
 *   2.  Declare the callback as a static member function of CAddonCallbacksHardware
 *   3.  Define the function in AddonCallbacksHardware.cpp and assign the callback
 *       to the callbacks table in the constructor.
 *   4.  Expose the function to the hardware add-on in libXBMC_hardware.cpp. This
 *       shared library allows for ABI compatibility if the API is unchanged
 *       across releases.
 *   5.  Add the callback to the helper class in libXBMC_hardware.h. Requires
 *       three modifications: register the symbol exported from the shared
 *       library, expose the callback using a member function wrapper, and
 *       declare the function pointer as a protected member variable.
 */

#include "addons/Addon.h"
#include "addons/AddonDll.h"
#include "addons/DllHardwareAddon.h"
#include "addons/include/xbmc_hardware_types.h"
#include "threads/CriticalSection.h"

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

namespace HARDWARE
{

class CHardwareAddon;
typedef boost::shared_ptr<CHardwareAddon> HardwareAddonPtr;
typedef std::vector<HardwareAddonPtr>     HardwareAddonVector;

class CHardwareAddon : public ADDON::CAddonDll<DllHardwareAddon, HardwareAddon, hardware_addon_properties>
{
public:
  CHardwareAddon(const ADDON::AddonProps& props);
  CHardwareAddon(const cp_extension_t* props);
  virtual ~CHardwareAddon(void);

  // Return true if this instance is initialised, false otherwise
  bool ReadyToUse(void) const { return m_bReadyToUse; }

  // Initialise the instance of this add-on
  ADDON_STATUS Create(void);

  // Destroy the instance of this add-on
  void Destroy(void);

private:
  // Called by the constructors
  void InitializeProperties(void);

  void LogAddonProperties(void);

  bool LogError(HARDWARE_ERROR error, const char* strMethod) const;
  void LogException(const char* strFunctionName) const;
  static const char* ToString(HARDWARE_ERROR error);

  ADDON::AddonVersion       m_apiVersion;
  bool                      m_bReadyToUse; // True if this add-on is connected to the device, false otherwise
  hardware_addon_properties m_props;

  // Hardware API xml parameters
  bool                      m_bHasMediaReader;

  CCriticalSection          m_critSection;
};

} // namespace HARDWARE
