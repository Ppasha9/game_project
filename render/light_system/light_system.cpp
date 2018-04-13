/*
 * Game project
 *
 * FILE: light_system.cpp
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 13.04.2018
 * NOTE: light system implementation file
 */

#include "light_system.h"

using namespace render;

/* Create light source function */
LightSystem::LightSource::LightSource( const math::Vec3f &Pos,
  float Radius, const math::Vec3f &Color ) : _pos(Pos), _w(1), _radius(Radius), _color(Color)
{
} /* End of 'LightSystem::LightSource::LightSource' function */

/* Initialize light system function */
LightSystem & LightSystem::init( LightSource *Lights )
{
  _lights = Lights;
  memset(_lights, 0, sizeof(LightSource) * MaxLights);

  return *this;
} /* End of 'LightSystem::init' function */

/* Set light source parameters function */
LightSystem & LightSystem::setLight( int Id, const LightSource &Light )
{
  if (Id >= 0 && Id < MaxLights)
    _lights[Id] = Light;

  return *this;
} /* End of 'LightSystem::setLight' function */

/* Set light source parameters function */
LightSystem & LightSystem::moveLight( int Id, const math::Vec3f &Translation )
{
  if (Id >= 0 && Id < MaxLights)
    _lights[Id]._pos += Translation;

  return *this;
} /* End of 'LightSystem::moveLight' function */

/* Set light source parameters function */
LightSystem & LightSystem::setLightRadius( int Id, float Radius )
{
  if (Id >= 0 && Id < MaxLights)
    _lights[Id]._radius = Radius;

  return *this;
} /* End of 'LightSystem::setLightRadius' function */

/* Set light source parameters function */
LightSystem & LightSystem::setLightPos( int Id, const math::Vec3f &Pos )
{
  if (Id >= 0 && Id < MaxLights)
    _lights[Id]._pos = Pos;

  return *this;
} /* End of 'LightSystem::setLightPos' function */

/* Set light source parameters function */
LightSystem & LightSystem::setLightColor( int Id, const math::Vec3f &Color )
{
  if (Id >= 0 && Id < MaxLights)
    _lights[Id]._color = Color;

  return *this;
} /* End of 'LightSystem::setLightColor' function */

/* END OF 'light_system.cpp' FILE */
