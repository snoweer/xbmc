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

#include "GameClient.h"

// Systems that XBMC is currently aware of. It is not required for this list
// to be all-inclusive; as long as a game core returns a list of valid
// extensions (see CGameClient::GetExtensions()), XBMC will be able to do the
// right thing.
typedef enum GameSystemType
{
  SYSTEM_UNKNOWN,
  SYSTEM_OTHER,
  SYSTEM_GameBoy,
  SYSTEM_GameBoyColor,
  SYSTEM_GameBoyAdvance,
  SYSTEM_SuperNintendo
};

/*!
 * Manages game clients and game info tags.
 */
class CGameManager
{
private:
  CGameManager() { }

  // Pertinent information captured by the game client addon. We use these
  // objects as a cache to avoid loading the DLL every time, as this is required
  // by the extensions list.
  struct GameClientObject
  {
    CStdString id;
    CStdString extensions;
    std::vector<GameSystemType> systems;
  };

  std::vector<GameClientObject> m_gameClients;

public:
  static CGameManager &Get();

  /*!
   * Create a personal cache of game client addon information. This requires
   * requries the DLL be loaded, and our goal is to only do this once on
   * startup or when addons are installed/enabled.
   */
  void RegisterAddons(const ADDON::VECADDONS &addons);

  void RegisterAddon(ADDON::GameClientPtr clientAddon);

  void UnregisterAddon(const CStdString &ID);

  /*!
   * Resolve a file path to a game client addon. The extension is used to
   * screen for compatible game clients. If a game client specifies no
   * extensions, but its system matches the system belonging to the file's
   * extension, then it is addde as a possible game client.

   */
  ADDON::GameClientPtr GetGameClient(const CStdString &strFile);

  /*!
   * Resolve an extension to a system type enum.
   * @param strExtension The extension. May be lower case and may contain or
   * omit a period.
   * @return SYSTEM_UNKNOWN if no system was found. SYSTEM_OTHER if a game
   * client was found, but didn't specify its system or its system type is 
   * unknown to XBMC. If a game client specifies multiple systems, only the
   * first is returned.
   */
  GameSystemType GetSystemTypeByExtension(const CStdString &strExtension);

  /*!
   * Resolve a system name (ASCII string) into a system type enum.
   * @param The system name, either the short name or the long name found in
   * the SystemMapping systems struct in GameManager.cpp. These strings are
   * the system names expected to be found in the <system> tag of the game
   * client's addon.xml
   */
  static GameSystemType GetSystemType(const CStdString &systemName);

  /*!
   * Returns true if strExt is in strExtensionList.
   * @param strExtensionList Like "gba|agb|elf|mb"
   * @param strExt           Like "gba", may contain a period or upper case
   */
  static bool ContainsExtesion(const CStdString &strExtensionList, const CStdString &strExt);

  /*!
   * Return the number of controller ports the specified system supports.
   * @return -1 if unknown
   */
  static int GetPlayers(GameSystemType system);
};
