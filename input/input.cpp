/* 
 * Game project 
 * 
 * FILE: input.cpp 
 * AUTHORS:
 *   Lebed Pavel
 * LAST UPDATE: 25.03.2018 
 * NOTE: input devices module 
 */

#include "../def.h"
#include "../win/win.h"

#include "input.h"

using namespace error;
using namespace input;

static Input * s_Input; // Singleton instance of input class

Input::Input( HWND HWnd, HINSTANCE HInstace )
{
  HRESULT hr = DirectInput8Create(HInstace, DIRECTINPUT_VERSION,
                                  IID_IDirectInput8, (void **)_inputObj, 0);
  // Create main input object
  if (FAILED(hr))
    throw RunError::runtime_error("Direct input object was not created");
  KeyboardInit(HWnd);
  MouseInit(HWnd);
} /* End of 'Input::Input' function */

 Input::~Input()
 {
 } /* End of 'Input::~Input' function */

void Input::KeyboardInit( HWND HWnd )
{
  // Create keyboard device
  HRESULT hr = _inputObj->CreateDevice(GUID_SysKeyboard, &_keyboardDevice, 0);
  if (FAILED(hr))
    throw RunError::runtime_error("Direct input keyboard device was not created");
  // Set keyboard format
  hr = _keyboardDevice->SetDataFormat(&c_dfDIKeyboard);
  if (FAILED(hr))
    throw RunError::runtime_error("Direct input keyboard format could not be set");
  // Set keyboard cooperative level
  // DISCL_FOREGROUND - when win is active
  // DISCL_NONEXCLUSIVE - other applications get it too
  hr = _keyboardDevice->SetCooperativeLevel(HWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
  if (FAILED(hr))
    throw RunError::runtime_error("Direct input keyboard cooperative level could not be set");
  hr = _keyboardDevice->Acquire();
  if(FAILED(hr))
    throw RunError::runtime_error("Direct input keyboard doesn't running");
} /* End of 'Input::KeyboardInit' function */

void Input::MouseInit( HWND HWnd )
{
} /* End of 'Input::MouseInit' function */

 void Input::Update( void )
 {
 } /* End of 'Input::Update' function */

void Input::Initizalize( HWND HWnd, HINSTANCE HInstace )
{
  if (s_Input != nullptr)
    throw RunError::runtime_error("Input double initialization");
  s_Input = new Input(HWnd, HInstace);
} /* End of 'Input::Initizalize' function */

void Input::Destroy()
{
  if (s_Input == nullptr)
    throw LogError::logic_error("Input not initialized");
  delete s_Input;
  s_Input = nullptr;
} /* End of 'Input::Destroy' function */