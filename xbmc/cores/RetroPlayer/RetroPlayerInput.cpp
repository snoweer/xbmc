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

#include "RetroPlayerInput.h"
#include "games/libretro/libretro.h"
#include "guilib/GUIWindowManager.h"
#include "input/ButtonTranslator.h"
#include "input/KeyboardStat.h"
#include "utils/log.h"

#if defined(TARGET_WINDOWS)
#include "input/windows/WINJoystick.h"
#elif defined(HAS_SDL_JOYSTICK) || defined(HAS_EVENT_SERVER)
#include "SDLJoystick.h"
#endif

#define JOY_POV_360  JOY_POVBACKWARD * 2
#define JOY_POV_NE   (JOY_POVFORWARD + JOY_POVRIGHT) / 2
#define JOY_POV_SE   (JOY_POVRIGHT + JOY_POVBACKWARD) / 2
#define JOY_POV_SW   (JOY_POVBACKWARD + JOY_POVLEFT) / 2
#define JOY_POV_NW   (JOY_POVLEFT + JOY_POV_360) / 2


CRetroPlayerInput::CRetroPlayerInput() : m_bActive(false)
{
}

void CRetroPlayerInput::Begin()
{
  memset(m_joypadState, 0, sizeof(m_joypadState));
  memset(m_buttonState, 0, sizeof(m_buttonState));
  memset(m_hatState,    0, sizeof(m_hatState));
  m_bActive = true;
}

void CRetroPlayerInput::Finish()
{
  m_bActive = false;
}

int16_t CRetroPlayerInput::GetInput(unsigned port, unsigned device, unsigned index, unsigned id)
{
  CSingleLock lock(m_statesGuard);

  if (port == 0)
  {
    device &= RETRO_DEVICE_MASK;

    switch (device)
    {
    case RETRO_DEVICE_JOYPAD:
      if (id < sizeof(m_joypadState) / sizeof(m_joypadState[0]))
        return m_joypadState[id];
      else
        CLog::Log(LOGDEBUG, "RetroPlayerInput: GetInput() called with invalid ID: %d", id);
      break;
    default: // Only RETRO_DEVICE_JOYPAD is supported currently
      CLog::Log(LOGDEBUG, "RetroPlayerInput: GetInput() called with invalid device: %d", device);
      break;
    }
  }
  return 0;
}

void CRetroPlayerInput::ProcessKeyDown(const CKey &key)
{
  // TODO: Use ID of current window, in case it defines joypad keys outside of
  // <FullscreenGame>. Maybe, if current window ID finds no action, then fallback
  // to WINDOW_FULLSCREEN_GAME.
  CAction action = CButtonTranslator::GetInstance().GetAction(WINDOW_FULLSCREEN_GAME, key);

  CSingleLock lock(m_statesGuard);

  int id = TranslateActionID(action.GetID());
  if (0 <= id && id < (int)(sizeof(m_joypadState) / sizeof(m_joypadState[0])))
  {
    CLog::Log(LOGDEBUG, "RetroPlayerInput: KeyDown, action=%s, ID=%d", action.GetName().c_str(), id);
    m_joypadState[id] = 1;
  }
  else
  {
    CLog::Log(LOGDEBUG, "RetroPlayerInput: Invalid KeyDown, action=%s, ID=%d", action.GetName().c_str(), action.GetID());
  }
}

void CRetroPlayerInput::ProcessKeyUp(const CKey &key)
{
  // TODO: Use ID of current window, in case it defines joypad keys outside of
  // <FullscreenGame>. Maybe, if current window ID finds no action, then fallback
  // to WINDOW_FULLSCREEN_GAME.
  CAction action = CButtonTranslator::GetInstance().GetAction(WINDOW_FULLSCREEN_GAME, key);

  CSingleLock lock(m_statesGuard);

  int id = TranslateActionID(action.GetID());
  if (0 <= id && id < (int)(sizeof(m_joypadState) / sizeof(m_joypadState[0])))
  {
    CLog::Log(LOGDEBUG, "RetroPlayerInput: KeyUp, action=%s, ID=%d", action.GetName().c_str(), id);
    m_joypadState[id] = 0;
  }
  else
  {
    CLog::Log(LOGDEBUG, "RetroPlayerInput: Invalid KeyUp, action=%s, ID=%d", action.GetName().c_str(), action.GetID());
  }
}

void CRetroPlayerInput::ProcessGamepad(const std::string &device, const unsigned char buttons[GAMEPAD_BUTTON_COUNT],
  int numHats, const unsigned long hats[4])
{
  // Only process buttons sent to our window
  // TODO: Same fallback procedure as the one mentioned above
  int window = !g_windowManager.HasModalDialog() ? g_windowManager.GetActiveWindow() : g_windowManager.GetTopMostModalDialogID();

  CSingleLock lock(m_statesGuard);

  for (unsigned int bid = 0; bid < GAMEPAD_BUTTON_COUNT; bid++)
  {
    // We only care if a change in state is detected
    unsigned char bPressed = buttons[bid] & 0x80 ? 1 : 0;
    if (bPressed == m_buttonState[bid])
      continue;

    // Don't record presses outside of fullscreen video (unpresses are ok)
    if (bPressed && (window & WINDOW_ID_MASK) != WINDOW_FULLSCREEN_VIDEO)
      continue;

    // Record the new state
    m_buttonState[bid] = bPressed;

    int        actionID;
    CStdString actionName;
    bool       fullrange; // unused
    // Actual button ID is bid + 1
    if (!CButtonTranslator::GetInstance().TranslateJoystickString(WINDOW_FULLSCREEN_VIDEO,
        device.c_str(), bid + 1, JACTIVE_BUTTON, actionID, actionName, fullrange))
      continue;

    int id = TranslateActionID(actionID);
    if (0 <= id && id < (int)(sizeof(m_joypadState) / sizeof(m_joypadState[0])))
    {
      // Record the new joypad state
      m_joypadState[id] = bPressed;
      CLog::Log(LOGDEBUG, "RetroPlayerInput: %s, action=%s, ID=%d",
        bPressed ? "button press" : "button unpress", actionName.c_str(), actionID);
    }
    else
    {
      CLog::Log(LOGDEBUG, "RetroPlayerInput: Invalid %s, action=%s, ID=%d",
        bPressed ? "button press" : "button unpress", actionName.c_str(), actionID);
    }
  }

  // numHats is capped at 4 in WINJoystick.h, but just in case...
  if (numHats < 0)
    numHats = 0;
  else if (numHats > 4)
    numHats = 4;

  for (unsigned int iHat = 0; iHat < numHats; iHat++)
  {
    // Unpack unsigned long hats[iHat] into a Hat struct
    Hat hat;
    bool bCentered = (hats[iHat] & 0xFFFF == 0xFFFF);
    if (!bCentered)
    {
      if ((JOY_POV_NW <= hats[iHat] && hats[iHat] <= JOY_POV_360) || hats[iHat] <= JOY_POV_NE)
        hat.up = 1;
      if (JOY_POV_NE <= hats[iHat] && hats[iHat] <= JOY_POV_SE)
        hat.right = 1;
      if (JOY_POV_SE <= hats[iHat] && hats[iHat] <= JOY_POV_SW)
        hat.down = 1;
      if (JOY_POV_SW <= hats[iHat] && hats[iHat] <= JOY_POV_NW)
        hat.left = 1;
    }

    // Using ordinal directions instead of cardinal directions lets us use a for loop
    for (unsigned int i = 0; i < 4; i++)
    {
      // We only care if a change in state is detected
      if (hat[i] == m_hatState[iHat][i])
        continue;

      // Don't record presses outside of fullscreen video (unpresses are ok)
      if (hat[i] && (window & WINDOW_ID_MASK) != WINDOW_FULLSCREEN_VIDEO)
        continue;

      // Record the new state
      m_hatState[iHat][i] = hat[i];

      // Compose button ID (SDL_HAT_UP is (1 << 0), SDL_HAT_RIGHT is (1 << 1), etc.)
      int        buttonID = (1 << i) << 16 | (iHat + 1); // Hat ID is iHat + 1
      int        actionID;
      CStdString actionName;
      bool       fullrange; // unused
      // Actual button ID is bid + 1
      if (!CButtonTranslator::GetInstance().TranslateJoystickString(WINDOW_FULLSCREEN_VIDEO,
          device.c_str(), buttonID, JACTIVE_HAT, actionID, actionName, fullrange))
        continue;

      int id = TranslateActionID(actionID);
      if (0 <= id && id < (int)(sizeof(m_joypadState) / sizeof(m_joypadState[0])))
      {
        // Record the new joypad state
        m_joypadState[id] = hat[i];
        CLog::Log(LOGDEBUG, "RetroPlayerInput: %s, action=%s, ID=%d",
          hat[i] ? "hat press" : "hat unpress", actionName.c_str(), actionID);
      }
      else
      {
        CLog::Log(LOGDEBUG, "RetroPlayerInput: Invalid %s, action=%s, ID=%d",
          hat[i] ? "hat press" : "hat unpress", actionName.c_str(), actionID);
      }
    }
  }
}

int CRetroPlayerInput::TranslateActionID(int id) const
{
  int m_device = RETRO_DEVICE_JOYPAD; // Until we keep track of multiple devices

  switch (m_device)
  {
  case RETRO_DEVICE_JOYPAD:
    return id - ACTION_JOYBAD_B;
  case RETRO_DEVICE_MOUSE:
    return id - ACTION_MOUSE_CONTROLLER_X;
  case RETRO_DEVICE_LIGHTGUN:
    return id - ACTION_LIGHTGUN_X;
  case RETRO_DEVICE_ANALOG:
    return id - ACTION_ANALOG_CONTROLLER_LEFT;
  case RETRO_DEVICE_KEYBOARD:
    //return id - ACTION_JOYBAD_B; // Keyboard is trickier
  default:
    break;
  }
  return -1; // Invalid device ID
}
