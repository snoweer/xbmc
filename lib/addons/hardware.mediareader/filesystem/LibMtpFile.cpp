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

#include "LibMtpFile.h"

//#include <libmtp.h>

namespace MEDIA_READER
{

CLibMtpFile::CLibMtpFile(const std::string& strPath)
 : IFile(strPath)
{
}

bool CLibMtpFile::Exists(void)
{
  // TODO
  return false;
}

bool CLibMtpFile::Stat(FILE_STATUS& status)
{
  // TODO
  return false;
}

bool CLibMtpFile::Move(const std::string &strPathNew, bool bOverWrite /* = false */)
{
  // TODO
  return false;
}

bool CLibMtpFile::Delete()
{
  // TODO
  return false;
}

bool CLibMtpFile::IsHidden(void) const
{
  // TODO
  return false;
}

bool CLibMtpFile::SetHidden(bool bHidden)
{
  // TODO
  return false;
}

bool CLibMtpFile::Open(unsigned int flags)
{
  // TODO
  return false;
}

bool CLibMtpFile::OpenForWrite(bool bOverWrite /* = false */)
{
  // TODO
  return false;
}

bool CLibMtpFile::IsOpen(void) const
{
  // TODO
  return false;
}

uint64_t CLibMtpFile::Read(std::vector<uint8_t>& data, uint64_t byte_count /* = std::numeric_limits<uint64_t>::max() */)
{
  // TODO
  return 0;
}

uint64_t CLibMtpFile::Write(const std::vector<uint8_t>& data)
{
  // TODO
  return 0;
}

void CLibMtpFile::Flush(void)
{
  // TODO
}

int64_t CLibMtpFile::Seek(int64_t filePosition, int iWhence /* = SEEK_SET */)
{
  // TODO
  return false;
}

int64_t CLibMtpFile::GetPosition(void)
{
  // TODO
  return -1;
}

int64_t CLibMtpFile::GetLength(void)
{
  // TODO
  return -1;
}

void CLibMtpFile::Close(void)
{
  // TODO
}

}
