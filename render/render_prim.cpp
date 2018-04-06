/*
 * Game project
 *
 * FILE: render_prim.cpp
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 01.04.2018
 * NOTE: render primitive resource handle implementation file
 */

#include "render.h"

using namespace render;

/* Create primitive function */
PrimPtr Render::createPrim( const string &PrimName )
{
  PrimPtr tmp;

  if ((tmp = getPrim(PrimName))._resource != nullptr)
    return tmp;

  Prim *P = new Prim(PrimName);

  P->_geometry = createGeom(PrimName);
  P->_material = createMaterial("test_material", {{0.01f, 0.01f, 0.01f, 1}, {0.69f, 0, 0, 1}, {0.7f, 0.7f, 0.7f, 1}, 1000});
  //P->_material = createMaterial("test_material", {{0.01f, 0.01f, 0.01f}, {0.69f, 0, 0}, {0.7f, 0.7f, 0.7f}, 1000});
  P->_shader = createShader("test_shader");

  _primitives[PrimName] = P;

  return P;
} /* End of 'Render::createPrim' function */

/* Get primitive interface function */
PrimPtr Render::getPrim( const string &PrimName ) const
{
  return getRes<Prim>(PrimName, _primitives);
} /* End of 'Render::getPrim' function */

/* Set primitive world matrix function */
void Render::setPrimMatrix( PrimPtr &P, const math::Matr4f &World )
{
  P._resource->_world = World;
} /* End of 'setPrimMatrix' function */

/* Set primitive shader function */
void Render::setPrimShader( PrimPtr &P, ShaderPtr &NewShader )
{
  P._resource->_shader = NewShader;
} /* End of 'Render::setPrimShader' function */

/* Set primitive material function */
void Render::setPrimMaterial( PrimPtr &P, MaterialPtr &NewMaterial )
{
} /* End of 'Render::setPrimMaterial' function */

/* Draw primitive function */
void Render::drawPrim( Prim *P )
{
  _constBuffer._data._world = P->_world;
  setMaterial(P->_material);

  updateConstBuffer();
  setShader(P->_shader._resource);
  drawGeom(P->_geometry._resource);
} /* End of 'Render::drawPrim' function */

/* Realease primitive function */
void Render::releasePrim( PrimPtr &P )
{
  releaseRes<Prim>(P, releasePrim, _primitives);
} /* End of 'Render::releasePrim' function */

/* END OF 'render_prim.cpp' FILE */
