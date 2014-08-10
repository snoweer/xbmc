#pragma once
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

#include "AddonCallbacks.h"
#include "include/xbmc_hardware_callbacks.h"

namespace HARDWARE { class CHardwareAddon; }

namespace ADDON
{

class CAddon;

/*!
 * Callbacks for a hardware add-on to XBMC.
 *
 * Also translates the addon's C structures to XBMC's C++ structures.
 */
class CAddonCallbacksHardware
{
public:
  CAddonCallbacksHardware(CAddon* addon);
  ~CAddonCallbacksHardware(void);

  /*!
   * @return The callback table.
   */
  CB_HardwareLib* GetCallbacks() const { return m_callbacks; }

  static void MediaInsert(void* addonData, const MEDIA_PORT_METADATA* metadata);
  static void MediaRemove(void* addonData, const MEDIA_PORT_METADATA* metadata);

private:
  static HARDWARE::CHardwareAddon* GetHardwareAddon(void* addonData, const char* strFunction);

  CB_HardwareLib*  m_callbacks; /*!< callback addresses */
  CAddon*          m_addon;     /*!< the add-on */
};

}; /* namespace ADDON */
