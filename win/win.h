/*
 * Game project
 *
 * FILE: win.h
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 27.03.2018
 * NOTE: windows handle class declaration file
 */

#pragma once

#include <windows.h>

/* Windows handle class */
class Win
{
protected:
  HWND _hWnd;           // Window descriptor
  HINSTANCE _hInstance; // App descriptor
  int _width, _height;  // Window size
  bool _isActive;       // Window is active boolean

public:
  /* Create window function */
  Win( HINSTANCE hInstance = GetModuleHandle(nullptr), int CmdShow = SW_SHOWNORMAL );

  /* Process window messages function */
  static LRESULT CALLBACK winFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

  /* Window message loop function */
  WPARAM run( VOID );

  /* Changing the window activity state */
  void setActive( bool ActiveState )
  {
    _isActive = ActiveState;
  } /* End of 'setActive' function */

  /* Virtual render function */
  virtual void render( void )
  {
  } /* End of 'render' function */

  /* Virtual resize for rendering system function */
  virtual void resize( int W, int H )
  {
  } /* End of 'resize' function */

  /* Get window handler function */
  HWND getHWnd( void );

  /* Get window handler function */
  HINSTANCE getHInstanse( void );

  /* WM_CREATE window message handle function */
   bool onCreate( CREATESTRUCT *CS );

  /* WM_DESTROY window message handle function */
   void onDestroy( void );

  /* WM_SIZE window message handle function */
   void onSize( UINT State, int W, int H );

  /* WM_PAINT window message handle function */
   void onPaint( void );

  /* WM_CLOSE window message handle function */
  void onClose( void );

  /* WM_ERASEBKGND window message handle function */
  bool onErase( HDC hDC );

  /* WM_TIMER window message handle function */
  void onTimer( void );

  /* Exit window function */
  void exit( void );
}; /* End of 'Win' class */

/* END OF 'win.h' CLASS */
