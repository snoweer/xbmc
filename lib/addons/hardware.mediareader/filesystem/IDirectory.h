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

#include <set>
#include <stddef.h>
#include <string>
#include <vector>

namespace MEDIA_READER
{

struct FILE_LABEL
{
  std::string strName;
  bool        bHidden;
};

typedef std::vector<FILE_LABEL> FILE_LIST;

/*!
 * \brief Available directory flags
 *
 * The defaults are to allow file directories, no prompting, retrieve file
 * information, hide hidden files, and utilise the directory cache.
 *
 */
enum DIR_FLAG
{
  DEFAULTS      = 0,
  NO_FILE_INFO  = (1 << 0), // Don't read additional file info (stat for example)
  GET_HIDDEN    = (1 << 1), // Get hidden files
  READ_CACHE    = (1 << 2), // Force reading from the directory cache (if available)
};

struct DISK_SPACE
{
  size_t size;
  size_t used;
  size_t free;
};

/*!
 * \brief Interface to the directory on a file system
 */
class IDirectory
{
public:
  virtual ~IDirectory(void);

  /*!
   * \brief Get the contents of a directory (synchronous)
   * \param strPath Directory to read
   * \param items The directory entries retrieved by GetDirectory()
   * \return true on success
   */
  virtual bool GetDirectory(const std::string& strPath,
                            FILE_LIST& items,
                            DIR_FLAG flags = DEFAULTS,
                            const std::set<std::string>& extensionsAllowed = std::set<std::string>()) = 0;

  /*!
  * \brief Create the directory
  * \param strPath Directory to create
  * \return true if directory is created or already exists
  */
  virtual bool Create(const std::string& strPath) { return false; }

  /*!
  * \brief Check if the directory exists
  * \param strPath Directory to check
  * \return true if directory exists
  */
  virtual bool Exists(const std::string& strPath) { return false; }

  /*!
   * Get disk space info for this directory
   * \param strPath The path to get the info for
   * \param space Disk space information struct
   * \return True when fetched successfully
   */
  virtual bool DiskSpace(const std::string& strPath, DISK_SPACE& space) { return false; }

  /*!
  * \brief Removes the directory
  * \param strPath Directory to remove
  * \return true on success
  */
  virtual bool Remove(const std::string& strPath) { return false; }

  /*!
  * \brief Renames the directory
  * \param strPath Directory to rename
  * \param strNewPath Target directory name
  * \return true on success
  */
  virtual bool Rename(const std::string& strPath, const std::string& strNewPath) { return false; }

  /*!
  * \brief Check if the directory can be modified
  * \param strPath Directory to check
  * \param strNewPath Target directory name
  * \return true if the directory can be modified, false if unwritable or unknown
  */
  virtual bool CanWrite(const std::string& strPath) { return false; }
};

}
