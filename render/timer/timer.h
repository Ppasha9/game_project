/*
 * Game project
 *
 * FILE: timer.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 22.04.2018
 * NOTE: timer handle class declaration file
 */

#ifndef _TIMER_H_INCLUDED__
#define _TIMER_H_INCLUDED__
#pragma once

/* Render's namespace */
namespace render
{
  /* Timer handle class */
  class Timer
  {
  public:
    /* Maximum delta that we have to deal with */
    static const float MAX_ENABLED_DELTA_TIME;

  private:
    __int64
      _timeFreq,         // timer frequency
      _timeStart,        // start time
      _timeOld,          // temperary time
      _timePause,        // pause time
      _timeFPS;          // FPS time
    int _frameCounter;   // frame counter

    /* Class constructor */
    Timer(void);

  public:
    /* Getting timer class instance function */
    static Timer & getInstance(void);

    /* Time system */
    double
      _time,             // the main time
      _globalTime,       // global main time
      _deltaTime,        // delta time
      _globalDeltaTime,  // global delta time
      _fps;              // Frame Per Second
    bool _isPause;       // flag for pause

    /* Response timer function */
    void response(void);

    /* Increasing the counter of frame */
    void incrFrame(void);
  }; /* End of 'Timer' class */
} /* End of 'render' namespace */

#endif /* _TIMER_H_INCLUDED__ */

/* END OF 'timer.h' FILE */
