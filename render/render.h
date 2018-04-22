/*
 * Game project
 *
 * FILE: render.h
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 22.04.2018
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
#include "..\geometry\geometry.h"

/* Render handle namespace */
namespace render
{
  using ShaderMap = ResMap<Shader>;
  using MaterialMap = ResMap<Material>;
  using TextureMap = ResMap<Texture>;
  using GeomMap = ResMap<Geom>;
  using PrimMap = ResMap<Prim>;

  /* Render handle class */
  class Render : public Win, public LightSystem
  {
  public:
    /* Zwrite mode enum */
    enum struct ZWriteMode
    {
      ON,    // all write zclipping mode
      OFF   // zero write zclipping mode
    }; /* End of 'ZWriteMode' enum */

    /* Spliting the screen mode */
    enum struct SplitScreenMode
    {
      FULL,
      HALVES,
      QUARTERS
    }; /* End of 'SplitScreenMode' enum */

    /* Alpha blending mode */
    enum struct BlendMode
    {
      OFF,
      ON
    }; /* End of 'BlendMode' enum */

  private:
    PrimMap _primitives;    // Registered primitives map
    MaterialMap _materials; // Registered materials map
    ShaderMap _shaders;     // Registered shaders map
    TextureMap _textures;   // Registered textures map
    GeomMap _geometries;    // Registered geometry map

    using PrimVector = std::vector<Prim *>;
    PrimVector _frustumPrims;
    PrimVector _pixelPrims;
    PrimVector _unormPrims;

    /* DirectX resources */
    IDXGISwapChain          *_swapChain;
    ID3D11Device            *_device;
    ID3D11DeviceContext     *_deviceContext;
    ID3D11RenderTargetView  *_renderTargetView;
    ID3D11Texture2D         *_depthStencilBuffer;
    ID3D11DepthStencilState *_depthStencilState;
    ID3D11DepthStencilState *_depthStencilStateZeroWriting;
    ID3D11DepthStencilView  *_depthStencilView;
    ID3D11RasterizerState   *_rasterStateSolid;
    ID3D11RasterizerState   *_rasterStateWireframe;
    ID3D11SamplerState      *_samplerState;
    ID3D11BlendState        *_blendStateOff;
    ID3D11BlendState        *_blendStateOn;

    ConstBuffer _constBuffer;

    void (*_responseFunc)( void );

    SplitScreenMode _splitScreenMode;
    Camera _camera[4];

    /* Create render function */
    Render( void );

    /* Create depth stencil buffer and view function (releases old if exists) */
    void createDepthStencil( int Width, int Height );

    /* Set viewport function */
    void setViewport( float TopLeftX, float TopLeftY, float Width, float Height );

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

    /* Initing blend states function */
    void initBlendStates(void);

    /* Create default resources function */
    void createDefResources( void );

    /* Render timer response function */
    void response( void );

    /* Start frame function */
    void startFrame( void );

    /* Render all primitives function */
    void render( void );

    /* End frame function */
    void endFrame( void );

    /* Resize render system function */
    void resize( int Width, int Height );

    /* Resize cameras function */
    void resizeCameras( void );

    /* Set camera as active function */
    void applyCamera( int Id );

    /* Set shader as active function */
    void applyShader( ShaderPtr &Sh );

    /* Set texture as active function */
    void applyTexture( TexturePtr &Tex, int Id );

    /* Set material as active function */
    void applyMaterial( MaterialPtr &Mtl );

    /* Set fill mode function */
    void setFillMode( Prim::FillMode Mode );

    /* Set projection method function */
    void setProjMode( Prim::ProjMode Method );

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
    void init( void (*ResponseFunc)( void ) );

    /* Release DirectX function */
    void release( void );

    /* Set split-screen mode function */
    void setSplitScreen( SplitScreenMode Mode );

    /* Set camera 3d space parameters function */
    void setCamera( int Id, bool IsLookAt,
      const math::Vec3f &Loc, const math::Vec3f &Dir, const math::Vec3f &Up );

    /* Getting screen width function */
    int getScreenWidth(void) const;

    /* Getting screen height function */
    int getScreenHeight(void) const;

    /* Setting z-write mode function */
    void setZWriteMode(const ZWriteMode Mode);

    /* Setting blending mode function */
    void setBlendMode(const BlendMode Mode);

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

    /* Setting material coefficients */
    void setMaterialCoeffs(MaterialPtr &Mtl, const Material::Coeffs &Coeffs);

    /* Realease material function */
    void releaseMaterial( MaterialPtr &Mtl );


    /***
     * Geometry resource handle
     ***/

    /* Create geometry render resource function */
     GeomPtr createGeom( const string &GeomName, const geom::Geom &Geometry );

    /* Create geometry render resource from file function */
    GeomPtr createGeom( const string &GeomName );

    /* Get geometry interface function */
    GeomPtr getGeom( const string &GeomName ) const;

    /* Realease geometry function */
    void releaseGeom( GeomPtr &G );


    /***
     * Primitive handle
     ***/

    /* Create primitive function */
    PrimPtr createPrim( const string &PrimName, const string &GeomName,
      const string &MtlName = "default", const string &ShName = "default",
      Prim::ProjMode ProjM = Prim::ProjMode::FRUSTUM,
      Prim::FillMode FillM = Prim::FillMode::SOLID );

    /* Create primitive function */
    PrimPtr createPrim( const string &PrimName, const GeomPtr &Geometry,
      const MaterialPtr &Mtl = nullptr, const ShaderPtr &Sh = nullptr,
      Prim::ProjMode ProjM = Prim::ProjMode::FRUSTUM,
      Prim::FillMode FillM = Prim::FillMode::SOLID );

    /* Get primitive interface function */
    PrimPtr getPrim( const string &PrimName ) const;

    /* Set primitive world matrix function */
    void setPrimMatrix( PrimPtr &P, const math::Matr4f &World );

    /* Set primitive shader function */
    void setPrimShader( PrimPtr &P, ShaderPtr &NewShader );

    /* Set primitive material function */
    void setPrimMaterial( PrimPtr &P, MaterialPtr &NewMaterial );

    /* Set primitive fill mode function */
    void setPrimFillMode( PrimPtr &P, Prim::FillMode NewFillMode );

    /* Set primitive geometry function */
    void setPrimGeom( PrimPtr &P, const GeomPtr &NewGeom );

    /* Releasing primitive geometry function */
    void releasePrimGeom( PrimPtr &P );

    /* Register primitive for rendering on this frame function */
    void drawPrim( PrimPtr &P );

    /* Realease primitive function */
    void releasePrim( PrimPtr &P );
  }; /* End of 'Render' class */
}; /* end of 'render' namespace */

/* END OF 'render.h' FILE */
