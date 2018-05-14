/*
 * Game project
 *
 * FILE: environment.h
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 13.05.2018
 * NOTE: scene environment class implementation file
 */

#include "environment.h"
#include "..\..\render\render.h"
#include "..\..\render\timer\timer.h"
#include "..\..\physics\phys_system.h"

using namespace render;
using namespace scene;

const float Environment::_width = 90;
const float Environment::_standsWidth = 5;
const float Environment::_standsHeight = 10;
const float Environment::_length = 120;
const float Environment::_height = 90;
const float Environment::_goalWidth = 40;
const float Environment::_goalHeight = 30;
const float Environment::_goalDepth = 30;

const math::Vec3f Environment::_center = {0, 0, 0};
const float Environment::_radius = Environment::_width / 4;

/* Create environment function */
Environment::Environment( void )
{
  auto &rnd = Render::getInstance();

  rnd.createGeom("envi_plane",
    geom::Geom().createPlane(math::Vec3f({0.5, 0, -0.5}),
      math::Vec3f({-1, 0, 0}), math::Vec3f({0, 0, 1}), 1.0f, 1.0f));

  Material::Coeffs coef = {{0.01f, 0.01f, 0.01f, 1}, {0.40f, 0.40f, 0.40f, 1}, {0.5f, 0.5f, 0.5f, 1}, 100};

  // Init render resources
  rnd.setMaterialTexture(rnd.createMaterial("petr_mtl", coef), rnd.createTexture("petr.tga"), 1);
  rnd.setMaterialTexture(rnd.createMaterial("envi_wall_mtl", coef), rnd.createTexture("flat_color.tga"), 1);
  rnd.setMaterialTexture(rnd.createMaterial("envi_purple_mtl", {{1, 0, 1, 1}}), rnd.createTexture("flat_color.tga"), 1);
  rnd.setMaterialTexture(rnd.createMaterial("envi_yellow_mtl", {{1, 1, 0, 1}}), rnd.createTexture("flat_color.tga"), 1);
  rnd.setMaterialTexture(rnd.createMaterial("envi_floor_mtl", coef), rnd.createTexture("floor.tga"), 1);

  _wallPrim = rnd.createPrim("envi_wall", "envi_plane", "envi_wall_mtl");
  _goalPrim = rnd.createPrim("envi_goal", "goal", "envi_wall_mtl");
  _floorPrim = rnd.createPrim("envi_floor", "envi_plane", "envi_floor_mtl");
  _purplePrim = rnd.createPrim("envi_purple", "envi_plane", "envi_purple_mtl");
  _yellowPrim = rnd.createPrim("envi_yellow", "envi_plane", "envi_yellow_mtl");
  _petrPrim = rnd.createPrim("petr", "petr", "petr_mtl");

  // Init physics resources
  auto &phs = phys::PhysicsSystem::getInstance();

  struct plane_bb
  {
    math::Vec3f _normal;
    math::Vec3f _fPoint;
    math::Vec3f _sPoint;
  };

  float slope_len = sqrt(_standsHeight * _standsHeight + _standsWidth * _standsWidth);

  plane_bb
    lw = {{1, 0, 0}, {-_width / 2 - _standsWidth, 0, 0}, {-_width / 2 - _standsWidth, 1, 0}},
    rw = {{-1, 0, 0}, {_width / 2 + _standsWidth, 0, 0}, {_width / 2 + _standsWidth, 1, 0}},
    ls = {{_standsHeight / slope_len, _standsWidth / slope_len, 0}, {-_width / 2, 0, 0}, {-_width / 2, 0, 1}},
    rs = {{-_standsHeight / slope_len, _standsWidth / slope_len, 0}, {_width / 2, 0, 0}, {_width / 2, 0, 1}},
    pw = {{0, 0, 1}, {0, 0, -_length / 2}, {0, 1, -_length / 2}},
    yw = {{0, 0, -1}, {0, 0, _length / 2}, {0, 1, _length / 2}},
    fl = {{0, 1, 0}, {0, 0, 0}, {1, 0, 0}},
    cl = {{0, -1, 0}, {0, _height, 0}, {1, _height, 0}};
  
  struct box_bb
  {
    math::Vec3f _dirVec;
    math::Vec3f _rightVec;
    float _halfHeight;
  };

  box_bb
    bs = {{0, 0, -_goalDepth / 2}, {(_width - _goalWidth) / 4, 0, 0}, _height / 2},
    bg = {{0, 0, -_goalDepth / 2}, {_goalWidth / 2, 0, 0}, _height / 2};

  phs.registerObject("l_wall", {0, 0, 0}, 0, 0, 0, phys::bounding_volume_type::PLANE, &lw);
  phs.registerObject("r_wall", {0, 0, 0}, 0, 0, 0, phys::bounding_volume_type::PLANE, &rw);
  phs.registerObject("l_stands", {0, 0, 0}, 0, 0, 0, phys::bounding_volume_type::PLANE, &ls);
  phs.registerObject("r_stands", {0, 0, 0}, 0, 0, 0, phys::bounding_volume_type::PLANE, &rs);
  phs.registerObject("floor", {0, 0, 0}, 0, 0, 0, phys::bounding_volume_type::PLANE, &fl);
  phs.registerObject("ceiling", {0, 0, 0}, 0, 0, 0, phys::bounding_volume_type::PLANE, &cl);
  //phs.registerObject("p_wall", {0, 0, 0}, 0, 0, 0, phys::bounding_volume_type::PLANE, &pw);
  //phs.registerObject("y_wall", {0, 0, 0}, 0, 0, 0, phys::bounding_volume_type::PLANE, &yw);
  phs.registerObject("p_left", {-(_width - _goalWidth) / 4 - _goalWidth / 2, _height / 2, -(_length + _goalDepth) / 2}, 0, 0, 0, phys::bounding_volume_type::BOX, &bs);
  phs.registerObject("p_right", {(_width - _goalWidth) / 4 + _goalWidth / 2, _height / 2, -(_length + _goalDepth) / 2}, 0, 0, 0, phys::bounding_volume_type::BOX, &bs);
  phs.registerObject("p_goal", {0, _height / 2 + _goalHeight, -(_length + _goalDepth) / 2}, 0, 0, 0, phys::bounding_volume_type::BOX, &bg);
  phs.registerObject("y_left", {-(_width - _goalWidth) / 4 - _goalWidth / 2, _height / 2, (_length + _goalDepth) / 2}, 0, 0, 0, phys::bounding_volume_type::BOX, &bs);
  phs.registerObject("y_right", {(_width - _goalWidth) / 4 + _goalWidth / 2, _height / 2, (_length + _goalDepth) / 2}, 0, 0, 0, phys::bounding_volume_type::BOX, &bs);
  phs.registerObject("y_goal", {0, _height / 2 + _goalHeight, (_length + _goalDepth) / 2}, 0, 0, 0, phys::bounding_volume_type::BOX, &bg);
} /* End of 'Environment::Environment' function */

/* Draw environment function */
void Environment::draw( void )
{
  auto &rnd = Render::getInstance();

  // floor
  rnd.drawPrim(_floorPrim, math::Matr4f().getScale({_width, 1, _length, 1}));

  // stands
  rnd.drawPrim(_wallPrim,
    math::Matr4f().getScale({_standsWidth / (float)cos(atan2(_standsHeight, _standsWidth)), 1, _length, 1}) *
    math::Matr4f().getRotateZ(atan2(_standsHeight, _standsWidth), false) *
    math::Matr4f().getTranslate({-(_width + _standsWidth) / 2, _standsHeight / 2, 0}));
  rnd.drawPrim(_wallPrim,
    math::Matr4f().getScale({_standsWidth / (float)cos(atan2(_standsHeight, _standsWidth)), 1, _length, 1}) *
    math::Matr4f().getRotateZ(-atan2(_standsHeight, _standsWidth), false) *
    math::Matr4f().getTranslate({(_width + _standsWidth) / 2, _standsHeight / 2, 0}));

  // walls
  rnd.drawPrim(_wallPrim,
    math::Matr4f().getScale({_height, 1.0F, _length, 1}) *
    math::Matr4f().getRotateZ(90) *
    math::Matr4f().getTranslate({-(_width / 2 + _standsWidth), _height / 2, 0}));
  rnd.drawPrim(_wallPrim,
    math::Matr4f().getScale({_height, 1.0F, _length, 1}) *
    math::Matr4f().getRotateZ(-90) *
    math::Matr4f().getTranslate({(_width / 2 + _standsWidth), _height / 2, 0}));

  // purple wall
  //rnd.drawPrim(_wallPrim,
  //  math::Matr4f().getScale({_width + 2 * _standsWidth, 1.0F, _height, 1}) *
  //  math::Matr4f().getRotateX(-90) *
  //  math::Matr4f().getTranslate({0, _height / 2, -_length / 2}));
  rnd.drawPrim(_goalPrim,
    math::Matr4f().getScale({_goalWidth, _goalHeight, _goalDepth, 1}) *
    math::Matr4f().getTranslate({0, 0, -_length / 2}));
  rnd.drawPrim(_purplePrim,
    math::Matr4f().getScale({_goalWidth, 1.0F, _goalHeight, 1}) *
    math::Matr4f().getRotateX(-90) *
    math::Matr4f().getTranslate({0, _goalHeight / 2, -_length / 2  - _goalDepth + 0.01F}));

  // yellow wall
  //rnd.drawPrim(_wallPrim,
  //  math::Matr4f().getScale({_width + 2 * _standsWidth, 1.0F, _height, 1}) *
  //  math::Matr4f().getRotateX(90) *
  //  math::Matr4f().getTranslate({0, _height / 2, _length / 2}));
  rnd.drawPrim(_goalPrim,
    math::Matr4f().getScale({_goalWidth, _goalHeight, _goalDepth, 1}) *
    math::Matr4f().getRotateY(180) *
    math::Matr4f().getTranslate({0, 0, _length / 2}));
  rnd.drawPrim(_yellowPrim,
    math::Matr4f().getScale({_goalWidth, 1.0F, _goalHeight, 1}) *
    math::Matr4f().getRotateX(90) *
    math::Matr4f().getTranslate({0, _goalHeight / 2, _length / 2 + _goalDepth - 0.01F}));

  // ceiling
  rnd.drawPrim(_wallPrim,
    math::Matr4f().getScale({_width + 2 * _standsWidth, 1.0F, _length, 1}) *
    math::Matr4f().getRotateX(180) *
    math::Matr4f().getTranslate({0, _height, 0}));

  // petrs
  auto &t = Timer::getInstance();
  const double max_angle = 15, w = 3;
  double angle = max_angle * sin(t._time * w);
  const static float num = 10;
  math::Matr4f world_l = math::Matr4f().getRotateY(-90) * math::Matr4f().getRotateX(angle) * math::Matr4f().getTranslate({-_width / 2, (float)fabs(0.69 * cos(t._time * w)), _length / 2 * (1 - 1.5F / num)});
  math::Matr4f world_r = math::Matr4f().getRotateY(90) * math::Matr4f().getRotateX(angle) * math::Matr4f().getTranslate({_width / 2, (float)fabs(0.69 * cos(t._time * w)), _length / 2 * (1 - 1.5F / num)});
  const static int rows = 3;
  for (int i = 0; i < num; i++)
  {
    for (int j = 0; j < rows; j++)
    {
      rnd.drawPrim(_petrPrim, world_l * math::Matr4f().getTranslate({-j * _standsWidth / rows, j * _standsHeight / rows, _length / num / 2 * (j % 2)}));
      rnd.drawPrim(_petrPrim, world_r * math::Matr4f().getTranslate({j * _standsWidth / rows, j * _standsHeight / rows, _length / num / 2 * (j % 2)}));
    }
    world_l *= math::Matr4f().getTranslate({0, 0, -_length / num});
    world_r *= math::Matr4f().getTranslate({0, 0, -_length / num});
  }
} /* End of 'Environment::draw' function */

/* END OF 'environment.cpp' FILE */
