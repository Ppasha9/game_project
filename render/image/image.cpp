/*
 * Game project
 *
 * FILE: image.cpp
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 09.04.2018
 * NOTE: image handle implementation file
 */

#include <cstdio>

#include "image.h"

using namespace render;

/* Create image function */
Image::Image( math::Vec4uc *Pixels, unsigned int Width, unsigned int Height ) :
  _width(Width), _height(Height)
{
  if (Pixels == nullptr)
  {
    _pixels = nullptr;
    return;
  }

  _pixels = new math::Vec4uc[Width * Height];
  for (unsigned int i = 0; i < Width * Height; i++)
    _pixels[i] = Pixels[i];
} /* End of 'Image::Image' function */

/* Destroy image function */
Image::~Image( void )
{
  if (_pixels != nullptr)
    delete[] _pixels;
} /* End of 'Image::~Image' function */

/* Clear image function */
Image & Image::clear( void )
{
  if (_pixels != nullptr)
    delete[] _pixels;
  _width = _height = 0;

  return *this;
} /* End of 'Image::clear' function */

/* Get image width function */
unsigned int Image::getWidth( void ) const
{
  return _width;
} /* End of 'Image::getWidth' function */

/* Get image height function */
unsigned int Image::getHeight( void ) const
{
  return _height;
} /* End of 'Image::getHeight' function */

/* Get image pixels array function */
const math::Vec4uc * Image::getPixels( void ) const
{
  return _pixels;
} /* End of 'Image::getPixels' function */

/* Load image from Targa file function */
Image & Image::loadTGA( const string &FileName )
{
  clear();

  unsigned int image_size;
  FILE *file;
  TargaHeader targa_header;
  math::Vec4uc *targa_image;

  // Open TGA file
  if ((file = fopen(FileName.c_str(), "rb")) == nullptr)
    return *this;

  // Read file header
  if ((unsigned int)fread(&targa_header, sizeof(TargaHeader), 1, file) != 1 ||
      targa_header._bpp != 32)
  {
    fclose(file);
    return *this;
  }

  // Get information from header
  _height = targa_header._height;
  _width = targa_header._width;

  // Calculate pixel array size
  image_size = _width * _height;

  // Allocate targa image data
  targa_image = new math::Vec4uc[image_size];

  // Read in the targa image data.
  if (fread(targa_image, 4, image_size, file) != image_size)
  {
    delete[] targa_image;
    fclose(file);
    clear();
    return *this;
  }

  fclose(file);

  // Allocate pixels array
  _pixels = new math::Vec4uc[image_size];

  // Image in targa_image is stored upside-down, BGRA
  for (unsigned int i = 0; i < _height; i++)
    for (unsigned int j = 0; j < _width; j++)
    {
      unsigned int index = (_height - 1 - i) * _width + j;
      _pixels[i * _width + j] = {targa_image[index][2], // R
                                 targa_image[index][1], // G
                                 targa_image[index][0], // B
                                 targa_image[index][3]};
    }
  delete[] targa_image;

  return *this;
} /* End of 'Image::loadTGA' function */

/* END OF 'image.cpp' FILE */
