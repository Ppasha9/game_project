/*
 * Game project
 *
 * FILE: render_texture.cpp
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 20.04.2018
 * NOTE: render texture handle implementation file
 */

#include "render.h"

using namespace render;

/* Create texture from Image buffer function */
TexturePtr Render::createTexture( const string &TexName )
{
  TexturePtr tmp;
  if ((tmp = getTexture(TexName))._resource != nullptr)
    return tmp;

  return createTexture(TexName, Image().loadTGA(string("bin\\textures\\").append(TexName)));
} /* End of 'Render::createTexture' function */

/* Create texture from Image buffer function */
TexturePtr Render::createTexture( const string &TexName, const Image &Src )
{
  TexturePtr tmp;
  if ((tmp = getTexture(TexName))._resource != nullptr)
    return tmp;

  Texture *T = new Texture(TexName);

  D3D11_TEXTURE2D_DESC buffer_desc;
  buffer_desc.Width = Src.getWidth();
  buffer_desc.Height = Src.getHeight();
  buffer_desc.MipLevels = 0;
  buffer_desc.ArraySize = 1;
  buffer_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  buffer_desc.SampleDesc.Count = 1;
  buffer_desc.SampleDesc.Quality = 0;
  buffer_desc.Usage = D3D11_USAGE_DEFAULT;
  buffer_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
  buffer_desc.CPUAccessFlags = 0;
  buffer_desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

  HRESULT result;
  result = _device->CreateTexture2D(&buffer_desc, nullptr, &T->_texBuffer);

  if (FAILED(result))
  {
    delete T;
    return nullptr;
  }

  unsigned int rowPitch = (buffer_desc.Width * 4) * sizeof(unsigned char);
  _deviceContext->UpdateSubresource(T->_texBuffer, 0, nullptr,
    (unsigned char *)Src.getPixels(), rowPitch, 0);

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

  _deviceContext->GenerateMips(T->_texView);

  _textures.add(TexName, T);

  return T;
} /* End of 'Render::createTexture' function */

/* Set texture as active function */
void Render::applyTexture( TexturePtr &Tex, int Id )
{
  if (Tex._resource == nullptr)
    Tex = getTexture("default.tga");

  _deviceContext->PSSetShaderResources(Id, 1, &Tex._resource->_texView);
} /* End of 'Render::applyTexture' function */

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
