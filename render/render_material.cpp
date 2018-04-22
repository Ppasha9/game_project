/*
 * Game project
 *
 * FILE: render_material.cpp
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 22.04.2018
 * NOTE: render material handle implementation file
 */

#include "render.h"

using namespace render;

/* Get material interface function */
MaterialPtr Render::createMaterial( const string &MtlName, const Material::Coeffs &Coeffs )
{
  MaterialPtr tmp;

  if ((tmp = getMaterial(MtlName))._resource != nullptr)
    return tmp;

  Material *M = new Material(MtlName, Coeffs);
  _materials.add(MtlName, M);

  return M;
} /* End of 'Render::createMaterial' function */

/* Get material interface function */
MaterialPtr Render::getMaterial( const string &MtlName ) const
{
  return _materials.get(MtlName);
} /* End of 'Render::getMaterial' function */

/* Set material as active function */
void Render::applyMaterial( MaterialPtr &Mtl )
{
  if (Mtl._resource == nullptr)
    Mtl = getMaterial("default");

  _constBuffer._data._mtlCoeffs = Mtl._resource->_coeffs;

  for (int i = 0; i < 4; i++)
    applyTexture(Mtl._resource->_textures[i], i);
} /* End of 'Render::applyMaterial' function */
  
/* Set material texture function */
void Render::setMaterialTexture( MaterialPtr &Mtl, TexturePtr &NewTexture, int TexNo )
{
  if (TexNo >= 4 || TexNo < 0)
    return;

  Mtl._resource->_textures[TexNo] = NewTexture;
} /* End of 'Render::setMaterialTexture' function */

/* Realease material function */
void Render::releaseMaterial( MaterialPtr &Mtl )
{
  _materials.release(Mtl);
} /* End of 'Render::releaseMaterial' function */

/* Setting material coefficients */
void Render::setMaterialCoeffs(MaterialPtr &Mtl, const Material::Coeffs &Coeffs)
{
  Mtl._resource->_coeffs = Coeffs;
} /* End of 'Render::setMaterialCoeffs' function */

/* END OF 'render_material.cpp' FILE */
