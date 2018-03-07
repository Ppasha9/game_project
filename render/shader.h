/*
  Game project

  FILE: shader.h
  AUTHOR:
    Vasilyev Peter
  LAST UPDATE: 07.03.2018
  NOTE: shader resource handle file
 */

#pragma once

#include "..\def.h"

/* Render handle namespace */
namespace render
{
  /* Shader resource class */
  class shader
  {
    friend class render;

  private:
    string Name;
    // params

    /* Create shader function */
    shader( /* params */ );
  }; /* End of 'shader' class */
}; /* end of 'render' namespace */

/* END OF 'shader.h' FILE */
