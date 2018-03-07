/*
  Game project

  FILE: render.h
  AUTHOR:
    Vasilyev Peter
  LAST UPDATE: 07.03.2018
  NOTE: render handle file
 */

#pragma once

#include <map>

#include "..\def.h"
#include "prim.h"
#include "material.h"

/* Render handle namespace */
namespace render
{
  using shader_map = std::map<string, shader *>;
  using material_map = std::map<string, material *>;
  using texture_map = std::map<string, texture *>;
  using geom_map = std::map<string, geom *>;
  using prim_map = std::map<string, prim *>;

  /* Render handle class */
  class render
  {
  private:
    static prim_map Prims;         // Registered primitives map
    static material_map Materials; // Registered materials map
    static shader_map Shaders;     // Registered shaders map
    static texture_map Textures;   // Registered textures map
    static geom_map Geometries;    // Registered geometry map

    /* Create render function */
    render( void );

  public:
    /* Destroy render function */
    ~render( void );

    /* Start frame function */
    void StartFrame( void );

    /* End frame function */
    void EndFrame( void );

    /***
     * Texture handle
     ***/

    /* Get texture interface function */
    texture * GetTexture( const string &TexName );

    /* Release texture function */
    void ReleaseTexture( texture *Tex );

    /***
     * Shader handle
     ***/
    /* Get shader interface function */
    shader * GetShader( const string &TexName );

    /* Release primitive function */
    void ReleaseShader( shader *Shader );

    /***
     * Material handle
     ***/

    /* Get material interface function */
    material * CreateMaterial( /* params */ );

    /* Set primitive texture function */
    void SetMaterialTexture( material *Mtl, texture *NewTexture, int TexId );

    /* Realease material function */
    void ReleaseMaterial( material *Mtl );

    /***
     * Primitive handle
     ***/

    /* Create sphere primitive function */
    prim * CreateSphere( float Radius );

    /* Create box primitive function */
    prim * CreateBox( float Width, float Height );

    /* Set primitive shader function */
    void SetPrimShader( prim *Prim, shader *NewShader );

    /* Set primitive material function */
    void SetPrimMaterial( prim *Prim, material *NewMaterial );

    /* Draw primitive function */
    void DrawPrim( prim *Prim );

    /* Realease primitive function */
    void ReleasePrim( prim *Prim );
  }; /* End of 'render' class */
}; /* end of 'render' namespace */

/* END OF 'render.h' FILE */
