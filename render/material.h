/*
 * Game project
 *
 * FILE: material.h
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 13.04.2018
 * NOTE: material handle file
 */

#pragma once

#include <vector>

#include "texture.h"

/* Render handle namespace */
namespace render
{
  using TextureVector = std::vector<Texture *>;

  /* Material class */
  class Material : public Resource
  {
    friend class Render;
    friend class ResPtr<Material>;

  public:
    struct Coeffs
    {
      math::Vec4f _kA, _kD, _kS;
      float _kP;
      float _dummy[3];
    };

  private:
    Coeffs _coeffs;
    TexturePtr _textures[4];

    /* Create material function */
    Material( const string &MtlName, Coeffs Cfs ) : Resource(MtlName), _coeffs(Cfs) {}
  }; /* End of 'Material' class */

  using MaterialPtr = ResPtr<Material>;
}; /* end of 'render' namespace */

/* END OF 'material.h' FILE */
