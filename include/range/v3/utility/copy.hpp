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

#ifndef RANGES_V3_UTILITY_COPY_HPP
#define RANGES_V3_UTILITY_COPY_HPP

#include <utility>
#include <range/v3/range_fwd.hpp>
#include <range/v3/utility/concepts.hpp>
#include <range/v3/utility/functional.hpp>
#include <range/v3/utility/static_const.hpp>

namespace ranges
{
    inline namespace v3
    {
        /// \addtogroup group-utility
        /// @{
        namespace aux
        {
            struct copy_fn : pipeable<copy_fn>
            {
                template<typename T,
                    typename U = uncvref_t<T>,
                    CONCEPT_REQUIRES_(Constructible<U, T &&>())>
                U operator()(T && t) const
                {
                    return static_cast<T &&>(t);
                }
            };

            /// \ingroup group-utility
            /// \sa `copy_fn`
            namespace
            {
                constexpr auto&& copy = static_const<copy_fn>::value;
            }
        }
        /// @}
    }
}

#endif
