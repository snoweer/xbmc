/*
 *      Copyright (C) 2014 Team XBMC
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

//#include "LibretroDLL.h"
#include "filesystem/File.h"
#include "filesystem/Directory.h"

#include "libXBMC_addon.h"
#include "libXBMC_hardware.h"
#include "xbmc_addon_cpp_dll.h"
#include "xbmc_addon_dll.h"
#include "xbmc_addon_types.h"
#include "xbmc_hardware_dll.h"

#include <assert.h>
#include <string>
#include <string.h>
#include <vector>

using namespace ADDON;
using namespace MEDIA_READER;

// TODO: Move to settings API
#define SETTING_ENABLE_CHECKSUMS          "enablechecksums"
#define SETTING_ENABLE_CHECKSUMS_DESCR    "Calculate CRC checksums of ROM/SRAM data"

#define SETTING_ENABLE_SAVE_WRITING       "enablesavewriting"
#define SETTING_ENABLE_SAVE_WRITING_DESCR "Enable writing save files to the cartridge"

#ifndef SAFE_DELETE
  #define SAFE_DELETE(x)  do { delete x; x = NULL; } while (0)
#endif

#ifndef STATIC_ASSERT
  #if 0 && defined(__cplusplus11) // TODO
    #define STATIC_ASSERT(x)  static_assert(x)
  #else
    #define STATIC_ASSERT(x)  do { static const char sa[(x) ? 1 : -1] = { }; (void)sa; } while(0)
  #endif
#endif

#define MEDIA_READER_NAME     "Media Reader"
#define MEDIA_READER_PORT_URI "hardware://Media/SNES"

extern "C"
{

CHelper_libXBMC_addon*    XBMC          = NULL;
CHelper_libXBMC_hardware* HARDWARE      = NULL;
//CLibretroDLL*             CLIENT        = NULL;
//CClientBridge*            CLIENT_BRIDGE = NULL;

ADDON_STATUS ADDON_Create(void* callbacks, void* props)
{
  try
  {
    if (!callbacks || !props)
      throw ADDON_STATUS_UNKNOWN;

    HARDWARE_ADDON_PROPERTIES* hwAddonProps = static_cast<HARDWARE_ADDON_PROPERTIES*>(props);
    (void)hwAddonProps; // unused

    XBMC = new CHelper_libXBMC_addon;
    if (!XBMC || !XBMC->RegisterMe(callbacks))
      throw ADDON_STATUS_PERMANENT_FAILURE;

    HARDWARE = new CHelper_libXBMC_hardware;
    if (!HARDWARE || !HARDWARE->RegisterMe(callbacks))
      throw ADDON_STATUS_PERMANENT_FAILURE;

    /*
    CLIENT = new CLibretroDLL(XBMC);
    if (!CLIENT->Load(hardwareAddonProps))
    {
      XBMC->Log(LOG_ERROR, "Failed to load %s", hardwareAddonProps->library_path);
      throw ADDON_STATUS_PERMANENT_FAILURE;
    }
    */

    // TODO: Check version

    // TODO: Init hardware
  }
  catch (const ADDON_STATUS& status)
  {
    SAFE_DELETE(HARDWARE);
    SAFE_DELETE(XBMC);
    //SAFE_DELETE(CLIENT);
    return status;
  }

  return ADDON_STATUS_OK;
}

void ADDON_Stop(void)
{
}

void ADDON_Destroy(void)
{
  //if (CLIENT)
  //  CLIENT->retro_deinit();

  SAFE_DELETE(HARDWARE);
  SAFE_DELETE(XBMC);
  //SAFE_DELETE(CLIENT);
}

ADDON_STATUS ADDON_GetStatus(void)
{
  if (XBMC)
  {
    assert(XBMC != NULL);
    assert(HARDWARE != NULL);
    //assert(CLIENT != NULL);
    return ADDON_STATUS_OK;
  }
  else
  {
    assert(XBMC == NULL);
    assert(HARDWARE == NULL);
    //assert(CLIENT == NULL);
    return ADDON_STATUS_UNKNOWN;
  }
}

bool ADDON_HasSettings(void)
{
  return true;
}

static ADDON_StructSetting*** ADDON_SETTINGS = NULL;
unsigned int ADDON_SETTINGS_COUNT = 0;

unsigned int ADDON_GetSettings(ADDON_StructSetting*** sSettings)
{
  ADDON_FreeSettings();

  std::vector<DllSetting> vSettings;

  // TODO: Refactor into settings API
  {
    DllSetting dllSetting(DllSetting::CHECK, SETTING_ENABLE_CHECKSUMS, SETTING_ENABLE_CHECKSUMS_DESCR);
    dllSetting.current = 1; // TODO: Get value from settings API
    vSettings.push_back(dllSetting);
  }

  {
    DllSetting dllSetting(DllSetting::CHECK, SETTING_ENABLE_SAVE_WRITING, SETTING_ENABLE_SAVE_WRITING_DESCR);
    dllSetting.current = 0; // TODO: Get value from settings API
    vSettings.push_back(dllSetting);
  }

  DllUtils::VecToStruct(vSettings, ADDON_SETTINGS);
  ADDON_SETTINGS_COUNT = vSettings.size();

  *sSettings = *ADDON_SETTINGS;
  return ADDON_SETTINGS_COUNT;
}

ADDON_STATUS ADDON_SetSetting(const char* settingName, const void* settingValue)
{
  if (!settingName || !settingValue)
    return ADDON_STATUS_UNKNOWN;

  if (strcmp(settingName, SETTING_ENABLE_CHECKSUMS) == 0)
  {
    std::string strValue = static_cast<const char*>(settingValue);
    bool bEnableChecksums = (strValue == "true");
    // TODO: Report value to settings API
    return ADDON_STATUS_OK;
  }

  if (strcmp(settingName, SETTING_ENABLE_SAVE_WRITING) == 0)
  {
    std::string strValue = static_cast<const char*>(settingValue);
    bool bEnableSaveWriting = (strValue == "true");
    // TODO: Report value to settings API
    return ADDON_STATUS_OK;
  }

  return ADDON_STATUS_OK;
}

void ADDON_FreeSettings(void)
{
  if (ADDON_SETTINGS)
  {
    assert(ADDON_SETTINGS_COUNT != 0);

    DllUtils::FreeStruct(ADDON_SETTINGS_COUNT, ADDON_SETTINGS);
    ADDON_SETTINGS = NULL;
    ADDON_SETTINGS_COUNT = 0;
  }
}

void ADDON_Announce(const char *flag, const char *sender, const char *message, const void *data)
{
}

const char* GetHardwareAPIVersion(void)
{
  return HARDWARE_API_VERSION;
}

const char* GetMininumGameAPIVersion(void)
{
  return HARDWARE_MIN_API_VERSION;
}

HARDWARE_ERROR MediaGetMetadata(MEDIA_READER_METADATA* metadata)
{
  if (!metadata)
    return HARDWARE_ERROR_INVALID_PARAMETERS;

  // TODO: Move to utility class
  metadata->name = new char[sizeof(MEDIA_READER_NAME)];
  strcpy(metadata->name, MEDIA_READER_NAME);
  metadata->type = MEDIA_READER_TYPE_CARTRIDGE;
  metadata->port_count = 1;
  metadata->ports = new MEDIA_PORT_METADATA[1];

  metadata->ports[0].type = MEDIA_READER_TYPE_CARTRIDGE;
  metadata->ports[0].system = new char[sizeof(HARDWARE_SYSTEM_SNES)];
  strcpy(metadata->ports[0].system, HARDWARE_SYSTEM_SNES);
  metadata->ports[0].port_id = 0;
  metadata->ports[0].port_uri = new char[sizeof(MEDIA_READER_PORT_URI)];
  strcpy(metadata->ports[0].port_uri, MEDIA_READER_PORT_URI);

  return HARDWARE_NO_ERROR;
}

void MediaFreeMetadata(MEDIA_READER_METADATA* metadata)
{
  if (!metadata)
    return;

  // TODO: Move to utility class
  delete[] metadata->name;
  for (unsigned int iPtr = 0; iPtr < metadata->port_count; iPtr++)
  {
    delete[] metadata->ports[iPtr].system;
    delete[] metadata->ports[iPtr].port_uri;
  }
  delete[] metadata->ports;
}

HARDWARE_ERROR FileOpen(const char* strFileName, VFS_HANDLE* handle)
{
  if (!strFileName || !handle)
    return HARDWARE_ERROR_INVALID_PARAMETERS;

  CFile* file = new CFile(strFileName);
  if (file->Open())
  {
    *handle = file;
    return HARDWARE_NO_ERROR;
  }

  delete file;
  return HARDWARE_ERROR_FAILED;
}

void FileClose(VFS_HANDLE handle)
{
  CFile* file = static_cast<CFile*>(handle);
  delete file;
}

uint64_t FileRead(VFS_HANDLE handle, void* pBuffer, uint64_t iBufLen)
{
  CFile* file = static_cast<CFile*>(handle);
  if (!file || !pBuffer)
    return 0;

  std::vector<uint8_t> data;
  if (!file->Read(data, iBufLen))
    return 0;

  memcpy(pBuffer, data.data(), data.size());
  return data.size();
}

int64_t FileSeek(VFS_HANDLE handle, int64_t iFilePosition, int iWhence)
{
  CFile* file = static_cast<CFile*>(handle);
  if (!file)
    return -1;

  return file->Seek(iFilePosition, iWhence);
}

int64_t FileGetPosition(VFS_HANDLE handle)
{
  CFile* file = static_cast<CFile*>(handle);
  if (!file)
    return -1;

  return file->GetPosition();
}

int64_t FileGetLength(VFS_HANDLE handle)
{
  CFile* file = static_cast<CFile*>(handle);
  if (!file)
    return -1;

  return file->GetLength();
}

bool FileExists(const char* strFileName)
{
  if (!strFileName)
    return false;

  CFile file(strFileName);
  return file.Exists();
}

bool FileStat(const char* strFileName, struct VFS_FILE_STATUS* buffer)
{
  if (!strFileName || !buffer)
    return false;

  CFile file(strFileName);
  return file.Stat(*reinterpret_cast<FILE_STATUS*>(buffer));
}

HARDWARE_ERROR FileGetDirectory(VFS_FILE_ITEM_LIST* directory, const char* strPath)
{
  if (!directory || !strPath)
    return HARDWARE_ERROR_INVALID_PARAMETERS;

  CDirectory dir;
  FILE_LIST items;
  if (dir.GetDirectory(strPath, items))
  {
    // TODO: Move to utility function
    directory->item_count = items.size();
    directory->items = NULL;

    if (directory->item_count)
    {
      directory->items = new VFS_FILE_ITEM[directory->item_count];

      for (unsigned int i = 0; i < directory->item_count; i++)
      {
        directory->items[i].type = VFS_FILE_ITEM_TYPE_GAME;
        directory->items[i].property_count = 0;
        directory->items[i].properties = NULL;
      }
    }

    return HARDWARE_NO_ERROR;
  }
  return HARDWARE_ERROR_FAILED;
}

void FreeFileList(VFS_FILE_ITEM_LIST* items)
{
  if (!items)
    return;

  // TODO: Move to utility function
  for (unsigned int iPtr = 0; iPtr < items->item_count; iPtr++)
    delete[] items->items[iPtr].properties;

  delete[] items->items;
}

} // extern "C"
