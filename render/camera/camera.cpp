/*
 * Game project
 *
 * FILE: camera.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 31.03.2018
 * NOTE: camera handle definition file
 */

#include "camera.h"
using namespace render;

/* Class constructor */
Camera::Camera(bool IsLookAt, const math::Vec3f &Loc, const math::Vec3f &At, const math::Vec3f &Up,
  const int ScreenW, const int ScreenH, const float ProjW, const float ProjH, const float ProJDist, const float Far) :
  _loc(Loc), _up(Up), _farClip(Far), _screenHeight(ScreenH), _screenWidth(ScreenW), _projWidth(ProjW), _projHeight(ProjH), _projDist(ProJDist),
  _dir(At), _lookAt(At)
{
  if (IsLookAt)
    _dir = At - Loc;
  else
    _lookAt += Loc;

  _dir.normalize();
  _right = _dir & _up;
  _up = _right & _dir;
  _up.normalize();
  _right.normalize();

  updateViewMatr();
  updateProjData();
} /* End of constructor */

/* Update camera direction, position and up vectors function. */
void Camera::updateDirLocUp(void)
{
  _dir.normalize();
  _right = _dir & _up;
  _right.normalize();
  _up = _right & _dir;
  updateViewMatr();
} /* End of 'updateDirLocUp' function */

/* Update camera look at point, position and up vectors function. */
void Camera::updateLookAtLocUp(void)
{
  _dir = _lookAt - _loc;
  updateDirLocUp();
} /* End of 'updateLookAtLocUp' function */

/* Update camera matrices function. */
void Camera::updateViewMatr(void)
{
  /* Make view transformation matrix */
  _viewMatr = {      _right[0],      _up[0],      _dir[0], 0,
                     _right[1],      _up[1],      _dir[1], 0,
                     _right[2],      _up[2],      _dir[2], 0,
                -_loc * _right, -_loc * _up, -_loc * _dir, 1};
} /* End of 'updateViewMatr' function */

/* Update projection data of camera function. */
void Camera::updateProjData(void)
{
  float xScale, yScale, Scale;

  /* Rescale project plane size for keep proportional aspect ratio */
  xScale = _projWidth / _screenWidth;
  yScale = _projHeight / _screenHeight;
  if (xScale > yScale)
    Scale = yScale;
  else
    Scale = xScale;

  float w = Scale * _screenWidth;
  float h = Scale * _screenHeight;

  /* Evaluate perspective projection matrix */
  _projMatr = { 2.0f * _projDist / w,                    0,                                             0, 0,
                                   0, 2.0f * _projDist / h,                                             0, 0,
                                   0,                    0,             _farClip / (_farClip - _projDist), 1,
                                   0,                    0, _farClip * _projDist / (_projDist - _farClip), 0 };
} /* End of 'updateProjData' function */

/* Setting camera position function. */
void Camera::setPos(const math::Vec3f &Pos)
{
  _loc = Pos;
  updateViewMatr();
} /* End of 'setPos' function */

/* Setting camera direction by look at point function. */
void Camera::setDir(const math::Vec3f &At)
{
  _lookAt = At;
  updateLookAtLocUp();
} /* End of 'setDir' function */

/* Setting projection plane parameters function. */
void Camera::setProjParams(const float W, const float H, const float ProjD, const float Far)
{
  _projWidth = W;
  _projHeight = H;
  _projDist = ProjD;
  _farClip = Far;
  updateProjData();
} /* End of 'setProjParams' function */

/* Set camera look at, location and up vectors function. */
void Camera::setLookAtLocUp(const math::Vec3f &Loc, const math::Vec3f &LookAt, const math::Vec3f &Up)
{
  _loc = Loc;
  _lookAt = LookAt;
  _up = Up;
  updateLookAtLocUp();
} /* End of 'setLookAtLocUp' function */

/* Set camera direction, location and up vectors function. */
void Camera::setDirLocUp(const math::Vec3f &Loc, const math::Vec3f &Dir, const math::Vec3f &Up)
{
  _loc = Loc;
  _dir = Dir;
  _up = Up;
  updateDirLocUp();
} /* End of 'setDirLocUp' function */

/* Assignment operator for camera */
Camera & Camera::operator=(const Camera &Cam)
{
  _loc = Cam._loc;
  _lookAt = Cam._lookAt;
  _dir = Cam._dir;
  _right = Cam._right;
  _up = Cam._up;

  _projDist = Cam._projDist;
  _projHeight = Cam._projHeight;
  _projWidth = Cam._projWidth;
  _farClip = Cam._farClip;
  _screenHeight = Cam._screenHeight;
  _screenWidth = Cam._screenWidth;

  _viewMatr = Cam._viewMatr;
  _projMatr = Cam._projMatr;

  return *this;
} /* End of '=' operator */

/* Set camera parameters constructor */
void Camera::setCamera(bool IsLookAt, const math::Vec3f &Loc, const math::Vec3f &At, const math::Vec3f &Up,
  const int ScreenW, const int ScreenH, const float ProjW, const float ProjH, const float ProJDist, const float Far)
{
  *this = Camera(IsLookAt, Loc, At, Up, ScreenW, ScreenH, ProjW, ProjH, ProJDist, Far);
} /* End of 'setCamera' function */

/* Move forward camera function. */
Camera & Camera::moveForward(const float Dist)
{
  _loc += _dir * Dist;
  _lookAt += _dir * Dist;
  updateViewMatr();
  return *this;
} /* End of 'moveForward' function */

/* Translate camera by vector function. */
Camera & Camera::translate(const math::Vec3f &Trans)
{
  _loc += Trans;
  _lookAt += Trans;
  updateViewMatr();
  return *this;
} /* End of 'translate' function */

/* Translate camera by axis components function. */
Camera & Camera::translate(const float Dx, const float Dy, const float Dz)
{
  math::Vec3f trans = { Dx, Dy, Dz };
  return translate(trans);
} /* End of 'translate' function */

/* Move right camera function. */
Camera & Camera::moveRight(const float Dist)
{
  _loc += _right * Dist;
  _lookAt += _right * Dist;
  updateViewMatr();
  return *this;
} /* End of 'moveRight' function */

/* Move up camera function. */
Camera & Camera::moveUp(const float Dist)
{
  _loc += _up * Dist;
  _lookAt += _up * Dist;
  updateViewMatr();
  return *this;
} /* End of 'moveUp' function */

/* Rotate camera around right vector function. */
Camera & Camera::rotateAroundRight(const float Angle)
{
  // TODO: just do it!!!
  return *this;
} /* End of 'rotateAroundRight' function */

/* Rotate camera around direction vector function. */
Camera & Camera::rotateAroundDir(const float Angle)
{
  // TODO: just do it!!!
  return *this;
} /* End of 'rotateAroundDir' function */

/* Rotate camera around up vector function. */
Camera & Camera::rotateAroundUp(const float Angle)
{
  // TODO: just do it!!!
  return *this;
} /* End of 'rotateAroundUp' function */

/* Resize camera by new size of window */
void Camera::resize(const int NewW, const int NewH)
{
  _screenHeight = NewH;
  _screenWidth = NewW;

  updateViewMatr();
} /* End of 'resize' function */

/* END OF 'camera.cpp' FILE */
