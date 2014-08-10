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
#ifndef XBMC_HARDWARE_CALLBACKS_H_
#define XBMC_HARDWARE_CALLBACKS_H_

#include "xbmc_hardware_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct CB_HardwareLib
{
  /*!
    * Called when a cartridge or disk is first detected by the media reader.
    * @param metadata The metadata of the new ROM and the port that detected it.
    */
  void (*MediaInsert)(void* addonData, const MEDIA_PORT_METADATA* metadata);

  /*!
    * Called when a cartridge or disk is removed from the media reader.
    * @param metadata The metadata of the ROM and its port before being removed
    */
  void (*MediaRemove)(void* addonData, const MEDIA_PORT_METADATA* metadata);

} CB_HardwareLib;

#ifdef __cplusplus
};
#endif

#endif // XBMC_HARDWARE_CALLBACKS_H_
