/*
* Game project
*
* FILE: menu_system.cpp
* AUTHORS:
*   Kozlov Ilya
* LAST UPDATE: 13.05.2018
* NOTE: menu system class implementation
*/

#include "menu_system.h"

/* Checking line size */
bool MenuSystem::empty( string & Line )
{
  return Line.find_first_not_of(" \"\t\r\n") == string::npos;
} /* End of 'MenuSystem::s_empty' function */

/* Split line function */
svec MenuSystem::split( string & Line )
{ 
  svec res;

  while (!empty(Line))
  {
    auto off = Line.find_first_not_of(" \t\r\n");
    Line.erase(Line.begin(), Line.begin() + off);
    off = Line.find_first_of(" \t\r\n");

    if (Line[0] == '"')
    {
      Line.erase(Line.begin(), Line.begin() + 1);
      off = Line.find_first_of("\"");
    }
    string param = Line.substr(0, off);
    Line.erase(Line.begin(), Line.begin() + off);
    res.push_back(param);
  }

  return res;
} /* End of 'MenuSystem::s_split' function */

/* Menu system constructor */
MenuSystem::MenuSystem( std::ifstream & In )
{
  string line;

  Rect rt(0, 0, 1, 1);
  math::Vec4f colorBack, colorText, colorDef, colorHov;
  string name = "main", text;

  render::Render &inst = render::Render::getInstance();
  inst.createGeom("button_plane", geom::Geom().createPlane({0, 1, 0}, {1, 0, 0}, {0, 0, 0}));

  while (!In.eof())
  {
    std::getline(In, line);
    svec params = split(line.append("\n"));

    /* skip empty lines */
    if (params.empty())
      continue;

    if (params[0] == "button_click_begin")
    {
      name = "test";
      rt = Rect(0.3, 0.3, 0.07, 0.03);
      colorDef = math::Vec4f({200, 235, 134, 1});
      colorHov = math::Vec4f({220, 255, 154, 1});
      colorText = math::Vec4f({0, 0, 0, 1});
      text = "test";
    }
    else if (params[0] == "button_click_end")
    {
      ButtonClick *b = new ButtonClick(name, rt, colorDef, colorHov, colorText);
      Buttons.push_back(b);

      string mtlName = name;
      mtlName.append("_mtl");

      string geomName = name;
      mtlName.append("_mtl");
      inst.createMaterial(mtlName, {colorDef, {0, 0, 0, 1}, {0, 0, 0, 1}, 1});

      render::PrimPtr bp = inst.createPrim(name, "button_plane", mtlName);
      ButtonPrims.push_back(bp);
    }
    else if (params[0] == "name")
      name = params[1];
    else if (params[0] == "rect")
    {
      float x0 = stof(params[1]), y0 = stof(params[2]), w = stof(params[3]), h = stof(params[4]);
      rt = Rect(x0, y0, w, h);
    }
    else if (params[0] == "text")
      text = params[1];
    else if (params[0] == "coldef")
    {
      float r = stof(params[1]), g = stof(params[2]), b = stof(params[3]);
      colorDef = math::Vec4f({r, g, b, 255});
      colorDef *= 1.0f / 255;
    }
    else if (params[0] == "colhover")
    {
      float r = stof(params[1]), g = stof(params[2]), b = stof(params[3]);
      colorHov =  math::Vec4f({r, g, b, 255});
      colorHov *= 1.0f / 255;
    }
    else if (params[0] == "coltext")
    {
      float r = stof(params[1]), g = stof(params[2]), b = stof(params[3]);
      colorText =  math::Vec4f({r, g, b, 255});
      colorText *= 1.0f / 255;
    }
    else if (params[0] == "colback")
    {
      float r = stof(params[1]), g = stof(params[2]), b = stof(params[3]);
      colorBack =  math::Vec4f({r, g, b, 255});
      colorBack *= 1.0f / 255;
    }
  }
} /* End of constructor */

/* Response function */
std::string MenuSystem::response( bool Pressed, int X, int Y )
{
  render::Render &inst = render::Render::getInstance();

  for (auto bt : Buttons)
    if (bt->isInside(X, Y))
      if (Pressed)
        return bt->getName();
      else
      {
        bt->onHover();
        inst.setMaterialCoeffs(inst.getMaterial(bt->getName() + "_mtl"), {bt->getColor(), {0, 0, 0, 1}, {0, 0, 0, 1}, 1});
      }
    else
    {
      bt->onDef();
      inst.setMaterialCoeffs(inst.getMaterial(bt->getName() + "_mtl"), {bt->getColor(), {0, 0, 0, 1}, {0, 0, 0, 1}, 1});
    }

  return string();
} /* End of 'MenuSystem::response' function */

/* Render function */
void MenuSystem::render()
{
  render::Render &inst = render::Render::getInstance();
  for (int i = 0; i < Buttons.size(); i++)
  {
    math::Matr4f scale(1);
    Rect r = Buttons[i]->getRect();
    scale._values[0][0] *= 1 / r._w;
    scale._values[1][1] *= 1 / r._h;

    math::Matr4f tran(1);
    tran.getTranslate(r._x0, r._y0, 0);

    inst.drawPrim(ButtonPrims[i], scale * tran);
  }
} /* End of 'MenuSystem::render' function */
