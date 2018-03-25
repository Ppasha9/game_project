/*
 * Game project
 *
 * FILE: resource.h
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 24.03.2018
 * NOTE: resource handle file
 */

#pragma once

#include "..\def.h"

/* Render handle namespace */
namespace render
{
  /* Render resource class */
  class Resource
  {
    friend class Render;

  protected:
    string _name;
    int _nooInst;

  protected:
    /* Create resource function */
    Resource( const string &ResName ) : _name(ResName), _nooInst(0)
    {
    } /* End of 'Resource' function */
  }; /* End of 'resource' class */
} /* end of 'render' namespace */

/* END OF 'resource.h' FILE */
