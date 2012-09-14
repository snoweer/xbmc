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

/***********************************************************************************
  This file is a derivative work from the Snes9x project.

  Snes9x - Portable Super Nintendo Entertainment System (TM) emulator.

  (c) Copyright 1996 - 2002  Gary Henderson (gary.henderson@ntlworld.com),
                             Jerremy Koot (jkoot@snes9x.com)

  (c) Copyright 2002 - 2004  Matthew Kendora

  (c) Copyright 2002 - 2005 Peter Bortas (peter@bortas.org)

  (c) Copyright 2004 - 2005 Joel Yliluoma (http://iki.fi/bisqwit/)

  (c) Copyright 2001 - 2006 John Weidman (jweidman@slip.net)

  (c) Copyright 2002 - 2006 funkyass (funkyass@spam.shaw.ca),
                            Kris Bleakley (codeviolation@hotmail.com)

  (c) Copyright 2002 - 2010 Brad Jorsch (anomie@users.sourceforge.net),
                            Nach (n-a-c-h@users.sourceforge.net),
                            zones (kasumitokoduck@yahoo.com)

  (c) Copyright 2006 - 2007 nitsuja

  (c) Copyright 2009 - 2010 BearOso,
                             OV2


  BS-X C emulator code
  (c) Copyright 2005 - 2006  Dreamer Nom,
                             zones

  C4 x86 assembler and some C emulation code
  (c) Copyright 2000 - 2003  _Demo_ (_demo_@zsnes.com),
                             Nach,
                             zsKnight (zsknight@zsnes.com)

  C4 C++ code
  (c) Copyright 2003 - 2006  Brad Jorsch,
                             Nach

  DSP-1 emulator code
  (c) Copyright 1998 - 2006  _Demo_,
                             Andreas Naive (andreasnaive@gmail.com),
                             Gary Henderson,
                             Ivar (ivar@snes9x.com),
                             John Weidman,
                             Kris Bleakley,
                             Matthew Kendora,
                             Nach,
                             neviksti (neviksti@hotmail.com)

  DSP-2 emulator code
  (c) Copyright 2003         John Weidman,
                             Kris Bleakley,
                             Lord Nightmare (lord_nightmare@users.sourceforge.net),
                             Matthew Kendora,
                             neviksti

  DSP-3 emulator code
  (c) Copyright 2003 - 2006  John Weidman,
                             Kris Bleakley,
                             Lancer,
                             z80 gaiden

  DSP-4 emulator code
  (c) Copyright 2004 - 2006  Dreamer Nom,
                             John Weidman,
                             Kris Bleakley,
                             Nach,
                             z80 gaiden

  OBC1 emulator code
  (c) Copyright 2001 - 2004  zsKnight,
                             pagefault (pagefault@zsnes.com),
                             Kris Bleakley
                             Ported from x86 assembler to C by sanmaiwashi

  SPC7110 and RTC C++ emulator code used in 1.39-1.51
  (c) Copyright 2002         Matthew Kendora with research by
                             zsKnight,
                             John Weidman,
                             Dark Force

  SPC7110 and RTC C++ emulator code used in 1.52+
  (c) Copyright 2009         byuu,
                             neviksti

  S-DD1 C emulator code
  (c) Copyright 2003         Brad Jorsch with research by
                             Andreas Naive,
                             John Weidman

  S-RTC C emulator code
  (c) Copyright 2001 - 2006  byuu,
                             John Weidman

  ST010 C++ emulator code
  (c) Copyright 2003         Feather,
                             John Weidman,
                             Kris Bleakley,
                             Matthew Kendora

  Super FX x86 assembler emulator code
  (c) Copyright 1998 - 2003  _Demo_,
                             pagefault,
                             zsKnight

  Super FX C emulator code
  (c) Copyright 1997 - 1999  Ivar,
                             Gary Henderson,
                             John Weidman

  Sound emulator code used in 1.5-1.51
  (c) Copyright 1998 - 2003  Brad Martin
  (c) Copyright 1998 - 2006  Charles Bilyue'

  Sound emulator code used in 1.52+
  (c) Copyright 2004 - 2007  Shay Green (gblargg@gmail.com)

  SH assembler code partly based on x86 assembler code
  (c) Copyright 2002 - 2004  Marcus Comstedt (marcus@mc.pp.se)

  2xSaI filter
  (c) Copyright 1999 - 2001  Derek Liauw Kie Fa

  HQ2x, HQ3x, HQ4x filters
  (c) Copyright 2003         Maxim Stepin (maxim@hiend3d.com)

  NTSC filter
  (c) Copyright 2006 - 2007  Shay Green

  GTK+ GUI code
  (c) Copyright 2004 - 2010  BearOso

  Win32 GUI code
  (c) Copyright 2003 - 2006  blip,
                             funkyass,
                             Matthew Kendora,
                             Nach,
                             nitsuja
  (c) Copyright 2009 - 2010  OV2

  Mac OS GUI code
  (c) Copyright 1998 - 2001  John Stiles
  (c) Copyright 2001 - 2010  zones


  Specific ports contains the works of other authors. See headers in
  individual files.


  Snes9x homepage: http://www.snes9x.com/

  Permission to use, copy, modify and/or distribute Snes9x in both binary
  and source form, for non-commercial purposes, is hereby granted without
  fee, providing that this license information and copyright notice appear
  with all copies and any derived work.

  This software is provided 'as-is', without any express or implied
  warranty. In no event shall the authors be held liable for any damages
  arising from the use of this software or it's derivatives.

  Snes9x is freeware for PERSONAL USE only. Commercial users should
  seek permission of the copyright holders first. Commercial use includes,
  but is not limited to, charging money for Snes9x or software derived from
  Snes9x, including Snes9x or derivatives in commercial game bundles, and/or
  using Snes9x as a promotion for your commercial product.

  The copyright holders request that bug fixes and improvements to the code
  should be forwarded to them so everyone can benefit from the modifications
  in future versions.

  Super NES and Super Nintendo Entertainment System are trademarks of
  Nintendo Co., Limited and its subsidiary companies.
 ***********************************************************************************/

#include "SnesTag.h"
#include "filesystem/File.h"
#include "utils/log.h"
#include "utils/URIUtils.h"

using namespace GAME_INFO;
using namespace XFILE;

#define PLATFORM "Super Nintendo"

// Constants and file format data from libretro SNES9x-Next-v2 project
// https://github.com/libretro/snes9x-next-v2
// This code references snes9x-next-v2 at 95e2c57d2, later commits may contain fixes
#define SMC_HEADER_SIZE     0x200 // 512 B
#define MAX_IMAGE_SIZE      0x800000 + SMC_HEADER_SIZE // 8 MB + 512 B
#define IMAGE_NAME_LEN      23
#define NSRT_HEADER_LENGTH  32

CSnesTag::CSnesTag()
  : m_data(NULL),
    m_dataSize(0),
    m_bHeaderFound(false),
    m_extendedFormat(NOPE),
    m_calculatedSize(0),
    m_bForceNotInterleaved(false)
{
}

bool CSnesTag::Read(const CStdString& strFile)
{
  CTag::Read(strFile);

  // Allocate memory for the game image
  m_data = new uint8_t[MAX_IMAGE_SIZE];
  if (!m_data)
    return false;

  do
  {
    memset(m_data, 0, MAX_IMAGE_SIZE);
    m_dataSize = 0;
    m_bHeaderFound = false;

    Load(strFile);
    RemoveHeader();
    if (m_dataSize == 0)
      return false;
  }
  while (!LoadInternal());

  delete[] m_data;
  return m_gameInfoTag.Loaded();
}

void CSnesTag::Load(const CStdString &strFile)
{
  // Make sure we're loading a rom (perhaps in a zip) instead of that zip itself
  CStdString strExtension;
  URIUtils::GetExtension(strFile, strExtension);
  if (strExtension.CompareNoCase(".zip") == 0 || strExtension.CompareNoCase(".jma") == 0)
  {
    CLog::Log(LOGDEBUG, "CSnesTag: No ZIP or JMA support, use vfs to load game image directly");
    return;
  }

  CFile file;
  if (file.Open(strFile))
  {
    if (file.GetLength() > MAX_IMAGE_SIZE)
    {
      CLog::Log(LOGERROR, "CSnesTag: Game file size (%d KB) is greater than the max (%d KB)",
          file.GetLength() / 1024, MAX_IMAGE_SIZE / 1024);
    }
    else
    {
      // Check for multi-file game images
      if (IsExtensionDigit(strFile))
        CLog::Log(LOGINFO, "CSnesTag: Multi-file game image detected, ignoring extra files");
      else if (IsFileNameMulti(strFile))
        CLog::Log(LOGINFO, "CSnesTag: Multi-file game image detected, ignoring extra files");

      m_dataSize = file.Read(m_data, MAX_IMAGE_SIZE);
    }
  }
}

bool CSnesTag::IsExtensionDigit(const CStdString &strFilePath)
{
  CStdString strExtension;
  URIUtils::GetExtension(strFilePath, strExtension);
  return strExtension.length() == 1 && isdigit(strExtension[0]);
}

bool CSnesTag::IsFileNameMulti(const CStdString &strFilePath)
{
  CStdString strFileName;
  strFileName = URIUtils::GetFileName(strFilePath);
  URIUtils::RemoveExtension(strFileName);
  return (strFileName.length() == 7 || strFileName.length() == 8) &&
         CStdString(strFileName.substr(0, 2)).CompareNoCase("sf") == 0 &&
         isdigit(strFileName[2]) && isdigit(strFileName[3]) && isdigit(strFileName[4]) && isdigit(strFileName[5]) &&
         isalpha(strFileName[strFileName.length() - 1]);
}

void CSnesTag::RemoveHeader()
{
  int calcSize = (m_dataSize / 0x2000) * 0x2000;
  if (m_dataSize - calcSize == SMC_HEADER_SIZE)
  {
    m_bHeaderFound = true;

    // Detect NSRT header
    uint8_t *NSRTHead = m_data + 0x1D0; // NSRT header location
    if (strncmp("NSRT", reinterpret_cast<char*>(&NSRTHead[24]), 4) == 0 && NSRTHead[28] == 22)
      CLog::Log(LOGDEBUG, "CSnesTag: Found 32-byte NSRT header at offset 0x1D0, ignoring");
    
    memmove(m_data, m_data + SMC_HEADER_SIZE, calcSize);
    m_dataSize -= SMC_HEADER_SIZE;
  }
}

bool CSnesTag::LoadInternal()
{
  m_calculatedSize = 0;
  m_extendedFormat = NOPE;

  int hi_score, lo_score;

  hi_score = ScoreHi(false);
  lo_score = ScoreLo(false);

  if (!m_bHeaderFound &&
      ((hi_score > lo_score && ScoreHi(true) > hi_score) ||
      (hi_score <= lo_score && ScoreLo(true) > lo_score)))
  {
    CLog::Log(LOGDEBUG, "CSnesTag: No header found, looking harder");
    memmove(m_data, m_data + SMC_HEADER_SIZE, m_dataSize - SMC_HEADER_SIZE);
    m_dataSize -= SMC_HEADER_SIZE;
    // modifying ROM, so we need to rescore
    hi_score = ScoreHi(false);
    lo_score = ScoreLo(false);
  }

  m_calculatedSize = (m_dataSize / 0x2000) * 0x2000;

  if (m_calculatedSize > 0x400000 &&
      m_data[0x7fd5] + (m_data[0x7fd6] << 8) != 0x3423 && // exclude SA-1
      m_data[0x7fd5] + (m_data[0x7fd6] << 8) != 0x3523 &&
      m_data[0x7fd5] + (m_data[0x7fd6] << 8) != 0x4332 && // exclude S-DD1
      m_data[0x7fd5] + (m_data[0x7fd6] << 8) != 0x4532 &&
      m_data[0x7fd5] + (m_data[0x7fd6] << 8) != 0xF93a && // exclude SPC7110
      m_data[0x7fd5] + (m_data[0x7fd6] << 8) != 0xF53a)
  {
    m_extendedFormat = YEAH;
  }

  // If both vectors are invalid, it's type 1 interleaved LoROM
  if (m_extendedFormat == NOPE &&
      m_data[0x7ffc] + (m_data[0x7ffd] << 8) < 0x8000 &&
      m_data[0xfffc] + (m_data[0xfffd] << 8) < 0x8000)
  {
    if (!m_bForceNotInterleaved)
      S9xDeinterleaveType1(m_data, m_dataSize);
  }

  // m_calculatedSize is now set, so rescore
  hi_score = ScoreHi(false);
  lo_score = ScoreLo(false);

  uint8_t *header = m_data;

  if (m_extendedFormat != NOPE)
  {
    int	swappedhirom, swappedlorom;
    
    swappedhirom = ScoreHi(false, 0x400000);
    swappedlorom = ScoreLo(false, 0x400000);
    
    // set swapped here
    if (std::max(swappedlorom, swappedhirom) >= std::max(lo_score, hi_score))
    {
      m_extendedFormat = BIGFIRST;
      hi_score = swappedhirom;
      lo_score = swappedlorom;
      header += 0x400000;
    }
    else
    {
      m_extendedFormat = SMALLFIRST;
    }
  }

  bool interleaved = false;
  bool tales = false;

  MemoryLayout mapType;

  if (lo_score >= hi_score)
  {
    mapType = LoROM;

    // ignore map type byte if not 0x2x or 0x3x
    if ((header[0x7fd5] & 0xf0) == 0x20 || (header[0x7fd5] & 0xf0) == 0x30)
    {
      switch (header[0x7fd5] & 0xf)
      {
      case 1:
        interleaved = true;
        break;
      case 5:
        interleaved = true;
        tales = true;
        break;
      default:
        break;
      }
    }
  }
  else
  {
    mapType = HiROM;

    if ((header[0xffd5] & 0xf0) == 0x20 || (header[0xffd5] & 0xf0) == 0x30)
    {
      switch (header[0xffd5] & 0xf)
      {
      case 0:
      case 3:
        interleaved = true;
        break;
      default:
        break;
      }
    }
  }

  // These two games fail to be detected
  if (strncmp(reinterpret_cast<char*>(&m_data[0x7fc0]), "YUYU NO QUIZ DE GO!GO!", 22) == 0 ||
      strncmp(reinterpret_cast<char*>(&m_data[0xffc0]), "BATMAN--REVENGE JOKER",  21) == 0)
  {
    mapType = LoROM;
    interleaved = false;
    tales = false;
  }

  if (!m_bForceNotInterleaved && interleaved)
  {
    CLog::Log(LOGDEBUG, "CSnesTag: Game image is in interleaved format, converting...");
    if (tales)
    {
      if (m_extendedFormat == BIGFIRST)
      {
        S9xDeinterleaveType1(m_data, 0x400000);
        S9xDeinterleaveType1(m_data + 0x400000, m_calculatedSize - 0x400000);
      }
      else
      {
        S9xDeinterleaveType1(m_data, m_calculatedSize - 0x400000);
        S9xDeinterleaveType1(m_data + m_calculatedSize - 0x400000, 0x400000);
      }

      mapType = HiROM;
    }
    else
    {
      // Swap memory models
      if (mapType == HiROM) mapType = LoROM;
      else                  mapType = HiROM;

      S9xDeinterleaveType1(m_data, m_calculatedSize);
    }

    hi_score = ScoreHi(false);
    lo_score = ScoreLo(false);

    if ((mapType == HiROM && (lo_score >= hi_score || hi_score < 0)) ||
        (mapType == LoROM && (hi_score >  lo_score || lo_score < 0)))
    {
      CLog::Log(LOGDEBUG, "CSnesTag: Game image lied about its type! Trying again.");
      m_bForceNotInterleaved = true;;
      return false;
    }
  }

  if (m_extendedFormat == SMALLFIRST)
    tales = true;

  if (tales)
  {
    uint8_t *tmp = new uint8_t[m_calculatedSize - 0x400000];
    if (tmp)
    {
      CLog::Log(LOGDEBUG, "CSnesTag: Fixing swapped ExHiROM...");
      memmove(tmp, m_data, m_calculatedSize - 0x400000);
      memmove(m_data, m_data + m_calculatedSize - 0x400000, 0x400000);
      memmove(m_data + 0x400000, tmp, m_calculatedSize - 0x400000);
      delete[] tmp;
    }
  }
  
  // Re-calculate the header
  header = m_data + 0x7FB0;
  if (m_extendedFormat == BIGFIRST)
    header += 0x400000;
  if (mapType == HiROM)
    header += 0x8000;

  bool bs, bsxItself, bsHeader, flashMode;
  if (!memcmp(&m_data[0x7FC0], "Satellaview BS-X     ", 21))
  {
    bs = true;
    bsxItself = true;
    mapType = LoROM;
    flashMode = false;
  }
  else
  {
    bsxItself = false;
    flashMode = false;

    bool bLo = (IsBSX(m_data + 0x7FC0) == 1);
    bool bHi = (IsBSX(m_data + 0xFFC0) == 1);
    bs = bLo || bHi;

    if (bs)
    {
      if (bLo) mapType = LoROM;
      else     mapType = HiROM;

      if (bLo)
        flashMode = ((m_data[0x7FC0 + 0x18] & 0xEF) != 0x20);
      else
        flashMode = ((m_data[0xFFC0 + 0x18] & 0xEF) != 0x20);
    }
  }

  bsHeader = bs && !bsxItself;
  uint8_t romType = bsHeader ? 0xE5 /* BS-X */ : header[0x26];
  
  CStdString cartridgeType;
  if (romType == 0 && !bs)
    cartridgeType = "ROM";
  else
  {
    CStdString chip = "";
    if (romType == 0x03 || 0x05)
      chip = "DSP";
    else
    {
      uint8_t romSpeed = bs ? header[0x28] : header[0x25];
      int identifier = ((romType & 0xff) << 8) + (romSpeed & 0xff);
      switch (identifier)
      {
      case 0x5535:
        chip = "SRTC";
        break;
      case 0xF93A:
        chip = "SPC7110+RTC";
        break;
      case 0xF53A:
        chip = "SPC7110";
        break;
      case 0x2530:
        chip = "OBC1";
        break;
      case 0x3423:
      case 0x3523:
        chip = "SA1";
        break;
      case 0x1320:
      case 0x1420:
      case 0x1520:
      case 0x1A20:
        chip = "SuperFX";
        break;
      case 0x4332:
      case 0x4532:
        chip = "SDD1";
        break;
      case 0xF530:
        chip = (m_data[0x7FD7] == 0x09) ? "ST-011" : "ST-010";
        break;
      case 0xF630:
        chip = "SETA";
        break;
      case 0xF320:
        chip = "C4";
      default:
        break;
      }
    }

    static const char	*contents[3] = { "ROM", "ROM+RAM", "ROM+RAM+BAT" };
    cartridgeType = contents[(romType & 0xf) % 3];
    if (!chip.empty())
      cartridgeType += "+" + chip;
  }

  // And now, the moment we've all been waiting for
  ParseSNESHeader(header, bsHeader, cartridgeType);
  return true;
}

int CSnesTag::ScoreHi(bool skipHeader, int offset /* = 0 */)
{
  uint8_t *buf = m_data + 0xff00 + offset + (skipHeader ? SMC_HEADER_SIZE : 0);
  int score = 0;

  if (buf[0xd5] & 0x1)                           score += 2;
  if (buf[0xd5] == 0x23)                         score -= 2; // Mode23 is SA-1
  if (buf[0xd4] == 0x20)                         score += 2;
  if (buf[0xdc] + (buf[0xdd] << 8) + buf[0xde] + (buf[0xdf] << 8) == 0xffff)
  {
                                                 score += 2;
    if (0 != buf[0xde] + (buf[0xdf] << 8))       score += 1;
  }
  if (buf[0xda] == 0x33)                         score += 2;
  if ((buf[0xd5] & 0xf) < 4)                     score += 2;
  if (!(buf[0xfd] & 0x80))                       score -= 6;
  if (buf[0xfc] + (buf[0xfd] << 8) > 0xffb0)     score -= 2; // Reduced after looking at a scan by Cowering
  if (m_calculatedSize > 1024 * 1024 * 3)        score += 4;
  if ((1 << (buf[0xd7] - 7)) > 48)               score -= 1;
  if (!AllASCII(&buf[0xb0], 6))                  score -= 1;
  if (!AllASCII(&buf[0xc0], IMAGE_NAME_LEN - 1)) score -= 1;
  return score;
}

int CSnesTag::ScoreLo(bool skipHeader, int offset /* = 0 */)
{    
  uint8_t *buf = m_data + 0x7f00 + offset + (skipHeader ? SMC_HEADER_SIZE : 0);
  int score = 0;
  
  if (!(buf[0xd5] & 0x1))                        score += 3;
  if (buf[0xd5] == 0x23)                         score += 2; // Mode23 is SA-1
  if (buf[0xdc] + (buf[0xdd] << 8) + buf[0xde] + (buf[0xdf] << 8) == 0xffff)
  {
                                                 score += 2;
    if (0 != (buf[0xde] + (buf[0xdf] << 8)))     score += 1;
  }
  if (buf[0xda] == 0x33)                         score += 2;
  if ((buf[0xd5] & 0xf) < 4)                     score += 2;
  if (!(buf[0xfd] & 0x80))                       score -= 6;
  if (buf[0xfc] + (buf[0xfd] << 8) > 0xffb0)     score -= 2; // Reduced after looking at a scan by Cowering
  if (m_calculatedSize > 1024 * 1024 * 16)       score += 4;
  if ((1 << (buf[0xd7] - 7)) > 48)               score -= 1;
  if (!AllASCII(&buf[0xb0], 6))                  score -= 1;
  if (!AllASCII(&buf[0xc0], IMAGE_NAME_LEN - 1)) score -= 1;
  return score;
}

bool CSnesTag::AllASCII(uint8_t *b, int size)
{
  for (int i = 0; i < size; i++)
    if (b[i] < 32 || b[i] > 126)
      return false;
  return true;
}

void CSnesTag::S9xDeinterleaveType1(uint8_t *base, int size)
{
  uint8_t  blocks[256];
  int      nblocks = size >> 16;

  for (int i = 0; i < nblocks; i++)
  {
    blocks[i * 2] = i + nblocks;
    blocks[i * 2 + 1] = i;
  }

  uint8_t *tmp = new uint8_t[0x8000];
  if (tmp)
  {
    for (int i = 0; i < nblocks * 2; i++)
    {
      for (int j = 0; j < nblocks * 2; j++)
      {
        if (blocks[j] == i)
        {
          memmove(tmp, &base[blocks[j] * 0x8000], 0x8000);
          memmove(&base[blocks[j] * 0x8000], &base[blocks[i] * 0x8000], 0x8000);
          memmove(&base[blocks[i] * 0x8000], tmp, 0x8000);
          uint8_t b = blocks[j];
          blocks[j] = blocks[i];
          blocks[i] = b;
          break;
        }
      }
    }
    delete[] tmp;
  }
}

int CSnesTag::IsBSX(uint8_t *p)
{
  if ((p[26] == 0x33 || p[26] == 0xFF) && (!p[21] || (p[21] & 131) == 128) && IsValidNormalBank(p[24]))
  {
    uint8_t m = p[22];

    if (!m && !p[23])
      return 2;

    if ((m == 0xFF && p[23] == 0xFF) || (!(m & 0xF) && ((m >> 4) - 1 < 12)))
      return 1;
  }
  return 0;
}

bool CSnesTag::IsValidNormalBank(uint8_t bankbyte)
{
  return bankbyte == 32 || bankbyte == 33 || bankbyte == 48 || bankbyte == 49;
}

void CSnesTag::ParseSNESHeader(uint8_t *header, bool bBsHeader, const CStdString &cartridgeType)
{
  // Name
  char name[IMAGE_NAME_LEN];
  strncpy(name, reinterpret_cast<char*>(&header[0x10]), IMAGE_NAME_LEN - 1);
  if (bBsHeader)
    memset(name + 16, 0x20, IMAGE_NAME_LEN - 17);
  // If the name isn't ASCII, there's a good chance it's not actually a game image
  if (!AllASCII(reinterpret_cast<uint8_t*>(name), IMAGE_NAME_LEN - 1))
    return;
  m_gameInfoTag.SetName(CStdString(std::string(name, IMAGE_NAME_LEN - 1)).Trim());
  
  // Region and Format
  int region = bBsHeader ? 0 : header[0x29];
  if (0 <= region && region <= 13) // Test if region is valid
  {
    m_gameInfoTag.SetRegion(TranslateCountry(region));
    m_gameInfoTag.SetFormat((2 <= region && region <= 12) ? "PAL" : "NTSC");
  }

  // Code (ID)
  char id[4];
  strncpy(id, reinterpret_cast<char*>(&header[0x02]), 4);
  if (AllASCII(reinterpret_cast<uint8_t*>(id), 4))
    m_gameInfoTag.SetName(std::string(id, 4));

  // Cartridge type
  m_gameInfoTag.SetCartridgeType(cartridgeType);

  // Publisher
  int companyID = -1;
  if (header[0x2A] != 0x33)
    companyID = ((header[0x2A] >> 4) & 0x0F) * 36 + (header[0x2A] & 0x0F);
  else if (isalnum(header[0x00]) && isalnum(header[0x01]))
  {
    int	l, r, l2, r2;
    l = toupper(header[0x00]);
    r = toupper(header[0x01]);
    l2 = (l > '9') ? l - '7' : l - '0';
    r2 = (r > '9') ? r - '7' : r - '0';
    companyID = l2 * 36 + r2;
  }
  m_gameInfoTag.SetPublisher(TranslatePublisher(companyID) ? TranslatePublisher(companyID) : "");

  // Platform (duh)
  m_gameInfoTag.SetPlatform(PLATFORM);

  // Finished
  m_gameInfoTag.SetLoaded(true);
}

const char *CSnesTag::TranslateCountry(int region)
{
  switch (region)
  {
    case 0:   return "Japan";
    case 1:   return "USA and Canada";
    case 2:   return "Oceania, Europe and Asia";
    case 3:   return "Sweden";
    case 4:   return "Finland";
    case 5:   return "Denmark";
    case 6:   return "France";
    case 7:   return "Holland";
    case 8:   return "Spain";
    case 9:   return "Germany, Austria and Switzerland";
    case 10:  return "Italy";
    case 11:  return "Hong Kong and China";
    case 12:  return "Indonesia";
    case 13:  return "South Korea";
    default:  return "";
  }
}

const char *CSnesTag::TranslatePublisher(int company)
{
  static const char *nintendoLicensees[] =
  {
    "Unlicensed",
    "Nintendo",
    "Rocket Games/Ajinomoto",
    "Imagineer-Zoom",
    "Gray Matter",
    "Zamuse",
    "Falcom",
    NULL,
    "Capcom",
    "Hot B Co.",
    "Jaleco",
    "Coconuts Japan",
    "Coconuts Japan/G.X.Media",
    "Micronet",
    "Technos",
    "Mebio Software",
    "Shouei System",
    "Starfish",
    NULL,
    "Mitsui Fudosan/Dentsu",
    NULL,
    "Warashi Inc.",
    NULL,
    "Nowpro",
    NULL,
    "Game Village",
    "IE Institute",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "Banarex",
    "Starfish",
    "Infocom",
    "Electronic Arts Japan",
    NULL,
    "Cobra Team",
    "Human/Field",
    "KOEI",
    "Hudson Soft",
    "S.C.P./Game Village",
    "Yanoman",
    NULL,
    "Tecmo Products",
    "Japan Glary Business",
    "Forum/OpenSystem",
    "Virgin Games (Japan)",
    "SMDE",
    "Yojigen",
    NULL,
    "Daikokudenki",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "Creatures Inc.",
    "TDK Deep Impresion",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "Destination Software/KSS",
    "Sunsoft/Tokai Engineering",
    "POW (Planning Office Wada)/VR 1 Japan",
    "Micro World",
    NULL,
    "San-X",
    "Enix",
    "Loriciel/Electro Brain",
    "Kemco Japan",
    "Seta Co.,Ltd.",
    "Culture Brain",
    "Irem Corp.",
    "Palsoft",
    "Visit Co., Ltd.",
    "Intec",
    "System Sacom",
    "Poppo",
    "Ubisoft Japan",
    NULL,
    "Media Works",
    "NEC InterChannel",
    "Tam",
    "Gajin/Jordan",
    "Smilesoft",
    NULL,
    NULL,
    "Mediakite",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "Viacom",
    "Carrozzeria",
    "Dynamic",
    NULL,
    "Magifact",
    "Hect",
    "Codemasters",
    "Taito/GAGA Communications",
    "Laguna",
    "Telstar Fun & Games/Event/Taito",
    NULL,
    "Arcade Zone Ltd.",
    "Entertainment International/Empire Software",
    "Loriciel",
    "Gremlin Graphics",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "Seika Corp.",
    "UBI SOFT Entertainment Software",
    "Sunsoft US",
    NULL,
    "Life Fitness",
    NULL,
    "System 3",
    "Spectrum Holobyte",
    NULL,
    "Irem",
    NULL,
    "Raya Systems",
    "Renovation Products",
    "Malibu Games",
    NULL,
    "Eidos/U.S. Gold",
    "Playmates Interactive",
    NULL,
    NULL,
    "Fox Interactive",
    "Time Warner Interactive",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "Disney Interactive",
    NULL,
    "Black Pearl",
    NULL,
    "Advanced Productions",
    NULL,
    NULL,
    "GT Interactive",
    "RARE",
    "Crave Entertainment",
    "Absolute Entertainment",
    "Acclaim",
    "Activision",
    "American Sammy",
    "Take 2/GameTek",
    "Hi Tech",
    "LJN Ltd.",
    NULL,
    "Mattel",
    NULL,
    "Mindscape/Red Orb Entertainment",
    "Romstar",
    "Taxan",
    "Midway/Tradewest",
    NULL,
    "American Softworks Corp.",
    "Majesco Sales Inc.",
    "3DO",
    NULL,
    NULL,
    "Hasbro",
    "NewKidCo",
    "Telegames",
    "Metro3D",
    NULL,
    "Vatical Entertainment",
    "LEGO Media",
    NULL,
    "Xicat Interactive",
    "Cryo Interactive",
    NULL,
    NULL,
    "Red Storm Entertainment",
    "Microids",
    NULL,
    "Conspiracy/Swing",
    "Titus",
    "Virgin Interactive",
    "Maxis",
    NULL,
    "LucasArts Entertainment",
    NULL,
    NULL,
    "Ocean",
    NULL,
    "Electronic Arts",
    NULL,
    "Laser Beam",
    NULL,
    NULL,
    "Elite Systems",
    "Electro Brain",
    "The Learning Company",
    "BBC",
    NULL,
    "Software 2000",
    NULL,
    "BAM! Entertainment",
    "Studio 3",
    NULL,
    NULL,
    NULL,
    "Classified Games",
    NULL,
    "TDK Mediactive",
    NULL,
    "DreamCatcher",
    "JoWood Produtions",
    "SEGA",
    "Wannado Edition",
    "LSP (Light & Shadow Prod.)",
    "ITE Media",
    "Infogrames",
    "Interplay",
    "JVC (US)",
    "Parker Brothers",
    NULL,
    "SCI (Sales Curve Interactive)/Storm",
    NULL,
    NULL,
    "THQ Software",
    "Accolade Inc.",
    "Triffix Entertainment",
    NULL,
    "Microprose Software",
    "Universal Interactive/Sierra/Simon & Schuster",
    NULL,
    "Kemco",
    "Rage Software",
    "Encore",
    NULL,
    "Zoo",
    "Kiddinx",
    "Simon & Schuster Interactive",
    "Asmik Ace Entertainment Inc./AIA",
    "Empire Interactive",
    NULL,
    NULL,
    "Jester Interactive",
    NULL,
    "Rockstar Games",
    "Scholastic",
    "Ignition Entertainment",
    "Summitsoft",
    "Stadlbauer",
    NULL,
    NULL,
    NULL,
    "Misawa",
    "Teichiku",
    "Namco Ltd.",
    "LOZC",
    "KOEI",
    NULL,
    "Tokuma Shoten Intermedia",
    "Tsukuda Original",
    "DATAM-Polystar",
    NULL,
    NULL,
    "Bullet-Proof Software",
    "Vic Tokai Inc.",
    NULL,
    "Character Soft",
    "I'Max",
    "Saurus",
    NULL,
    NULL,
    "General Entertainment",
    NULL,
    NULL,
    "I'Max",
    "Success",
    NULL,
    "SEGA Japan",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "Takara",
    "Chun Soft",
    "Video System Co., Ltd./McO'River",
    "BEC",
    NULL,
    "Varie",
    "Yonezawa/S'pal",
    "Kaneko",
    NULL,
    "Victor Interactive Software/Pack-in-Video",
    "Nichibutsu/Nihon Bussan",
    "Tecmo",
    "Imagineer",
    NULL,
    NULL,
    "Nova",
    "Den'Z",
    "Bottom Up",
    NULL,
    "TGL (Technical Group Laboratory)",
    NULL,
    "Hasbro Japan",
    NULL,
    "Marvelous Entertainment",
    NULL,
    "Keynet Inc.",
    "Hands-On Entertainment",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "Telenet",
    "Hori",
    NULL,
    NULL,
    "Konami",
    "K.Amusement Leasing Co.",
    "Kawada",
    "Takara",
    NULL,
    "Technos Japan Corp.",
    "JVC (Europe/Japan)/Victor Musical Industries",
    NULL,
    "Toei Animation",
    "Toho",
    NULL,
    "Namco",
    "Media Rings Corp.",
    "J-Wing",
    NULL,
    "Pioneer LDC",
    "KID",
    "Mediafactory",
    NULL,
    NULL,
    NULL,
    "Infogrames Hudson",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "Acclaim Japan",
    "ASCII Co./Nexoft",
    "Bandai",
    NULL,
    "Enix",
    NULL,
    "HAL Laboratory/Halken",
    "SNK",
    NULL,
    "Pony Canyon Hanbai",
    "Culture Brain",
    "Sunsoft",
    "Toshiba EMI",
    "Sony Imagesoft",
    NULL,
    "Sammy",
    "Magical",
    "Visco",
    NULL,
    "Compile",
    NULL,
    "MTO Inc.",
    NULL,
    "Sunrise Interactive",
    NULL,
    "Global A Entertainment",
    "Fuuki",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "Taito",
    NULL,
    "Kemco",
    "Square",
    "Tokuma Shoten",
    "Data East",
    "Tonkin House",
    NULL,
    "KOEI",
    NULL,
    "Konami/Ultra/Palcom",
    "NTVIC/VAP",
    "Use Co., Ltd.",
    "Meldac",
    "Pony Canyon (Japan)/FCI (US)",
    "Angel/Sotsu Agency/Sunrise",
    "Yumedia/Aroma Co., Ltd.",
    NULL,
    NULL,
    "Boss",
    "Axela/Crea-Tech",
    "Sekaibunka-Sha/Sumire kobo/Marigul Management Inc.",
    "Konami Computer Entertainment Osaka",
    NULL,
    NULL,
    "Enterbrain",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "Taito/Disco",
    "Sofel",
    "Quest Corp.",
    "Sigma",
    "Ask Kodansha",
    NULL,
    "Naxat",
    "Copya System",
    "Capcom Co., Ltd.",
    "Banpresto",
    "TOMY",
    "Acclaim/LJN Japan",
    NULL,
    "NCS",
    "Human Entertainment",
    "Altron",
    "Jaleco",
    "Gaps Inc.",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "Elf",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "Jaleco",
    NULL,
    "Yutaka",
    "Varie",
    "T&ESoft",
    "Epoch Co., Ltd.",
    NULL,
    "Athena",
    "Asmik",
    "Natsume",
    "King Records",
    "Atlus",
    "Epic/Sony Records (Japan)",
    NULL,
    "IGS (Information Global Service)",
    NULL,
    "Chatnoir",
    "Right Stuff",
    NULL,
    "NTT COMWARE",
    NULL,
    "Spike",
    "Konami Computer Entertainment Tokyo",
    "Alphadream Corp.",
    NULL,
    "Sting",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "A Wave",
    "Motown Software",
    "Left Field Entertainment",
    "Extreme Entertainment Group",
    "TecMagik",
    NULL,
    NULL,
    NULL,
    NULL,
    "Cybersoft",
    NULL,
    "Psygnosis",
    NULL,
    NULL,
    "Davidson/Western Tech.",
    "Unlicensed",
    NULL,
    NULL,
    NULL,
    NULL,
    "The Game Factory Europe",
    "Hip Games",
    "Aspyr",
    NULL,
    NULL,
    "Mastiff",
    "iQue",
    "Digital Tainment Pool",
    "XS Games",
    "Daiwon",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "PCCW Japan",
    NULL,
    NULL,
    "KiKi Co. Ltd.",
    "Open Sesame Inc.",
    "Sims",
    "Broccoli",
    "Avex",
    "D3 Publisher",
    NULL,
    "Konami Computer Entertainment Japan",
    NULL,
    "Square-Enix",
    "KSG",
    "Micott & Basara Inc.",
    NULL,
    "Orbital Media",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "The Game Factory USA",
    NULL,
    NULL,
    "Treasure",
    "Aruze",
    "Ertain",
    "SNK Playmore",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "Yojigen"
  };

  if (0 <= company && company < (int)(sizeof(nintendoLicensees) / sizeof(nintendoLicensees[0])))
    return nintendoLicensees[company];

  return NULL;
}
