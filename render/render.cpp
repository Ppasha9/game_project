/*
 * Game project
 *
 * FILE: render.cpp
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 06.04.2018
 * NOTE: render handle implementation file
 */

#include <cassert>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>

#include "render.h"

using namespace render;

/* Create render function */
Render::Render( void ) :
  _swapChain(nullptr),
  _device(nullptr),
  _deviceContext(nullptr),
  _renderTargetView(nullptr),
  _depthStencilBuffer(nullptr),
  _depthStencilState(nullptr),
  _depthStencilView(nullptr),
  _rasterState(nullptr),
  _samplerState(nullptr),
  _shaders(this, releaseShader),
  _materials(this, releaseMaterial),
  _geometries(this, releaseGeom),
  _textures(this, releaseTexture),
  _primitives(this, releasePrim)
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
void Render::setViewport( int Width, int Height )
{
  if (!_deviceContext)
    return;

  D3D11_VIEWPORT viewport;

  // Init viewport information
  viewport.Width = (float)Width;
  viewport.Height = (float)Height;
  viewport.MinDepth = 0.0f;
  viewport.MaxDepth = 1.0f;
  viewport.TopLeftX = 0.0f;
  viewport.TopLeftY = 0.0f;

  // Set viewport
  _deviceContext->RSSetViewports(1, &viewport);
} /* End of 'Render::setViewport' function */

/* Initialize render function */
void Render::init( void )
{
  init(_width, _height, _hWnd);

  createPrim("test_prim");
} /* End of 'Render::init' function */

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

  // Create depth stencil state
  result = _device->CreateDepthStencilState(&depth_stencil_desc, &_depthStencilState);
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

  // Create rasterizer state
  result = _device->CreateRasterizerState(&raster_desc, &_rasterState);
  assert(!FAILED(result));

  // Set active rasterizer state
  _deviceContext->RSSetState(_rasterState);

  /*** Init sampler state ***/
  D3D11_SAMPLER_DESC sampler_desc;
  sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
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

  /*** Setup viewport ***/
  setViewport(Width, Height);

  /*** Init constant buffer ***/
  initConstBuffer();

  /*** Init camera ***/
  _camera.setCamera(true, { 0, 10, 10 }, { 0, 0, 0 }, { 0, 1, 0 }, Width, Height);
} /* End of 'Render::init' function */

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
  releaseRes<ID3D11RasterizerState>(_rasterState);
  releaseRes<ID3D11DepthStencilView>(_depthStencilView);
  releaseRes<ID3D11DepthStencilState>(_depthStencilState);
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

  // Reset viewport
  setViewport(Width, Height);

  // Redraw frame
  render();
} /* End of 'Render::resize' function */

/* Start frame function */
void Render::startFrame( void )
{
  float clear_color[4] = {0.3f, 0.5f, 0.7f, 1.0f};

  // Clear the back buffer.
  _deviceContext->ClearRenderTargetView(_renderTargetView, clear_color);

  // Clear the depth buffer.
  _deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
} /* End of 'Render::startFrame' function */

/* Render all primitives function */
void Render::render( void )
{
  startFrame();

  static float angle = 0;
  angle += 0.030f;

  _camera.setCamera(true, { 0, 0, 5 }, { 0, 0, 0 }, { 0, 1, 0 }, _width, _height);

  _constBuffer._data._view = _camera._viewMatr;
  _constBuffer._data._proj = _camera._projMatr;

  _constBuffer._data._lightPos = {sin(angle), cos(angle), 1, 1};

  _constBuffer._data._lightColor = {1, 1, 1, 1};

  _constBuffer._data._cameraDir = {_camera._dir[0], _camera._dir[1], _camera._dir[2], 1};
  _constBuffer._data._cameraPos = {_camera._loc[0], _camera._loc[1], _camera._loc[2], 1};

  class dummy
  {
  private:
    Render *_rnd;

  public:
    dummy( Render *Rnd ) : _rnd(Rnd) {}

    void operator()( Prim *P )
    {
      P->_world = math::Matr4f().setIdentity(1);
      _rnd->drawPrim(P);
    }
  };
  _primitives.iterate<dummy>(dummy(this));

  endFrame();
} /* End of 'Render::render' function */

/* End frame function */
void Render::endFrame( void )
{
  _swapChain->Present(1, 0);
} /* End of 'Render::endFrame' function */

/* END OF 'render.cpp' FILE */
