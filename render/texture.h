/*
  Game project

  FILE: texture.h
  AUTHOR:
    Vasilyev Peter
  LAST UPDATE: 07.03.2018
  NOTE: texture resource handle file
 */

#pragma once

#include "..\def.h"

/* Render handle namespace */
namespace render
{
  /* Texture resource class */
  class texture
  {
    friend class render;

  private:
    string Name;
    // params

    /* Create texture function */
    texture( /* params */ );
  }; /* End of 'texture' class */
}; /* end of 'render' namespace */

/* END OF 'texture.h' FILE */
