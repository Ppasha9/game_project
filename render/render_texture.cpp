/*
 * Game project
 *
 * FILE: render_texture.cpp
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 06.04.2018
 * NOTE: render texture handle implementation file
 */

#include "render.h"

using namespace render;

/* Get texture interface function */
TexturePtr Render::createTexture( const string &TexName )
{
  TexturePtr tmp;
  if ((tmp = getTexture(TexName))._resource != nullptr)
    return tmp;

  Texture *T = new Texture(TexName);

  const int w = 2, h = 2;

  D3D11_TEXTURE2D_DESC buffer_desc;
  buffer_desc.Width = w;
  buffer_desc.Height = w;
  buffer_desc.MipLevels = 0;
  buffer_desc.ArraySize = 1;
  buffer_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  buffer_desc.SampleDesc.Count = 1;
  buffer_desc.SampleDesc.Quality = 0;
  buffer_desc.Usage = D3D11_USAGE_DEFAULT;
  buffer_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  buffer_desc.MiscFlags = 0;//D3D11_RESOURCE_MISC_GENERATE_MIPS;

  HRESULT result;
  result = _device->CreateTexture2D(&buffer_desc, nullptr, &T->_texBuffer);

  if (FAILED(result))
  {
    delete T;
    return nullptr;
  }


  unsigned int rowPitch = (w * 4) * sizeof(unsigned char);
  unsigned char data[w * h * 4] = {255, 255, 255, 255,
                                   0, 0, 0, 255,
                                   0, 0, 0, 255,
                                   255, 255, 255, 255,};
  _deviceContext->UpdateSubresource(T->_texBuffer, 0, nullptr, data, rowPitch, 0);

  D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
  view_desc.Format = buffer_desc.Format;
  view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  view_desc.Texture2D.MostDetailedMip = 0;
  view_desc.Texture2D.MipLevels = -1;

  result = _device->CreateShaderResourceView(T->_texBuffer, &view_desc, &T->_texView);
  if (FAILED(result))
  {
    releaseRes<ID3D11Texture2D>(T->_texBuffer);
    delete T;
    return nullptr;
  }

  _textures.add(TexName, T);

  return T;
} /* End of 'Render::createTexture' function */

/* Set texture as active function */
void Render::setTexture( Texture *Tex, int Id )
{
  if (Tex == nullptr || Tex->_texView == nullptr)
    return;

  _deviceContext->PSSetShaderResources(Id, 1, &Tex->_texView);
} /* End of 'Render::setTexture' function */

/* Get texture interface function */
TexturePtr Render::getTexture( const string &TexName ) const
{
  return _textures.get(TexName);
} /* End of 'Render::getTexture' function */

/* Release texture function */
void Render::releaseTexture( Render *Rnd, Texture *Tex )
{
  Rnd->releaseRes<ID3D11ShaderResourceView>(Tex->_texView);
  Rnd->releaseRes<ID3D11Texture2D>(Tex->_texBuffer);
} /* End of 'Render::releaseTexture' function */

/* Release texture function */
void Render::releaseTexture( TexturePtr &Tex )
{
  _textures.release(Tex);
} /* End of 'Render::releaseTexture' function */

/* END OF 'render_texture.cpp' FILE */
