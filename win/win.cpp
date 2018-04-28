/*
 * Game project
 *
 * FILE: win.cpp
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 28.04.2018
 * NOTE: windows handle class implementation file
 */

#include "win.h"

/* Create window function */
Win::Win(  HINSTANCE hInstance, int CmdShow ) : _hInstance(hInstance)
{
  WNDCLASSEX wc;

  /* Fill and register window class */
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = sizeof(void *);
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = 0;
  wc.hIcon = 0;
  wc.hIconSm = 0;
  wc.lpszMenuName = 0;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = Win::winFunc;
  wc.lpszClassName = "Game Project";

  if (!RegisterClassEx(&wc))
  {
    MessageBox(nullptr, "Error register window class", "ERROR",
      MB_OK | MB_ICONSTOP);
    return;
  }

  /* Window creation */
  _hWnd = CreateWindow("Game Project",
    "Game Project",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
    nullptr, nullptr, hInstance, (void *)this);

  if (_hWnd == nullptr)
  {
    MessageBox(nullptr, "Error create window", "ERROR",
      MB_OK | MB_ICONSTOP);
    return;
  }

  /* Show and update window */
  ShowWindow(_hWnd, CmdShow);
  UpdateWindow(_hWnd);
} /* End of 'Win::Win' function */

/* Process window messages function */
LRESULT CALLBACK Win::winFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  Win *win;

  switch (Msg)
  {
  case WM_CREATE:
    /* Attach 'this' pointer to window class to window */
    SetWindowLong(hWnd, 0, (LONG)((CREATESTRUCT *)lParam)->lpCreateParams);
  default:
    win = (Win *)GetWindowLong(hWnd, 0);
    if (win != nullptr)
    {
      switch (Msg)
      {
      case WM_CREATE:
        win->_hWnd = hWnd;
        return win->onCreate((CREATESTRUCT *)lParam) ? 0 : -1;
      case WM_DESTROY:
        win->onDestroy();
        return 0;
      case WM_SIZE:
        win->onSize((UINT)wParam,
          (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
        return 0;
      case WM_ERASEBKGND:
        return (LRESULT)win->onErase((HDC)wParam);
      case WM_PAINT:
        win->onPaint();
        return 0;
      case WM_TIMER:
        win->onTimer();
        return 0;
      case WM_CLOSE:
        win->onClose();
        return 0;
      case WM_ACTIVATE:
        switch(LOWORD(wParam))
        {
        case WA_ACTIVE:
        case WA_CLICKACTIVE:
          win->setActive(true);
          break;
        case WA_INACTIVE:
          win->setActive(false);
        }
        return 0;
      }
      return DefWindowProc(hWnd, Msg, wParam, lParam);
    }
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'Win::winFunc' function */

/* Window message loop function */
WPARAM Win::run( VOID )
{
  MSG msg;

  while (TRUE)
  {
    if (PeekMessage(&msg, nullptr, 0, 0, TRUE))
    {
      if (msg.message == WM_QUIT)
        return msg.wParam;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
      SendMessage(_hWnd, WM_TIMER, 0, 0);
  }
  return msg.wParam;
} /* End of 'Win::run' function */

/* Get window handler function */
HWND Win::getHWnd( void ) const
{
  return _hWnd;
} /* End of 'Win::getHWnd' function */

/* Get window handler function */
HINSTANCE Win::getHInstanse( void ) const
{
  return _hInstance;
} /* End of 'Win::getHInstanse' function */

/* Get window width function */
int Win::getWidth(void) const
{
  return _width;
} /* End of 'Win::getWidth' function */

/* Get window height function */
int Win::getHeight(void) const
{
  return _height;
} /* End of 'Win::getHeight' function */

/* WM_CREATE window message handle function */
bool Win::onCreate( CREATESTRUCT *CS )
{
  SetTimer(_hWnd, 30, 1, nullptr);

  return true;
} /* End of 'Win::onCreate' function */

/* WM_DESTROY window message handle function */
void Win::onDestroy( void )
{
  PostQuitMessage(0);
  KillTimer(_hWnd, 30);
} /* End of 'Win::onDestroy' function */

/* WM_SIZE window message handle function */
void Win::onSize( UINT State, int W, int H )
{
  _width = W;
  _height = H;
  resize(W, H);
} /* End of 'Win::onSize' function */

/* WM_PAINT window message handle function */
void Win::onPaint( void )
{
  PAINTSTRUCT ps;
  
  HDC hDC = BeginPaint(_hWnd, &ps);
  EndPaint(_hWnd, &ps);
} /* End of 'Win::onPaint' function */

/* WM_CLOSE window message handle function */
void Win::onClose( void )
{
  int MessageId = MessageBox(_hWnd,
                    "Exit program?",
                    "Exit",
                    MB_OKCANCEL | MB_ICONQUESTION);
  if (MessageId == IDOK)
    DestroyWindow(_hWnd);
} /* End of 'Win::onClose' function */

/* WM_ERASEBKGND window message handle function */
bool Win::onErase( HDC hDC )
{
  return false;
} /* End of 'Win::onErase' function */

/* WM_TIMER window message handle function */
void Win::onTimer( void )
{
  HDC hDC = GetDC(_hWnd);

  response();

  ReleaseDC(_hWnd, hDC);
} /* End of 'Win::onTimer' function */

/* Exit window function */
void Win::exit( void )
{
  PostMessage(_hWnd, WM_CLOSE, 0, 0);
} /* End of 'Win::exit' function */

/* END OF 'win.cpp' CLASS */
