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

#include "IFile.h"

#define MIME_TYPE_NONE  "none/none"
#define MIME_TYPE_FILE  "application/octet-stream"

namespace MEDIA_READER
{

class CFile : public IFile
{
public:
  CFile(const std::string& url);
  virtual ~CFile(void);

  virtual bool Exists(void);
  virtual bool Stat(FILE_STATUS& status);
  virtual bool Move(const std::string &strPathNew, bool bOverWrite = false);
  virtual bool Delete();
  virtual bool IsHidden(void) const;
  virtual bool SetHidden(bool bHidden);
  virtual bool Open(unsigned int flags = 0);
  virtual bool OpenForWrite(bool bOverWrite = false);
  virtual bool IsOpen(void) const;
  virtual uint64_t Read(std::vector<uint8_t>& data, uint64_t byte_count = std::numeric_limits<uint64_t>::max());
  virtual bool ReadLine(std::string &strLine);
  virtual uint64_t Write(const std::vector<uint8_t>& data);
  virtual void Flush(void);
  virtual int64_t Seek(int64_t filePosition, int iWhence = SEEK_SET);
  virtual bool Truncate(uint64_t size);
  virtual int64_t GetPosition(void);
  virtual int64_t GetLength(void);
  virtual std::string GetMIMEType(void);
  virtual std::string GetContentCharset(void);
  virtual void Close(void);

  /*!
   * Check if the files are on the same filesystem
   * \return true if the filesystems are the same, false if different or unknown
   */
  static bool OnSameFileSystem(CFile& file1, CFile& file2);

private:
  /*!
   * Default functionality for unimplemented IFile methods
   */
  bool StatDefault(FILE_STATUS& status);
  bool MoveDefault(const std::string &strPathNew);
  bool ReadLineDefault(std::string &strLine);
  bool TruncateDefault(uint64_t size);
  std::string GetMIMETypeDefault(void);
  std::string GetContentCharsetDefault(void);

  IFile* m_pFileImpl;
};

}
