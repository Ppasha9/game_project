/*
 * Game project
 *
 * FILE: shader.h
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 07.03.2018
 * NOTE: shader resource handle file
 */

#pragma once

#include "..\def.h"

/* Render handle namespace */
namespace render
{
  /* Shader resource class */
  class Shader
  {
    friend class Render;

  private:
    string _name;
    // params

    /* Create shader function */
    Shader( /* params */ );
  }; /* End of 'Shader' class */
}; /* end of 'render' namespace */

/* END OF 'shader.h' FILE */
