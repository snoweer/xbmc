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

#include "games/libretro/libretro.h"
#include "guilib/Key.h"
#include "threads/Thread.h"

#include <stdint.h>

// From dinput.h
#define GAMEPAD_BUTTON_COUNT 128
#define GAMEPAD_HAT_COUNT    4

class CRetroPlayerInput
{
public:
  CRetroPlayerInput();
  ~CRetroPlayerInput() { Finish(); }

  void Begin();
  void Finish();

  /**
   * Called by the game client to query gamepad states.
   * \param port   The player #. Player 1 is port 0.
   * \param device The fundamental device abstraction. This can be changed from
   *               the default by calling CGameClient::SetDevice().
   * \param index  Only used for analog devices (RETRO_DEVICE_ANALOG)
   * \param id     The button ID being queried.
   * \return       Although this returns int16_t, it seems game clients cast
   *               to bool. Therefore, 0 = not pressed, 1 = pressed.
   */
  int16_t GetInput(unsigned port, unsigned device, unsigned index, unsigned id);

  /**
   * Marks a key as pressed.
   */
  void ProcessKeyDown(const CKey &key);

  /**
   * Marks a key as released.
   */
  void ProcessKeyUp(const CKey &key);

  void ProcessGamepad(const std::string &device, const unsigned char buttons[GAMEPAD_BUTTON_COUNT],
    int numHats, const unsigned long hats[4]);

private:
  struct Hat
  {
    Hat() { Center(); }
    void Center() { up = right = down = left = 0; }
    bool IsCentered() { return *this == Hat(); }
    bool operator==(const Hat &lhs) const { return up == lhs.up && right == lhs.right && down == lhs.down && left == lhs.left; }
    bool operator!=(const Hat &lhs) const { return !(*this == lhs); }
    unsigned char &operator[](unsigned int i)
    {
      switch (i)
      {
      case 0: return up;
      case 1: return right;
      case 2: return down;
      case 3: return left;
      default: return up;
      }
    }

    unsigned char up;
    unsigned char right;
    unsigned char down;
    unsigned char left;
  };

  int TranslateActionID(int id) const;

  bool m_bActive;

  // RETRO_DEVICE_ID_JOYPAD_R3 is the last key in libretro.h
  int16_t       m_joypadState[ACTION_JOYPAD_CONTROL_END - ACTION_GAME_CONTROL_START + 1];
  unsigned char m_buttonState[GAMEPAD_BUTTON_COUNT]; 
  Hat           m_hatState[GAMEPAD_HAT_COUNT];

  CCriticalSection m_statesGuard;
};
