/*
 * take_n_range.hpp
 *
 *  Created on: Nov 14, 2009
 *      Author: stewie
 */

#ifndef TAKE_N_RANGE_HPP_
#define TAKE_N_RANGE_HPP_

#include <cstddef>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/iterator_range.hpp>

namespace bn {

namespace gen {

namespace detail {

template<class It> class take_n_iterator : public boost::iterator_facade<
		take_n_iterator<It> , typename boost::iterator_value<It>::type,
		boost::forward_traversal_tag,
		typename boost::iterator_reference<It>::type> {
private:
	friend class boost::iterator_core_access;
	template<class > friend class take_n_iterator;

	typedef typename boost::iterator_difference<It>::type base_difference;

	It it;
	base_difference i;

	typename boost::iterator_reference<It>::type dereference() const {
		return *it;
	}

	template<class OtherIt> bool equal(const take_n_iterator<OtherIt>& other) const {
		return i == other.i || it == other.it;
	}

	void increment() {
		++it;
		++i;
	}

	template<class OtherIt> base_difference distance_to(const take_n_iterator<
			OtherIt>& other) const {
		return std::min(std::distance(other.it, it), other.i - i);
	}

public:

	template<class OtherIt> take_n_iterator(
			const take_n_iterator<OtherIt>& other, typename boost::enable_if<
					boost::is_convertible<OtherIt, It> >::type* = NULL) :
		it(other.it), i(other.i) {
	}

	take_n_iterator() {
	}

	take_n_iterator(It it) :
		it(it), i(0) {
	}

	take_n_iterator(It it, const base_difference i) :
		it(it), i(i) {
	}
};

struct take_n_range_adaptor {
	const size_t n;

	take_n_range_adaptor(const size_t n) :
		n(n) {
	}
};

} // namespace detail

template<class R> class take_n_range : boost::iterator_range<take_n_iterator<
		typename boost::range_iterator<R>::type> > {
private:
	typedef boost::iterator_range<take_n_iterator<
			typename boost::range_iterator<R>::type> > base;

public:
	typedef take_n_iterator<typename boost::range_iterator<R>::type> iterator;

	take_n_range(R& r, const size_t n) :
		base(iterator(boost::begin(r)), iterator(boost::end(r), n)) {
	}

	template<class OtherR> take_n_range(const take_n_range<OtherR>& other,
			typename boost::enable_if<boost::is_convertible<
					typename boost::range_iterator<OtherR>::type,
					typename boost::range_iterator<R>::type> >::type* = NULL) :
		base(other) {
	}
};

template<class R> take_n_range<const R> make_take_n_range(const R& r,
		const size_t n) {
	return take_n_range<const R> (r, n);
}

template<class R> take_n_range<R> make_take_n_range(R& r, const size_t n) {
	return take_n_range<R> (r, n);
}

namespace tag {

struct take_n {

	template<class R> struct apply {
		typedef take_n_range<R> type;
	};
};

} // namespace tag

detail::take_n_range_adaptor take_n(const size_t n) {
	return detail::take_n_range_adaptor(n);
}

template<class R> take_n_range<R> operator|(R& r,
		const detail::take_n_range_adaptor& x) {
	return make_take_n_range(r, x.n);
}

template<class R> take_n_range<const R> operator|(const R& r,
		const detail::take_n_range_adaptor& x) {
	return make_take_n_range(r, x.n);
}

} // namespace gen

} // namespace bn

#endif /* TAKE_N_RANGE_HPP_ */
