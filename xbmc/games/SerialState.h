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
#pragma once

#include <deque>
#include <vector>
#include <stdint.h>
#include <stdlib.h>

class CSerialState
{
public:
  CSerialState() : m_frameSize(0), m_maxFrames(0) { }

  void Init(size_t frameSize, size_t frameCount);
  void Reset(); // Clear up any memory allocated

  uint8_t *GetState() { return reinterpret_cast<uint8_t*>(m_state.data()); }
  uint8_t *GetNextState() { return reinterpret_cast<uint8_t*>(m_nextState.data()); }
  size_t GetFrameSize() const { return m_frameSize; }
  size_t GetMaxFrames() const { return m_maxFrames; }
  size_t GetFramesAvailable() const { return m_rewindBuffer.size(); }

  void AdvanceFrame();
  unsigned int RewindFrames(unsigned int frameCount);

private:
  size_t m_frameSize;
  size_t m_maxFrames;

  std::vector<uint32_t> m_state;
  std::vector<uint32_t> m_nextState;

  /**
   * Rewinding is implemented by applying XOR deltas on the specific parts of
   * the save state buffer which has changed. In practice, this is very fast
   * and simple (linear scan) and allows deltas to be compressed down to 1-3%
   * of original save state size depending on the system. The algorithm runs on
   * 32 bits at a time for speed. The state buffer has a fixed number of frames.
   *
   * Use std::deque here to achieve amortized O(1) on pop/push to front and back.
   */
  typedef std::pair<size_t, uint32_t> DeltaPair;
  typedef std::vector<DeltaPair>      DeltaPairVector;
  std::deque<DeltaPairVector>         m_rewindBuffer;
};
