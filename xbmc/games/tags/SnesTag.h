/*
 *      Copyright (C) 2012 Garrett Brown
 *      Copyright (C) 2012 Team XBMC
 *      http://www.xbmc.org
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
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */
#pragma once

#include "Tag.h"
#include "utils/StdString.h"

namespace GAME_INFO
{
  class CSnesTag : public CTag
  {
  public:
    CSnesTag();
    virtual ~CSnesTag() { }
    virtual bool Read(const CStdString& strFile);

    static bool  AllASCII(uint8_t *b, int size);

  private:
    /**
     * Load the file's contents into m_data. Postcondition: m_dataSize is the
     * size of m_data. If an error occured, m_dataSize will be zero.
     */
    void Load(const CStdString &strFile);
    /**
     * Remove the SMC file header (if detected) from m_data. m_dataSize is
     * updated accordingly. Precondition: m_bHeaderFound is false. If
     * m_dataSize is zero, nothing will happen.
     */
    void RemoveHeader();
    /**
     * Once the beheadered file is in memory, this will decode the file and
     * parse the desired headers from the decoded data.
     * \return true if successful, false if the game lied about its
     *         interleaving type. If false, this will trigger the do-while
     *         loop to repeat a second time, causing the file to be re-read.
     */
    bool LoadInternal();
    int  ScoreHi(bool skipHeader, int offset = 0);
    int  ScoreLo(bool skipHeader, int offset = 0);
    void ParseSNESHeader(uint8_t *header, bool bs, const CStdString &cartridgeType);

    static bool  IsExtensionDigit(const CStdString &strFilePath);
    static bool  IsFileNameMulti(const CStdString &strFilePath);
    static void  S9xDeinterleaveType1(uint8_t *base, int size);
    static int   IsBSX(uint8_t *p);
    static bool  IsValidNormalBank(uint8_t bankbyte);
    static const char *TranslateCountry(int region);
    static const char *TranslatePublisher(int company);

    // LoROM Memory Model (32k Banks) or HiROM Memory Model (64k Banks)
    enum MemoryLayout { LoROM, HiROM };
    enum ExtendedFormat { NOPE, YEAH, BIGFIRST, SMALLFIRST };

    uint8_t       *m_data;
    int            m_dataSize;
    bool           m_bHeaderFound;
    ExtendedFormat m_extendedFormat;
    int            m_calculatedSize;
    bool           m_bForceNotInterleaved;
  };
}
