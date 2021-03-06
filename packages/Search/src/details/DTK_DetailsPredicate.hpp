/****************************************************************************
 * Copyright (c) 2012-2017 by the DataTransferKit authors                   *
 * All rights reserved.                                                     *
 *                                                                          *
 * This file is part of the DataTransferKit library. DataTransferKit is     *
 * distributed under a BSD 3-clause license. For the licensing terms see    *
 * the LICENSE file in the top-level directory.                             *
 ****************************************************************************/
#ifndef DTK_PREDICATE_HPP
#define DTK_PREDICATE_HPP

#include <DTK_DetailsAlgorithms.hpp>
#include <DTK_DetailsNode.hpp>

namespace DataTransferKit
{
namespace Details
{

struct NearestPredicateTag
{
};
struct SpatialPredicateTag
{
};

template <typename Geometry>
struct Nearest
{
    using Tag = NearestPredicateTag;

    KOKKOS_INLINE_FUNCTION
    Nearest() = default;

    KOKKOS_INLINE_FUNCTION
    Nearest( Geometry const &geometry, int k )
        : _geometry( geometry )
        , _k( k )
    {
    }

    Geometry _geometry;
    int _k = 0;
};

template <typename Geometry>
struct Intersects
{
    using Tag = SpatialPredicateTag;

    KOKKOS_INLINE_FUNCTION Intersects() = default;

    KOKKOS_INLINE_FUNCTION Intersects( Geometry const &geometry )
        : _geometry( geometry )
    {
    }

    KOKKOS_INLINE_FUNCTION
    bool operator()( Node const *node ) const
    {
        return intersects( _geometry, node->bounding_box );
    }

    Geometry _geometry;
};

using Within = Intersects<Sphere>;
using Overlap = Intersects<Box>;

template <typename Geometry>
KOKKOS_INLINE_FUNCTION Nearest<Geometry> nearest( Geometry const &geometry,
                                                  int k = 1 )
{
    return Nearest<Geometry>( geometry, k );
}

KOKKOS_INLINE_FUNCTION
Within within( Point const &p, double r ) { return Within( {p, r} ); }

KOKKOS_INLINE_FUNCTION
Overlap overlap( Box const &b ) { return Overlap( b ); }

} // end namespace Details
} // end namespace DataTransferKit

#endif
