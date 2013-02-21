/*
 *      Copyright (C) 2013 Team XBMC
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

// Code is by Hans-Kristian Arntzen

#include "SerialState.h"

void CSerialState::Init(size_t frameSize, size_t frameCount)
{
  Reset();
  m_frameSize = frameSize;
  m_maxFrames = frameCount;
  m_state.resize((m_frameSize + sizeof(uint32_t) - 1) / sizeof(uint32_t)); // Pad to 4 bytes
  m_nextState.resize((m_frameSize + sizeof(uint32_t) - 1) / sizeof(uint32_t)); // Pad to 4 bytes
}

void CSerialState::Reset()
{
  m_state.clear();
  m_nextState.clear();
  m_rewindBuffer.clear();
}

void CSerialState::AdvanceFrame()
{
  m_rewindBuffer.push_back(DeltaPairVector());
  DeltaPairVector& buffer = m_rewindBuffer.back();

  // Remember, we padded these to the next 32 bytes
  size_t stateSize = m_state.size();
  for (size_t i = 0; i < stateSize; i++)
  {
    uint32_t xor_val = m_state[i] ^ m_nextState[i];
    if (xor_val)
      buffer.push_back(DeltaPair(i, xor_val));
  }

  m_state = m_nextState; // Can be optimized with std::move() in C++11 or some ring-buffer-esque thing.

  while (m_rewindBuffer.size() > m_maxFrames)
    m_rewindBuffer.pop_front();
}

unsigned int CSerialState::RewindFrames(unsigned int frameCount)
{
  unsigned int rewound = 0;
  while (frameCount > 0 && !m_rewindBuffer.empty())
  {
    const DeltaPairVector& buffer = m_rewindBuffer.back();

    size_t i;
    size_t bufferSize = buffer.size();
    for (i = 0; i + 8 <= bufferSize; i += 8)
    {
      m_state[buffer[i + 0].first] ^= buffer[i + 0].second;
      m_state[buffer[i + 1].first] ^= buffer[i + 1].second;
      m_state[buffer[i + 2].first] ^= buffer[i + 2].second;
      m_state[buffer[i + 3].first] ^= buffer[i + 3].second;
      m_state[buffer[i + 4].first] ^= buffer[i + 4].second;
      m_state[buffer[i + 5].first] ^= buffer[i + 5].second;
      m_state[buffer[i + 6].first] ^= buffer[i + 6].second;
      m_state[buffer[i + 7].first] ^= buffer[i + 7].second;
    }

    for ( ; i < bufferSize; i++)
      m_state[buffer[i + 0].first] ^= buffer[i + 0].second;

    rewound++;
    frameCount--;
    m_rewindBuffer.pop_back();
  }

  return rewound;
}
