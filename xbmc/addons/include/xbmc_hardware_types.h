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
#ifndef XBMC_HARDWARE_TYPES_H_
#define XBMC_HARDWARE_TYPES_H_

/* Current hardware API version */
#define HARDWARE_API_VERSION                "1.0.0"

/* Min. hardware API version */
#define HARDWARE_MIN_API_VERSION            "1.0.0"

#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>

#ifdef TARGET_WINDOWS
  #include <windows.h>
#else
  #include <sys/types.h>
  #ifndef __cdecl
    #define __cdecl
  #endif
  #ifndef __declspec
    #define __declspec(X)
  #endif
#endif

#undef ATTRIBUTE_PACKED
#undef PRAGMA_PACK_BEGIN
#undef PRAGMA_PACK_END

#if defined(__GNUC__)
  #if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 95)
    #define ATTRIBUTE_PACKED __attribute__ ((packed))
    #define PRAGMA_PACK 0
  #endif
#endif

#if !defined(ATTRIBUTE_PACKED)
  #define ATTRIBUTE_PACKED
  #define PRAGMA_PACK 1
#endif

// Checksum value when checksum cannot be computed or has not been computed yet
#define HARDWARE_CHECKSUM_UNKNOWN        0x00000000

// System names are defined using strings for flexibility
#define HARDWARE_SYSTEM_UNKNOWN          ""
#define HARDWARE_SYSTEM_ARCADE           "Arcade"
#define HARDWARE_SYSTEM_NES              "NES"
#define HARDWARE_SYSTEM_SNES             "SNES"
#define HARDWARE_SYSTEM_NINTENDO_64      "Nintendo 64"
#define HARDWARE_SYSTEM_GAMEBOY          "GameBoy"
#define HARDWARE_SYSTEM_GAMEBOY_COLOR    "GameBoy Color"
#define HARDWARE_SYSTEM_GAMEBOY_ADVANCE  "GameBoy Advance"

#ifdef __cplusplus
extern "C"
{
#endif

  /*! Hardware add-on error codes */
  typedef enum HARDWARE_ERROR
  {
    HARDWARE_NO_ERROR                      =  0, // no error occurred
    HARDWARE_ERROR_UNKNOWN                 = -1, // an unknown error occurred
    HARDWARE_ERROR_FAILED                  = -2, // the command failed
    HARDWARE_ERROR_INVALID_PARAMETERS      = -3, // the parameters of the method are invalid for this operation
    HARDWARE_ERROR_NOT_IMPLEMENTED         = -4, // the method that the frontend called is not implemented
    HARDWARE_ERROR_NOT_CONNECTED           = -5, // the hardware is not connected
    HARDWARE_ERROR_CONNECTION_FAILED       = -6, // the hardware is connected, but command was interrupted
  } HARDWARE_ERROR;

  enum HARDWARE_FILE_TYPE
  {
    HARDWARE_FILE_TYPE_DIRECTORY    = S_IFDIR,  // Directory
    HARDWARE_FILE_TYPE_CHAR_DEVICE  = S_IFCHR,  // Character device
    HARDWARE_FILE_TYPE_BLOCK_DEVICE = S_IFBLK,  // Block device
    HARDWARE_FILE_TYPE_FILE         = S_IFREG,  // Regular file
    HARDWARE_FILE_TYPE_FIFO         = S_IFIFO,  // FIFO
    HARDWARE_FILE_TYPE_SYMLINK      = S_IFLNK,  // Symbolic link
    HARDWARE_FILE_TYPE_SOCKET       = S_IFSOCK, // Socket
  };

  struct HARDWARE_FILE_STATUS
  {
    HARDWARE_FILE_TYPE type;              // File type
    int64_t            size;              // Total size, in bytes
    int64_t            block_size;        // Blocksize for file system I/O
    time_t             access_time;       // Time of last access
    time_t             modification_time; // Time of last modification
    uint32_t           device_id;         // ID of device containing the file
  };

  typedef void* HARDWARE_HANDLE;

  typedef struct HARDWARE_ADDON_FILEITEM
  {
    char*                    name;
  } ATTRIBUTE_PACKED HARDWARE_ADDON_FILEITEM;

  typedef struct HARDWARE_ADDON_FILELIST
  {
    unsigned int             item_count;
    HARDWARE_ADDON_FILEITEM* items;
  } ATTRIBUTE_PACKED HARDWARE_ADDON_FILELIST;

  typedef enum MEDIA_READER_TYPE
  {
    MEDIA_READER_TYPE_CARTRIDGE,
    MEDIA_READER_TYPE_DISK,
    MEDIA_READER_TYPE_MULTIPLE,
  } MEDIA_READER_TYPE;

  typedef struct MEDIA_ROM_METADATA
  {
    // ROM
    char*                uri;           // Hardware API filename, beginning with Media/ROMs/
    uint32_t             checksum;      // 32-bit CRC, or HARDWARE_CHECKSUM_UNKNOWN
    uint32_t             size;          // ROM size in bytes

    // Mapper
    char*                mapper;        // Mapper hardware (NULL if unknown/not present)

    // SRAM
    char*                sram_uri;      // SRAM filename beginning with Media/Saves/, or NULL if no sram
    uint32_t             sram_checksum; // 32-bit CRC, or HARDWARE_CHECKSUM_UNKNOWN
    uint32_t             sram_size;     // SRAM size in bytes, or 0 if no sram

    // TODO: Key/value pairs for game metadata
  } ATTRIBUTE_PACKED MEDIA_ROM_METADATA;

  typedef struct MEDIA_PORT_METADATA
  {
    unsigned int         port_number;   // Identifies the port
    MEDIA_READER_TYPE    type;          // Media type of the port
    char*                system;        // System name (see HARDWARE_SYSTEM #defines)
    MEDIA_ROM_METADATA*  rom;           // ROM metadata, or NULL if media is ejected
  } ATTRIBUTE_PACKED MEDIA_PORT_METADATA;

  typedef struct MEDIA_READER_METADATA
  {
    char*                name;          // Name of the media reader hardware
    MEDIA_READER_TYPE    type;          // The main port's type, or MULTIPLE if ports have multiple types
    unsigned int         port_count;
    MEDIA_PORT_METADATA* ports;
  } ATTRIBUTE_PACKED MEDIA_READER_METADATA;

  /*! Properties passed to the ADDON_Create() method of a hardware add-on */
  typedef struct hardware_addon_properties
  {
    // TODO
  } hardware_addon_properties;

  /*!
   * Structure to transfer the methods from xbmc_hardware_dll.h to XBMC.
   */
  typedef struct HardwareAddon
  {
    const char*    (__cdecl* GetHardwareAPIVersion)(void);
    const char*    (__cdecl* GetMinHardwareAPIVersion)(void);
    const char*    (__cdecl* GetHardwareName)(void);

    /// @name Media reader operations
    ///{
    HARDWARE_ERROR (__cdecl* MediaGetMetadata)(struct MEDIA_READER_METADATA* metadata);
    HARDWARE_ERROR (__cdecl* MediaSetPortMetadata)(const struct MEDIA_PORT_METADATA* metadata);
    void           (__cdecl* MediaFreeMetadata)(struct MEDIA_READER_METADATA* metadata);
    HARDWARE_ERROR (__cdecl* MediaEject)(unsigned int port);
    ///}

    /// @name VFS operations
    ///{
    HARDWARE_ERROR (__cdecl* FileOpen)(const char*, HARDWARE_HANDLE*);
    void           (__cdecl* FileClose)(HARDWARE_HANDLE);
    uint64_t       (__cdecl* FileRead)(HARDWARE_HANDLE, void*, uint64_t);
    bool           (__cdecl* FileExists)(const char*);
    bool           (__cdecl* FileStat)(const char*, struct HARDWARE_FILE_STATUS*);
    int64_t        (__cdecl* FileSeek)(HARDWARE_HANDLE, int64_t, int);
    int64_t        (__cdecl* FileGetPosition)(HARDWARE_HANDLE);
    int64_t        (__cdecl* FileGetLength)(HARDWARE_HANDLE);
    HARDWARE_ERROR (__cdecl* FileGetDirectory)(struct HARDWARE_ADDON_FILELIST*, const char*);
    void           (__cdecl* FreeFileList)(struct HARDWARE_ADDON_FILELIST*);
    ///}
  } HardwareAddon;

#ifdef __cplusplus
};
#endif

#endif // XBMC_HARDWARE_TYPES_H_
