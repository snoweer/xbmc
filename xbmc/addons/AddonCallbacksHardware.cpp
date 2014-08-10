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

#include "AddonCallbacksHardware.h"
#include "Addon.h"
#include "hardware/HardwareAddon.h"
#include "utils/log.h"

#include <string>

using namespace HARDWARE;
using namespace std;

namespace ADDON
{

CAddonCallbacksHardware::CAddonCallbacksHardware(CAddon* addon)
{
  m_addon     = addon;
  m_callbacks = new CB_HardwareLib;

  /* write XBMC game specific add-on function addresses to callback table */
  m_callbacks->MediaInsert = MediaInsert;
  m_callbacks->MediaRemove = MediaRemove;
}

CAddonCallbacksHardware::~CAddonCallbacksHardware(void)
{
  /* delete the callback table */
  delete m_callbacks;
}

CHardwareAddon* CAddonCallbacksHardware::GetHardwareAddon(void* addonData, const char* strFunction)
{
  CAddonCallbacks* addon = static_cast<CAddonCallbacks*>(addonData);
  if (!addon || !addon->GetHelperHardware())
  {
    CLog::Log(LOGERROR, "GAME - %s - called with a null pointer", strFunction);
    return NULL;
  }

  return dynamic_cast<CHardwareAddon*>(addon->GetHelperHardware()->m_addon);
}

void CAddonCallbacksHardware::MediaInsert(void* addonData, const MEDIA_PORT_METADATA* metadata)
{
  CHardwareAddon *addon = GetHardwareAddon(addonData, __FUNCTION__);
  if (!addon)
  {
    CLog::Log(LOGERROR, "HARDWARE - %s - invalid handler data", __FUNCTION__);
    return;
  }

  // get the metadata
  //CMediaPortMetadata data = SomethingUtils::Something(metadata);

  // TODO
}

void CAddonCallbacksHardware::MediaRemove(void* addonData, const MEDIA_PORT_METADATA* metadata)
{
  CHardwareAddon *addon = GetHardwareAddon(addonData, __FUNCTION__);
  if (!addon)
  {
    CLog::Log(LOGERROR, "HARDWARE - %s - invalid handler data", __FUNCTION__);
    return;
  }

  // get the metadata
  //CMediaPortMetadata data = SomethingUtils::Something(metadata);

  // TODO
}

}; /* namespace ADDON */
