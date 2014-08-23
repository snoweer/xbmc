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

// set the fanart_image or thumb to VFS_FILE_ITEM_LOGO to use the add-on's logo
#define VFS_FILE_ITEM_LOGO               "[logo]"

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

  /*!
   * Hardware types
   */
  ///{
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
  ///}

  /*!
   * VFS types
   */
  ///{
  typedef enum VFS_FILE_TYPE
  {
    VFS_FILE_TYPE_DIRECTORY    = 0040000, // Directory (S_IFDIR)
    VFS_FILE_TYPE_CHAR_DEVICE  = 0020000, // Character device (S_IFCHR)
    VFS_FILE_TYPE_BLOCK_DEVICE = 0060000, // Block device (S_IFBLK)
    VFS_FILE_TYPE_FILE         = 0100000, // Regular file (S_IFREG)
    VFS_FILE_TYPE_FIFO         = 0010000, // FIFO (S_IFIFO)
    VFS_FILE_TYPE_SYMLINK      = 0120000, // Symbolic link (S_IFLNK)
    VFS_FILE_TYPE_SOCKET       = 0140000, // Socket (S_IFSOCK)
  } VFS_FILE_TYPE;

  typedef struct VFS_FILE_STATUS
  {
    VFS_FILE_TYPE type;              // File type
    int64_t       size;              // Total size, in bytes
    int64_t       block_size;        // Blocksize for file system I/O
    time_t        access_time;       // Time of last access
    time_t        modification_time; // Time of last modification
    uint32_t      device_id;         // ID of device containing the file
    uint32_t      checksum_crc;      // CRC checksum of file (or 0x00000000 if unknown)
  } ATTRIBUTE_PACKED VFS_FILE_STATUS;

  typedef enum
  {
    VFS_FILE_ITEM_PROPERTY_TYPE_STRING,
    VFS_FILE_ITEM_PROPERTY_TYPE_INT,
    VFS_FILE_ITEM_PROPERTY_TYPE_DOUBLE
  } VFS_FILE_ITEM_PROPERTY_TYPE;

  typedef struct VFS_FILE_ITEM_PROPERTY
  {
    char*                       key;
    VFS_FILE_ITEM_PROPERTY_TYPE type;
    union
    {
      int    iValue;
      char*  strValue;
      double fValue;
    };
  } ATTRIBUTE_PACKED VFS_FILE_ITEM_PROPERTY;

  typedef enum VFS_FILE_ITEM_TYPE
  {
    VFS_FILE_ITEM_TYPE_ARTIST,
    VFS_FILE_ITEM_TYPE_ALBUM,
    VFS_FILE_ITEM_TYPE_SONG,
    VFS_FILE_ITEM_TYPE_PLAYLIST,
    VFS_FILE_ITEM_TYPE_VIDEO,
    VFS_FILE_ITEM_TYPE_GENRE,
    VFS_FILE_ITEM_TYPE_YEAR,
    VFS_FILE_ITEM_TYPE_GAME,

    VFS_FILE_ITEM_TYPE_DIRECTORY,
    VFS_FILE_ITEM_TYPE_FILE
  } VFS_FILE_ITEM_TYPE;

  /*!
   * Representation of a file or directory. Types are described below: (TODO actually describe them)
   *
   * VFS_FILE_ITEM_TYPE_ARTIST:
   *   required string path
   *   required string name
   *   optional string genres
   *   optional string biography
   *   optional string styles
   *   optional string moods
   *   optional string born
   *   optional string formed
   *   optional string died
   *   optional string disbanded
   *   optional string years_active
   *   optional string instruments
   *   optional string thumb
   *   optional string fanart_image
   *
   * VFS_FILE_ITEM_TYPE_ALBUM:
   *   required string path
   *   required string name
   *   optional int    year
   *   optional string artists
   *   optional string genres
   *   optional int    rating
   *   optional string review
   *   optional string styles
   *   optional string moods
   *   optional string themes
   *   optional string label
   *   optional string type
   *   optional int    compilation
   *   optional int    times_played
   *   optional string thumb
   *   optional string fanart_image
   *
   * VFS_FILE_ITEM_TYPE_SONG:
   *   required string path
   *   required string name
   *   optional int    track
   *   optional int    duration
   *   optional int    rating
   *   optional string artists
   *   optional int    year
   *   optional string album
   *   optional string album_artists
   *   optional string thumb
   *   optional string fanart_image
   *   optional string provider_icon    (relative to the add-on's path)
   *
   * VFS_FILE_ITEM_TYPE_PLAYLIST:
   *   required string path
   *   required string name
   *   optional string thumb
   *   optional string fanart_image
   *
   * VFS_FILE_ITEM_TYPE_GAME:
   *   required string path
   *   required string name
   *   optional int    year
   *   optional string thumb
   *   optional string fanart_image
   *   optional string provider_icon    (relative to the add-on's path)
   *
   * VFS_FILE_ITEM_TYPE_DIRECTORY:
   *   required string path
   *   required string name
   *   optional string thumb
   *   optional string fanart_image
   *
   * VFS_FILE_ITEM_TYPE_FILE:
   *   required string path
   *   required string name
   *   optional string thumb
   *   optional string fanart_image
   *
   * Properties that are found and that are not listed in this list will be
   * added as standard properties to fileitems in XBMC.
   *
   * The following texts will be replaced by XBMC when used as file name:
   *   [logo]   add-on icon
   */
  typedef struct VFS_FILE_ITEM
  {
    VFS_FILE_ITEM_TYPE      type;
    unsigned int            property_count;
    VFS_FILE_ITEM_PROPERTY* properties;
  } ATTRIBUTE_PACKED VFS_FILE_ITEM;

  typedef struct VFS_FILE_ITEM_LIST
  {
    unsigned int            item_count;
    VFS_FILE_ITEM*          items;
  } ATTRIBUTE_PACKED VFS_FILE_ITEM_LIST;

  typedef void* VFS_HANDLE;
  ///}

  /*!
   * Media reader types
   */
  ///{
  typedef enum MEDIA_READER_TYPE
  {
    MEDIA_READER_TYPE_CARTRIDGE,
    MEDIA_READER_TYPE_DISK,
    MEDIA_READER_TYPE_MULTIPLE,
  } MEDIA_READER_TYPE;

  /*!
   * Port metadata is used to discover the available ROMs and SRAM data. Pass
   * port_uri to FileGetDirectory() to access the ROM/SRAM provided by this port:
   *
   *   - If media is ejected, FileGetDirectory() will return true but yield no
   *     files.
   *
   *   - If media has no SRAM, FileGetDirectory() will provide a single file item
   *     (the ROM, possibly with additional file item metadata).
   *
   *   - If SRAM is present, FileGetDirectory() will return two files, one of
   *     type VFS_FILE_ITEM_TYPE_GAME (the ROM) and one of type
   *     VFS_FILE_ITEM_TYPE_FILE (the SRAM).
   *
   * The "path" property of the discovered file items can be passed to FileStat()
   * for file size/checksum and FileRead() for ROM/SRAM data.
   */
  typedef struct MEDIA_PORT_METADATA
  {
    MEDIA_READER_TYPE    type;          // Media type of the port
    char*                system;        // System type of the port (see HARDWARE_SYSTEM defines)
    unsigned int         port_id;       // Identifies the port
    char*                port_uri;      // Pass to FileGetDirectory() for ROM/SRAM access
  } ATTRIBUTE_PACKED MEDIA_PORT_METADATA;

  typedef struct MEDIA_READER_METADATA
  {
    char*                name;          // Name of the media reader hardware
    MEDIA_READER_TYPE    type;          // The main port's type, or MULTIPLE if ports have different types
    unsigned int         port_count;
    MEDIA_PORT_METADATA* ports;
  } ATTRIBUTE_PACKED MEDIA_READER_METADATA;
  ///}

  /*!
   * Properties passed to the ADDON_Create() method of a hardware add-on
   */
  typedef struct HARDWARE_ADDON_PROPERTIES
  {
    // TODO
  } ATTRIBUTE_PACKED HARDWARE_ADDON_PROPERTIES;

  /*!
   * Structure to transfer the methods from xbmc_hardware_dll.h to XBMC.
   */
  typedef struct HardwareAddon
  {
    const char*    (__cdecl* GetHardwareAPIVersion)(void);
    const char*    (__cdecl* GetMinHardwareAPIVersion)(void);

    /// @name Media reader operations
    ///{
    HARDWARE_ERROR (__cdecl* MediaGetMetadata)(struct MEDIA_READER_METADATA* metadata);
    void           (__cdecl* MediaFreeMetadata)(struct MEDIA_READER_METADATA* metadata);
    HARDWARE_ERROR (__cdecl* MediaEject)(unsigned int port);
    ///}

    /// @name VFS operations
    ///{
    HARDWARE_ERROR (__cdecl* FileOpen)(const char*, VFS_HANDLE*);
    void           (__cdecl* FileClose)(VFS_HANDLE);
    uint64_t       (__cdecl* FileRead)(VFS_HANDLE, void*, uint64_t);
    int64_t        (__cdecl* FileSeek)(VFS_HANDLE, int64_t, int);
    int64_t        (__cdecl* FileGetPosition)(VFS_HANDLE);
    int64_t        (__cdecl* FileGetLength)(VFS_HANDLE);
    bool           (__cdecl* FileExists)(const char*);
    bool           (__cdecl* FileStat)(const char*, struct FILE_STATUS*);
    HARDWARE_ERROR (__cdecl* FileGetDirectory)(struct VFS_FILE_ITEM_LIST*, const char*);
    void           (__cdecl* FreeFileList)(struct VFS_FILE_ITEM_LIST*);
    ///}
  } HardwareAddon;

#ifdef __cplusplus
};
#endif

#endif // XBMC_HARDWARE_TYPES_H_
