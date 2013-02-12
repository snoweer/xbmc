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

#include "GameManager.h"
#include "addons/AddonManager.h"
#include "utils/log.h"
#include "utils/StringUtils.h"
#include "threads/SingleLock.h"
#include "utils/URIUtils.h"
#include "utils/Variant.h"

using namespace ADDON;
using namespace GAME_INFO;

struct PlatformMap
{
  GamePlatform id;
  const char   *name;
  // Extensions must be unique to the platform (e.g. no zip). Currently unused
  const char   *extensions;
};

// Lookups are made using comparisons between case-insensitive alphanumeric
// strings. "CD-i" will match "CDi", "CD_I" and "CD I"
static const PlatformMap platformInfo[] =
{
  { PLATFORM_UNKNOWN,              "Unknown",              "" },
  { PLATFORM_OTHER,                "Other",                "" },
  { PALTFORM_3D0,                  "3DO",                  "" },
  { PLATFORM_AMIGA,                "Amiga",                "" },
  { PLATFORM_AMIGA_CD32,           "Amiga CD32",           "" },
  { PALTFORM_AMSTRAD_CPC,          "Amstrad CPC",          "" },
  { PALTFORM_APPLE_II,             "Apple II",             "" },
  { PALTFORM_ATARI_2600,           "Atari 2600",           "" },
  { PALTFORM_ATARI_5200,           "Atari 5200",           "" },
  { PLATFORM_ATARI_7800,           "Atari 7800",           "" },
  { PLATFORM_ATARI_8_BIT,          "Atari 8-bit",          "" },
  { PLATFORM_ATARI_ST,             "Atari ST",             "" },
  { PLATFORM_BBC_MICRO,            "BBC Micro",            "" },
  { PLATFORM_BREW,                 "BREW",                 "" },
  { PLATFORM_CD_I,                 "CD-i",                 "" },
  { PLATFORM_CHANNEL_F,            "Channel F",            "" },
  { PLATFORM_COLECO_VISION,        "ColecoVision",         "" },
  { PLATFORM_COMMODORE_128,        "Commodore 128",        "" },
  { PLATFORM_COMMODORE_64,         "Commodore 64",         "" },
  { PLATFORM_COMMODORE_PET_CBM,    "Commodore PET/CBM",    "" },
  { PLATFORM_DOJA,                 "DoJa",                 "" },
  { PLATFORM_DOS,                  "DOS",                  "" },
  { PLATFORM_DRAGON_32_64,         "Dragon 32/64",         "" },
  { PLATFORM_DREAMCAST,            "Dreamcast",            "" },
  { PLATFORM_ELECTRON,             "Electron",             "" },
  { PLATFORM_EXEN,                 "ExEn",                 "" },
  { PLATFORM_GAMEBOY,              "Game Boy",             "gb" },
  { PLATFORM_GAMEBOY_ADVANCE,      "Game Boy Advance",     "gba|agb|elf|mb|bin" },
  { PLATFORM_GAMEBOY_COLOR,        "Game Boy Color",       "gbc|cgb|sgb" },
  { PLATFORM_GAMECUBE,             "GameCube",             "" },
  { PLATFORM_GAME_GEAR,            "Game Gear",            "" },
  { PLATFORM_GENESIS,              "Genesis",              "" },
  { PLATFORM_GIZMONDO,             "Gizmondo",             "" },
  { PLATFORM_INTELLIVISION,        "Intellivision",        "" },
  { PLATFORM_JAGUAR,               "Jaguar",               "" },
  { PLATFORM_LINUX,                "Linux",                "" },
  { PLATFORM_LYNX,                 "Lynx",                 "" },
  { PLATFORM_MACINTOSH,            "Macintosh",            "" },
  { PLATFORM_MAME,                 "MAME",                 "" },
  { PLATFORM_MOPHUN,               "Mophun",               "" },
  { PLATFORM_MSX,                  "MSX",                  "" },
  { PLATFORM_NEO_GEO,              "Neo Geo",              "" },
  { PLATFORM_NEO_GEO_CD,           "Neo Geo CD",           "" },
  { PLATFORM_NEO_GEO_POCKET,       "Neo Geo Pocket",       "" },
  { PLATFORM_NEO_GEO_POCKET_COLOR, "Neo Geo Pocket Color", "" },
  { PLATFORM_NES,                  "NES",                  "" },
  { PLATFORM_N_GAGE,               "N-Gage",               "" },
  { PLATFORM_NINTENDO_64,          "Nintendo 64",          "" },
  { PLATFORM_NINTENDO_DS,          "Nintendo DS",          "" },
  { PLATFORM_NINTENDO_DSI,         "Nintendo DSi",         "" },
  { PLATFORM_ODYSSEY,              "Odyssey",              "" },
  { PLATFORM_ODYSSEY_2,            "Odyssey 2",            "" },
  { PLATFORM_PC_88,                "PC-88",                "" },
  { PLATFORM_PC_98,                "PC-98",                "" },
  { PLATFORM_PC_BOOTER,            "PC Booter",            "" },
  { PLATFORM_PC_FX,                "PC-FX",                "" },
  { PLATFORM_PLAYSTATION,          "PlayStation",          "" },
  { PLATFORM_PLAYSTATION_2,        "PlayStation 2",        "" },
  { PLATFORM_PLAYSTATION_3,        "PlayStation 3",        "" },
  { PLATFORM_PSP,                  "PSP",                  "" },
  { PLATFORM_SEGA_32X,             "SEGA 32X ",            "" },
  { PLATFORM_SEGA_CD,              "SEGA CD",              "" },
  { PLATFORM_SEGA_MASTER_SYSTEM,   "SEGA Master System",   "" },
  { PLATFORM_SEGA_SATURN,          "SEGA Saturn",          "" },
  { PLATFORM_SNES,                 "SNES",                 "smc|sfc|fig|gd3|gd7|dx2|bsx|swc" },
  { PLATFORM_SPECTRAVIDEO,         "Spectravideo",         "" },
  { PLATFORM_TI_99_4A,             "TI-99/4A",             "" },
  { PLATFORM_TRS_80,               "TRS-80",               "" },
  { PLATFORM_TRS_80_COCO,          "TRS-80 CoCo",          "" },
  { PLATFORM_TURBOGRAFX_16,        "TurboGrafx-16",        "" },
  { PLATFORM_TURBOGRAFX_CD,        "TurboGrafx CD",        "" },
  { PLATFORM_VECTREX,              "Vectrex",              "" },
  { PLATFORM_VIC_20,               "VIC-20",               "" },
  { PLATFORM_VIRTUAL_BOY,          "Virtual Boy",          "" },
  { PLATFORM_V_SMILE,              "V.Smile",              "" },
  { PLATFORM_WII,                  "Wii",                  "" },
  { PLATFORM_WINDOWS,              "Windows",              "" },
  { PLATFORM_WINDOWS_3X,           "Windows 3.x",          "" },
  { PLATFORM_WONDERSWAN,           "WonderSwan",           "" },
  { PLATFORM_WONDERSWAN_COLOR,     "WonderSwan Color",     "" },
  { PLATFORM_XBOX,                 "Xbox",                 "" },
  { PLATFORM_XBOX_360,             "Xbox 360",             "" },
  { PLATFORM_ZEEBO,                "Zeebo",                "" },
  { PLATFORM_ZODIAC,               "Zodiac",               "" },
  { PLATFORM_ZX_SPECTR,            "ZX Spectr",            "" },
};

/* TEMPORARY */
// Remove this struct when libretro has an API call to query the number of
// controller ports a game supports. If this code is still here in six months,
// Garrett will be very unhappy. I found a buffer overflow in SNES9x when
// trying to set controller ports 3-8, so this API call needs to happen.
struct PortMapping
{
  GamePlatform platform;
  int          ports;
};

static const PortMapping ports[] =
{
  { PLATFORM_GAMEBOY,              1 },
  { PLATFORM_GAMEBOY_COLOR,        1 },
  { PLATFORM_GAMEBOY_ADVANCE,      1 },
  { PLATFORM_NEO_GEO_POCKET_COLOR, 1 },
  { PLATFORM_SEGA_MASTER_SYSTEM,   2 },
  { PLATFORM_SNES,                 2 },
};

CGameManager CGameManager::m_gameManagerInstance;

/* static */
CGameManager &CGameManager::Get()
{
  return m_gameManagerInstance;
}

void CGameManager::RegisterAddons(const VECADDONS &addons)
{
  for (VECADDONS::const_iterator it = addons.begin(); it != addons.end(); it++)
    RegisterAddon(boost::dynamic_pointer_cast<CGameClient>(*it));
}

void CGameManager::RegisterAddon(GameClientPtr clientAddon)
{
  // Sanity check
  if (!clientAddon)
    return;

  CSingleLock lock(m_critSection);

  // If we are already tracking the add-on, erase it so we can refresh the data
  for (std::vector<GameClientObject>::const_iterator it = m_gameClients.begin(); it != m_gameClients.end(); it++)
  {
    if (clientAddon->ID() == it->id)
    {
      CLog::Log(LOGERROR, "CGameManager: Refreshing information for add-on %s!", clientAddon->ID());
      m_gameClients.erase(it);
      break;
    }
  }

  // Load the DLL
  if (!clientAddon->Init())
  {
    CLog::Log(LOGERROR, "CGameManager: failed to load the DLL for add-on %s", clientAddon->ID());
    return;
  }

  GameClientObject clientObject;
  clientObject.id = clientAddon->ID();
  clientObject.extensions = clientAddon->GetExtensions();
  TranslatePlatformArray(clientAddon->GetPlatforms(), clientObject.paltforms);

  // Currently we only store these three fields, so we're all done here
  m_gameClients.push_back(clientObject);

  // Unload the DLL
  clientAddon->DeInit();
}

void CGameManager::UnregisterAddonByID(const CStdString &ID)
{
  CSingleLock lock(m_critSection);

  for (std::vector<GameClientObject>::iterator it = m_gameClients.begin(); it != m_gameClients.end(); it++)
    if (ID == it->id)
      { m_gameClients.erase(it); return; }
  CLog::Log(LOGERROR, "CGameManager: can't unregister %s - not registered!", ID.c_str());
}

GameClientPtr CGameManager::GetGameClient(const CFileItem& file) const
{
  CStdStringArray candidates = GetGameClientIDs(file);

  AddonPtr addon;
  if (candidates.size() == 1)
  {
    CAddonMgr::Get().GetAddon(candidates[0], addon, ADDON_GAMEDLL);
    return boost::dynamic_pointer_cast<CGameClient>(addon);
  }
  else if (candidates.size() > 1)
  {
    CLog::Log(LOGINFO, "GameManager: Multiple game clients found, using %s", candidates[0].c_str());
    CLog::Log(LOGINFO, "GameManager: Candidates were %s", StringUtils::JoinString(candidates, ", ").c_str());
    CAddonMgr::Get().GetAddon(candidates[0], addon, ADDON_GAMEDLL);
    return boost::dynamic_pointer_cast<CGameClient>(addon);
  }
  return GameClientPtr();
}

CStdStringArray CGameManager::GetGameClientIDs(const CFileItem& file) const
{
  // Look for a "platform" hint in the file item
  GamePlatform platformHint = PLATFORM_UNKNOWN;
  CVariant varPlatform(file.GetProperty("platform"));
  if (varPlatform.isString())
    platformHint = GetPlatform(varPlatform.asString());

  // Get the file extension
  CStdString strFile = file.GetPath();
  CStdString strExtension;
  URIUtils::GetExtension(strFile, strExtension);
  strExtension.TrimLeft('.');
  strExtension.ToLower();
  
  // TODO: Use platformInfo.extensions as a platform hint, if needed

  CSingleLock lock(m_critSection);

  CStdStringArray candidates; // Container to hold discovered client IDs
  for (std::vector<GameClientObject>::const_iterator it = m_gameClients.begin(); it != m_gameClients.end(); it++)
  {
    // If a platform hint was given, and a game client specifies a platform
    // (doesn't omit the <platforms> tag from its addon.xml), and the game
    // client doesn't list the platform, then filter it out.
    if ((platformHint != PLATFORM_UNKNOWN) && (!it->paltforms.empty()) &&
        (std::find(it->paltforms.begin(), it->paltforms.end(), platformHint) == it->paltforms.end()))
      continue;

    // If the game client lists supported extensions, filter by those as well
    if ((!strExtension.empty()) &&
        (std::find(it->extensions.begin(), it->extensions.end(), strExtension) == it->extensions.end()))
      continue;

    candidates.push_back(it->id);
  }
  return candidates;
}

/* static */
GamePlatform CGameManager::GetPlatform(const CStdString &strPlatform)
{
  // Search our structure, platformInfo
  for (size_t i = 0; i < sizeof(platformInfo) / sizeof(platformInfo[0]); i++)
    if (SanitizedEquals(strPlatform.c_str(), platformInfo[i].name))
      return platformInfo[i].id;
  return PLATFORM_UNKNOWN;
}

#define IS_ALPHANUMERIC(c) (('a' <= (c) && (c) <= 'z') || ('A' <= (c) && (c) <= 'Z') || ('0' <= (c) && (c) <= '0'))
#define LOWER(c) (('A' <= (c) && (c) <= 'Z') ? (c) - 'A' + 'a' : (c))

/* static */
bool CGameManager::SanitizedEquals(const char *str1, const char *str2)
{
  // Sanity check
  if (!str1 || !str2)
    return false;

  // Break at the first null character
  for (; *str1 && *str2; )
  {
    // Advance to the next alphanumeric character
    while (*str1 && !IS_ALPHANUMERIC(*str1))
      str1++;
    while (*str2 && !IS_ALPHANUMERIC(*str2))
      str2++;

    // If they differ, we're done here, otherwise increment and continue
    if (LOWER(*str1) != LOWER(*str2))
      return false;

    str1++;
    str2++;
  }

  // Final test, return true if these are both null
  return *str1 == *str2;
}

/* static */
void CGameManager::TranslatePlatformArray(const CStdStringArray &strPlatforms, GamePlatformArray &vecPlatforms)
{
  vecPlatforms.clear();
  for (CStdStringArray::const_iterator it = strPlatforms.begin(); it != strPlatforms.end(); it++)
  {
    GamePlatform id = GetPlatform(*it);
    if (id != PLATFORM_UNKNOWN)
      vecPlatforms.push_back(id);
  }
}

/* static */
unsigned int CGameManager::GetPlayers(GamePlatform platform)
{
  for (size_t i = 0; i < sizeof(ports) / sizeof(ports[0]); i++)
    if (ports[i].platform == platform)
      return ports[i].ports;
  return 0; // Unknown
}
