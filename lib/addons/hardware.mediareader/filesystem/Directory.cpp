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

#include "Directory.h"
#include "DirectoryFactory.h"

namespace MEDIA_READER
{

bool CDirectory::GetDirectory(const std::string& strPath,
                              FILE_LIST& items,
                              DIR_FLAG flags /* = DEFAULTS */,
                              const std::set<std::string>& extensionsAllowed /* = std::set<std::string>() */)
{
  IDirectory* dir = CDirectoryFactory::CreateDirectory(strPath);
  if (!dir)
    return false;

  bool bSuccess = dir->GetDirectory(strPath, items, flags, extensionsAllowed);
  delete dir;
  return bSuccess;
}

bool CDirectory::Create(const std::string& strPath)
{
  IDirectory* dir = CDirectoryFactory::CreateDirectory(strPath);
  if (!dir)
    return false;

  bool bSuccess = dir->Create(strPath);
  delete dir;
  return bSuccess;
}

bool CDirectory::Exists(const std::string& strPath)
{
  IDirectory* dir = CDirectoryFactory::CreateDirectory(strPath);
  if (!dir)
    return false;

  bool bSuccess = dir->Exists(strPath);
  delete dir;
  return bSuccess;
}

bool CDirectory::Remove(const std::string& strPath)
{
  IDirectory* dir = CDirectoryFactory::CreateDirectory(strPath);
  if (!dir)
    return false;

  bool bSuccess = dir->Remove(strPath);
  delete dir;
  return bSuccess;
}

bool CDirectory::Rename(const std::string& strPath, const std::string &strNewPath)
{
  IDirectory* dir = CDirectoryFactory::CreateDirectory(strPath);
  if (!dir)
    return false;

  bool bSuccess = dir->Rename(strPath, strNewPath);
  delete dir;
  return bSuccess;
}

bool CDirectory::DiskSpace(const std::string& strPath, DISK_SPACE& space)
{
  IDirectory* dir = CDirectoryFactory::CreateDirectory(strPath);
  if (!dir)
    return false;

  bool bSuccess = dir->DiskSpace(strPath, space);
  delete dir;
  return bSuccess;
}

bool CDirectory::CanWrite(const std::string& strPath)
{
  IDirectory* dir = CDirectoryFactory::CreateDirectory(strPath);
  if (!dir)
    return false;

  /* TODO: Move to native hd file
  struct stat64 ds;
  if (stat64(strPath.c_str(), &ds) == 0)
  {
    if (S_ISDIR(ds.st_mode))
    {
      if (access(strPath.c_str(), R_OK | W_OK | X_OK) == 0)
        return true;
      return false; // Can't access directory
    }
    return false; // Not a directory
  }
  return false;
  */

  bool bSuccess = dir->CanWrite(strPath);
  delete dir;
  return bSuccess;
}

}
