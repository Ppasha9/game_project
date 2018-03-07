/*
  Game project

  FILE: geom.h
  AUTHOR:
    Vasilyev Peter
  LAST UPDATE: 07.03.2018
  NOTE: geometry directx resource handle file
 */

#pragma once

#include "..\def.h"

/* Render handle namespace */
namespace render
{
  /* Geometry resource class */
  class geom
  {
    friend class render;

  private:
    string Name;
    // params

    /* Create geometry function */
    geom( /* params */ );
  }; /* End of 'geom' class */
} /* end of 'render' namespace */

/* END OF 'geom.h' FILE */
