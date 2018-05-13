/*
 * Game project
 *
 * FILE: Wave.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 13.05.2018
 * NOTE: Wave class declaration module
 */

#pragma once

#include <Windows.h>
#include <string>
#include <xaudio2.h>
#include <fstream>
#include <string>

#pragma comment (lib, "Xaudio2")

/* Sound namespace */
namespace snd
{
  class Wave
  {
  private:
    WAVEFORMATEX _WF;       /* Wave descriptor */
    XAUDIO2_BUFFER _XABuf;  /* Xaudio2 buffer */
    BYTE *_data;            /* Wave data */
    std::string _fileName;  /* Source file name */
    bool _isInited;         /* Initialization flag */
  public:
    /* Wave class constructor function. */
    Wave( void );

    /* Wave class constructor function. */
    Wave( const std::string &FileName ) : _fileName(FileName), _data(nullptr)
    {
      memset(&_WF, 0, sizeof(WAVEFORMATEX));
      memset(&_XABuf, 0, sizeof(XAUDIO2_BUFFER));
    } /* End of constructor */

    /* Wave class constructor function. */
    Wave( const Wave &W ) : _fileName(W._fileName), _WF(W._WF), _XABuf(W._XABuf), _data(nullptr)
    {
      if (W._data != nullptr)
      {
        _data = new BYTE[_XABuf.AudioBytes];
        memcpy(_data, W._data, _XABuf.AudioBytes);
        _XABuf.pAudioData = _data;
      }
    } /* End of constructor */

    /* Wave class distructor function. */
    ~Wave( void )
    {
      if (_data != nullptr)
        delete[] _data;
    } /* End of destructor */

    /* Audio buffer get ptr function. */
    XAUDIO2_BUFFER * XABufferPtr( void )
    {
      return &_XABuf;
    } /* End of 'XABufferPtr' function */
    
    /* Wave description structure get ptr function. */
    const WAVEFORMATEX * waveFormat( void ) const
    {
      return &_WF;
    } /* End of 'waveFormat' function */

    /* Wave load from file function. */
    bool load( void )
    {
      FILE *F;

      std::string fullPath = std::string("bin//sound//").append(_fileName);
      _fileName = fullPath;
      if ((F = fopen(_fileName.c_str(), "rb")) == NULL)
        return false;

      DWORD 
        dwChunkId = 0, dwFileSize = 0,
        dwChunkSize = 0, dwExtra = 0;
      /* Read file signature */
      fread(&dwChunkId, sizeof(dwChunkId), 1, F);
      /* Look if right signature */
      if(dwChunkId != 'FFIR')
      {
        fclose(F);
        return false;
      }
      /* Read file size */
      fread(&dwFileSize, sizeof(dwFileSize), 1, F);
      if (dwFileSize <= 16)
      {
        fclose(F);
        return false;
      }
      /* Read file extra */
      fread(&dwExtra, sizeof(dwExtra), 1, F);
      if (dwExtra != 'EVAW')
      {
        fclose(F);
        return false;
      }
      /* look for 'fmt ' chunk id */
      bool FilledFormat = false;
      UINT i; 
      for (i = 12; i < dwFileSize; )
      {
        /* Read file signature */
        fread(&dwChunkId, sizeof(dwChunkId), 1, F);
        /* Read chunk size */
        fread(&dwChunkSize, sizeof(dwChunkSize), 1, F);
        if(dwChunkId == ' tmf')
        {
           fread(&_WF, sizeof(_WF), 1, F);
           FilledFormat = TRUE;
           break;
        }
        /* guarantees WORD padding alignment */
        dwChunkSize &= 0xfffffffe;
        i += dwChunkSize;
        fseek(F, dwChunkSize, SEEK_CUR);
      }
      /* We don't know how to read this format */
      if(!FilledFormat)
      {
        fclose(F);
        return false;
      }
      /* Reset file */
      FilledFormat = false;

      bool FilledData = false;
      fseek(F, 12, SEEK_SET);

      for (i = 12; i < dwFileSize; )
      {
        /* Read file signature */
        fread(&dwChunkId, sizeof(dwChunkId), 1, F);
        /* Read chunk size */
        fread(&dwChunkSize, sizeof(dwChunkSize), 1, F);
        if (dwChunkId == 'atad')
        {
          _data = new BYTE[dwChunkSize];
          /* Read chunk size */
          fread(_data, dwChunkSize, 1, F);
          _XABuf.AudioBytes = dwChunkSize;
          _XABuf.pAudioData = _data;
          _XABuf.PlayBegin = 0;
          _XABuf.PlayLength = dwChunkSize / (_WF.wBitsPerSample / _WF.nChannels) * 2;
          FilledData = TRUE;
          break;
        }
        /* guarantees WORD padding alignment */
        dwChunkSize &= 0xfffffffe;
        i += dwChunkSize;
        fseek(F, dwChunkSize, SEEK_CUR);
      }
      if (!FilledData)
      {
        fclose(F);
        return false;
      }
      fclose(F);
      return TRUE;
    } /* End of 'load' function */
  }; /* End of 'Wave' class */
} /* End of 'snd' namespace */

/* END OF 'wave.h' FILE */
