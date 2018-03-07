/*
 * Game project
 *
 * FILE: texture.h
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 07.03.2018
 * NOTE: texture resource handle file
 */

#pragma once

#include "..\def.h"

/* Render handle namespace */
namespace render
{
  /* Texture resource class */
  class Texture
  {
    friend class Render;

  private:
    string _name;
    // params

    /* Create texture function */
    Texture( /* params */ );
  }; /* End of 'Texture' class */
}; /* end of 'render' namespace */

/* END OF 'texture.h' FILE */
