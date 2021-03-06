/*
 * Game project
 *
 * FILE: render.cpp
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 14.05.2018
 * NOTE: render handle implementation file
 */

#include <cassert>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>

#include "render.h"
#include "..\\render\timer\timer.h"
#include "text\text.h"

using namespace render;

/* Create render function */
Render::Render( void ) :
  _swapChain(nullptr),
  _device(nullptr),
  _deviceContext(nullptr),
  _renderTargetView(nullptr),
  _depthStencilBuffer(nullptr),
  _depthStencilState(nullptr),
  _depthStencilStateZeroWriting(nullptr),
  _depthStencilView(nullptr),
  _rasterStateSolid(nullptr),
  _rasterStateWireframe(nullptr),
  _samplerState(nullptr),
  _blendStateOn(nullptr),
  _blendStateOff(nullptr),
  _shaders(this, releaseShader),
  _materials(this, releaseMaterial),
  _geometries(this, releaseGeom),
  _textures(this, releaseTexture),
  _primitives(this, releasePrim),
  _splitScreenMode(SplitScreenMode::FULL)
{
} /* End of 'Render::Render' function */

/* Destroy render function */
Render::~Render( void )
{
  release();
} /* End of 'Render::~Render' function */

/* Get render instance function */
Render & Render::getInstance( void )
{
  static Render instance;

  return instance;
} /* End of 'Render::getInstance' function */

/* Create depth buffer function */
void Render::createDepthStencil( int Width, int Height )
{
  HRESULT result;

  if (!_device)
    return;

  // Release old depth stencil buffer and view
  releaseRes<ID3D11DepthStencilView>(_depthStencilView);
  releaseRes<ID3D11Texture2D>(_depthStencilBuffer);

  /*** Buffer ***/

  D3D11_TEXTURE2D_DESC depth_buffer_desc;
  ZeroMemory(&depth_buffer_desc, sizeof(depth_buffer_desc));
  
  // Set up depth buffer description
  depth_buffer_desc.Width = Width;
  depth_buffer_desc.Height = Height;
  depth_buffer_desc.MipLevels = 1;
  depth_buffer_desc.ArraySize = 1;
  depth_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  depth_buffer_desc.SampleDesc.Count = 1;
  depth_buffer_desc.SampleDesc.Quality = 0;
  depth_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
  depth_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  depth_buffer_desc.CPUAccessFlags = 0;
  depth_buffer_desc.MiscFlags = 0;

  result = _device->CreateTexture2D(&depth_buffer_desc, nullptr, &_depthStencilBuffer);
  assert(!FAILED(result));

  /*** View ***/

  D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
  ZeroMemory(&depth_stencil_view_desc, sizeof(depth_stencil_view_desc));

  // Set depth stencil view description
  depth_stencil_view_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  depth_stencil_view_desc.Texture2D.MipSlice = 0;

  // Create depth stencil view
  result = _device->CreateDepthStencilView(_depthStencilBuffer, &depth_stencil_view_desc, &_depthStencilView);
  assert(!FAILED(result));
} /* End of 'Render::createDepthBuffer' function */

/* Set viewport function */
void Render::setViewport( float TopLeftX, float TopLeftY, float Width, float Height )
{
  if (!_deviceContext)
    return;

  D3D11_VIEWPORT viewport;

  // Init viewport information
  viewport.Width = Width;
  viewport.Height = Height;
  viewport.MinDepth = 0.0f;
  viewport.MaxDepth = 1.0f;
  viewport.TopLeftX = TopLeftX;
  viewport.TopLeftY = TopLeftY;

  // Set viewport
  _deviceContext->RSSetViewports(1, &viewport);
} /* End of 'Render::setViewport' function */

/* Initialize render function */
void Render::init( void (*ResponseFunc)( void ) )
{
  init(_width, _height, _hWnd);
  LightSystem::init(_constBuffer._data._lights);
  createDefResources();
  _responseFunc = ResponseFunc;
} /* End of 'Render::init' function */

/* Create default resources function */
void Render::createDefResources( void )
{
  math::Vec4uc def_texture[16 * 16 * 4];

  for (unsigned char i = 0; i < 16; i++)
    for (unsigned char j = 0; j < 16; j++)
      def_texture[i * 16 + j] = {(unsigned char)(255 * ((i + j) % 2)),
                                 (unsigned char)(255 * ((i + j) % 2)),
                                 (unsigned char)(255 * ((i + j) % 2)), 255};

  createShader("default");

  auto tex = createTexture("default.tga");
  //auto tex = createTexture("default.tga", Image(def_texture, 16, 16));
  auto mtl = createMaterial("default", {{0.01f, 0.01f, 0.01f, 1}, {0.69f, 0.69f, 0.69f, 1}, {0.7f, 0.7f, 0.7f, 1}, 100});
  setMaterialTexture(mtl, tex, 0);
  setMaterialTexture(mtl, tex, 1);
  setMaterialTexture(mtl, tex, 2);
  setMaterialTexture(mtl, tex, 3);
} /* End of 'Render::createDefResources' function */

/* Initialize DirectX function */
void Render::init( int Width, int Height, HWND hWnd )
{
  HRESULT result;

  if (Width == 0)
    Width = 1;
  if (Height == 0)
    Height = 1;

  _hWnd = hWnd;

  /*** Init swap chain ***/

  DXGI_SWAP_CHAIN_DESC swap_chain_desc;
  ZeroMemory(&swap_chain_desc, sizeof(DXGI_SWAP_CHAIN_DESC));

  // Set number of back buffers
  swap_chain_desc.BufferCount = 1;

  // Set back buffer description
  swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swap_chain_desc.BufferDesc.Width = Width;
  swap_chain_desc.BufferDesc.Height = Height;
  swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
  swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;

  // Set back buffer usage as render target
  swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

  // Set output window
  swap_chain_desc.OutputWindow = hWnd;

  // Turn off multisampling
  swap_chain_desc.SampleDesc.Count = 1;
  swap_chain_desc.SampleDesc.Quality = 0;

  swap_chain_desc.Windowed = true;

  // Set scan line ordering and scaling to unspecified
  swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

  // Discard the back buffer contents after presenting
  swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

  // Don't set the advanced flags
  swap_chain_desc.Flags = 0;

  // Set feature level to DirectX 11
  D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;

  // Create swap chain, device and device context
  result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
    &feature_level, 1, D3D11_SDK_VERSION, &swap_chain_desc, &_swapChain, &_device, nullptr, &_deviceContext);
  assert(!FAILED(result));

  // Get back buffer resource pointer
  ID3D11Texture2D *back_buffer_ptr;
  result = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer_ptr);
  assert(!FAILED(result));
  // Create render target view in the back buffer resource
  result = _device->CreateRenderTargetView(back_buffer_ptr, nullptr, &_renderTargetView);
  assert(!FAILED(result));

  back_buffer_ptr->Release();

  /*** Init depth buffer and view ***/

  createDepthStencil(Width, Height);

  /*** Init depth stencil state ***/

  D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
  ZeroMemory(&depth_stencil_desc, sizeof(depth_stencil_desc));

  // Set up depth stencil description
  depth_stencil_desc.DepthEnable = true;
  depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;

  depth_stencil_desc.StencilEnable = true;
  depth_stencil_desc.StencilReadMask = 0xFF;
  depth_stencil_desc.StencilWriteMask = 0xFF;

  // Stencil operations if pixel is front-facing.
  depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
  depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  // Stencil operations if pixel is back-facing.
  depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
  depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  // Create default depth stencil state
  result = _device->CreateDepthStencilState(&depth_stencil_desc, &_depthStencilState);
  assert(!FAILED(result));

  // Create depth stencil state
  depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
  result = _device->CreateDepthStencilState(&depth_stencil_desc, &_depthStencilStateZeroWriting);
  assert(!FAILED(result));

  // Set depth stencil state as active
  _deviceContext->OMSetDepthStencilState(_depthStencilState, 1);

  // Set active render target and depth stencil
  _deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

  /*** Init rasterize state ***/

  D3D11_RASTERIZER_DESC raster_desc;

  // Setup the raster description
  raster_desc.AntialiasedLineEnable = false;
  raster_desc.CullMode = D3D11_CULL_BACK;
  //raster_desc.CullMode = D3D11_CULL_NONE;
  raster_desc.DepthBias = 0;
  raster_desc.DepthBiasClamp = 0.0f;
  raster_desc.DepthClipEnable = true;
  raster_desc.FillMode = D3D11_FILL_SOLID;
  raster_desc.FrontCounterClockwise = true;
  raster_desc.MultisampleEnable = false;
  raster_desc.ScissorEnable = false;
  raster_desc.SlopeScaledDepthBias = 0.0f;

  // Create fill solid rasterizer state
  result = _device->CreateRasterizerState(&raster_desc, &_rasterStateSolid);
  assert(!FAILED(result));

  // Create wireframe rasterizer state
  raster_desc.FillMode = D3D11_FILL_WIREFRAME;
  raster_desc.CullMode = D3D11_CULL_NONE;
  result = _device->CreateRasterizerState(&raster_desc, &_rasterStateWireframe);
  assert(!FAILED(result));

  // Set active rasterizer state as solid fill
  _deviceContext->RSSetState(_rasterStateSolid);

  /*** Init sampler state ***/
  D3D11_SAMPLER_DESC sampler_desc;
  sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  //sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
  sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  sampler_desc.MipLODBias = 0.0f;
  sampler_desc.MaxAnisotropy = 1;
  sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
  sampler_desc.BorderColor[0] = 0;
  sampler_desc.BorderColor[1] = 0;
  sampler_desc.BorderColor[2] = 0;
  sampler_desc.BorderColor[3] = 0;
  sampler_desc.MinLOD = 0;
  sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

  result = _device->CreateSamplerState(&sampler_desc, &_samplerState);
  assert(!FAILED(result));

  /*** Init blend states ***/
  initBlendStates();
  setBlendMode(BlendMode::OFF);

  /*** Setup viewport ***/
  setViewport(0, 0, (float)Width, (float)Height);

  /*** Init constant buffer ***/
  initConstBuffer();

  /*** Init cameras ***/
  _camera[0].setCamera(true, { 10, 10, 10 }, { 0, 0, 0 }, { 0, 1, 0 }, Width, Height);
  _camera[1].setCamera(true, { 10, 10, 10 }, { 0, 0, 0 }, { 0, 1, 0 }, Width, Height);
  _camera[2].setCamera(true, { 10, 10, 10 }, { 0, 0, 0 }, { 0, 1, 0 }, Width, Height);
  _camera[3].setCamera(true, { 10, 10, 10 }, { 0, 0, 0 }, { 0, 1, 0 }, Width, Height);
} /* End of 'Render::init' function */

/* Initialize blend states function */
void Render::initBlendStates( void )
{
  D3D11_BLEND_DESC blend_desc;

  /* Set default blending parametres */
  blend_desc.AlphaToCoverageEnable = false;
  blend_desc.IndependentBlendEnable = false;

  /* Set the render target setting */
  blend_desc.RenderTarget[0].BlendEnable = true;
  blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
  blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
  blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
  blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
  blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
  blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
  blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

  /* Create the off blend state */
  _device->CreateBlendState(&blend_desc, &_blendStateOff);

  /* Create the on blend state */
  blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
  _device->CreateBlendState(&blend_desc, &_blendStateOn);
} /* End of 'initBlendStates' function */

/* Release DirectX function */
void Render::release( void )
{
  // Realease all resources
  _shaders.releaseAll();
  _primitives.releaseAll();
  _textures.releaseAll();
  _materials.releaseAll();
  _geometries.releaseAll();

  // Release constant buffer
  releaseConstBuffer();

  // Set to windowed mode or when released swap chain throws an exception.
  if(_swapChain)
    _swapChain->SetFullscreenState(false, NULL);

  // Release DirectX resources
  releaseRes<ID3D11SamplerState>(_samplerState);
  releaseRes<ID3D11RasterizerState>(_rasterStateSolid);
  releaseRes<ID3D11RasterizerState>(_rasterStateWireframe);
  releaseRes<ID3D11DepthStencilView>(_depthStencilView);
  releaseRes<ID3D11DepthStencilState>(_depthStencilState);
  releaseRes<ID3D11DepthStencilState>(_depthStencilStateZeroWriting);
  releaseRes<ID3D11BlendState>(_blendStateOn);
  releaseRes<ID3D11BlendState>(_blendStateOff);
  releaseRes<ID3D11Texture2D>(_depthStencilBuffer);
  releaseRes<ID3D11RenderTargetView>(_renderTargetView);
  releaseRes<ID3D11DeviceContext>(_deviceContext);
  releaseRes<ID3D11Device>(_device);
  releaseRes<IDXGISwapChain>(_swapChain);
} /* End of 'Render::release' function */

/* Resize render system function */
void Render::resize( int Width, int Height )
{
  if (!_swapChain) // Render not initialized
    return;

  // Set render target inactive
  _deviceContext->OMSetRenderTargets(0, nullptr, nullptr);

  // Release render target view
  _renderTargetView->Release();

  HRESULT result;
  // Preserve the existing buffer count and format.
  // Automatically choose the width and height to match the client rect for HWNDs.
  result = _swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
  assert(!FAILED(result));

  // Get buffer and create a render-target-view.
  ID3D11Texture2D *pBuffer;
  result = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&pBuffer);
  assert(!FAILED(result));

  result = _device->CreateRenderTargetView(pBuffer, nullptr, &_renderTargetView);
  assert(!FAILED(result));
  pBuffer->Release();

  // Create new depth stencil buffer and view
  createDepthStencil(Width, Height);

  // Reset active render target and depth stencil
  _deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

  // Resize cameras according to split-screen mode
  resizeCameras();

  // Redraw frame
  render();
} /* End of 'Render::resize' function */

/* Resize cameras function */
void Render::resizeCameras( void )
{
  switch (_splitScreenMode)
  {
  case SplitScreenMode::FULL:
    _camera[0].resize(_width, _height);
    break;
  case SplitScreenMode::HALVES:
    _camera[0].resize(_width, _height / 2);
    _camera[1].resize(_width, _height / 2);
    break;
  case SplitScreenMode::QUARTERS:
    _camera[0].resize(_width / 2, _height / 2);
    _camera[1].resize(_width / 2, _height / 2);
    _camera[2].resize(_width / 2, _height / 2);
    _camera[3].resize(_width / 2, _height / 2);
    break;
  }
} /* End of 'Render::resizeCameras' function */

/* Set split-screen mode function */
void Render::setSplitScreen( SplitScreenMode Mode )
{
  _splitScreenMode = Mode;
  resizeCameras();
} /* End of 'Render::setSplitScreen' function */

/* Set camera 3d space parameters function */
void Render::setCamera( int Id, bool IsLookAt,
  const math::Vec3f &Loc, const math::Vec3f &Dir, const math::Vec3f &Up )
{
  if (Id < 0 || Id >= 4)
    return;

  if (IsLookAt)
    _camera[Id].setLookAtLocUp(Loc, Dir, Up);
  else
    _camera[Id].setDirLocUp(Loc, Dir, Up);
} /* End of 'Render::setCamera' function */

/* Set camera as active function */
void Render::applyCamera( int Id )
{
  if (Id < 0 || Id >= 4)
    return;

  _constBuffer._data._cameraPos = {_camera[Id]._loc[0], _camera[Id]._loc[1], _camera[Id]._loc[2], 1};
  _constBuffer._data._cameraDir = {_camera[Id]._dir[0], _camera[Id]._dir[1], _camera[Id]._dir[2], 0};

  _constBuffer._data._view = _camera[Id]._viewMatr;
  _constBuffer._data._proj = _camera[Id]._projMatr;
} /* End of 'Render::applyCamera' function */

/* Set fill mode function */
void Render::setFillMode( Prim::FillMode Mode )
{
  switch (Mode)
  {
  case Prim::FillMode::SOLID:
    _deviceContext->RSSetState(_rasterStateSolid);
    break;
  case Prim::FillMode::WIREFRAME:
    _deviceContext->RSSetState(_rasterStateWireframe);
    break;
  }
} /* End of 'Render::setFillMode' function */

/* Setting z-write mode function */
void Render::setZWriteMode(const ZWriteMode Mode)
{
  switch (Mode)
  {
  case ZWriteMode::ON:
    _deviceContext->OMSetDepthStencilState(_depthStencilState, 1);
    break;
  case ZWriteMode::OFF:
    _deviceContext->OMSetDepthStencilState(_depthStencilStateZeroWriting, 1);
    break;
  }
} /* End of 'Render::setZWriteMode' function */

/* Setting blending mode function */
void Render::setBlendMode(const BlendMode Mode)
{
  float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

  switch (Mode)
  {
  case BlendMode::OFF:
    _deviceContext->OMSetBlendState(_blendStateOff, blendFactor, 0xFFFFFF);
    break;
  case BlendMode::ON:
    _deviceContext->OMSetBlendState(_blendStateOn, blendFactor, 0xFFFFFF);
    break;
  }
} /* End of 'setBlendMode' function */

/* Set projection method function */
void Render::setProjMode( Prim::ProjMode Method )
{
  switch (Method)
  {
  case Prim::ProjMode::SCREENSPACE_PIXEL:
    _constBuffer._data._view = math::Matr4f().setIdentity();
    _constBuffer._data._proj = { 2.0F / _width,               0, 0, 0,
                                             0, -2.0F / _height, 0, 0,
                                             0,               0, 0, 0,
                                            -1,               1, 0, 1};
    break;
  case Prim::ProjMode::SCREENSPACE_UNORM:
    _constBuffer._data._view = math::Matr4f().setIdentity();
    _constBuffer._data._proj = { 2,  0, 0, 0,
                                 0, -2, 0, 0,
                                 0,  0, 0, 0,
                                -1,  1, 0, 1};
    break;
  }
} /* End of 'Render::setProjMode' function */

/* Start frame function */
void Render::startFrame( void )
{
  //float clear_color[4] = {0.3f, 0.5f, 0.7f, 1.0f};
  float clear_color[4] = {0, 0, 0, 1};

  // Clear the back buffer.
  _deviceContext->ClearRenderTargetView(_renderTargetView, clear_color);

  // Clear the depth buffer.
  _deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
} /* End of 'Render::startFrame' function */

/* Render timer response function */
void Render::response( void )
{
  Timer &timer = Timer::getInstance();
  timer.response();
  _responseFunc();

  render();

  timer.incrFrame();
} /* End of 'Render::response' function */

/* Render all primitives function */
void Render::render( void )
{
  startFrame();

  // Render frustum primitives
  setProjMode(Prim::ProjMode::FRUSTUM);
  setZWriteMode(ZWriteMode::ON);
  switch (_splitScreenMode)
  {
  case SplitScreenMode::FULL:
    applyCamera(0);
    setViewport(0, 0, (float)_width, (float)_height);
    for (auto &p : _frustumPrims)
      drawPrim(p);
    break;
  case SplitScreenMode::HALVES:
    applyCamera(0);
    setViewport(0, 0, (float)_width, _height / 2.0F);
    for (auto &p : _frustumPrims)
      drawPrim(p);

    applyCamera(1);
    setViewport(0, _height / 2.0F, (float)_width, _height / 2.0F);
    for (auto &p : _frustumPrims)
      drawPrim(p);
    break;
  case SplitScreenMode::QUARTERS:
    applyCamera(0);
    setViewport(0, 0, _width / 2.0F, _height / 2.0F);
    for (auto &p : _frustumPrims)
      drawPrim(p);

    applyCamera(1);
    setViewport(_width / 2.0F, 0, _width / 2.0F, _height / 2.0F);
    for (auto &p : _frustumPrims)
      drawPrim(p);

    applyCamera(2);
    setViewport(0, _height / 2.0F, _width / 2.0F, _height / 2.0F);
    for (auto &p : _frustumPrims)
      drawPrim(p);

    applyCamera(3);
    setViewport(_width / 2.0F, _height / 2.0F, _width / 2.0F, _height / 2.0F);
    for (auto &p : _frustumPrims)
      drawPrim(p);
    break;
  }
  setViewport(0, 0, (float)_width, (float)_height);

  // Render pixel screen-space primitives
  setProjMode(Prim::ProjMode::SCREENSPACE_PIXEL);
  setZWriteMode(ZWriteMode::OFF);
  setBlendMode(BlendMode::ON);
  for (auto &p : _pixelPrims)
    drawPrim(p);
  setBlendMode(BlendMode::OFF);

  // Render unsigned normalized screen-space primitives
  setProjMode(Prim::ProjMode::SCREENSPACE_UNORM);
  for (auto &p : _unormPrims)
    drawPrim(p);

  endFrame();
} /* End of 'Render::render' function */

/* End frame function */
void Render::endFrame( void )
{
  _swapChain->Present(1, 0);

  _frustumPrims.clear();
  _pixelPrims.clear();
  _unormPrims.clear();
} /* End of 'Render::endFrame' function */

/* END OF 'render.cpp' FILE */
