/*
 * Game project
 *
 * FILE: render.cpp
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 07.03.2018
 * NOTE: render handle implementation file
 */

#include "render.h"

using namespace render;

/* Create render function */
Render::Render( void )
{
} /* End of 'Render::Render' function */

/* Destroy render function */
Render::~Render( void )
{
} /* End of 'Render::~Render' function */

/* Get render instance function */
Render & Render::getInstance( void ) const
{
  static Render instance;

  return instance;
} /* End of 'Render::getInstance' function */

/* Initialize DirectX function */
void Render::init( /* params */ )
{
} /* End of 'Render::init' function */

/* Start frame function */
void Render::startFrame( void )
{
} /* End of 'Render::startFrame' function */

/* Render all primitives function */
void Render::render( void )
{
} /* End of 'Render::render' function */

/* End frame function */
void Render::endFrame( void )
{
} /* End of 'Render::endFrame' function */

/***
 * Texture handle
 ***/

/* Get texture interface function */
Texture * Render::getTexture( const string &TexName ) const
{
  auto tex = _textures.find(TexName);

  if (tex == _textures.end())
    return nullptr;

  return tex->second;
} /* End of 'Render::getTexture' function */

/* Release texture function */
void Render::releaseTexture( Texture *Tex )
{
} /* End of 'Render::releaseTexture' function */

/***
 * Shader handle
 ***/
/* Get shader interface function */
Shader * Render::getShader( const string &TexName ) const
{
  auto shdr = _shaders.find(TexName);

  if (shdr == _shaders.end())
    return nullptr;

  return shdr->second;
} /* End of 'Render::getShader' function */

/* Release primitive function */
void Render::releaseShader( Shader *Shader )
{
} /* End of 'Render::releaseShader' function */

/***
 * Material handle
 ***/

/* Get material interface function */
Material * Render::createMaterial( /* params */ )
{
  return nullptr;
} /* End of 'Render::createMaterial' function */

/* Set material texture function */
void Render::setMaterialTexture( Material *Mtl, Texture *NewTexture, int TexNo )
{
} /* End of 'Render::setMaterialTexture' function */

/* Realease material function */
void Render::releaseMaterial( Material *Mtl )
{
} /* End of 'Render::releaseMaterial' function */

/***
 * Primitive handle
 ***/

/* Create sphere primitive function */
Prim * Render::createSphere( float Radius )
{
  return nullptr;
} /* End of 'Render::createSphere' function */

/* Create box primitive function */
Prim * Render::createBox( float Width, float Height )
{
  return nullptr;
} /* End of 'Render::createBox' function */

/* Set primitive shader function */
void Render::setPrimShader( Prim *Prim, Shader *NewShader )
{
} /* End of 'Render::setPrimShader' function */

/* Set primitive material function */
void Render::setPrimMaterial( Prim *Prim, Material *NewMaterial )
{
} /* End of 'Render::setPrimMaterial' function */

/* Draw primitive function */
void Render::drawPrim( Prim *Prim )
{
} /* End of 'Render::drawPrim' function */

/* Realease primitive function */
void Render::releasePrim( Prim *Prim )
{
} /* End of 'Render::releasePrim' function */

/* END OF 'render.cpp' FILE */
