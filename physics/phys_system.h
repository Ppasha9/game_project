/*
 * Game project
 *
 * FILE: phys_system.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 22.04.2018
 * NOTE: physics system simulation declaration file
 */

#ifndef _PHYS_SYSTEM_H_INCLUDED__
#define _PHYS_SYSTEM_H_INCLUDED__
#pragma once

#include "collision\resolution\collision_resolver.h"
#include "forces\force.h"

 /* Physics namespace */
namespace phys
{
  /* Physics system class */
  class PhysicsSystem
  {
  private:
    /* Three main classes of collision system */
    ContactDetector _detector;
    ContactGenerator _generator;
    ContactResolver _resolver;

    /* Default class constructor */
    PhysicsSystem(void) = default;

  public:
    /* Getting instance to class function */
    static PhysicsSystem & getInstance(void);

    /* Register new object function */
    void registerObject(const std::string &Name, const math::Vec3f &Pos, const float InverseMass, const float LinDamping,
      const float AngDamping, const bounding_volume_type VolumeType, void *Params);

    /* Register new object function */
    void registerObject(const std::string &Name, PhysObject *Obj, const bounding_volume_type VolumeType, void *Params);

    /* Response function */
    void response(void (*RespFunc)(const math::Vec3f &, const phys::bounding_volume_type, const phys::bounding_volume_type));

    /* Getting physics object transformation matrix for rendering function */
    const math::Matr4f getObjectMatrix(const std::string &Name) const;

    /* Apply force function */
    void applyForceToObj(const std::string &ObjName, const Force *Force);

    /* Getting the pointer to physics object function */
    PhysObject * getObject(const std::string &Name);

    /* Drawing debug primitives function */
    void debugDraw(void) const;

    /* Removing physic object function */
    void removeObject(const std::string &Name);

    /* Class destructor */
    ~PhysicsSystem(void);
  }; /* End of 'PhysicsSystem' class */
}; /* End of 'phys' namespace */

#endif /* _PHYS_SYSTEM_H_INCLUDED__ */

/* END OF 'phys_system.h' FILE */
