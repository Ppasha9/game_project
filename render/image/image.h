/*
 * Game project
 *
 * FILE: image.h
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 09.04.2018
 * NOTE: image handle declaration file
 */

#pragma once

#include "..\..\def.h"

/* Render handle namespace */
namespace render
{
  /* Raster image class */
  class Image
  {
  private:
    struct TargaHeader
    {
      unsigned char _data1[12];
      unsigned short _width;
      unsigned short _height;
      unsigned char _bpp;     // bits per pixel
      unsigned char _data2;
    };

    math::Vec4uc *_pixels;
    unsigned int _width, _height;

  public:
    /* Create image function */
    Image( math::Vec4uc *Pixels = nullptr, unsigned int Width = 0, unsigned int Height = 0 );

    /* Destroy image function */
    ~Image( void );

    /* Clear image function */
    Image & clear( void );

    /* Get image width function */
    unsigned int getWidth( void ) const;

    /* Get image height function */
    unsigned int getHeight( void ) const;

    /* Get image pixels array function */
    const math::Vec4uc * getPixels( void ) const;

    /* Load image from Targa file function */
    Image & loadTGA( const string &FileName );
  }; /* End of 'Image' class */
}; /* end of 'render' namespace */

/* END OF 'image.h' FILE */
