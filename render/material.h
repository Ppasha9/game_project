/*
 * Game project
 *
 * FILE: material.h
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 07.03.2018
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
  class Material
  {
    friend class Render;

  private:
    string _name;
    // Ka, Kd, Ks, Kp

     TextureVector Textures;
    // Or fixed number

    /* Create material function */
    Material( /* params */ );
  }; /* End of 'Material' class */
}; /* end of 'render' namespace */

/* END OF 'material.h' FILE */
