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

#include "LibMtpDirectory.h"

//#include <libmtp.h>
#include <vector>

namespace MEDIA_READER
{

bool CLibMtpDirectory::GetDirectory(const std::string& strPath,
                                    FILE_LIST& items,
                                    DIR_FLAG flags /* = DEFAULTS */,
                                    const std::set<std::string>& extensionsAllowed /* = std::set<std::string>() */)
{
  // TODO
  return false;
}

}
