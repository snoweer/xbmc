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
#ifndef XBMC_HARDWARE_DLL_H_
#define XBMC_HARDWARE_DLL_H_

#define HARDWARE_ADDON_MEDIA_READER // Remove this line in production

#include "xbmc_hardware_types.h"

#ifdef __cplusplus
extern "C"
{
#endif
  /*!
   * Get the Hardware API version that was used to compile this add-on.
   * Used to check if this add-on is compatible with XBMC.
   * @return The XBMC_PVR_API_VERSION that was used to compile this add-on.
   * @remarks Valid implementation required.
   */
  const char* GetHardwareAPIVersion(void);

  /*!
   * Get the minimum Hardware API version that is compatible with this API.
   * Used to check if this add-on is compatible with XBMC.
   * @return The XBMC_PVR_MIN_API_VERSION that was used to compile this add-on.
   * @remarks Valid implementation required.
   */
  const char* GetMinHardwareAPIVersion(void);

  /*!
   * Get the name of the hardware that will be displayed in the UI.
   * @return The name of the hardware that this add-on interfaces with.
   * @remarks Valid implementation required.
   */
  const char* GetHardwareName(void);

  /*!
   * The media reader sub-interface abstracts cartridge readers and CD/DVD drives.
   * Multiple cartridges per reader/disks per drive are allowed (ports). The media
   * reader interface is based on a simple filesystem that provides media images,
   * cartridge SRAM, and their metadata:
   *
   *  Media/                          required   Namespace folder
   *    |
   *    |-ROMs/                       required   Interface with the ROM of attached cartridges
   *    |   |-<ExampleGameName>.gb    --------   All files in this directory are cartridge ROMs
   *    |   |-<ExampleDiskImage>.iso  --------      or disk images of any valid name.
   *    |   |-...
   *    |
   *    |-Saves/                      required   Interface with the SRAM of attached cartridges
   *    |   |-<ExampleGameName>.sav   --------   All files in this directory are cartridge SRAMs
   *    |   |-...                                   of any valid name
   *    |
   *    |-metadata/
   *    |   |-metadata.yaml           required   Metadata of ALL connected media in YAML format
   *    |   |-metadata.json           optional   Metadata of ALL connected media in JSON format
   *    |   |-metadata.proto          optional   Metadata of ALL connected media in protobuffers format
   *    |   |-metadata.xml            optional   Metadata of ALL connected media in XML format
   *    |
   *    |-settings/
   *        |-settings.yaml           required   Current settings (using XMLRPC data types) in YAML format
   *        |-settings.json           optional   Current settings in JSON format
   *        |-settings.proto          optional   Current settings in protobuffers format
   *        |-settings.xml            optional   Current settings in XML format
   *
   * @note #define HARDWARE_ADDON_MEDIA_READER before including xbmc_hardware_dll.h
   *       if the hardware has a media reader and add has_media_reader="true"
   *       to the xbmc.hardware extension point in addon.xml
   */
  ///{
#ifdef HARDWARE_ADDON_MEDIA_READER

  /*!
   * Get the media reader metadata from a metadata file
   * @param The handle for the metadata, must be freed by MediaFreeMetadata()
   * @remarks Valid implementation required if HARDWARE_ADDON_MEDIA_READER is defined.
   */
  HARDWARE_ERROR MediaGetMetadata(struct MEDIA_READER_METADATA* metadata);

  /*!
   * Override metadata auto-detected by the metadata reader. This can be used
   * if the reader reports incorrect sizes or mappers. The port_number field of
   * identifies the media reader's port, so this is typically called after
   * changing the result of MediaGetMetadata(). If a field contains an unchanged
   * or invalid value, it will be ignored.
   * @param metadata The handle for the metadata. NOTE this is of type
   *                 MEDIA_PORT_METADATA*, not MEDIA_READER_METADATA* as
   *                 returned by MediaGetMetadata()
   * @remarks Optional, in which case HARDWARE_ERROR_NOT_IMPLEMENTED should be returned.
   */
  HARDWARE_ERROR MediaSetPortMetadata(const struct MEDIA_PORT_METADATA* metadata);

  /*!
   * Free a metadata struct allocated by the add-on.
   * @param metadata The metadata struct to free.
   * @remarks Valid implementation required if HARDWARE_ADDON_MEDIA_READER is defined.
   */
  void MediaFreeMetadata(struct MEDIA_READER_METADATA* metadata);

  /*!
   * Eject the specified port number
   * @param port The port number, as assigned by MediaGetMetadata()
   * @remarks Optional, in which case HARDWARE_ERROR_NOT_IMPLEMENTED should be returned.
   */
  HARDWARE_ERROR MediaEject(unsigned int port);

#endif // HARDWARE_ADDON_MEDIA_READER
  ///}

  /*!
   * VFS functions to support interfaces that transfer data
   */
  ///{
#if defined(HARDWARE_ADDON_MEDIA_READER)
  /*!
   * Open a file to be read by FileRead
   * @param strFileName The file to open
   * @param handle The handle for this file, set by the add-on, that is passed to file operations.
   * @return HARDWARE_NO_ERROR when the file has been opened successfully.
   */
  HARDWARE_ERROR FileOpen(const char* fileName, HARDWARE_HANDLE* handle);

  /*!
   * Close a file opened by FileOpen, and invalidate the handle
   * @param handle The handle to close
   */
  void FileClose(HARDWARE_HANDLE handle);

  /*!
   * Read raw bytes from a file that has been opened with FileOpen
   * @param handle The handle assigned by FileOpen
   * @param pBuffer The buffer to write the data to
   * @param iBufLen The maximum size to read
   * @return The number of bytes that were added to the buffer
   */
  uint64_t FileRead(HARDWARE_HANDLE handle, void* pBuffer, uint64_t iBufLen);

  /*!
   * Check whether the add-on can open a file
   * @param strFileName The file to check
   * @return true when the file can be opened, false otherwise
   */
  bool FileExists(const char* strFileName);

  /*!
   * Perform a stat operation on the specified file
   * @param strFileName The file to stat
   * @param buffer The zero-initialized stat structure
   * @return true is returned on success, false is returned on error
   */
  bool FileStat(const char* strFileName, struct HARDWARE_FILE_STATUS* buffer);

  /*!
   * Seek to the given position
   * @param handle The handle of the file to seek
   * @param iFilePosition The position to seek to
   * @param iWhence Seek type. See stdio.h for possible value
   * @return The new file position, or -1 if unknown
   */
  int64_t FileSeek(HARDWARE_HANDLE handle, int64_t iFilePosition, int iWhence);

  /*!
   * Get the current position
   * @param handle The handle of the file to get the position for
   * @return The position. -1 when eof or not found
   */
  int64_t FileGetPosition(HARDWARE_HANDLE handle);

  /*!
   * Total number of bytes in an open file
   * @param handle The handle of the file to get the length for
   * @return The total length or -1 when not found
   */
  int64_t FileGetLength(HARDWARE_HANDLE handle);

  /*!
   * Get the files in a directory
   * @param directory The directory contents. Must be freed by calling FreeFileList when done
   * @param strPath The directory to get
   * @return HARDWARE_NO_ERROR if the files were assigned successfully
   */
  HARDWARE_ERROR FileGetDirectory(struct HARDWARE_ADDON_FILELIST* directory, const char* strPath);

  /*!
   * Free a file list allocated by the add-on
   * @param items The file list to free
   */
  void FreeFileList(struct HARDWARE_ADDON_FILELIST* items);

#endif // HARDWARE_ADDON_MEDIA_READER
  ///}


  /*!
   * Called by XBMC to assign the function pointers of this add-on to pClient.
   * Note that get_addon() is defined here, so it will be compiled into all
   * hardware add-ons.
   * @param pClient The struct to assign the function pointers to.
   */
  void __declspec(dllexport) get_addon(struct HardwareAddon* pClient)
  {
    pClient->GetHardwareAPIVersion    = GetHardwareAPIVersion;
    pClient->GetMinHardwareAPIVersion = GetMinHardwareAPIVersion;
    pClient->GetHardwareName          = GetHardwareName;
    pClient->MediaGetMetadata         = MediaGetMetadata;
    pClient->MediaSetPortMetadata     = MediaSetPortMetadata;
    pClient->MediaFreeMetadata        = MediaFreeMetadata;
    pClient->MediaEject               = MediaEject;
    pClient->FileOpen                 = FileOpen;
    pClient->FileClose                = FileClose;
    pClient->FileRead                 = FileRead;
    pClient->FileExists               = FileExists;
    pClient->FileStat                 = FileStat;
    pClient->FileSeek                 = FileSeek;
    pClient->FileGetPosition          = FileGetPosition;
    pClient->FileGetLength            = FileGetLength;
    pClient->FileGetDirectory         = FileGetDirectory;
    pClient->FreeFileList             = FreeFileList;
  };

#ifdef __cplusplus
};
#endif

#endif // XBMC_HARDWARE_DLL_H_
