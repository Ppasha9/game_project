/*
 * Game project
 *
 * FILE: render.h
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 27.03.2018
 * NOTE: render handle declaration file
 */

#pragma once

#include <map>


#include "..\def.h"
#include "..\win\win.h"
#include "prim.h"
#include "material.h"
#include "res_ptr.h"
#include "const_buffer.h"
#include "camera\camera.h"

/* Render handle namespace */
namespace render
{
  using ShaderMap = std::map<string, Shader *>;
  using MaterialMap = std::map<string, Material *>;
  using TextureMap = std::map<string, Texture *>;
  using GeomMap = std::map<string, Geom *>;
  using PrimMap = std::map<string, Prim *>;

  /* Render handle class */
  class Render : public Win
  {
  private:
    PrimMap _primitives;         // Registered primitives map
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

  public:
    /* Destroy render function */
    ~Render( void );

    /* Get render instance function */
    static Render & getInstance( void );

    /* Initialize render function */
    void init( void );

  private:
    /* Initialize DirectX function */
    void init( int Width, int Height, HWND hWnd );

  public:
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

  private:
    template<typename ResType>
      static ResType * getRes( const string &ResName, const std::map<string, ResType *> &ResMap )
      {
        auto res = ResMap.find(ResName);

        if (res == ResMap.end())
          return nullptr;

        return res->second;
      }

    template<typename ResType>
      void releaseRes( ResPtr<ResType> &Res,
        void (*ReleaseFunc)( Render *Rnd, ResType *Res ), std::map<string, ResType *> &ResMap )
      {
        if (Res._resource == nullptr)
          return;

        if (Res._resource->_nooInst > 1)
        {
          Res._resource->_nooInst--;
          return;
        }

        ReleaseFunc(this, Res._resource);

        ResMap.erase(Res._resource->_name);
        delete Res._resource;
        Res._resource = nullptr;
      }

    template<typename ResType>
      void releaseAllRes( void (*ReleaseFunc)( Render *Rnd, ResType *Res ),
        std::map<string, ResType *> &ResMap )
      {
        while (ResMap.size() != 0)
        {
          ResType *r = ResMap.begin()->second;
          ReleaseFunc(this, r);
          ResMap.erase(ResMap.begin());
        }
      }

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

    /***
     * Texture handle
     ***/

    /* Get texture interface function */
    TexturePtr createTexture( const string &TexName );

    /* Get texture interface function */
    TexturePtr getTexture( const string &TexName ) const;

  private:
    /* Release texture function */
    static void releaseTexture( Render *Rnd, Texture *Tex );

  public:
    /* Release texture function */
    void releaseTexture( TexturePtr &Tex );


    /***
     * Shader handle
     ***/

  private:
    /* Set shader as active function */
    void setShader( Shader *Sh );

  public:
    /* Get shader interface function */
    ShaderPtr createShader( const string &ShName );

    /* Get shader interface function */
    ShaderPtr getShader( const string &ShName ) const;

  public:
    /* Release shader function */
    static void releaseShader( Render *Rnd, Shader *Sh );

  public:
    /* Release shader function */
    void releaseShader( ShaderPtr &Sh );

    /***
     * Material handle
     ***/

    /* Create material function */
    MaterialPtr createMaterial( /* params */ );

    /* Get material interface function */
    MaterialPtr getMaterial( const string &MtlName ) const;

    /* Set material texture function */
    void setMaterialTexture( MaterialPtr &Mtl, TexturePtr &NewTexture, int TexNo );

  private:
    /* Realease material function */
    static void releaseMaterial( Render *Rnd, Material *Mtl );

  public:
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

  private:
    /* Draw geometry function */
    void drawGeom( Geom *G );

    /* Realease geometry function */
    static void releaseGeom( Render *Rnd, Geom *Geom );

  public:
    /* Realease geometry function */
    void releaseGeom( GeomPtr &G );

    /***
     * Primitive handle
     ***/

    /* Create primitive function */
    PrimPtr createPrim( const string &PrimName );

    /* Get primitive interface function */
    PrimPtr getPrim( const string &PrimName ) const;

    /* Set primitive shader function */
    void setPrimShader( PrimPtr &P, Shader *NewShader );

    /* Set primitive material function */
    void setPrimMaterial( PrimPtr &P, Material *NewMaterial );

  private:
    /* Draw primitive function */
    void drawPrim( Prim *P );

    /* Realease primitive function */
    static void releasePrim( Render *Rnd, Prim *P ) {}

  public:
    /* Realease primitive function */
    void releasePrim( PrimPtr &P );
  }; /* End of 'Render' class */
}; /* end of 'render' namespace */

/* END OF 'render.h' FILE */
