/*
* Game project
*
* FILE: Sound.h
* AUTHORS:
*   Denisov Pavel
* LAST UPDATE: 13.05.2018
* NOTE: Sound class declaration module
*/

#pragma once

#include <Windows.h>
#include <x3daudio.h>
#include <map>
#include <vector>
#include "wave.h"
#include "../math/math_def.h"

/* Sound namespace */
namespace snd
{
  class Sound;

  /* Audio class */
  class Audio
  {
    struct SoundInfo
    {
      int _id;       /* Sound id */
      Sound *_sound; /* Sound pointer */

    /* Audio initialization function. */
     SoundInfo( int Id, Sound *Sound ) : _id(Id), _sound(Sound)
     {
     } /* End of 'SoundInfo' function */
    };
  private:
    IXAudio2 *_engine;                     /* Audio 2 control class */
    IXAudio2MasteringVoice *_masterVoice;  /* Master voice class */
    bool _isInited;                        /* Initialization flag */
    bool _isPaused;                        /* Need tp replay Sound flag */
    XAUDIO2_VOICE_SENDS _sendList;         /* Mixed voice send list */
    XAUDIO2_SEND_DESCRIPTOR _send;         /* Mixed voice descriptor */
    IXAudio2SubmixVoice *_submixVoice;     /* Mixed voice ptr */
    std::map<int, Sound *> _sounds;        /* Sound's Sound */
    std::vector<int> _replaySounds;        /* Sounds to replay */

    /* Audio initialization function. */
    Audio(void) : _engine(nullptr), _masterVoice(nullptr), _isInited(false), _isPaused(false)
    {
    } /* End of 'Audio' function */

  public:
    /* Audi manager single tone class */
    static Audio AudioManager;

    /* Sound initialization function. */
    bool init( void )
    {
      if (_isInited)
        return true;

      /* Must call this for COM */
      CoInitializeEx(nullptr, COINIT_MULTITHREADED);

      /* Create the engine */
      if (FAILED(XAudio2Create(&_engine)))
      {
        CoUninitialize();
        return false;
      }

      /* Create the mastering voice */
      if (FAILED(_engine->CreateMasteringVoice(&_masterVoice)))
      {
        _engine->Release();
        CoUninitialize();
        return false;
      }
      /* Create mixed voice */
      if (FAILED(_engine->CreateSubmixVoice(&_submixVoice, 1, 44100, 0, 0, 0, 0)))
      {
        _engine->Release();
        CoUninitialize();
        return false;
      }

      /* Set send list and mix descriptor */
      _send = {0, _submixVoice};
      _sendList = {1, &_send};

      _isInited = true;
      return true;
    } /* End of 'init' function */

    /* IXAudio get intrface function. */
    IXAudio2 * getIXAudio( void )
    {
      return _engine;
    } /* End of 'getIXAudio' function */
    
    /* IXAudio2MasteringVoice get intrface function. */
    IXAudio2MasteringVoice * getMasterVoice( void )
    {
      return _masterVoice;
    } /* End of 'getMasterVoice' function */

    /* XAUDIO2_VOICE_SENDS get list function. */
    XAUDIO2_VOICE_SENDS * getVoiceSends( void )
    {
      return &_sendList;
    } /* End of 'getVoiceSends' function */

    /* Xaudeio sends get descriptor function. */
    XAUDIO2_SEND_DESCRIPTOR * getSendsDescriptor( void )
    {
      return &_send;
    } /* End of 'getSendsDescriptor' function */
    
    /* Submix voice get ptr function. */
    IXAudio2SubmixVoice * getSubmixVoice( void )
    {
      return _submixVoice;
    } /* End of 'getSubmixVoice' function */

    /* Audio deinitialization function. */
    ~Audio( void )
    {
      if (_isInited)
      {
        _engine->Release();
        CoUninitialize();
      }
    } /* End of destructor */

    /* Sound register function. */
    void soundRegister( Sound *Sound );

    /* Sound delete function. */
    void soundDelete( Sound &Sound );

    /* Sounds all pause function. */
    void pause( void );

    /* Sounds all stop function. */
    void stop( void );

    /* Sounds replay function. */
    void replay( void );
  }; /* End of 'Audio' class */

  /* Sound class */
  class Sound
  {
    /* Friend class */
    friend class Audio;
  public:
    /* Sound initialization function. */
    Sound( std::string FileName, bool IsLooped = false );

    /* Sound deinitialization function. */
    ~Sound( void );

    /* Sound start play function. */
    void play( void );

    /* Sound stop play function. */
    void stop( void );

    /* Sound pause function. */
    void pause( void );

    /* Sound set 3d function. */
    bool set3D( const math::Vec3f &EmitterPos, const math::Vec3f &EmitterDir, const math::Vec3f &EmitterUp,
                const math::Vec3f &ListenerPos, const math::Vec3f &ListenerDir, const math::Vec3f &ListenerUp);

    /* Sound set 3d function. */
    bool set3DPos( const math::Vec3f &Pos );

    /* Sound set 3d velocity function. */
    bool set3DVelocity(const math::Vec3f &Velocity );

    /* Sound check play is over function. */
    bool isEnd( void ) const;

    /* Sound check play function. */
    bool isPlayed( void ) const
    {
      return this->_isStarted;
    } /* End of 'IsPlayed' function */

    /* Sound set loop flag function. */
    void setLooped( bool IsLooped );

  private:
    bool _isInited;                              /* Initialization flag */
    bool _isStarted;                             /* Prepare for playing buffer flag */
    bool _is3D;                                  /* 3D Sound flag */
    bool _isLooped;                              /* Looping flag */
    IXAudio2SourceVoice *_sourceVoice;           /* Source voice */
    Wave _buffer;                                /* Wave buffer for Sound */
    X3DAUDIO_LISTENER _listener;                 /* Listener parametres descriptor */
    X3DAUDIO_EMITTER _emitter;                   /* Sound emmit parametres */
    X3DAUDIO_HANDLE _X3DInstance;                /* X3DAudio handle */
    X3DAUDIO_DSP_SETTINGS _DSPSettings;          /* Direct Sound point settings */
    XAUDIO2_VOICE_DETAILS _voiceDetails;         /* X3D voice extra info */
    XAUDIO2_FILTER_PARAMETERS _filterParameters; /* Filter parametres */
    int _id;                                     /* Sound identificator */

    /* Sound 3d update parametres function. */
    void update3D( void );
  }; /* End of 'Sound' class */
} /* End of 'snd' namespace */

/* END OF 'sound.h' FILE */
