/*
 * Game project
 *
 * FILE: render_prim.cpp
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 24.03.2018
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
  //P->_material = nullptr;
  P->_shader = createShader("test_shader");

  _primitives[PrimName] = P;

  return P;
} /* End of 'Render::createPrim' function */

/* Get primitive interface function */
PrimPtr Render::getPrim( const string &PrimName ) const
{
  return getRes<Prim>(PrimName, _primitives);
} /* End of 'Render::getPrim' function */

/* Set primitive shader function */
void Render::setPrimShader( PrimPtr &Prim, Shader *NewShader )
{
} /* End of 'Render::setPrimShader' function */

/* Set primitive material function */
void Render::setPrimMaterial( PrimPtr &Prim, Material *NewMaterial )
{
} /* End of 'Render::setPrimMaterial' function */

/* Draw primitive function */
void Render::drawPrim( Prim *P )
{
  setShader(P->_shader._resource);
  drawGeom(P->_geometry._resource);
} /* End of 'Render::drawPrim' function */

/* Realease primitive function */
void Render::releasePrim( PrimPtr &P )
{
  releaseRes<Prim>(P, releasePrim, _primitives);
} /* End of 'Render::releasePrim' function */

/* END OF 'render_prim.cpp' FILE */
