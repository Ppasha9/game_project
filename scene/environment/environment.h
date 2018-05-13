/*
 * Game project
 *
 * FILE: environment.h
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 13.05.2018
 * NOTE: scene environment class declaration file
 */

#pragma once

#include "..//..//def.h"
#include "..//..//render/render.h"

/* Scene namespace */
namespace scene
{
  /* Scene environment class */
  class Environment
  {
  private:
    static const float _width;
    static const float _standsWidth;
    static const float _standsHeight;
    static const float _length;
    static const float _height;
    static const float _goalWidth;
    static const float _goalHeight;
    static const float _goalDepth;

    render::PrimPtr
      _wallPrim,
      _goalPrim,
      _floorPrim,
      _purplePrim,
      _yellowPrim,
      _petrPrim;

  public:
    static const math::Vec3f _center;
    static const float _radius;

    /* Create environment function */
    Environment( void );

    /* Draw environment function */
    void draw( void );
  }; /* End of 'Environment' class */
} /* End of 'scene' namespace */

/* END OF 'environment.h' FILE */