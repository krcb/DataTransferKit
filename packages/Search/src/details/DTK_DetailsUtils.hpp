/****************************************************************************
 * Copyright (c) 2012-2017 by the DataTransferKit authors                   *
 * All rights reserved.                                                     *
 *                                                                          *
 * This file is part of the DataTransferKit library. DataTransferKit is     *
 * distributed under a BSD 3-clause license. For the licensing terms see    *
 * the LICENSE file in the top-level directory.                             *
 ****************************************************************************/

#ifndef DTK_DETAILS_UTILS_HPP
#define DTK_DETAILS_UTILS_HPP

#include <DTK_DBC.hpp>

#include <Kokkos_Parallel.hpp>
#include <Kokkos_Sort.hpp> // min_max_functor
#include <Kokkos_View.hpp>

namespace DataTransferKit
{

namespace Details
{
// NOTE: This functor is used in exclusivePrefixSum( src, dst ).  We were
// getting a compile error on CUDA when using a KOKKOS_LAMBDA.
template <typename T, typename DeviceType>
class ExclusiveScanFunctor
{
  public:
    ExclusiveScanFunctor( Kokkos::View<T *, DeviceType> const &in,
                          Kokkos::View<T *, DeviceType> const &out )
        : _in( in )
        , _out( out )
    {
    }
    KOKKOS_INLINE_FUNCTION void operator()( int i, T &update,
                                            bool final_pass ) const
    {
        T const in_i = _in( i );
        if ( final_pass )
            _out( i ) = update;
        update += in_i;
    }

  private:
    Kokkos::View<T *, DeviceType> _in;
    Kokkos::View<T *, DeviceType> _out;
};
} // end namespace Details

/** \brief Computes an exclusive scan.
 *
 *  \param[in] src Input view with range of elements to sum
 *  \param[out] dst Output view; may be equal to \p src
 *
 *  When \p dst is not provided or if \p src and \p dst are the same view, the
 *  scan is performed in-place.  "Exclusive" means that the i-th input element
 *  is not included in the i-th sum.
 *
 *  \pre \p src and \p dst must be of rank 1 and have the same size.
 */
template <typename ST, typename... SP, typename DT, typename... DP>
void exclusivePrefixSum( Kokkos::View<ST, SP...> const &src,
                         Kokkos::View<DT, DP...> const &dst )
{
    static_assert(
        std::is_same<typename Kokkos::ViewTraits<DT, DP...>::value_type,
                     typename Kokkos::ViewTraits<
                         DT, DP...>::non_const_value_type>::value,
        "exclusivePrefixSum requires non-const destination type" );

    static_assert( ( unsigned( Kokkos::ViewTraits<DT, DP...>::rank ) ==
                     unsigned( Kokkos::ViewTraits<ST, SP...>::rank ) ) &&
                       ( unsigned( Kokkos::ViewTraits<DT, DP...>::rank ) ==
                         unsigned( 1 ) ),
                   "exclusivePrefixSum requires Views of rank 1" );

    using ExecutionSpace =
        typename Kokkos::ViewTraits<DT, DP...>::execution_space;
    using ValueType = typename Kokkos::ViewTraits<DT, DP...>::value_type;

    auto const n = src.extent( 0 );
    DTK_REQUIRE( n == dst.extent( 0 ) );
    Kokkos::parallel_scan(
        "exclusive_scan", Kokkos::RangePolicy<ExecutionSpace>( 0, n ),
        Details::ExclusiveScanFunctor<ValueType, ExecutionSpace>( src, dst ) );
    Kokkos::fence();
}

/** \brief In-place exclusive scan.
 *
 *  \param[in,out] v View with range of elements to sum
 *
 *  Calls \c exclusivePrefixSum(v, v)
 */
template <typename T, typename... P>
inline void exclusivePrefixSum( Kokkos::View<T, P...> const &v )
{
    exclusivePrefixSum( v, v );
}

/** \brief Get a copy of the last element.
 *
 *  Returns a copy of the last element in the view on the host.  Note that it
 *  may require communication between host and device (e.g. if the view passed
 *  as an argument lives on the device).
 *
 *  \pre \c v is of rank 1 and not empty.
 */
template <typename T, typename... P>
typename Kokkos::ViewTraits<T, P...>::value_type
lastElement( Kokkos::View<T, P...> const &v )
{
    static_assert(
        ( unsigned( Kokkos::ViewTraits<T, P...>::rank ) == unsigned( 1 ) ),
        "lastElement requires Views of rank 1" );
    auto const n = v.extent( 0 );
    DTK_REQUIRE( n > 0 );
    auto v_subview = Kokkos::subview( v, n - 1 );
    auto v_host = Kokkos::create_mirror_view( v_subview );
    Kokkos::deep_copy( v_host, v_subview );
    return v_host( 0 );
}

/** \brief Fills the view with a sequence of numbers
 *
 *  \param[out] v Output view
 *  \param[in] value (optional) Initial value
 *
 *  \note Similar to \c std::iota() but differs in that it directly assigns
 *  <code>v(i) = value + i</code> instead of repetitively evaluating
 *  <code>++value</code> which would be difficult to achieve in a performant
 *  manner while still guaranteeing the order of execution.
 */
template <typename T, typename... P>
void iota( Kokkos::View<T, P...> const &v,
           typename Kokkos::ViewTraits<T, P...>::value_type value = 0 )
{
    using ExecutionSpace =
        typename Kokkos::ViewTraits<T, P...>::execution_space;
    using ValueType = typename Kokkos::ViewTraits<T, P...>::value_type;
    static_assert(
        ( unsigned( Kokkos::ViewTraits<T, P...>::rank ) == unsigned( 1 ) ),
        "iota requires a View of rank 1" );
    static_assert( std::is_arithmetic<ValueType>::value,
                   "iota requires a View with an arithmetic value type" );
    static_assert(
        std::is_same<ValueType, typename Kokkos::ViewTraits<
                                    T, P...>::non_const_value_type>::value,
        "iota requires a View with non-const value type" );
    auto const n = v.extent( 0 );
    Kokkos::parallel_for(
        "iota", Kokkos::RangePolicy<ExecutionSpace>( 0, n ),
        KOKKOS_LAMBDA( int i ) { v( i ) = value + (ValueType)i; } );
    Kokkos::fence();
}

/** \brief Returns the smallest and the greatest element in the view
 *
 *  \param[in] v Input view
 *
 *  Returns a pair on the host with the smallest value in the view as the first
 *  element and the greatest as the second.
 */
template <typename ViewType>
std::pair<typename ViewType::non_const_value_type,
          typename ViewType::non_const_value_type>
minMax( ViewType const &v )
{
    static_assert( ViewType::rank == 1, "minMax requires a View of rank 1" );
    using ExecutionSpace = typename ViewType::execution_space;
    auto const n = v.extent( 0 );
    DTK_REQUIRE( n > 0 );
    Kokkos::Experimental::MinMaxScalar<typename ViewType::non_const_value_type>
        result;
    Kokkos::Experimental::MinMax<typename ViewType::non_const_value_type>
        reducer( result );
    Kokkos::parallel_reduce(
        "minMax", Kokkos::RangePolicy<ExecutionSpace>( 0, n ),
        Kokkos::Impl::min_max_functor<ViewType>( v ), reducer );
    return std::make_pair( result.min_val, result.max_val );
}

} // end namespace DataTransferKit

#endif
