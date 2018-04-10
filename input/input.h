/* 
 * Game project 
 * 
 * FILE: input.h 
 * AUTHORS:
 *   Lebed Pavel
 * LAST UPDATE: 25.03.2018 
 * NOTE: input devices module 
 */

#pragma once

#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

namespace input
{
  // Defualt keyboard buttons count
  const size_t KEYBOARD_SIZE = 256;

  class Input
  {
  private:
    HWND HWnd;                                  // Window descroptor for devices

    LPDIRECTINPUT8 _inputObj;                   // Main input object
    LPDIRECTINPUTDEVICE8 _keyboardDevice;       // Keyboard process device
    LPDIRECTINPUTDEVICE8 _mouseDevice;          // Mouse process device
    /// Keyboard states
    char _keyStates[KEYBOARD_SIZE];             // Real time keboard state
    bool _isNewKeyStates[KEYBOARD_SIZE];        // Keyboard new hit state
    /// Mouse states
    DIMOUSESTATE2 _mouseState;                  // Real time mouse state
    bool _isNewMouseState[8];                   // Keyboard new hit state

    Input( HWND HWnd, HINSTANCE Hinstace );

    ~Input();
  public:
    void Initizalize( HWND HWnd, HINSTANCE Hinstace = GetModuleHandle(nullptr) );

    void KeyboardInit( void );

    void MouseInit( void );

    void Update( void );

    void Destroy( void );
  };
}

/* END OF 'input.h' FILE */

