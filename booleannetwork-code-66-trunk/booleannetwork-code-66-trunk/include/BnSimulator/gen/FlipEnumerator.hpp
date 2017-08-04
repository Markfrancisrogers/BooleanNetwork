/*
 * FlipEnumerator.hpp
 *
 *  Created on: Aug 26, 2009
 *      Author: stewie
 */

#ifndef FLIPENUMERATOR_HPP_
#define FLIPENUMERATOR_HPP_

#include <cstddef>

#include <boost/type_traits/is_convertible.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/iterator_range.hpp>

#include "../core/BooleanNetwork_fwd.hpp"

namespace bn {

namespace gen {

namespace detail {

template<class StateIt> class FlipIterator;

template<class StateIt> struct FlipIteratorBase {
	typedef typename boost::iterator_value<StateIt>::type value_type;
	typedef boost::iterator_adaptor<FlipIterator<StateIt> , StateIt,
			value_type, boost::use_default, const value_type> base;
};

/**
 * ForwardTraversal Readable Iterator.
 */
template<class StateIt> class FlipIterator : public FlipIteratorBase<StateIt>::base {
private:
	friend class boost::iterator_core_access;

	typedef typename FlipIteratorBase<StateIt>::value_type value_t;

	StateIt end;
	value_t current;
	typename value_t::size_type i;

	value_t dereference() const {
		return value_t(current).flip(i);
	}

	bool equal(const FlipIterator& other) const {
		return i == other.i && this->base_reference() == other.base_reference();
	}

	void increment() {
		if ((++i) == current.size()) {
			ensureNotEmpty();
			i = 0;
		}
	}

	void ensureNotEmpty() {
		if (++this->base_reference() != end)
			current = *(this->base_reference());
	}

public:

	FlipIterator() {
	}

	FlipIterator(const StateIt first, const StateIt last) :
		FlipIterator::iterator_adaptor_(first), end(last), i(0) {
		if (this->base_reference() != end)
			current = *(this->base_reference());
	}
};

}

/**
 * @ingroup generator
 *
 * Forward Traversal Readable range.
 */
template<class SinglePassRange> class FlipEnumerator : public boost::iterator_range<
		detail::FlipIterator<
				typename boost::range_iterator<SinglePassRange>::type> > {
private:
	typedef detail::FlipIterator<
			typename boost::range_iterator<SinglePassRange>::type> iter;
	typedef boost::iterator_range<iter> base;

public:

	FlipEnumerator() {
	}

	FlipEnumerator(SinglePassRange& gen) :
		base(iter(boost::begin(gen), boost::end(gen)), iter(boost::end(gen),
				boost::end(gen))) {
	}

	template<class OtherRange> FlipEnumerator(
			const FlipEnumerator<OtherRange>& other) :
		base(other) {
	}
};

namespace tag {

struct enumerate_flips {

	template<class SinglePassRange> struct apply {
		typedef FlipEnumerator<SinglePassRange> type;
	};
};

} // namespace tag


struct enumerate_flips {
};

template<class SinglePassRange> FlipEnumerator<SinglePassRange> operator|(
		SinglePassRange& sr, const enumerate_flips&) {
	return FlipEnumerator<SinglePassRange> (sr);
}

template<class SinglePassRange> FlipEnumerator<const SinglePassRange> operator|(
		const SinglePassRange& sr, const enumerate_flips&) {
	return FlipEnumerator<const SinglePassRange> (sr);
}

} // namespace gen

} // namespace bn

#endif /* FLIPENUMERATOR_HPP_ */
