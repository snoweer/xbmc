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

#include "GameInfoTagLoader.h"
#include "GameboyTag.h"
#include "GBATag.h"
#include "games/GameManager.h"
#include "utils/log.h"
#include "utils/URIUtils.h"

using namespace GAME_INFO;

bool CGameInfoTagLoader::Load(const CStdString &strFileName, CGameInfoTag &tag)
{
  // GetSystemType() is static, but could become non-static in the future
  GamePlatform platform = CGameManager::GetPlatform(strFileName);
  CTag *myTag = NULL;

  switch (platform)
  {
  case PLATFORM_GAMEBOY:
  case PLATFORM_GAMEBOY_COLOR:
    myTag = new CGameboyTag();
    break;
  case PLATFORM_GAMEBOY_ADVANCE:
    myTag = new CGBATag();
    break;
  default:
    break;
  }

  try
  {
    if (myTag && myTag->Read(strFileName))
    {
      myTag->GetGameInfoTag(tag);
      delete myTag;
      return true;
    }
  }
  catch (...)
  {
    CLog::Log(LOGERROR, "CGameInfoTagLoader: exception in file %s", strFileName.c_str());
  }
  // If reading failed or an error was thrown, we may still have an allocated tag
  delete myTag;
  return false;
}
