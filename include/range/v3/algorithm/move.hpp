/// \file
// Range v3 library
//
//  Copyright Eric Niebler 2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//
#ifndef RANGES_V3_ALGORITHM_MOVE_HPP
#define RANGES_V3_ALGORITHM_MOVE_HPP

#include <utility>
#include <range/v3/range_fwd.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/range_concepts.hpp>
#include <range/v3/range_traits.hpp>
#include <range/v3/utility/iterator_concepts.hpp>
#include <range/v3/utility/iterator_traits.hpp>
#include <range/v3/utility/invokable.hpp>
#include <range/v3/utility/functional.hpp>
#include <range/v3/utility/move.hpp>
#include <range/v3/utility/static_const.hpp>

namespace ranges
{
    inline namespace v3
    {
        /// \addtogroup group-algorithms
        /// @{
        struct move_fn : aux::move_fn
        {
            using aux::move_fn::operator();

            template<typename I, typename S, typename O, typename P = ident,
                CONCEPT_REQUIRES_(InputIterator<I>() && IteratorRange<I, S>() &&
                    WeaklyIncrementable<O>() && IndirectlyMovable<I, O, P>())>
            std::pair<I, O> operator()(I begin, S end, O out, P proj_ = P{}) const
            {
                auto &&proj = invokable(proj_);
                for(; begin != end; ++begin, ++out)
                {
                    // BUGBUG should the projection be applied *before* the move?
                    auto &&x = iter_move(begin);
                    *out = proj((decltype(x) &&) x);
                }
                return {begin, out};
            }

            template<typename Rng, typename O, typename P = ident,
                typename I = range_iterator_t<Rng>,
                CONCEPT_REQUIRES_(InputIterable<Rng &>() && WeaklyIncrementable<O>() &&
                    IndirectlyMovable<I, O, P>())>
            std::pair<I, O> operator()(Rng &rng, O out, P proj = P{}) const
            {
                return (*this)(begin(rng), end(rng), std::move(out), std::move(proj));
            }
        };

        /// \sa `move_fn`
        /// \ingroup group-algorithms
        namespace
        {
            constexpr auto&& move = static_const<move_fn>::value;
        }

        /// @}
    } // namespace v3
} // namespace ranges

#endif // include guard
