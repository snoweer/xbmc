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

#include "File.h"
#include "FileFactory.h"

#include <assert.h>
#include <string.h>

#define DEFAULT_BLOCK_SIZE  (16 * 1024) // Read by 16 KB blocks in ReadLineDefault()

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)  do { delete (p); (p) = NULL; } while (0)
#endif

namespace MEDIA_READER
{

CFile::CFile(const std::string& strPath)
 : IFile(strPath),
   m_pFileImpl(CFileFactory::CreateFile(strPath))
{
}

CFile::~CFile(void)
{
  SAFE_DELETE(m_pFileImpl);
}

bool CFile::Exists(void)
{
  if (!m_pFileImpl)
    return false;

  return m_pFileImpl->Exists();
}

bool CFile::Stat(FILE_STATUS& status)
{
  if (!m_pFileImpl)
    return false;

  if (m_pFileImpl->Stat(status))
    return true;

  return StatDefault(status);
}

bool CFile::StatDefault(FILE_STATUS& status)
{
  bool bWasOpen = IsOpen();

  if (!bWasOpen && !Open())
    return false;

  memset(&status, 0, sizeof(status));
  status.type = FILE_TYPE_FILE;
  status.size = GetLength();

  // Reset open state
  if (!bWasOpen)
    Close();

  return true;
}

bool CFile::Move(const std::string &strPathNew, bool bOverWrite /* = false */)
{
  if (!m_pFileImpl)
    return false;

  if (m_pFileImpl->Move(strPathNew))
    return true;

  return MoveDefault(strPathNew);
}

bool CFile::MoveDefault(const std::string &strPathNew)
{
  if (!Open())
    return false;

  IFile* pNewFileImpl = CFileFactory::CreateFile(strPathNew);
  if (pNewFileImpl == NULL || !pNewFileImpl->OpenForWrite())
    return false;

  std::vector<uint8_t> data;
  const int64_t fileSize = GetLength();
  if (fileSize >= 0 && Read(data, fileSize) != (uint64_t)fileSize)
    return false;

  if (pNewFileImpl->Write(data) != (uint64_t)fileSize)
    return false;

  Close();
  SAFE_DELETE(m_pFileImpl);
  m_pFileImpl = pNewFileImpl;

  return true;
}

bool CFile::Delete()
{
  if (!m_pFileImpl)
    return false;

  return m_pFileImpl->Delete();
}

bool CFile::IsHidden(void) const
{
  if (!m_pFileImpl)
    return false;

  return m_pFileImpl->IsHidden();
}

bool CFile::SetHidden(bool bHidden)
{
  if (!m_pFileImpl)
    return false;

  return m_pFileImpl->SetHidden(bHidden);
}

bool CFile::Open(unsigned int flags)
{
  if (!m_pFileImpl)
    return false;

  return m_pFileImpl->Open(flags);
}

bool CFile::OpenForWrite(bool bOverWrite /* = false */)
{
  if (!m_pFileImpl)
    return false;

  return m_pFileImpl->OpenForWrite(bOverWrite);
}

bool CFile::IsOpen(void) const
{
  if (!m_pFileImpl)
    return false;

  return m_pFileImpl->IsOpen();
}

uint64_t CFile::Read(std::vector<uint8_t>& data, uint64_t byte_count /* = std::numeric_limits<uint64_t>::max() */)
{
  if (!m_pFileImpl)
    return false;

  return m_pFileImpl->Read(data, byte_count);
}

bool CFile::ReadLine(std::string &strLine)
{
  if (!m_pFileImpl)
    return false;

  if (m_pFileImpl->ReadLine(strLine))
    return true;

  return ReadLineDefault(strLine);
}

/*
bool IsNewline(char c)
{
  return c == '\x0D' || c == '\x0A';
}
*/

bool CFile::ReadLineDefault(std::string &strLine)
{
  // TODO
  strLine.clear();
  return false;

  /* TODO: Use STL
  int64_t currentPos = GetPosition();

  // Get block size
  uint64_t blockSize = 0;
  FILE_STATUS status;
  if (Stat(status))
    blockSize = status.block_size;
  if (blockSize <= 0)
    blockSize = DEFAULT_BLOCK_SIZE;

  std::vector<uint8_t> data;
  std::vector<uint8_t>::const_iterator itData = data.begin();
  uint64_t bytesRead = 0;
  bool bSeenNewLine = false;

  while (!bSeenNewLine && Read(data, blockSize))
  {
    for ( ; itData != data.end(); ++itData)
    {
      if (IsNewline(*itData))
      {
        bSeenNewLine = true;
        break;
      }
    }
  }

  if (bSeenNewLine)
    assert(itData != data.end());
  if (itData != data.end())
    assert(bSeenNewLine);

  strLine.assign(data.data(), data.data() + (itData - data.begin()));

  if (bSeenNewLine && (itData + 1 != data.end()) && *itData == '\0x0D' && *itData == '\0x0A')
    ++itData;

  // TODO: Seek
  return !strLine.empty();
  */
  return false;
}

uint64_t CFile::Write(const std::vector<uint8_t>& data)
{
  if (!m_pFileImpl)
    return false;

  return m_pFileImpl->Write(data);
}

void CFile::Flush(void)
{
  if (!m_pFileImpl)
    return;

  m_pFileImpl->Flush();
}

int64_t CFile::Seek(int64_t filePosition, int iWhence /* = SEEK_SET */)
{
  if (!m_pFileImpl)
    return false;

  return m_pFileImpl->Seek(filePosition, iWhence);
}

bool CFile::Truncate(uint64_t size)
{
  if (!m_pFileImpl)
    return false;

  if (m_pFileImpl->Truncate(size))
    return true;

  return TruncateDefault(size);
}

bool TruncateDefault(uint64_t size)
{
  // TODO
  return false;
}

int64_t CFile::GetPosition(void)
{
  if (!m_pFileImpl)
    return false;

  return m_pFileImpl->GetPosition();
}

int64_t CFile::GetLength(void)
{
  if (!m_pFileImpl)
    return false;

  return m_pFileImpl->GetLength();
}

std::string CFile::GetMIMEType(void)
{
  if (!m_pFileImpl)
    return "";

  std::string mimeType = m_pFileImpl->GetMIMEType();
  if (!mimeType.empty())
    return mimeType;

  return GetMIMETypeDefault();
}

std::string CFile::GetMIMETypeDefault(void)
{
  return MIME_TYPE_FILE;
}

std::string CFile::GetContentCharset(void)
{
  if (!m_pFileImpl)
    return "";

  std::string charset = m_pFileImpl->GetContentCharset();
  if (!charset.empty())
    return charset;

  return GetContentCharsetDefault();
}

std::string CFile::GetContentCharsetDefault(void)
{
  return "";
}

void CFile::Close(void)
{
  if (!m_pFileImpl)
    return;

  m_pFileImpl->Close();
}

bool CFile::OnSameFileSystem(CFile& file1, CFile& file2)
{
  FILE_STATUS status1;
  FILE_STATUS status2;
  if (file1.Stat(status1) && status1.device_id != 0 && file2.Stat(status2))
    return status1.device_id == status2.device_id;

  return false;
}

}
