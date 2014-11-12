//---------------------------------------------------------------------------//
/*
  Copyright (c) 2014, Stuart R. Slattery
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

  *: Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  *: Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

  *: Neither the name of the Oak Ridge National Laboratory nor the
  names of its contributors may be used to endorse or promote products
  derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
//---------------------------------------------------------------------------//
/*!
 * \brief DTK_EntityPredicates.hpp
 * \author Stuart R. Slattery
 * \brief Basic entity predicates.
 */
//---------------------------------------------------------------------------//

#ifndef DTK_ENTITYPREDICATES_HPP
#define DTK_ENTITYPREDICATES_HPP

#include <functional>

#include "DTK_Types.hpp"
#include "DTK_Entity.hpp"

#include <Teuchos_Array.hpp>

namespace DataTransferKit
{
//---------------------------------------------------------------------------//
/*!
  \class SelectAllPredicate
  \brief Predicates for selecting all entities.
*/
class SelectAllPredicate
{
  public:

    SelectAllPredicate()
    { /* ... */ }

    ~SelectAllPredicate() { /* ... */ }

    bool operator()( Entity entity ) { return true; }

    std::function<bool(Entity)> getFunction() const
    { return std::function<bool(Entity)>(*this); }
};

//---------------------------------------------------------------------------//
/*!
  \class BlockPredicate
  \brief Predicates for selecting entities in a block.
*/
class BlockPredicate
{
  public:

    BlockPredicate( const Teuchos::Array<int>& block_ids ) 
	: d_block_ids( block_ids )
    { /* ... */ }

    ~BlockPredicate() { /* ... */ }

    bool operator()( Entity entity );

    std::function<bool(Entity)> getFunction() const;

  private:

    // Blocks
    Teuchos::Array<int> d_block_ids;
};

//---------------------------------------------------------------------------//
/*!
  \class BoundaryPredicate
  \brief Predicates for selecting entities on a boundary.
*/
class BoundaryPredicate
{
  public:

    BoundaryPredicate( const Teuchos::Array<int>& boundary_ids ) 
	: d_boundary_ids( boundary_ids )
    { /* ... */ }

    ~BoundaryPredicate() { /* ... */ }

    bool operator()( Entity entity );

    std::function<bool(Entity)> getFunction() const;

  private:

    // Boundaries.
    Teuchos::Array<int> d_boundary_ids;
};

//---------------------------------------------------------------------------//

} // end namespace DataTransferKit

//---------------------------------------------------------------------------//

#endif // end DTK_ENTITYPREDICATES_HPP

//---------------------------------------------------------------------------//
// end DTK_EntityPredicates.hpp
//---------------------------------------------------------------------------//
