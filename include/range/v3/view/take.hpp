/// \file
// Range v3 library
//
//  Copyright Eric Niebler 2013-2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//

#ifndef RANGES_V3_VIEW_TAKE_HPP
#define RANGES_V3_VIEW_TAKE_HPP

#include <type_traits>
#include <range/v3/range_fwd.hpp>
#include <range/v3/range_traits.hpp>
#include <range/v3/range_concepts.hpp>
#include <range/v3/range_interface.hpp>
#include <range/v3/range.hpp>
#include <range/v3/utility/meta.hpp>
#include <range/v3/utility/iterator_traits.hpp>
#include <range/v3/utility/counted_iterator.hpp>
#include <range/v3/utility/static_const.hpp>
#include <range/v3/view/all.hpp>
#include <range/v3/view/counted.hpp>
#include <range/v3/view/view.hpp>

namespace ranges
{
    inline namespace v3
    {
        /// \cond
        namespace detail
        {
            template<typename Rng, bool IsRandomAccess = RandomAccessIterable<Rng>()>
            struct take_view_
              : range_facade<take_view<Rng>, false>
            {
            private:
                friend range_access;
                using base_range_t = view::all_t<Rng>;
                using difference_type_ = range_difference_t<Rng>;
                base_range_t rng_;
                difference_type_ n_;

                detail::counted_cursor<range_iterator_t<Rng>> begin_cursor()
                {
                    return {ranges::begin(rng_), n_};
                }
                CONCEPT_REQUIRES(Iterable<Rng const>())
                detail::counted_cursor<range_iterator_t<Rng const>> begin_cursor() const
                {
                    return {ranges::begin(rng_), n_};
                }
                detail::counted_sentinel end_cursor() const
                {
                    return {};
                }
            public:
                take_view_() = default;
                take_view_(Rng && rng, difference_type_ n)
                  : rng_(view::all(std::forward<Rng>(rng))), n_(n)
                {
                    RANGES_ASSERT(n >= 0);
                }
                range_size_t<Rng> size() const
                {
                    return static_cast<range_size_t<Rng>>(n_);
                }
                base_range_t & base()
                {
                    return rng_;
                }
                base_range_t const & base() const
                {
                    return rng_;
                }
            };

            template<typename Rng>
            struct take_view_<Rng, true>
              : range_interface<take_view<Rng>>
            {
            private:
                using base_range_t = view::all_t<Rng>;
                using difference_type_ = range_difference_t<Rng>;
                base_range_t rng_;
                difference_type_ n_;
            public:
                take_view_() = default;
                take_view_(Rng && rng, difference_type_ n)
                  : rng_(view::all(std::forward<Rng>(rng))), n_(n)
                {
                    RANGES_ASSERT(n >= 0);
                }
                range_iterator_t<Rng> begin()
                {
                    return ranges::begin(rng_);
                }
                range_iterator_t<Rng> end()
                {
                    return next(ranges::begin(rng_), n_);
                }
                CONCEPT_REQUIRES(Iterable<Rng const>())
                range_iterator_t<Rng const> begin() const
                {
                    return ranges::begin(rng_);
                }
                CONCEPT_REQUIRES(Iterable<Rng const>())
                range_iterator_t<Rng const> end() const
                {
                    return next(ranges::begin(rng_), n_);
                }
                range_size_t<Rng> size() const
                {
                    return static_cast<range_size_t<Rng>>(n_);
                }
                base_range_t & base()
                {
                    return rng_;
                }
                base_range_t const & base() const
                {
                    return rng_;
                }
            };
        }
        /// \endcond

        /// \addtogroup group-views
        /// @{
        template<typename Rng>
        struct take_view
          : detail::take_view_<Rng>
        {
            using detail::take_view_<Rng>::take_view_;
        };

        namespace view
        {
            struct take_fn
            {
            private:
                friend view_access;

                template<typename Rng>
                static take_view<Rng>
                invoke_(Rng && rng, range_difference_t<Rng> to, concepts::InputIterable*)
                {
                    return {std::forward<Rng>(rng), to};
                }
                template<typename Rng, CONCEPT_REQUIRES_(!Range<Rng>() && std::is_lvalue_reference<Rng>())>
                static range<range_iterator_t<Rng>>
                invoke_(Rng && rng, range_difference_t<Rng> to, concepts::RandomAccessIterable*)
                {
                    return {begin(rng), next(begin(rng), to)};
                }

                template<typename Int, CONCEPT_REQUIRES_(Integral<Int>())>
                static auto bind(take_fn take, Int to)
                RANGES_DECLTYPE_AUTO_RETURN
                (
                    make_pipeable(std::bind(take, std::placeholders::_1, to))
                )
            #ifndef RANGES_DOXYGEN_INVOKED
                template<typename Int, CONCEPT_REQUIRES_(!Integral<Int>())>
                static detail::null_pipe bind(take_fn, Int)
                {
                    CONCEPT_ASSERT_MSG(Integral<Int>(),
                        "The object passed to view::take must be a model of the Integral concept.");
                    return {};
                }
            #endif
            public:
                template<typename Rng, CONCEPT_REQUIRES_(InputIterable<Rng>())>
                auto operator()(Rng && rng, range_difference_t<Rng> to) const
                RANGES_DECLTYPE_AUTO_RETURN
                (
                    take_fn::invoke_(std::forward<Rng>(rng), to, iterable_concept<Rng>{})
                )
            #ifndef RANGES_DOXYGEN_INVOKED
                template<typename Rng, typename T, CONCEPT_REQUIRES_(!InputIterable<Rng>())>
                void operator()(Rng &&, T &&) const
                {
                    CONCEPT_ASSERT_MSG(InputIterable<T>(),
                        "The object on which view::take operates must be a model of the InputIterable "
                        "concept.");
                    CONCEPT_ASSERT_MSG(Integral<T>(),
                        "The second argument to view::take must be a model of the Integral concept.");
                }
            #endif
            };

            /// \relates take_fn
            /// \ingroup group-views
            namespace
            {
                constexpr auto&& take = static_const<view<take_fn>>::value;
            }
        }
        /// @}
    }
}

#endif
