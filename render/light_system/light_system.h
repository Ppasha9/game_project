/*
 * Game project
 *
 * FILE: light_system.h
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 13.04.2018
 * NOTE: light system declaration file
 */

#pragma once

#include "..\..\def.h"

/* Render handle namespace */
namespace render
{
  /* Light system class */
  class LightSystem
  {
  public:
    static const int MaxLights = 5;

    /* Point light source class */
    struct LightSource
    {
    public:
      math::Vec3f _pos;
      float _w;
      math::Vec3f _color;
      float _radius;

      /* Create light source function */
      LightSource( void ) {}

      /* Create light source function */
      LightSource( const math::Vec3f &Pos, float Radius, const math::Vec3f &Color );
    }; /* End of 'LightSource' class */

  private:
    LightSource *_lights;

  public:
    /* Initialize light system function */
    LightSystem & init( LightSource *Lights );

    /* Set light source parameters function */
    LightSystem & setLight( int Id, const LightSource &Light );

    /* Set light source parameters function */
    LightSystem & moveLight( int Id, const math::Vec3f &Translation );

    /* Set light source parameters function */
    LightSystem & setLightRadius( int Id, float Radius );

    /* Set light source parameters function */
    LightSystem & setLightPos( int Id, const math::Vec3f &Pos );

    /* Set light source parameters function */
    LightSystem & setLightColor( int Id, const math::Vec3f &Color );
  }; /* End of 'LightSystem' class */
}; /* end of 'render' namespace */

/* END OF 'light_system.h' FILE */
