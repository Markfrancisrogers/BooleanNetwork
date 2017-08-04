/*
 * concat_map.hpp
 *
 *  Created on: May 1, 2011
 *      Author: stewie
 */

#ifndef CONCAT_MAP_HPP_
#define CONCAT_MAP_HPP_

#include <boost/utility/result_of.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/reference.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/concepts.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace bn {

namespace gen {

namespace detail {

template<class It, class F> struct ConcatMapIterator;

template<class It, class F> struct ConcatMapIteratorBase {
	typedef typename boost::result_of<F(
			typename boost::iterator_value<It>::type)>::type range_t;
	typedef typename boost::range_value<range_t>::type value_type;
	typedef typename boost::range_reference<range_t>::type reference;
	typedef boost::iterator_adaptor<ConcatMapIterator<It, F> , It, value_type,
			boost::single_pass_traversal_tag, reference> base;
};

template<class It, class F> struct ConcatMapIterator : ConcatMapIteratorBase<
		It, F>::base {
private:
	friend class boost::iterator_core_access;
	typedef ConcatMapIteratorBase<It, F> base;
	typedef typename base::range_t range_t;
	It end;
	F f;
	range_t r;

	typename base::value_type dereference() const {
		return r.front();
	}

	bool equal(const ConcatMapIterator& other) const {
		return this->base_reference() == other.base_reference();
	}

	void increment() {
		r.pop_front();
		ensureNotEmpty();
	}

	void ensureNotEmpty() {
		while (r.empty() && ++this->base_reference() != end) {
			r = f(*(this->base_reference()));
		}
	}

public:

	ConcatMapIterator(const It first, const It last, const F& fun) :
		ConcatMapIterator::iterator_adaptor_(first), end(last), f(fun) {
		// r is now a singular range (thus empty)
		if (this->base_reference() != end) {
			r = f(*(this->base_reference()));
			ensureNotEmpty();
		}
	}
};

template<class It, class F> ConcatMapIterator<It, F> operator++(
		ConcatMapIterator<It, F>& it, int) {
	ConcatMapIterator<It, F> tmp(it);
	++it;
	return tmp;
}

} // namespace detail

template<class R, class F> struct ConcatMapRange : boost::iterator_range<
		detail::ConcatMapIterator<typename boost::range_iterator<R>::type, F> > {
	typedef detail::ConcatMapIterator<typename boost::range_iterator<R>::type,
			F> iter;
	typedef boost::iterator_range<iter> base;
	ConcatMapRange(const R& r, const F& f) :
				base(iter(boost::begin(r), boost::end(r), f),
						iter(boost::end(r), boost::end(r), f)) {
	}
};

template<class R, class F> ConcatMapRange<const R, F> concat_map(const R& r,
		const F& f) {
	return ConcatMapRange<const R, F> (r, f);
}

} // namespace gen

} // namespace bn

#endif /* CONCAT_MAP_HPP_ */
