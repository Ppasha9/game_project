/*
* Game project
*
* FILE: input.h
* AUTHORS:
*   Lebed Pavel
* LAST UPDATE: 23.04.2018
* NOTE: input devices module
*/

#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

#include <xinput.h>

#include <vector>

#include <map>

#include "../math/math_def.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "XInput.lib")

namespace input
{
  using namespace math;
  using uvec = std::vector<UINT>;


  // Defualt keyboard buttons count
  const size_t KEYBOARD_SIZE = 256;

  class Input
  {
  private:
    LPDIRECTINPUT8 _inputObj;                   // Main input object
    LPDIRECTINPUTDEVICE8 _keyboardDevice;       // Keyboard process device
    LPDIRECTINPUTDEVICE8 _mouseDevice;          // Mouse process device
                                                /// Keyboard states
    char _keyStates[KEYBOARD_SIZE];             // Real time keyboard state
    char _lastKeyStates[KEYBOARD_SIZE];         // Keyboard new hit state
                                                /// Mouse states
    DIMOUSESTATE2 _mouseState;                  // Real time mouse state
    DIMOUSESTATE2 _mouseLastState;              // Keyboard new hit state

    /// Joysticks


    Input( void );

    ~Input();

    struct JoyStick
    {
      XINPUT_STATE _lastState;
      XINPUT_STATE _curState;

      JoyStick(void) = default;
      JoyStick(const XINPUT_STATE &Last,
        const XINPUT_STATE &Cur) :
        _lastState(Last), _curState(Cur)
      {
      } /* End of 'JoyStick' function */
    };
    using gmap = std::map<UINT, JoyStick>;
    gmap JoySticks;
    Vec2f _deadZone;

  public:
    bool KeyboardInit( void );

    bool MouseInit( void );

    bool JoySticksInit(const Vec2f &DeadZone = Vec2f{0.05f, 0.02f});

    static void Initizalize( void );

    static bool IsInited( void );

    bool KeyboardUpdate( void );

    bool MouseUpdate( void );

    void GetJoyIds( uvec &Ids );

    bool JoyUpdate( UINT Id );

    static void Destroy( void );

    bool IsMouseAvailable( void ) const;

    bool IsKeyboardAvailable( void ) const;

    static Input & GetInstance( void );

    // Keyboard work functions
    bool KeyHit( size_t KeyCode ) const;

    bool KeyNewHit( size_t KeyCode ) const;
    // Mouse work functions
    Vec3f MouseSpeed( float Dt = 1.0f ) const; 

    Vec2i MousePos( void ) const;

    bool MouseKeyHit( size_t Key ) const;

    bool MouseNewKeyHit( size_t Key ) const;
    // Joystick work functions
    bool JoyHit(UINT Id, UINT Button) const;

    bool IsNewJoyHit(UINT Id, UINT Button)  const;

    Vec2f LeftStick(UINT Id) const;

    Vec2f LeftStickSpeed(UINT Id) const;

    Vec2f RightStick(UINT Id) const;

    Vec2f RightStickSpeed(UINT Id) const;

    Vec2f Trigers(UINT Id) const;

    Vec2f TrigersSpeed(UINT Id) const;
  };
}

/* END OF 'input.h' FILE */
