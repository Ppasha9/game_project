/*
 * Game project
 *
 * FILE: render.h
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 08.03.2018
 * NOTE: render handle declaration file
 */

#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>

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

    /* DirectX resources */
    IDXGISwapChain          *_swapChain;
    ID3D11Device            *_device;
    ID3D11DeviceContext     *_deviceContext;
    ID3D11RenderTargetView  *_renderTargetView;
    ID3D11Texture2D         *_depthStencilBuffer;
    ID3D11DepthStencilState *_depthStencilState;
    ID3D11DepthStencilView  *_depthStencilView;
    ID3D11RasterizerState   *_rasterState;
    // _proj;
    // _world;
    // _view;

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

  public:
    /* Destroy render function */
    ~Render( void );

    /* Get render instance function */
    static Render & getInstance( void );

    /* Initialize DirectX function */
    void init( int Width, int Height, HWND hWnd );

    /* Release DirectX function */
    void release( void );

    /* Resize render system function */
    void resize( int Width, int Height );

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
