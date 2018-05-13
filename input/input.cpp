/* 
 * Game project 
 * 
 * FILE: input.cpp 
 * AUTHORS:
 *   Lebed Pavel
 * LAST UPDATE: 23.04.2018 
 * NOTE: input devices module 
 */

#include "../def.h"
#include "../render/render.h"

#include "input.h"

#include <algorithm>

using namespace error;
using namespace input;
using namespace render;

static Input * s_Input; // Singleton instance of input class

Input::Input( void ) :
  _inputObj(nullptr), _keyboardDevice(nullptr),
  _mouseDevice(nullptr)
{
  Render &render = Render::getInstance();

  HRESULT hr = DirectInput8Create(render.getHInstanse(), DIRECTINPUT_VERSION,
                                  IID_IDirectInput8, (void **)&_inputObj, 0);
  // Create main input object
  if (FAILED(hr))
    throw RunError::runtime_error("Direct input object was not created");
} /* End of 'Input::Input' function */

Input & Input::GetInstance( void )
{
  return *s_Input;
} /* End of 'Input::GetInstance' function */

Input::~Input()
{
  if (_mouseDevice)
  {
    _mouseDevice->Unacquire();
    _mouseDevice->Release();
    _mouseDevice = nullptr;
  }
  if (_keyboardDevice)
  {
    _keyboardDevice->Unacquire();
    _keyboardDevice->Release();
    _keyboardDevice = nullptr;
  }
  if (_inputObj)
    _inputObj->Release();
} /* End of 'Input::~Input' function */

bool Input::KeyboardInit( void )
{
  // Create keyboard device
  HRESULT hr = _inputObj->CreateDevice(GUID_SysKeyboard, &_keyboardDevice, 0);
  if (FAILED(hr))
  {
    _keyboardDevice = nullptr;
    return false;
  }
  // Set keyboard format
  hr = _keyboardDevice->SetDataFormat(&c_dfDIKeyboard);
  if (FAILED(hr))
  {
    _keyboardDevice->Release();
    _keyboardDevice = nullptr;
    return false;
  }
  // Set keyboard cooperative level
  // DISCL_FOREGROUND - when win is active
  // DISCL_NONEXCLUSIVE - other applications get it too
  hr = _keyboardDevice->SetCooperativeLevel(Render::getInstance().getHWnd(),
                                            DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
  if (FAILED(hr))
  {
    _keyboardDevice->Release();
    _keyboardDevice = nullptr;
    return false;
  }
  // Set default params
  std::memset(_keyStates, 0, sizeof(_keyStates));
  std::memset(_lastKeyStates, 0, sizeof(_lastKeyStates));
  return true;
} /* End of 'Input::KeyboardInit' function */

bool Input::JoySticksInit(const Vec2f &DeadZone)
{
  _deadZone = DeadZone;
  JoySticks.clear();

  XINPUT_STATE state, zeroState;
  ZeroMemory(&zeroState, sizeof(XINPUT_STATE));
  for (UINT i = 0; i < XUSER_MAX_COUNT; i++)
  {
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    if (XInputGetState(i, &state) == ERROR_SUCCESS)
      JoySticks.insert(std::pair<UINT, JoyStick>(i, JoyStick(zeroState, state)));
  }
  return JoySticks.size() != 0;
} /* End of 'Input::JoySticksInit' function */

bool Input::KeyboardUpdate( void )
{
  if (!IsKeyboardAvailable())
    return false;

  HRESULT hr;

  // Copy last state
  std::memcpy(_lastKeyStates, _keyStates, sizeof(_keyStates));

  // Read the keyboard device.
  hr = _keyboardDevice->GetDeviceState(sizeof(_keyStates), _keyStates);

  if (FAILED(hr))
    // If the mouse lost focus or was not acquired then try to get control back.
    if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
      hr = _keyboardDevice->Acquire();
  if (FAILED(hr))
    return false;
  else
    _keyboardDevice->GetDeviceState(sizeof(_keyStates), _keyStates);

  return true;
} /* End of 'Input::KeyboardUpdate' function */


bool Input::MouseInit( void )
{
  // Create keyboard device
  HRESULT hr = _inputObj->CreateDevice(GUID_SysMouse, &_mouseDevice, 0);
  if (FAILED(hr))
  {
    _mouseDevice = nullptr;
    return false;
  }
  // Set keyboard format
  hr = _mouseDevice->SetDataFormat(&c_dfDIMouse2);
  if (FAILED(hr))
  {
    _mouseDevice->Release();
    _mouseDevice = nullptr;
    return false;
  }
  // Set keyboard cooperative level
  // DISCL_FOREGROUND - when win is active
  // DISCL_NONEXCLUSIVE - other applications get it too
  hr = _mouseDevice->SetCooperativeLevel(Render::getInstance().getHWnd(),
                                         DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
  if (FAILED(hr))
  {  
    _mouseDevice->Release();
    _mouseDevice = nullptr;
    return false;
  }
  std::memset(&_mouseState, 0, sizeof(_mouseState));
  std::memset(&_mouseLastState, 0, sizeof(_mouseLastState));
  return true;
} /* End of 'Input::MouseInit' function */

void Input::InitAll(void)
{
  KeyboardInit();
  MouseInit();
  JoySticksInit();
} /* End of 'Input::InitAll' function */

bool Input::MouseUpdate( void )
{
  if (!IsMouseAvailable())
    return false;
  // Acquire device
  HRESULT hr;

  // Read the mouse device.
  hr = _mouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &_mouseState);

  if (FAILED(hr))
    // If the mouse lost focus or was not acquired then try to get control back.
    if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
      hr = _mouseDevice->Acquire();
  if (FAILED(hr))
    return false;
  // Clump mouse position
  Render &render = Render::getInstance();
  int width = render.getWidth();
  int height = render.getHeight();
  // Clump X
  if (_mouseState.lX < 0)
    _mouseState.lX = 0;
  else if (_mouseState.lX > width)
    _mouseState.lX = width;
  // Clump Y
  if (_mouseState.lY < 0)
    _mouseState.lY = 0;
  else if (_mouseState.lY > height)
    _mouseState.lY = height;

  // Copy last state
  std::memcpy(&_mouseLastState, &_mouseState, sizeof(DIMOUSESTATE2));

  return true;
} /* End of 'Input::MouseUpdate' function */

void Input::GetJoyIds(uvec &Ids)
{
  Ids.clear();
  for (auto &it = JoySticks.begin(); it != JoySticks.end(); it++)
    Ids.push_back(it->first);
} /* End of 'Input::GetJoyIds' function */

bool Input::JoyUpdate(UINT Id)
{
  auto joyStick = JoySticks.find(Id);
  if (joyStick == JoySticks.end())
    throw RunError::runtime_error("That joystick doesn't exist");

  XINPUT_STATE state;
  ZeroMemory(&state, sizeof(XINPUT_STATE));
  if (XInputGetState(Id, &state) == ERROR_SUCCESS)
  {
    joyStick->second._lastState = joyStick->second._curState;
    joyStick->second._curState = state;
    return true;
  }
  XINPUT_GAMEPAD_Y;
  return false;
} /* End of 'JoyUpdate' function */

void Input::UpdateAll(void)
{
  KeyboardUpdate();
  MouseUpdate();
  for (auto &it = JoySticks.begin(); it != JoySticks.end(); it++)
    JoyUpdate(it->first);
} /* End of 'Input::UpdateAll' function */

void Input::Initizalize( void )
{
  if (s_Input != nullptr)
    throw RunError::runtime_error("Input double initialization");
  s_Input = new Input();
} /* End of 'Input::Initizalize' function */

bool Input::IsInited( void )
{
  return s_Input != nullptr;
} /* End of 'Input::IsInited' function */

void Input::Destroy( void )
{
  delete s_Input;
  s_Input = nullptr;
} /* End of 'Input::Destroy' function */

bool Input::IsMouseAvailable( void ) const
{
  return _mouseDevice != nullptr;
} /* End of 'Input::IsMouseAvailable' function */

bool Input::IsKeyboardAvailable( void ) const
{
  return _keyboardDevice != nullptr;
} /* End of 'Input::IsKeyboardAvailable' function */

/// Keyboard work functions
bool Input::KeyHit( size_t KeyCode ) const
{
  return (_keyStates[KeyCode] & 0x80) != 0;
} /* End of 'Input::KeyHit' function */

bool Input::KeyNewHit( size_t KeyCode ) const
{
  return (_keyStates[KeyCode] & 0x80) != 0 &&
         (_lastKeyStates[KeyCode] & 0x80) == 0;
} /* End of 'Input::KeyNewHit' function */
/// Mouse work functions
Vec3f Input::MouseSpeed( float Dt ) const
{
  Render &render = Render::getInstance();

  float width = (float)render.getWidth();
  float height = (float)render.getHeight();

  return Vec3f{(_mouseState.lX - _mouseLastState.lX) / width / Dt,
               (_mouseState.lY - _mouseLastState.lY) / height / Dt,
               (float)(_mouseState.lZ - _mouseLastState.lZ) / Dt};
} /* End of 'Input::MouseSpeed' function */

Vec2i Input::MousePos( void ) const
{
  return Vec2i{_mouseState.lX, _mouseState.lY};
} /* End of 'Input::MousePos' function */

bool Input::MouseKeyHit( size_t Key ) const
{
  return (_mouseState.rgbButtons[Key] & 0x80) != 0;
} /* End of 'Input::MouseKeyHit' function */

bool Input::MouseNewKeyHit( size_t Key ) const
{
  return _mouseState.rgbButtons[Key] && 
         !_mouseLastState.rgbButtons[Key];
} /* End of 'Input::MouseKeyHit' function */

/// JoyStick
bool Input::JoyHit(UINT Id, UINT Button) const
{
  auto joy = JoySticks.find(Id);
  if (joy == JoySticks.end())
    throw RunError::runtime_error("That joystick doesn't exist");
  return (joy->second._curState.Gamepad.wButtons & Button) != 0;
} /* End of Input::JoyHit function */

bool Input::IsNewJoyHit(UINT Id, UINT Button) const
{
  auto joy = JoySticks.find(Id);
  if (joy == JoySticks.end())
    throw RunError::runtime_error("That joystick doesn't exist");
  return (joy->second._curState.Gamepad.wButtons & Button) != 0 && 
         (joy->second._lastState.Gamepad.wButtons & Button) == 0;
} /* End of Input::IsNewJoyHit function */

Vec2f Input::LeftStick(UINT Id) const
{
  auto joy = JoySticks.find(Id);
  if (joy == JoySticks.end())
    throw RunError::runtime_error("That joystick doesn't exist");
  const XINPUT_STATE &state = joy->second._curState;

  float normLX = fmaxf(-1, (float)state.Gamepad.sThumbLX / 32767);
  float normLY = fmaxf(-1, (float)state.Gamepad.sThumbLY / 32767);

  Vec2f res;
  res._coords[0] = (abs(normLX) < _deadZone[0] ? 0 : (abs(normLX) - _deadZone[0]) * (normLX / abs(normLX)));
  res._coords[1] = (abs(normLY) < _deadZone[1] ? 0 : (abs(normLY) - _deadZone[1]) * (normLY / abs(normLY)));

  if (_deadZone[0] > 0)
    res._coords[0] *= 1 / (1 - _deadZone[0]);
  if (_deadZone[1] > 0)
    res._coords[1] *= 1 / (1 - _deadZone[1]);
  return res;
} /* End of 'Input::LeftStick' function */

Vec2f Input::LeftStickSpeed(UINT Id) const
{
  auto joy = JoySticks.find(Id);
  if (joy == JoySticks.end())
    throw RunError::runtime_error("That joystick doesn't exist");

  // Get current left
  const XINPUT_STATE &state = joy->second._curState;
  float normLX = fmaxf(-1, (float)state.Gamepad.sThumbLX / 32767);
  float normLY = fmaxf(-1, (float)state.Gamepad.sThumbLY / 32767);

  Vec2f cur;
  cur._coords[0] = (abs(normLX) < _deadZone[0] ? 0 : (abs(normLX) - _deadZone[0]) * (normLX / abs(normLX)));
  cur._coords[1] = (abs(normLY) < _deadZone[1] ? 0 : (abs(normLY) - _deadZone[1]) * (normLY / abs(normLY)));

  if (_deadZone[0] > 0)
    cur._coords[0] *= 1 / (1 - _deadZone[0]);
  if (_deadZone[1] > 0)
    cur._coords[1] *= 1 / (1 - _deadZone[1]);
  // Get last left
  const XINPUT_STATE &statel = joy->second._lastState;
  normLX = fmaxf(-1, (float)statel.Gamepad.sThumbLX / 32767);
  normLY = fmaxf(-1, (float)statel.Gamepad.sThumbLY / 32767);

  Vec2f last;
  last._coords[0] = (abs(normLX) < _deadZone[0] ? 0 : (abs(normLX) - _deadZone[0]) * (normLX / abs(normLX)));
  last._coords[1] = (abs(normLY) < _deadZone[1] ? 0 : (abs(normLY) - _deadZone[1]) * (normLY / abs(normLY)));

  if (_deadZone[0] > 0)
    last._coords[0] *= 1 / (1 - _deadZone[0]);
  if (_deadZone[1] > 0)
    last._coords[1] *= 1 / (1 - _deadZone[1]);
  return cur - last;
} /* End of 'Input::LeftStickSpeed' function */

Vec2f Input::RightStick(UINT Id) const
{
  auto joy = JoySticks.find(Id);
  if (joy == JoySticks.end())
    throw RunError::runtime_error("That joystick doesn't exist");
  const XINPUT_STATE &state = joy->second._curState;

  float normRX = fmaxf(-1, (float)state.Gamepad.sThumbRX / 32767);
  float normRY = fmaxf(-1, (float)state.Gamepad.sThumbRY / 32767);

  Vec2f res;
  res._coords[0] = (abs(normRX) < _deadZone[0] ? 0 : (abs(normRX) - _deadZone[0]) * (normRX / abs(normRX)));
  res._coords[1] = (abs(normRY) < _deadZone[1] ? 0 : (abs(normRY) - _deadZone[1]) * (normRY / abs(normRY)));

  if (_deadZone[0] > 0)
    res._coords[0] *= 1 / (1 - _deadZone[0]);
  if (_deadZone[1] > 0)
    res._coords[1] *= 1 / (1 - _deadZone[1]);
  return res;
} /* End of 'Input::RightStick' function */

Vec2f Input::RightStickSpeed(UINT Id) const
{
  auto joy = JoySticks.find(Id);
  if (joy == JoySticks.end())
    throw RunError::runtime_error("That joystick doesn't exist");
  // gET CURRENT RIEGHT
  const XINPUT_STATE &state = joy->second._curState;
  float normRX = fmaxf(-1, (float)state.Gamepad.sThumbRX / 32767);
  float normRY = fmaxf(-1, (float)state.Gamepad.sThumbRY / 32767);

  Vec2f cur;
  cur._coords[0] = (abs(normRX) < _deadZone[0] ? 0 : (abs(normRX) - _deadZone[0]) * (normRX / abs(normRX)));
  cur._coords[1] = (abs(normRY) < _deadZone[1] ? 0 : (abs(normRY) - _deadZone[1]) * (normRY / abs(normRY)));

  if (_deadZone[0] > 0)
    cur._coords[0] *= 1 / (1 - _deadZone[0]);
  if (_deadZone[1] > 0)
    cur._coords[1] *= 1 / (1 - _deadZone[1]);
 // Get last right
  const XINPUT_STATE &statel = joy->second._curState;
  normRX = fmaxf(-1, (float)statel.Gamepad.sThumbRX / 32767);
  normRY = fmaxf(-1, (float)statel.Gamepad.sThumbRY / 32767);

  Vec2f last;
  last._coords[0] = (abs(normRX) < _deadZone[0] ? 0 : (abs(normRX) - _deadZone[0]) * (normRX / abs(normRX)));
  last._coords[1] = (abs(normRY) < _deadZone[1] ? 0 : (abs(normRY) - _deadZone[1]) * (normRY / abs(normRY)));

  if (_deadZone[0] > 0)
    last._coords[0] *= 1 / (1 - _deadZone[0]);
  if (_deadZone[1] > 0)
    last._coords[1] *= 1 / (1 - _deadZone[1]);
  return cur - last;
} /* End of 'Input::RightStickSpeed' function */

Vec2f Input::Trigers(UINT Id) const
{
  auto joy = JoySticks.find(Id);
  if (joy == JoySticks.end())
    throw RunError::runtime_error("That joystick doesn't exist");
  // Get current triggers
  const XINPUT_STATE &state = joy->second._curState;
  Vec2f res;
  res._coords[0] = (float)state.Gamepad.bLeftTrigger / 255;
  res._coords[1] = (float)state.Gamepad.bRightTrigger / 255;
  return res;
} /* End of 'Input::Trigers' function */

Vec2f Input::TrigersSpeed(UINT Id) const
{
  auto joy = JoySticks.find(Id);
  if (joy == JoySticks.end())
    throw RunError::runtime_error("That joystick doesn't exist");
  // gET CURRENT RIEGHT
  const XINPUT_STATE &state = joy->second._curState;
  Vec2f cur;
  cur._coords[0] = (float)state.Gamepad.bLeftTrigger / 255;
  cur._coords[1] = (float)state.Gamepad.bRightTrigger / 255;

  const XINPUT_STATE &statel = joy->second._lastState;
  Vec2f last;
  last._coords[0] = (float)statel.Gamepad.bLeftTrigger / 255;
  last._coords[1] = (float)statel.Gamepad.bRightTrigger / 255;
  return cur - last;
} /* End of 'Input::TrigersSpeed' function */

/* END OF 'input.cpp' FILE */
