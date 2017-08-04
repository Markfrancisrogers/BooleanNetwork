/*
 * FlipEnumerator.hpp
 *
 *  Created on: Aug 26, 2009
 *      Author: stewie
 */

#ifndef PERTURBATIONENUMERATOR_HPP_
#define PERTURBATIONENUMERATOR_HPP_

#include <cassert>
#include <cstddef>
#include <vector>
#include <algorithm>

#include <boost/type_traits/is_convertible.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/iterator_range.hpp>

namespace bn {

namespace gen {

namespace detail {

template<class StateIt> class PerturbationIterator;

template<class StateIt> struct PerturbationIteratorBase {
	typedef typename boost::iterator_value<StateIt>::type value_type;
	typedef boost::iterator_adaptor<PerturbationIterator<StateIt> , StateIt,
			value_type, boost::use_default, const value_type> base;
};

/**
 * ForwardTraversal Readable Iterator.
 */
template<class StateIt> class PerturbationIterator : public PerturbationIteratorBase<
		StateIt>::base {
private:
	friend class boost::iterator_core_access;

	typedef typename PerturbationIteratorBase<StateIt>::value_type value_t;

	StateIt end;
	value_t current;
	std::vector<bool> sitesToFlip;

	value_t dereference() const {
		value_t res(current);
		typename value_t::size_type i = 0;
		for (std::vector<bool>::const_iterator it = sitesToFlip.begin(), end =
				sitesToFlip.end(); it != end; ++it, ++i) {
			if (*it)
				res.flip(i);
		}
		return res;
	}

	bool equal(const PerturbationIterator& other) const {
		return this->base_reference() == other.base_reference();
	}

	void increment() {
		if (!std::next_permutation(sitesToFlip.begin(), sitesToFlip.end()))
			ensureNotEmpty();
	}

	void ensureNotEmpty() {
		if (++this->base_reference() != end) {
			current = *(this->base_reference());
			assert(current.size() == sitesToFlip.size());
		}
	}

public:

	PerturbationIterator() {
	}

	PerturbationIterator(const StateIt first, const StateIt last, size_t h) :
		PerturbationIterator::iterator_adaptor_(first), end(last) {
		if (this->base_reference() != end) {
			current = *(this->base_reference());
			sitesToFlip.resize(current.size(), false);
			for (std::vector<bool>::reverse_iterator it = sitesToFlip.rbegin(),
					end = sitesToFlip.rend(); it != end && h > 0; ++it, --h) {
				*it = true;
			}
		}
	}
};

}

/**
 * @ingroup generator
 *
 * Forward Traversal Readable range.
 */
template<class SinglePassRange> class PerturbationEnumerator : public boost::iterator_range<
		detail::PerturbationIterator<typename boost::range_iterator<
				SinglePassRange>::type> > {
private:
	typedef detail::PerturbationIterator<typename boost::range_iterator<
			SinglePassRange>::type> iter;
	typedef boost::iterator_range<iter> base;

public:

	PerturbationEnumerator() {
	}

	PerturbationEnumerator(SinglePassRange& gen, const size_t n) :
		base(iter(boost::begin(gen), boost::end(gen), n), iter(boost::end(gen),
				boost::end(gen), n)) {
	}

	template<class OtherRange> PerturbationEnumerator(
			const PerturbationEnumerator<OtherRange>& other) :
		base(other) {
	}
};

namespace tag {

struct enumerate_perturbations {

	template<class SinglePassRange> struct apply {
		typedef PerturbationEnumerator<SinglePassRange> type;
	};
};

} // namespace tag

struct enumerate_perturbations {
	const size_t n;

	enumerate_perturbations(const size_t n) :
		n(n) {
	}
};

template<class SinglePassRange> PerturbationEnumerator<SinglePassRange> operator|(
		SinglePassRange& sr, const enumerate_perturbations ep) {
	return PerturbationEnumerator<SinglePassRange> (sr, ep.n);
}

template<class SinglePassRange> PerturbationEnumerator<const SinglePassRange> operator|(
		const SinglePassRange& sr, const enumerate_perturbations ep) {
	return PerturbationEnumerator<const SinglePassRange> (sr, ep.n);
}

} // namespace gen

} // namespace bn

#endif /* PERTURBATIONENUMERATOR_HPP_ */
