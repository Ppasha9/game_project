/*
 * Game project
 *
 * FILE: timer.cpp
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 22.04.2018
 * NOTE: timer handle class definition file
 */

#include <windows.h>
#include "timer.h"

using namespace render;

// Initializing the constant
const float Timer::MAX_ENABLED_DELTA_TIME = 0.05;

/* Class constructor */
Timer::Timer(void) : _time(0), _globalTime(0), _deltaTime(0), _globalDeltaTime(0), _fps(0), _timeFPS(0), _timeFreq(0), _timeStart(0),
  _timeOld(0), _timePause(0), _frameCounter(0), _isPause(false)
{
  LARGE_INTEGER li;

  QueryPerformanceFrequency(&li);
  _timeFreq = li.QuadPart;
  QueryPerformanceCounter(&li);
  _timeStart = _timeOld = _timeFPS = li.QuadPart;
  _frameCounter = 0;
} /* End of constructor */

/* Getting timer class instance function */
Timer & Timer::getInstance(void)
{
  static Timer instance;
  return instance;
} /* End of 'getInstance' function */

/* Response timer function */
void Timer::response(void)
{
  LARGE_INTEGER li;

  QueryPerformanceCounter(&li);
  _globalTime = (double)(li.QuadPart - _timeStart) / _timeFreq;
  _globalDeltaTime = (double)(li.QuadPart - _timeOld) / _timeFreq;

  if (!_isPause)
    _deltaTime = _globalDeltaTime;
  else
  {
    _timePause += li.QuadPart - _timeOld;
    _deltaTime = 0;
  }

  _time = (double)(li.QuadPart - _timePause - _timeStart) / _timeFreq;

  if (li.QuadPart - _timeFPS > _timeFreq)
  {
    _fps = (double)_frameCounter / ((double)(li.QuadPart - _timeFPS) / _timeFreq);
    _timeFPS = li.QuadPart;
    _frameCounter = 0;
  }

  _timeOld = li.QuadPart;
} /* End of 'response' function */

/* Increasing the counter of frame */
void Timer::incrFrame(void)
{
  _frameCounter++;
} /* End of 'incrFrame' function */

/* END OF 'timer.cpp' FILE */
