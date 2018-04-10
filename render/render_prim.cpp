/*
 * Game project
 *
 * FILE: render_prim.cpp
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 09.04.2018
 * NOTE: render primitive resource handle implementation file
 */

#include "render.h"

using namespace render;

/* Create primitive function */
PrimPtr Render::createPrim( const string &PrimName,
  const string &MtlName, const string &ShName )
{
  PrimPtr tmp;

  if ((tmp = getPrim(PrimName))._resource != nullptr)
    return tmp;

  Prim *P = new Prim(PrimName);

  P->_geometry = createGeom(PrimName);
  P->_material = getMaterial(MtlName);
  P->_shader = getShader(ShName);

  _primitives.add(PrimName, P);

  return P;
} /* End of 'Render::createPrim' function */

/* Create primitive function */
PrimPtr Render::createPrim( const string &PrimName,
  const MaterialPtr &Mtl, const ShaderPtr &Sh )
{
  PrimPtr tmp;

  if ((tmp = getPrim(PrimName))._resource != nullptr)
    return tmp;

  Prim *P = new Prim(PrimName);

  P->_geometry = createGeom(PrimName);
  P->_material = Mtl;
  P->_shader = Sh;

  _primitives.add(PrimName, P);

  return P;
} /* End of 'Render::createPrim' function */

/* Get primitive interface function */
PrimPtr Render::getPrim( const string &PrimName ) const
{
  return _primitives.get(PrimName);
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
  P._resource->_material = NewMaterial;
} /* End of 'Render::setPrimMaterial' function */

/* Draw primitive function */
void Render::drawPrim( Prim *P )
{
  setShader(P->_shader);

  _constBuffer._data._world = P->_world;
  setMaterial(P->_material);

  updateConstBuffer();
  drawGeom(P->_geometry._resource);
} /* End of 'Render::drawPrim' function */

/* Realease primitive function */
void Render::releasePrim( PrimPtr &P )
{
  _primitives.release(P);
} /* End of 'Render::releasePrim' function */

/* END OF 'render_prim.cpp' FILE */
