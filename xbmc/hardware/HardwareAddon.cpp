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

#include "HardwareAddon.h"
#include "addons/AddonManager.h"
#include "cores/RetroPlayer/RetroPlayer.h"
#include "FileItem.h"
#include "filesystem/Directory.h"
#include "filesystem/SpecialProtocol.h"
#include "settings/Settings.h"
#include "threads/SingleLock.h"
#include "URL.h"
#include "utils/log.h"
#include "utils/StdString.h"
#include "utils/StringUtils.h"
#include "utils/URIUtils.h"
#include "utils/Variant.h"

using namespace ADDON;
using namespace HARDWARE;
using namespace std;

#define PROP_SUPPORTS_MEDIA_READER  "supports_media_reader"

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)  do { delete (p); (p)=NULL; } while (0)
#endif

CHardwareAddon::CHardwareAddon(const AddonProps& props)
  : CAddonDll<DllHardwareAddon, HardwareAddon, hardware_addon_properties>(props),
    m_apiVersion("0.0.0"),
    m_props()
{
  InitializeProperties();

  InfoMap::const_iterator it;
  if ((it = props.extrainfo.find(PROP_SUPPORTS_MEDIA_READER)) != props.extrainfo.end())
    m_bHasMediaReader = (it->second == "true" || it->second == "yes");
}

CHardwareAddon::CHardwareAddon(const cp_extension_t* ext)
  : CAddonDll<DllHardwareAddon, HardwareAddon, hardware_addon_properties>(ext),
    m_apiVersion("0.0.0"),
    m_props()
{
  InitializeProperties();

  if (ext)
  {
    string strHasMediaReader = CAddonMgr::Get().GetExtValue(ext->configuration, PROP_SUPPORTS_MEDIA_READER);
    if (!strHasMediaReader.empty())
    {
      Props().extrainfo.insert(make_pair(PROP_SUPPORTS_MEDIA_READER, strHasMediaReader));
      m_bHasMediaReader = (strHasMediaReader == "true" || strHasMediaReader == "yes");
    }
  }
}

void CHardwareAddon::InitializeProperties(void)
{
  m_bReadyToUse = false;
  m_bHasMediaReader = false;
}

CHardwareAddon::~CHardwareAddon(void)
{
  Destroy();
  SAFE_DELETE(m_pInfo); // TODO: Move to CAddonDll
}

ADDON_STATUS CHardwareAddon::Create(void)
{
  ADDON_STATUS status = ADDON_STATUS_UNKNOWN;

  // Ensure that a previous instance is destroyed
  Destroy();

  // Initialise the add-on
  bool bReadyToUse = false;
  m_pInfo = &m_props;

  CLog::Log(LOGDEBUG, "HARDWARE - %s - creating game add-on instance '%s'", __FUNCTION__, Name().c_str());
  try
  {
    status = CAddonDll<DllHardwareAddon, HardwareAddon, hardware_addon_properties>::Create();
    if (status == ADDON_STATUS_OK)
    {
      bReadyToUse = true;
      LogAddonProperties();
    }
  }
  catch (...) { LogException(__FUNCTION__); }

  m_bReadyToUse = bReadyToUse;

  return status;
}

void CHardwareAddon::Destroy(void)
{
  // Reset 'ready to use' to false
  if (!m_bReadyToUse)
    return;
  m_bReadyToUse = false;

  CLog::Log(LOGDEBUG, "HARDWARE: %s - destroying game add-on %s", __FUNCTION__, ID().c_str());

  // Destroy the add-on
  try { CAddonDll<DllHardwareAddon, HardwareAddon, hardware_addon_properties>::Destroy(); }
  catch (...) { LogException(__FUNCTION__); }
}

void CHardwareAddon::LogAddonProperties(void)
{
  CLog::Log(LOGINFO, "HARDWARE: ------------------------------------");
  CLog::Log(LOGINFO, "HARDWARE: Loaded DLL for %s", ID().c_str());
  CLog::Log(LOGINFO, "HARDWARE: Client: %s at version %s", Name().c_str(), Version().c_str());
  CLog::Log(LOGINFO, "HARDWARE: Has media reader: %s", m_bHasMediaReader ? "yes" : "no");
  CLog::Log(LOGINFO, "HARDWARE: ------------------------------------");
}

bool CHardwareAddon::LogError(HARDWARE_ERROR error, const char* strMethod) const
{
  if (error != HARDWARE_NO_ERROR)
  {
    CLog::Log(LOGERROR, "HARDWARE - %s - addon '%s' returned an error: %s",
        strMethod, ID().c_str(), ToString(error));
    return false;
  }
  return true;
}

void CHardwareAddon::LogException(const char* strFunctionName) const
{
  CLog::Log(LOGERROR, "HARDWARE: exception caught while trying to call '%s' on add-on %s",
      strFunctionName, ID().c_str());
  CLog::Log(LOGERROR, "Please contact the developer of this add-on: %s", Author().c_str());
}

const char* CHardwareAddon::ToString(HARDWARE_ERROR error)
{
  switch (error)
  {
  case HARDWARE_NO_ERROR:
    return "no error";
  case HARDWARE_ERROR_FAILED:
    return "the command failed";
  case HARDWARE_ERROR_INVALID_PARAMETERS:
    return "invalid parameters for this method";
  case HARDWARE_ERROR_NOT_IMPLEMENTED:
    return "not implemented";
  case HARDWARE_ERROR_NOT_CONNECTED:
    return "not connected";
  case HARDWARE_ERROR_CONNECTION_FAILED:
    return "connection failed";
  case HARDWARE_ERROR_UNKNOWN:
  default:
    return "unknown error";
  }
}
