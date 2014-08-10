#pragma once
/*
 *      Copyright (C) 2005-2014 Team XBMC
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

#include "libXBMC_addon.h"
#include "xbmc_hardware_callbacks.h"

#include <string>
#include <stdio.h>

#if defined(ANDROID)
  #include <sys/stat.h>
#endif

#ifdef _WIN32
  #define HARDWARE_HELPER_DLL "\\library.xbmc.hardware\\libXBMC_hardware" ADDON_HELPER_EXT
#else
  #define HARDWARE_HELPER_DLL_NAME "libXBMC_hardware-" ADDON_HELPER_ARCH ADDON_HELPER_EXT
  #define HARDWARE_HELPER_DLL "/library.xbmc.hardware/" HARDWARE_HELPER_DLL_NAME
#endif

#define HARDWARE_REGISTER_SYMBOL(dll, functionPtr) \
  CHelper_libXBMC_hardware::RegisterSymbol(dll, functionPtr, #functionPtr)

class CHelper_libXBMC_hardware
{
public:
  CHelper_libXBMC_hardware(void)
  {
    m_handle       = NULL;
    m_callbacks    = NULL;
    m_libXBMC_hardware = NULL;
  }

  ~CHelper_libXBMC_hardware(void)
  {
    if (m_libXBMC_hardware)
    {
      HARDWARE_unregister_me(m_handle, m_callbacks);
      dlclose(m_libXBMC_hardware);
    }
  }

  template <typename T>
  static bool RegisterSymbol(void* dll, T& functionPtr, const char* strFunctionPtr)
  {
    return (functionPtr = (T)dlsym(dll, strFunctionPtr)) != NULL;
  }

  /*!
    * @brief Resolve all callback methods
    * @param handle Pointer to the add-on
    * @return True when all methods were resolved, false otherwise.
    */
  bool RegisterMe(void* handle)
  {
    m_handle = handle;

    std::string libBasePath;
    libBasePath  = ((cb_array*)m_handle)->libPath;
    libBasePath += HARDWARE_HELPER_DLL;

#if defined(ANDROID)
      struct stat st;
      if (stat(libBasePath.c_str(),&st) != 0)
      {
        std::string tempbin = getenv("XBMC_ANDROID_LIBS");
        libBasePath = tempbin + "/" + HARDWARE_HELPER_DLL_NAME;
      }
#endif

    m_libXBMC_hardware = dlopen(libBasePath.c_str(), RTLD_LAZY);
    if (m_libXBMC_hardware == NULL)
    {
      fprintf(stderr, "Unable to load %s\n", dlerror());
      return false;
    }

    try
    {
      if (!HARDWARE_REGISTER_SYMBOL(m_libXBMC_hardware, HARDWARE_register_me)) throw false;
      if (!HARDWARE_REGISTER_SYMBOL(m_libXBMC_hardware, HARDWARE_unregister_me)) throw false;
      if (!HARDWARE_REGISTER_SYMBOL(m_libXBMC_hardware, HARDWARE_media_insert)) throw false;
      if (!HARDWARE_REGISTER_SYMBOL(m_libXBMC_hardware, HARDWARE_media_remove)) throw false;
    }
    catch (const bool& bSuccess)
    {
      fprintf(stderr, "ERROR: Unable to assign function %s\n", dlerror());
      return bSuccess;
    }

    m_callbacks = HARDWARE_register_me(m_handle);
    return m_callbacks != NULL;
  }

  void MediaInsert(const MEDIA_PORT_METADATA* metadata)
  {
    return HARDWARE_media_insert(m_handle, m_callbacks, metadata);
  }

  void MediaRemove(const MEDIA_PORT_METADATA* metadata)
  {
    return HARDWARE_media_insert(m_handle, m_callbacks, metadata);
  }

protected:
    CB_HardwareLib* (*HARDWARE_register_me)(void* handle);
    void (*HARDWARE_unregister_me)(void* handle, CB_HardwareLib* cb);
    void (*HARDWARE_media_insert)(void* handle, CB_HardwareLib* cb, const MEDIA_PORT_METADATA* metadata);
    void (*HARDWARE_media_remove)(void* handle, CB_HardwareLib* cb, const MEDIA_PORT_METADATA* metadata);

private:
  void*           m_handle;
  CB_HardwareLib* m_callbacks;
  void*           m_libXBMC_hardware;

  struct cb_array
  {
    const char* libPath;
  };
};
