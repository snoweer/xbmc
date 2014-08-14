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

    hardware_addon_properties* hwAddonProps = static_cast<hardware_addon_properties*>(props);
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
  DllSetting dllSetting(DllSetting::CHECK, SETTING_ENABLE_SAVE_WRITING, SETTING_ENABLE_SAVE_WRITING_DESCR);
  dllSetting.current = 0; // TODO: Get value from settings API
  vSettings.push_back(dllSetting);

  DllUtils::VecToStruct(vSettings, ADDON_SETTINGS);

  *sSettings = *ADDON_SETTINGS;
  return ADDON_SETTINGS_COUNT;
}

ADDON_STATUS ADDON_SetSetting(const char* settingName, const void* settingValue)
{
  if (!settingName || !settingValue)
    return ADDON_STATUS_UNKNOWN;

  if (strcmp(settingName, SETTING_ENABLE_SAVE_WRITING) == 0)
  {
    std::string strValue = static_cast<const char*>(settingValue);
    bool bEnableSaveUpload = (strValue == "true");
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

const char* GetHardwareName(void)
{
  static std::string strHardwareName;
  if (strHardwareName.empty())
  {
    MEDIA_READER_METADATA metadata;
    MediaGetMetadata(&metadata);
    if (metadata.name)
      strHardwareName = metadata.name;
    MediaFreeMetadata(&metadata);
  }
  return strHardwareName.c_str();
}

HARDWARE_ERROR MediaGetMetadata(MEDIA_READER_METADATA* metadata)
{
  if (!metadata)
    return HARDWARE_ERROR_INVALID_PARAMETERS;

  CFile file("Media/metadata.yaml");

  // Pass to YAML parser

  // TODO
  return HARDWARE_ERROR_NOT_IMPLEMENTED;
}

HARDWARE_ERROR MediaSetPortMetadata(const MEDIA_PORT_METADATA* metadata)
{
  if (!metadata)
    return HARDWARE_ERROR_INVALID_PARAMETERS;

  // TODO
  return HARDWARE_ERROR_NOT_IMPLEMENTED;
}

void MediaFreeMetadata(MEDIA_READER_METADATA* metadata)
{
  if (!metadata)
    return;

  // TODO: Move to utility class
  delete[] metadata->name;
  for (unsigned int iPtr = 0; iPtr < metadata->port_count; iPtr++)
  {
    if (metadata->ports[iPtr].rom)
    {
      delete[] metadata->ports[iPtr].rom->uri;
      delete[] metadata->ports[iPtr].rom->mapper;
      delete[] metadata->ports[iPtr].rom->sram_uri;
      delete[] metadata->ports[iPtr].rom;
    }
    delete[] metadata->ports[iPtr].system;
  }
  delete[] metadata->ports;
}

HARDWARE_ERROR FileOpen(const char* strFileName, HARDWARE_HANDLE* handle)
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

void FileClose(HARDWARE_HANDLE handle)
{
  CFile* file = static_cast<CFile*>(handle);
  delete file;
}

uint64_t FileRead(HARDWARE_HANDLE handle, void* pBuffer, uint64_t iBufLen)
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

bool FileExists(const char* strFileName)
{
  if (!strFileName)
    return false;

  CFile file(strFileName);
  return file.Exists();
}

bool FileStat(const char* strFileName, struct HARDWARE_FILE_STATUS* buffer)
{
  if (!strFileName || !buffer)
    return false;

  STATIC_ASSERT(sizeof(HARDWARE_FILE_STATUS) == sizeof(FILE_STATUS));

  CFile file(strFileName);
  return file.Stat(*reinterpret_cast<FILE_STATUS*>(buffer));
}

int64_t FileSeek(HARDWARE_HANDLE handle, int64_t iFilePosition, int iWhence)
{
  CFile* file = static_cast<CFile*>(handle);
  if (!file)
    return -1;

  return file->Seek(iFilePosition, iWhence);
}

int64_t FileGetPosition(HARDWARE_HANDLE handle)
{
  CFile* file = static_cast<CFile*>(handle);
  if (!file)
    return -1;

  return file->GetPosition();
}

int64_t FileGetLength(HARDWARE_HANDLE handle)
{
  CFile* file = static_cast<CFile*>(handle);
  if (!file)
    return -1;

  return file->GetLength();
}

HARDWARE_ERROR FileGetDirectory(HARDWARE_ADDON_FILELIST* directory, const char* strPath)
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
      directory->items = (HARDWARE_ADDON_FILEITEM*)malloc(directory->item_count * sizeof(HARDWARE_ADDON_FILEITEM));

      for (unsigned int i = 0; i < directory->item_count; i++)
        directory->items[i].name = strdup(items[i].strName.c_str());
    }

    return HARDWARE_NO_ERROR;
  }
  return HARDWARE_ERROR_FAILED;
}

void FreeFileList(HARDWARE_ADDON_FILELIST* items)
{
  if (!items)
    return;

  // TODO: Move to utility function
  for (unsigned int iPtr = 0; iPtr < items->item_count; iPtr++)
    free(items->items[iPtr].name);

  free(items->items);
}

} // extern "C"
