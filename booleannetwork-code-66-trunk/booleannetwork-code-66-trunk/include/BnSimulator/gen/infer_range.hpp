/*
 * infer_range.hpp
 *
 *  Created on: Oct 16, 2009
 *      Author: stewie
 */

#ifndef INFER_RANGE_HPP_
#define INFER_RANGE_HPP_

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/remove_if.hpp>
#include <boost/mpl/vector.hpp>

namespace bn {

namespace gen {

namespace detail {

template<class Seq> class infer_range_impl {
private:
	template<class F, class Arg> struct invoke_apply
		: boost::mpl::apply<F, Arg>::type {
	};

	typedef typename boost::mpl::remove_if<Seq
			, boost::is_same<boost::mpl::_1, boost::mpl::void_>
		>::type filtered;

public:
	typedef typename boost::mpl::fold<
			typename boost::mpl::pop_front<filtered>::type
			, typename boost::mpl::front<filtered>::type
			, invoke_apply<boost::mpl::_2, boost::mpl::_1>
		>::type type;
};

}  // namespace detail

template<class T1
	, class T2 = boost::mpl::void_
	, class T3 = boost::mpl::void_
	, class T4 = boost::mpl::void_
	, class T5 = boost::mpl::void_> struct infer_range
		: detail::infer_range_impl<boost::mpl::vector<T1, T2, T3, T4, T5> > {
};

} // namespace gen

} // namespace bn

#endif /* INFER_RANGE_HPP_ */
