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

#define PAD_TO_CEIL(x, bytes) (((x) + (bytes) - 1) / (bytes))

void CSerialState::Init(size_t frameSize, size_t frameCount)
{
  Reset();
  m_frameSize = frameSize;
  m_maxFrames = frameCount;
  m_state.resize(PAD_TO_CEIL(m_frameSize, sizeof(uint32_t))); // Pad to 4 bytes
  m_nextState.resize(PAD_TO_CEIL(m_frameSize, sizeof(uint32_t))); // Pad to 4 bytes
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
    {
      DeltaPair pair = {i, xor_val};
      buffer.push_back(pair);
    }
  }

  memcpy(m_state.data(), m_nextState.data(), m_state.size());

  while (m_rewindBuffer.size() > m_maxFrames)
    m_rewindBuffer.pop_front();
}

unsigned int CSerialState::RewindFrames(unsigned int frameCount)
{
  unsigned int rewound = 0;
  while (frameCount > 0 && !m_rewindBuffer.empty())
  {
    const DeltaPair *buffer = m_rewindBuffer.back().data();

    size_t i;
    size_t bufferSize = m_rewindBuffer.back().size();
    // buffer pointer redirection violates data-dependency requirements... no
    // vecorization for us :(
    for (int i = 0; i < bufferSize; i++)
      m_state[buffer[i].pos] ^= buffer[i].delta;

    rewound++;
    frameCount--;
    m_rewindBuffer.pop_back();
  }

  return rewound;
}
