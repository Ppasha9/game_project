/*
 * Game project
 *
 * FILE: sound.cpp
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 13.05.2018
 * NOTE: Sound class implementation module
 */

#include "sound.h"

using namespace snd;

/* Audio manager */
Audio Audio::AudioManager;

/* Sound initialization function.
 * ARGUMENTS:
 *  - file name:
 *     string FileName;
 *  - looping flag:
 *      bool IsLooped = false;
 * RETURNS: None.
 */
Sound::Sound( std::string FileName, bool IsLooped ) :  _isInited(false), _buffer(FileName),
  _isStarted(false), _is3D(false), _isLooped(false)
{
  bool right = false;
  if (Audio::AudioManager.init())
  {
    /* load a Wave file */
    if(_buffer.load())
    {
      /* Create the source voices, based on loaded Wave format */
      if (!FAILED(Audio::AudioManager.getIXAudio()->CreateSourceVoice(&_sourceVoice, _buffer.waveFormat(),
                                                                           0, XAUDIO2_DEFAULT_FREQ_RATIO,
                                                                           NULL,
                                                                           Audio::AudioManager.getVoiceSends(),
                                                                           NULL)))
      {
        /* Start consuming Audio in the source voice */
        _isInited = true;
        right = true;
      }
    }
  }

  if (right)
  {
    if (IsLooped)
      setLooped(IsLooped);

    Audio::AudioManager.soundRegister(this);
  }
} /* End of 'tvc::Sound::Sound' function */

/* Sound deinitialization function. */
Sound::~Sound( void )
{
  if (_is3D)
    delete _DSPSettings.pMatrixCoefficients;
  Audio::AudioManager.soundDelete(*this);
} /* End of 'Sound::~Sound' function */

/* Sound start play function. */
void Sound::play( void )
{
  if (!_isStarted)
    if (!_isInited)
      return;
    else
      _sourceVoice->Start(), _isStarted = true;
  else
  {
    _sourceVoice->Stop();
    _sourceVoice->FlushSourceBuffers();
    _sourceVoice->Start();
  }
  _sourceVoice->SubmitSourceBuffer(_buffer.XABufferPtr());
} /* End of 'play' function */

/* Sound stop play function. */
void Sound::stop( void )
{
  if (_isStarted)
  {
    _sourceVoice->Stop();
    _sourceVoice->FlushSourceBuffers();
    _isStarted = false;
  }
} /* End of 'stop' function */

/* Sound pause function. */
void Sound::pause( void )
{
  if (_isStarted)
    _sourceVoice->Stop(), _isStarted = false;
} /* End of 'pause' function */

/* Sound set 3d function. */
bool Sound::set3D(const math::Vec3f &EmitterPos, const math::Vec3f &EmitterDir, const math::Vec3f &EmitterUp,
                  const math::Vec3f &ListenerPos, const math::Vec3f &ListenerDir, const math::Vec3f &ListenerUp)
{
  if (!_isInited)
    return false;

  if (_is3D)
    return true;

  /* Get channel mask */
  DWORD ChannelMask;
  Audio::AudioManager.getMasterVoice()->GetChannelMask(&ChannelMask);

  /* X3Daudio instance */
  X3DAudioInitialize(ChannelMask, X3DAUDIO_SPEED_OF_SOUND, _X3DInstance);

  /* Direct Sound points parametres set */
  memset(&_DSPSettings, 0, sizeof(_DSPSettings));
  Audio::AudioManager.getMasterVoice()->GetVoiceDetails(&_voiceDetails);
  float *matrix = new float[_voiceDetails.InputChannels];
  _DSPSettings.SrcChannelCount = 1;
  _DSPSettings.DstChannelCount = _voiceDetails.InputChannels;
  _DSPSettings.pMatrixCoefficients = matrix;

  /* Zero listener and emitter */
  memset(&_emitter, 0, sizeof(_emitter));
  memset(&_listener, 0, sizeof(_listener));
  _emitter.ChannelCount = 1;
  _emitter.CurveDistanceScaler = 75.0f;
  _emitter.DopplerScaler   = 1.0f;
  _emitter.InnerRadius = 2.0f;
  _emitter.InnerRadiusAngle = X3DAUDIO_PI / 4.0f;

  /* Set listener front coordinates */
  _listener.OrientFront = X3DAUDIO_VECTOR(ListenerDir[0], ListenerDir[1], ListenerDir[2]);
  /* Set listener up vector */
  _listener.OrientTop = X3DAUDIO_VECTOR(ListenerUp[0], ListenerUp[1], ListenerUp[2]);
  /* Set listener position */
  _listener.Position = X3DAUDIO_VECTOR(ListenerPos[0], ListenerPos[1], ListenerPos[2]);
  /* Set listener front coordinates */
  _emitter.OrientFront = X3DAUDIO_VECTOR(EmitterDir[0], EmitterDir[1], EmitterDir[2]);
  /* Set listener up vector */
  _emitter.OrientTop = X3DAUDIO_VECTOR(EmitterUp[0], EmitterUp[1], EmitterUp[2]);
  /* Set listener position */
  _emitter.Position = X3DAUDIO_VECTOR(EmitterPos[0], EmitterPos[1], EmitterPos[2]);

  update3D();

  _is3D = true;
  return true;
} /* End of 'set3D' function */

/* Sound set 3d function. */
bool Sound::set3DPos( const math::Vec3f &Pos )
{
  if (!_is3D)
    return false;
  _emitter.Position = X3DAUDIO_VECTOR(Pos[0], Pos[1], Pos[2]);

  update3D();
  return true;
} /* End of 'set3DPos' function */

/* Sound 3d update parametres function. */
void Sound::update3D( void )
{
  X3DAudioCalculate(_X3DInstance, &_listener, &_emitter,
    X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_REVERB,
    &_DSPSettings);
  /* Master voice set output matrix */
  _sourceVoice->SetOutputMatrix(Audio::AudioManager.getMasterVoice(), 1,
                                _voiceDetails.InputChannels, _DSPSettings.pMatrixCoefficients);
  /* Set frequency rotation */
  _sourceVoice->SetFrequencyRatio(_DSPSettings.DopplerFactor);
  /* Set output matrix */
  _sourceVoice->SetOutputMatrix(Audio::AudioManager.getSubmixVoice(),
                                1, 1, &_DSPSettings.ReverbLevel);

  _filterParameters = {LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 6.0f * _DSPSettings.LPFDirectCoefficient), 1.0f};
  _sourceVoice->SetFilterParameters(&_filterParameters);
} /* End of 'update3D' function */

/* Sound set 3d velocity function. */
bool Sound::set3DVelocity( const math::Vec3f &Velocity )
{
  if (!_is3D)
    return false;
  _emitter.Velocity = X3DAUDIO_VECTOR(Velocity[0], Velocity[1], Velocity[2]);
  update3D();
  return true;
} /* End of 'set3DVelocity' function */

/* Sound set loop flag function. */
void Sound::setLooped( bool IsLooped )
{
  if (IsLooped == _isLooped)
    return;

  XAUDIO2_BUFFER *buf = _buffer.XABufferPtr();
  if (IsLooped)
  {
    buf->LoopCount = XAUDIO2_MAX_LOOP_COUNT;
    buf->LoopLength = buf->PlayBegin + buf->PlayLength - 1;
  }
  else
    buf->LoopCount = buf->LoopLength = 0, buf->LoopCount = 0;

  _isLooped = IsLooped;
} /* End of 'setLooped' function */

/* Sound register function. */
void Audio::soundRegister( Sound *Sound )
{
  static int i = 0;
  _sounds[i] = Sound;
  Sound->_id = i++;
} /* End of 'soundRegister' function */

/* Sound delete function. */
void Audio::soundDelete( Sound &Sound )
{
  if (_sounds.size() == 0)
    return;
  auto Find = _sounds.find(Sound._id);
  if (Find != _sounds.end())
    _sounds.erase(Find);

  if (_isPaused)
    for (auto it = _replaySounds.begin(); it != _replaySounds.end(); it++)
      if (*it == Sound._id)
      {
        _replaySounds.erase(it);
        break;
      }
} /* End of 'soundDelete' function */

/* Sounds all pause function. */
void Audio::pause( void )
{
  if (_isPaused)
    return;

  for (auto it = _sounds.begin(); it != _sounds.end(); it++)
  {
    if (it->second->isPlayed())
      _replaySounds.push_back(it->first);
    it->second->pause();
  }
  if (_replaySounds.size() != 0)
    _isPaused = true;
} /* End of 'pause' function */

/* Sounds all stop function. */
void Audio::stop( void )
{
  if (_isPaused)
    return;

  if (_sounds.size() == 0)
    return;

  for (auto it : _sounds)
    it.second->stop();
} /* End of 'stop' function */

/* Sounds replay function. */
void Audio::replay( void )
{
  if (!_isPaused)
    return;

  for (unsigned int i = 0; i < _replaySounds.size(); i++)
  {
    auto Find = _sounds.find(_replaySounds[i]);
    if (Find != _sounds.end())
      Find->second->play();
  }
  _replaySounds.clear();
  _isPaused = false;
} /* End of 'replay' function */

/* END OF 'sound.cpp' FILE */
