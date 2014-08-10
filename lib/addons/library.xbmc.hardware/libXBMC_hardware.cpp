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

#include "addons/AddonCallbacks.h"
#include "addons/include/xbmc_hardware_types.h"

#include <stdio.h>

#ifdef _WIN32
  #include <windows.h>
  #define DLLEXPORT __declspec(dllexport)
#else
  #define DLLEXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT CB_HardwareLib* HARDWARE_register_me(AddonCB* frontend)
{
  CB_HardwareLib* cb = NULL;
  if (!frontend)
    fprintf(stderr, "ERROR: HARDWARE_register_frontend is called with NULL handle!!!\n");
  else
  {
    cb = frontend->HardwareLib_RegisterMe(frontend->addonData);
    if (!cb)
      fprintf(stderr, "ERROR: HARDWARE_register_frontend can't get callback table from frontend!!!\n");
  }
  return cb;
}

DLLEXPORT void HARDWARE_unregister_me(AddonCB* frontend, CB_HardwareLib* cb)
{
  if (frontend == NULL || cb == NULL)
    return;
  return frontend->HardwareLib_UnRegisterMe(frontend->addonData, cb);
}

DLLEXPORT void HARDWARE_media_insert(AddonCB* frontend, CB_HardwareLib* cb, const MEDIA_PORT_METADATA* metadata)
{
  if (frontend == NULL || cb == NULL)
    return;
  return cb->MediaInsert(frontend->addonData, metadata);
}

DLLEXPORT void HARDWARE_media_remove(AddonCB* frontend, CB_HardwareLib* cb, const MEDIA_PORT_METADATA* metadata)
{
  if (frontend == NULL || cb == NULL)
    return;
  return cb->MediaRemove(frontend->addonData, metadata);
}

#ifdef __cplusplus
}
#endif
