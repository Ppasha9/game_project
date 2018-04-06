/*
 * Game project
 *
 * FILE: render_shader.cpp
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 06.04.2018
 * NOTE: render shader resource handle implementation file
 */

#include <cassert>

#include <d3dcompiler.h>

#include "render.h"

using namespace render;

/* Get shader interface function */
ShaderPtr Render::createShader( const string &ShName )
{
  ShaderPtr tmp;

  if ((tmp = getShader(ShName))._resource != nullptr)
    return tmp;

  Shader *out;
  HRESULT result;
  ID3D10Blob *error_message;
  ID3D10Blob *vertex_shader_buffer;
  ID3D10Blob *pixel_shader_buffer;
  //D3D11_BUFFER_DESC matrix_buffer_desc;

  // Initialize the pointers this function will use to null.
  error_message = nullptr;
  vertex_shader_buffer = nullptr;
  pixel_shader_buffer = nullptr;

  std::wstring vs_name = L"shaders\\vs_";
  vs_name.append(std::wstring(ShName.begin(), ShName.end()));
  vs_name.append(L".hlsl");

  // Compile the vertex shader code.
  result = D3DCompileFromFile(vs_name.c_str(), NULL, NULL, "main",
    "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertex_shader_buffer, &error_message);
  if (FAILED(result))
  {
    // If the shader failed to compile it should have writen something to the error message.
    if (error_message)
      ;//OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
    // If there was  nothing in the error message then it simply could not find the shader file itself.
    else
      MessageBox(_hWnd, ShName.c_str(), "Missing Shader File", MB_OK);

    return nullptr;
  }

  std::wstring ps_name = L"shaders\\ps_";
  ps_name.append(std::wstring(ShName.begin(), ShName.end()));
  ps_name.append(L".hlsl");

  // Compile the pixel shader code.
  result = D3DCompileFromFile(ps_name.c_str(), NULL, NULL, "main",
    "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixel_shader_buffer, &error_message);
  if (FAILED(result))
  {
    // If the shader failed to compile it should have writen something to the error message.
    if (error_message)
      ;//OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
    // If there was  nothing in the error message then it simply could not find the shader file itself.
    else
      MessageBox(_hWnd, ShName.c_str(), "Missing Shader File", MB_OK);

    return nullptr;
  }

  /*** Create shaders from buffers ***/

  out = new Shader(ShName);

  // Create vertex shader
  result = _device->CreateVertexShader(vertex_shader_buffer->GetBufferPointer(),
    vertex_shader_buffer->GetBufferSize(), NULL, &out->_vertexShader);
  if (FAILED(result))
  {
    delete out;

    return nullptr;
  }

  // Create pixel shader
  result = _device->CreatePixelShader(pixel_shader_buffer->GetBufferPointer(),
    pixel_shader_buffer->GetBufferSize(), NULL, &out->_pixelShader);
  if (FAILED(result))
  {
    delete out;

    return nullptr;
  }

  D3D11_INPUT_ELEMENT_DESC polygon_layout[3];

  polygon_layout[0].SemanticName = "POSITION";
  polygon_layout[0].SemanticIndex = 0;
  polygon_layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
  polygon_layout[0].InputSlot = 0;
  polygon_layout[0].AlignedByteOffset = 0;
  polygon_layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  polygon_layout[0].InstanceDataStepRate = 0;

  polygon_layout[1].SemanticName = "NORMAL";
  polygon_layout[1].SemanticIndex = 0;
  polygon_layout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
  polygon_layout[1].InputSlot = 0;
  polygon_layout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
  polygon_layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  polygon_layout[1].InstanceDataStepRate = 0;

  polygon_layout[2].SemanticName = "TEXTURE";
  polygon_layout[2].SemanticIndex = 0;
  polygon_layout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
  polygon_layout[2].InputSlot = 0;
  polygon_layout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
  polygon_layout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  polygon_layout[2].InstanceDataStepRate = 0;

  // Create the vertex input layout.
  result = _device->CreateInputLayout(polygon_layout, 3, vertex_shader_buffer->GetBufferPointer(), 
    vertex_shader_buffer->GetBufferSize(), &out->_inputLayout);

  if (FAILED(result))
  {
    delete out;

    return nullptr;
  }

  vertex_shader_buffer->Release();
  pixel_shader_buffer->Release();

  _shaders.add(ShName, out);

  return out;
} /* End of 'Render::createShader' function */

/* Get shader interface function */
ShaderPtr Render::getShader( const string &ShName ) const
{
  return _shaders.get(ShName);
} /* End of 'Render::getShader' function */

/* Release primitive function */
void Render::releaseShader( Render *Rnd, Shader *Sh )
{
  if (Sh == nullptr)
    return;

  Rnd->releaseRes<ID3D11InputLayout>(Sh->_inputLayout);
  Rnd->releaseRes<ID3D11PixelShader>(Sh->_pixelShader);
  Rnd->releaseRes<ID3D11VertexShader>(Sh->_vertexShader);
} /* End of 'Render::releaseShader' function */

/* Release primitive function */
void Render::releaseShader( ShaderPtr &Sh )
{
  _shaders.release(Sh);
} /* End of 'Render::releaseShader' function */

/* Set shader as active function */
void Render::setShader( Shader *Sh )
{
  // Set vertex input layout
  _deviceContext->IASetInputLayout(Sh->_inputLayout);

  // Set vertex and pixel shaders
  _deviceContext->VSSetShader(Sh->_vertexShader, NULL, 0);
  _deviceContext->PSSetShader(Sh->_pixelShader, NULL, 0);
} /* End of 'Render::setShader' function */

/* END OF 'render_shader.cpp' FILE */
