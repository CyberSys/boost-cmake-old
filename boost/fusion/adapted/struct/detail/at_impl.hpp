/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_STRUCT_AT_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_STRUCT_AT_IMPL_HPP

#include <boost/fusion/support/detail/access.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/is_const.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct at_impl;

    template <>
    struct at_impl<struct_tag>
    {
        template <typename Seq, typename N>
        struct apply
        {
            typedef
                extension::struct_member<
                    typename remove_const<Seq>::type
                  , N::value
                >
            member;

            typedef typename
                mpl::eval_if<
                    is_const<Seq>
                  , detail::cref_result<member>
                  , detail::ref_result<member>
                >::type
            type;

            static type
            call(Seq& seq)
            {
                return member::call(seq);
            }
        };
    };

    template <>
    struct at_impl<assoc_struct_tag>
      : at_impl<struct_tag>
    {};
}}}

#endif
