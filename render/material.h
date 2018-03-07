/*
  Game project

  FILE: material.h
  AUTHOR:
    Vasilyev Peter
  LAST UPDATE: 07.03.2018
  NOTE: material handle file
 */

#pragma once

#include <vector>

#include "shader.h"
#include "texture.h"

using texture_vector = std::vector<texture *>;

/* Render handle namespace */
namespace render
{
  /* Material class */
  class material
  {
    friend class render;

  private:
    string Name;
    // params

    texture_vector Textures;

    /* Create material function */
    material( /* params */ );
  }; /* End of 'material' class */
}; /* end of 'render' namespace */

/* END OF 'material.h' FILE */
