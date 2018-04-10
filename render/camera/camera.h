/*
 * Game project
 *
 * FILE: camera.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 31.03.2018
 * NOTE: camera handle declaration file
 */

#ifndef _CAMERA_H_INCLUDED__
#define _CAMERA_H_INCLUDED__
#pragma once

#include "../../math/math_def.h"

/* Render handle namespace */
namespace render
{
  /* Camera class */
  class Camera
  {
  public:
    ///*** Camera vectors ***///
    math::Vec3f _loc;     // Camera's location vector
    math::Vec3f _lookAt;  // Look at point of camera
    math::Vec3f _dir;     // Camera's direction vector
    math::Vec3f _up;      // Identity vector of camera which direction is up
    math::Vec3f _right;   // Identity vector of camera which direction is right

    ///*** Camera projection plane parametrs ***///
    float _projDist;      // Project plane distance
    float _projWidth;     // Width of project plane
    float _projHeight;    // Height of project plane
    float _farClip;       // FarClip distance
    int _screenWidth;     // Screen width
    int _screenHeight;    // Screen height

    ///*** Camera matrices ***///
    math::Matr4f _viewMatr;   // View matrix of camera
    math::Matr4f _projMatr;   // Perscpective projection matrix

  private:
    /* Disallow copy and assignment */
    Camera(const Camera &Cam);
    Camera & operator=(const Camera &Cam);

    /* Update camera direction, position and up vectors function. */
    void updateDirLocUp(void);

    /* Update camera look at point, position and up vectors function. */
    void updateLookAtLocUp(void);

    /* Update camera matrices function. */
    void updateViewMatr(void);

    /* Update projection data of camera function. */
    void updateProjData(void);

  public:
    /* Default class constructor */
    Camera(void) = default;

    /* Class constructor */
    Camera(bool IsLookAt, const math::Vec3f &Loc, const math::Vec3f &At, const math::Vec3f &Up,
      int ScreenW = 500, int ScreenH = 500, float ProjW = 0.4, float ProjH = 0.3, float ProJDist = 1.0, float Far = 10000.0);

    ///*** Setting functions ***///

    /* Setting camera position function. */
    void setPos(const math::Vec3f &Pos);

    /* Setting camera direction by look at point function. */
    void setDir(const math::Vec3f &At);

    /* Setting projection plane parameters function. */
    void setProjParams(float W, float H, float ProjD = 1.0, float Far = 10000.0);

    /* Set camera look at, location and up vectors function. */
    void setLookAtLocUp(const math::Vec3f &Loc, const math::Vec3f &LookAt, const math::Vec3f &Up);

    /* Set camera direction, location and up vectors function. */
    void setDirLocUp(const math::Vec3f &Loc, const math::Vec3f &Dir, const math::Vec3f &Up);

    /* Set camera parameters constructor */
    void setCamera(bool IsLookAt, const math::Vec3f &Loc, const math::Vec3f &At, const math::Vec3f &Up,
      int ScreenW = 500, int ScreenH = 500, float ProjW = 1, float ProjH = 1, float ProJDist = 1.0, float Far = 10000.0);

    ///*** Transformation functions ***///

    /* Move forward camera function. */
    Camera & moveForward(float Dist);

    /* Translate camera by vector function. */
    Camera & translate(const math::Vec3f &Trans);

    /* Translate camera by axis components function. */
    Camera & translate(float Dx, float Dy, float Dz);

    /* Move right camera function. */
    Camera & moveRight(float Dist);

    /* Move up camera function. */
    Camera & moveUp(float Dist);

    /* Rotate camera around right vector function. */
    Camera & rotateAroundRight(float Angle);

    /* Rotate camera around direction vector function. */
    Camera & rotateAroundDir(float Angle);

    /* Rotate camera around up vector function. */
    Camera & rotateAroundUp(float Angle);
  }; /* End of 'Camera' class */
} /* End of 'render' namespace */

#endif /* _CAMERA_H_INCLUDED__ */

/* END OF 'camera.h' FILE */
