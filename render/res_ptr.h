/*
 * Game project
 *
 * FILE: res_ptr.h
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 06.04.2018
 * NOTE: resource pointer handle file
 */

#pragma once

#include "..\def.h"

/* Render handle namespace */
namespace render
{
  /* Resource pointer class */
  template<typename ResType>
    class ResPtr
    {
      friend class ResMap<ResType>;
      friend class Render;

    private:
      ResType *_resource;

    public:
      /* Create resource pointer function */
      ResPtr( void ) : _resource(nullptr)
      {
      } /* End of 'ResPtr' function */

      /* Create resource pointer function */
      ResPtr( ResType *Resource ) : _resource(Resource)
      {
        if (Resource != nullptr)
          _resource->_nooInst++;
      } /* End of 'ResPtr' function */

      /* Destroy resource pointer function */
      ResPtr & operator=( const ResPtr &Ptr )
      {
        if (_resource != nullptr)
          _resource->_nooInst--;

        _resource = Ptr._resource;
        if (_resource != nullptr)
          _resource->_nooInst++;

        return *this;
      } /* End of '~ResPtr' function */

      /* Destroy resource pointer function */
      ~ResPtr( void )
      {
        if (_resource != nullptr)
          _resource->_nooInst--;
      } /* End of '~ResPtr' function */
    }; /* End of 'Resource' class */
} /* end of 'render' namespace */

/* END OF 'res_ptr.h' FILE */
