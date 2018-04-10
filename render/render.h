/*
 * Game project
 *
 * FILE: render.h
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 09.04.2018
 * NOTE: render handle declaration file
 */

#pragma once

#include "..\def.h"
#include "..\win\win.h"
#include "const_buffer.h"
#include "res_map.h"
#include "material.h"
#include "prim.h"
#include "image\image.h"
#include "camera\camera.h"

/* Render handle namespace */
namespace render
{
  using ShaderMap = ResMap<Shader>;
  using MaterialMap = ResMap<Material>;
  using TextureMap = ResMap<Texture>;
  using GeomMap = ResMap<Geom>;
  using PrimMap = ResMap<Prim>;

  /* Render handle class */
  class Render : public Win
  {
  private:
    PrimMap _primitives;    // Registered primitives map
    MaterialMap _materials; // Registered materials map
    ShaderMap _shaders;     // Registered shaders map
    TextureMap _textures;   // Registered textures map
    GeomMap _geometries;    // Registered geometry map

    /* DirectX resources */
    IDXGISwapChain          *_swapChain;
    ID3D11Device            *_device;
    ID3D11DeviceContext     *_deviceContext;
    ID3D11RenderTargetView  *_renderTargetView;
    ID3D11Texture2D         *_depthStencilBuffer;
    ID3D11DepthStencilState *_depthStencilState;
    ID3D11DepthStencilView  *_depthStencilView;
    ID3D11RasterizerState   *_rasterState;
    ID3D11SamplerState      *_samplerState;

    ConstBuffer _constBuffer;
    Camera _camera;

    /* Create render function */
    Render( void );

    /* Create depth stencil buffer and view function (releases old if exists) */
    void createDepthStencil( int Width, int Height );

    /* Set viewport function */
    void setViewport( int Width, int Height );

    /* Release resource function */
    template<typename Resource>
      inline void releaseRes( Resource *(&R) )
      {
        if (R != nullptr)
        {
          R->Release();
          R = nullptr;
        }
      } /* End of 'releaseRes' function */

    /* Initialize DirectX function */
    void init( int Width, int Height, HWND hWnd );

    /* Create default resources function */
    void createDefResources( void );

    /* Start frame function */
    void startFrame( void );

    /* Render all primitives function */
    void render( void );

    /* End frame function */
    void endFrame( void );

    /* Resize render system function */
    void resize( int Width, int Height );

    /* Set shader as active function */
    void setShader( ShaderPtr &Sh );

    /* Set texture as active function */
    void setTexture( TexturePtr &Tex, int Id );

    /* Set material as active function */
    void setMaterial( MaterialPtr &Mtl );

    /* Draw geometry function */
    void drawGeom( Geom *G );

    /* Draw primitive function */
    void drawPrim( Prim *P );

    /* Release texture function */
    static void releaseTexture( Render *Rnd, Texture *Tex );

    /* Release shader function */
    static void releaseShader( Render *Rnd, Shader *Sh );

    /* Realease material function */
    static void releaseMaterial( Render *Rnd, Material *Mtl ) {}

    /* Realease geometry function */
    static void releaseGeom( Render *Rnd, Geom *Geom );

    /* Realease primitive function */
    static void releasePrim( Render *Rnd, Prim *P ) {}

    /***
     * Constant buffer handle
     ***/

    /* Create shader constant buffer function */
    void initConstBuffer( void );

    /* Update const buffer function */
    void updateConstBuffer( void );

    /* Release shader constant buffer function */
    void releaseConstBuffer( void );

  public:
    /* Destroy render function */
    ~Render( void );

    /* Get render instance function */
    static Render & getInstance( void );

    /* Initialize render function */
    void init( void );

    /* Release DirectX function */
    void release( void );


    /***
     * Texture handle
     ***/

    /* Create texture from 'TexName' file function */
    TexturePtr createTexture( const string &TexName );

    /* Create texture from Image buffer function */
    TexturePtr createTexture( const string &TexName, const Image &Src );

    /* Get texture interface function */
    TexturePtr getTexture( const string &TexName ) const;

    /* Release texture function */
    void releaseTexture( TexturePtr &Tex );


    /***
     * Shader handle
     ***/

    /* Get shader interface function */
    ShaderPtr createShader( const string &ShName );

    /* Get shader interface function */
    ShaderPtr getShader( const string &ShName ) const;

    /* Release shader function */
    void releaseShader( ShaderPtr &Sh );


    /***
     * Material handle
     ***/

    /* Create material function */
    MaterialPtr createMaterial( const string &MtlName, const Material::Coeffs &Coeffs );

    /* Get material interface function */
    MaterialPtr getMaterial( const string &MtlName ) const;

    /* Set material texture function */
    void setMaterialTexture( MaterialPtr &Mtl, TexturePtr &NewTexture, int TexNo );

    /* Realease material function */
    void releaseMaterial( MaterialPtr &Mtl );


    /***
     * Geometry resource handle
     ***/

    /* Create geometry function */
    // Geom * createGeom( const geometry &Geomery, const string &GeomName );
    GeomPtr createGeom( const string &GeomName );

    /* Get geometry interface function */
    GeomPtr getGeom( const string &GeomName ) const;

    /* Realease geometry function */
    void releaseGeom( GeomPtr &G );


    /***
     * Primitive handle
     ***/

    /* Create primitive function */
    PrimPtr createPrim( const string &PrimName,
      const string &MtlName = "default", const string &ShName = "default" );

    /* Create primitive function */
    PrimPtr createPrim( const string &PrimName,
      const MaterialPtr &Mtl, const ShaderPtr &Sh );

    /* Get primitive interface function */
    PrimPtr getPrim( const string &PrimName ) const;

    /* Set primitive world matrix function */
    void setPrimMatrix( PrimPtr &P, const math::Matr4f &World );

    /* Set primitive shader function */
    void setPrimShader( PrimPtr &P, ShaderPtr &NewShader );

    /* Set primitive material function */
    void setPrimMaterial( PrimPtr &P, MaterialPtr &NewMaterial );

    /* Realease primitive function */
    void releasePrim( PrimPtr &P );
  }; /* End of 'Render' class */
}; /* end of 'render' namespace */

/* END OF 'render.h' FILE */
