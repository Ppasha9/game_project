/*
 * Game project
 *
 * FILE: res_map.h
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 06.04.2018
 * NOTE: render resource map handle file
 */

#pragma once

#include <map>

#include "..\def.h"
#include "res_ptr.h"

/* Render handle namespace */
namespace render
{
  /* Render resource map class */
  template<typename ResType>
    class ResMap
    {
    private:
      std::map<string, ResType *> _map;
      Render *_rnd;
      void (*_releaseFunc)( Render *Rnd, ResType *Res );

    public:
      /* Create resource map function */
      ResMap( Render *Rnd, void (*ReleaseFunc)( Render *Rnd, ResType *Res ) ) : 
        _rnd(Rnd), _releaseFunc(ReleaseFunc)
      {
      } /* End of '~ResMap' function */

      /* Get resource from map function */
      ResType * get( const string &ResName ) const
      {
        auto res = _map.find(ResName);

        if (res == _map.end())
          return nullptr;

        return res->second;
      } /* End of 'get' function */

      /* Release resource from map function */
      void release( ResPtr<ResType> &Res )
      {
        if (Res._resource == nullptr)
          return;

        if (Res._resource->_nooInst > 1)
        {
          Res._resource->_nooInst--;
          return;
        }

        _releaseFunc(_rnd, Res._resource);

        _map.erase(Res._resource->_name);
        delete Res._resource;
        Res._resource = nullptr;
      } /* End of 'release' function */

      /* Release all resources from map function */
      void releaseAll( void )
      {
        while (_map.size() != 0)
        {
          ResType *r = _map.begin()->second;
          _releaseFunc(_rnd, r);
          _map.erase(_map.begin());
        }
      } /* End of 'releaseAll' function */

      /* Add resource to map function */
      void add( const string &ResName, ResType *NewRes )
      {
        _map[ResName] = NewRes;
      } /* End of 'add' function */

      /* Iterate map function */
      template<typename Functor>
        void iterate( Functor &Func )
        {
          for (auto r : _map)
            Func(r.second);
        } /* End of 'iterate' function */
    }; /* End of 'ResMap' class */
} /* end of 'render' namespace */

/* END OF 'res_map.h' FILE */
