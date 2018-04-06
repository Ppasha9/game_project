/*
 * Game project
 *
 * FILE: render_material.cpp
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 01.04.2018
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
  _materials[MtlName] = M;

  return M;
} /* End of 'Render::createMaterial' function */

/* Get material interface function */
MaterialPtr Render::getMaterial( const string &MtlName ) const
{
  return getRes<Material>(MtlName, _materials);
} /* End of 'Render::getMaterial' function */

/* Set material as active function */
void Render::setMaterial( const MaterialPtr &Mtl )
{
  if (Mtl._resource == nullptr)
  {
    // set default material
  }
  else
    _constBuffer._data._mtlCoeffs = Mtl._resource->_coeffs;

  // set active textures here
} /* End of 'Render::setMaterial' function */
  
/* Set material texture function */
void Render::setMaterialTexture( MaterialPtr &Mtl, TexturePtr &NewTexture, int TexNo )
{
} /* End of 'Render::setMaterialTexture' function */

/* Realease material function */
void Render::releaseMaterial( MaterialPtr &Mtl )
{
  releaseRes<Material>(Mtl, releaseMaterial, _materials);
} /* End of 'Render::releaseMaterial' function */

/* END OF 'render_material.cpp' FILE */
