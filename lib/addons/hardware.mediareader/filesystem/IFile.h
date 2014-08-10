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
#pragma once

#include <fcntl.h>
#include <limits>
#include <stdint.h>
#include <string>
#include <sys/stat.h>
#include <vector>

// File flags from File.h of XBMC project

// Indicate that caller can handle truncated reads, where function returns before entire buffer has been filled
#define READ_TRUNCATED     0x01
// Indicate that that caller support read in the minimum defined chunk size, this disables internal cache then
#define READ_CHUNKED       0x02
// Use cache to access this file
#define READ_CACHED        0x04
// Open without caching. regardless to file type.
#define READ_NO_CACHE      0x08
// Calcuate bitrate for file while reading
#define READ_BITRATE       0x10
// Indicate the caller will seek between multiple streams in the file frequently
#define READ_MULTI_STREAM  0x20

namespace MEDIA_READER
{

// Must be kept in sync with HARDWARE_FILE_TYPE in xbmc_hardware_types.h
enum FILE_TYPE
{
  FILE_TYPE_DIRECTORY    = S_IFDIR,  // Directory
  FILE_TYPE_CHAR_DEVICE  = S_IFCHR,  // Character device
  FILE_TYPE_BLOCK_DEVICE = S_IFBLK,  // Block device
  FILE_TYPE_FILE         = S_IFREG,  // Regular file
  FILE_TYPE_FIFO         = S_IFIFO,  // FIFO
  FILE_TYPE_SYMLINK      = S_IFLNK,  // Symbolic link
  FILE_TYPE_SOCKET       = S_IFSOCK, // Socket
};

// Must be kept in sync with HARDWARE_FILE_STATUS in xbmc_hardware_types.h
struct FILE_STATUS
{
  FILE_TYPE type;              // File type
  int64_t   size;              // Total size, in bytes
  int64_t   block_size;        // Blocksize for file system I/O
  time_t    access_time;       // Time of last access
  time_t    modification_time; // Time of last modification
  uint32_t  device_id;         // ID of device containing the file
};

/*!
 * File abstraction. Methods with a default implementation (not pure virtual)
 * have fall-back behavior defined in File.cpp.
 */
class IFile
{
public:
  IFile(const std::string &strPath) : m_strPath(strPath) { }
  virtual ~IFile(void) { }

  /*!
   * Get the path that this object was constructed with
   */
  const std::string& Path(void) const { return m_strPath; }

  /*!
   * Test if file exists
   */
  virtual bool Exists(void) = 0;

  /*!
   * Query properties of the file
   */
  virtual bool Stat(FILE_STATUS& status) { return false; }

  /*!
   * Rename the file
   */
  virtual bool Move(const std::string &strPathNew, bool bOverWrite = false) { return false; }

  /*!
   * Delete the file
   */
  virtual bool Delete() = 0;

  /*!
   * Returns true if the file is hidden (implementation-specific)
   */
  virtual bool IsHidden(void) const = 0;

  /*!
   * Set hidden status, if the implementation supports it
   */
  virtual bool SetHidden(bool bHidden) = 0;

  /*!
   * Open the file. If the file is already open with the specified flags, this
   * returns true and sets the position to 0.
   *
   * After successful open, the functions declared between Open() and Close()
   * below may be called.
   */
  virtual bool Open(unsigned int flags = 0) = 0;

  /*!
   * Open the file for writing
   */
  virtual bool OpenForWrite(bool bOverWrite = false) = 0;

  /*!
   * Returns true if the file is open for reading or writing
   */
  virtual bool IsOpen(void) const = 0;

  /*!
   * Read at most byte_count from the file and append to a buffer
   * \param data the buffer to read into
   * \param byte_count the maximum amount of bytes that will be read
   * \return the number of bytes added to the buffer after the read attempt
   */
  virtual uint64_t Read(std::vector<uint8_t>& data, uint64_t byte_count = std::numeric_limits<uint64_t>::max()) = 0;

  /*!
   * Read data from the file until a newline is encountered
   * \param strLine the data read, excluding the newline, or empty if false is returned
   * \return true if the read succeeded, false if the read failed or no bytes were read
   */
  virtual bool ReadLine(std::string &strLine) { return false; }

  /*!
   * Write data to the file
   * \return the number of bytes written
   */
  virtual uint64_t Write(const std::vector<uint8_t>& data) = 0;

  /*!
   * Flush any remaining buffers
   */
  virtual void Flush(void) = 0;

  /*!
   * Seek to a position in the file relative to iWhence
   * \return the new position, or -1 if unknown
   */
  virtual int64_t Seek(int64_t filePosition, int iWhence = SEEK_SET) = 0;

  /*!
   * Truncate the file to a given size
   * \return true if the file is the specified size after the truncation
   */
  virtual bool Truncate(uint64_t size) { return false; }

  /*!
   * Get the current position in the file, or -1 if file is not open or position
   * is unknown
   */
  virtual int64_t GetPosition(void) = 0;

  /*!
   * Get the file's size, or -1 if file is not open or size is unknown
   */
  virtual int64_t GetLength(void) = 0;

  /*!
   * Get a MIME type representing this file
   */
  virtual std::string GetMIMEType(void) { return ""; }

  /*!
   * Get the character set of the file's data
   */
  virtual std::string GetContentCharset(void) { return ""; }

  /*
   * Close the file
   */
  virtual void Close(void) = 0;

private:
  const std::string m_strPath;
};

}
