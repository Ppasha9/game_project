/*
 * Game project
 *
 * FILE: def.h
 * AUTHORS:
 *   Vasilyev Peter
 *   Denisov Pavel,
 *   Kozlov Ilya,
 *   Lebed Pavel
 * LAST UPDATE: 10.04.2018
 * NOTE: default declaration file
 */

#pragma once

#include <string>
#include <stdexcept>

#include "math\math_def.h"

using string = std::string;

namespace render
{
  // Forward declaration
  class Render;
  template<typename ResType>
    class ResMap;
}

namespace error
{
  using RunError = std::runtime_error;
}

/* END OF 'def.h' FILE */
