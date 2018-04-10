/*
 * Game project
 *
 * FILE: def.h
 * AUTHORS:
 *   Denisov Pavel,
 *   Lebed Pavel,
 *   Vasilyev Peter
 * LAST UPDATE: 07.03.2018
 * NOTE: default declaration file
 */

#pragma once

#include <string>

#include <exception>
#include <stdexcept>

using string = std::string;

namespace render
{
  // Forward declaration
  class Render;
}

namespace error
{
  /// Errors definitions
  using RunError = std::runtime_error;
  using LogError = std::logic_error;
}
