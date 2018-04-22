/*
 * Game project
 *
 * FILE: render_prim.cpp
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 22.04.2018
 * NOTE: render primitive resource handle implementation file
 */

#include "render.h"

using namespace render;

/* Create primitive function */
PrimPtr Render::createPrim( const string &PrimName, const string &GeomName,
  const string &MtlName, const string &ShName, Prim::ProjMode ProjM, Prim::FillMode FillM )
{
  PrimPtr tmp;

  if ((tmp = getPrim(PrimName))._resource != nullptr)
    return tmp;

  Prim *P = new Prim(PrimName);

  P->_geometry = createGeom(GeomName);
  P->_material = getMaterial(MtlName);
  P->_shader = getShader(ShName);
  P->_fillMode = FillM;
  P->_projMode = ProjM;
  P->_world = math::Matr4f().setIdentity();

  _primitives.add(PrimName, P);

  return P;
} /* End of 'Render::createPrim' function */

/* Create primitive function */
PrimPtr Render::createPrim( const string &PrimName, const GeomPtr &Geometry,
  const MaterialPtr &Mtl, const ShaderPtr &Sh, Prim::ProjMode ProjM, Prim::FillMode FillM )
{
  PrimPtr tmp;

  if ((tmp = getPrim(PrimName))._resource != nullptr)
    return tmp;

  Prim *P = new Prim(PrimName);

  P->_geometry = Geometry;
  P->_material = Mtl;
  P->_shader = Sh;
  P->_fillMode = FillM;
  P->_projMode = ProjM;
  P->_world = math::Matr4f().setIdentity();

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
  applyShader(P->_shader);
  _constBuffer._data._world = P->_world;
  applyMaterial(P->_material);

  setFillMode(P->_fillMode);

  updateConstBuffer();
  drawGeom(P->_geometry._resource);
} /* End of 'Render::drawPrim' function */

/* Set primitive fill mode function */
void Render::setPrimFillMode( PrimPtr &P, Prim::FillMode NewFillMode )
{
  P._resource->_fillMode = NewFillMode;
} /* End of 'Render::setPrimFillMode' function */

/* Register primitive for rendering on this frame function */
void Render::drawPrim( PrimPtr &P )
{
  if (P._resource == nullptr)
    return;

  switch (P._resource->_projMode)
  {
  case Prim::ProjMode::FRUSTUM:
    _frustumPrims.push_back(P._resource);
    break;
  case Prim::ProjMode::SCREENSPACE_PIXEL:
    _pixelPrims.push_back(P._resource);
    break;
  case Prim::ProjMode::SCREENSPACE_UNORM:
    _unormPrims.push_back(P._resource);
    break;
  }
} /* End of 'Render::drawPrim' function */

/* Set primitive geometry function */
void Render::setPrimGeom( PrimPtr &P, const GeomPtr &NewGeom )
{
  releasePrimGeom(P);
  P._resource->_geometry = NewGeom;
} /* End of 'Render::setPrimGeom' function */

/* Releasing primitive geometry function */
void Render::releasePrimGeom( PrimPtr &P )
{
  GeomPtr oldGeom = P._resource->_geometry;
  P._resource->_geometry = nullptr;
  releaseGeom(oldGeom);
} /* End of 'Render::releasePrimGeom' function */

/* Realease primitive function */
void Render::releasePrim( PrimPtr &P )
{
  _primitives.release(P);
} /* End of 'Render::releasePrim' function */

/* END OF 'render_prim.cpp' FILE */
