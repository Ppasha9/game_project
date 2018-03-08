/*
 * Game project
 *
 * FILE: render.h
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 07.03.2018
 * NOTE: render handle declaration file
 */

#pragma once

#include <map>

#include "..\def.h"
#include "Prim.h"
#include "material.h"

/* Render handle namespace */
namespace render
{
  using ShaderMap = std::map<string, Shader *>;
  using MaterialMap = std::map<string, Material *>;
  using TextureMap = std::map<string, Texture *>;
  using GeomMap = std::map<string, Geom *>;
  using PrimMap = std::map<string, Prim *>;

  /* Render handle class */
  class Render
  {
  private:
    PrimMap _prims;         // Registered primitives map
    MaterialMap _materials; // Registered materials map
    ShaderMap _shaders;     // Registered shaders map
    TextureMap _textures;   // Registered textures map
    GeomMap _geometries;    // Registered geometry map

    /* resources */

    /* Create render function */
    Render( void );

  public:
    /* Destroy render function */
    ~Render( void );

    /* Get render instance function */
    Render & getInstance( void ) const;

    /* Initialize DirectX function */
    void init( /* params */ );

    /* Release DirectX function */
    void release( /* params */ );

    /* Start frame function */
    void startFrame( void );

    /* Render all primitives function */
    void render( void );

    /* End frame function */
    void endFrame( void );

    /***
     * Texture handle
     ***/

    /* Get texture interface function */
    Texture * createTexture( const string &TexName );

    /* Get texture interface function */
    Texture * getTexture( const string &TexName ) const;

    /* Release texture function */
    void releaseTexture( Texture *Tex );

    /***
     * Shader handle
     ***/

    /* Get shader interface function */
    Shader * createShader( const string &ShName );

    /* Get shader interface function */
    Shader * getShader( const string &ShName ) const;

    /* Release primitive function */
    void releaseShader( Shader *Shader );

    /***
     * Material handle
     ***/

    /* Create material function */
    Material * createMaterial( /* params */ );

    /* Get material interface function */
    Material * getMaterial( const string &MtlName ) const;

    /* Set material texture function */
    void setMaterialTexture( Material *Mtl, Texture *NewTexture, int TexNo );

    /* Realease material function */
    void releaseMaterial( Material *Mtl );

    /***
     * Primitive handle
     ***/

    /* Create sphere primitive function */
    Prim * createSphere( float Radius );

    /* Create box primitive function */
    Prim * createBox( float Width, float Height );

    /* Get primitive interface function */
    Prim * getPrim( const string &PrimName ) const;

    /* Set primitive shader function */
    void setPrimShader( Prim *Prim, Shader *NewShader );

    /* Set primitive material function */
    void setPrimMaterial( Prim *Prim, Material *NewMaterial );

    /* Draw primitive function */
    void drawPrim( Prim *Prim );

    /* Realease primitive function */
    void releasePrim( Prim *Prim );
  }; /* End of 'Render' class */
}; /* end of 'render' namespace */

/* END OF 'render.h' FILE */
