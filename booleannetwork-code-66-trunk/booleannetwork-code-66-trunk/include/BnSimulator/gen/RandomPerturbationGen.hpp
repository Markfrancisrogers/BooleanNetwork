/*
 * RandomPerturbationGen.hpp
 *
 *  Created on: Aug 26, 2009
 *      Author: stewie
 */

#ifndef RANDOMPERTURBATIONGEN_HPP_
#define RANDOMPERTURBATIONGEN_HPP_

#include <cassert>
#include <cstddef>

#include <boost/type_traits/is_convertible.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/iterator_range.hpp>

#include "../core/network_state.hpp"
#include "../util/state_util.hpp"

namespace bn {

namespace gen {

namespace detail {

template<class StateIt> class RandomPerturbationIterator;

template<class StateIt> struct RandomPerturbationIteratorBase {
	typedef typename boost::iterator_value<StateIt>::type value_type;
	typedef boost::iterator_adaptor<RandomPerturbationIterator<StateIt> ,
			StateIt, value_type, boost::use_default, const value_type> base;
};

/**
 * ForwardTraversal Readable Iterator.
 */
template<class StateIt> class RandomPerturbationIterator : public RandomPerturbationIteratorBase<
		StateIt>::base {
private:
	friend class boost::iterator_core_access;

	typedef typename RandomPerturbationIteratorBase<StateIt>::value_type
			value_t;

	StateIt end;
	value_t current;
	size_t count;
	size_t i;
	typename value_t::size_type h;

	value_t dereference() const {
		return util::perturb_state_randomly(current, h);
	}

	bool equal(const RandomPerturbationIterator& other) const {
		return i == other.i && this->base_reference() == other.base_reference();
	}

	void increment() {
		if ((++i) == count) {
			ensureNotEmpty();
			i = 0;
		}
	}

	void ensureNotEmpty() {
		if (++this->base_reference() != end)
			current = *(this->base_reference());
	}

public:

	RandomPerturbationIterator() {
	}

	RandomPerturbationIterator(const StateIt first, const StateIt last,
			const size_t count, const typename value_t::size_type h) :
		RandomPerturbationIterator::iterator_adaptor_(first), end(last), count(
				count), i(0), h(h) {
		if (this->base_reference() != end)
			current = *(this->base_reference());
	}
};

} // namespace detail

/**
 * @ingroup generator
 *
 * Forward Traversal Readable range.
 */
template<class SinglePassRange> class RandomPerturbationGen : public boost::iterator_range<
		detail::RandomPerturbationIterator<typename boost::range_iterator<
				SinglePassRange>::type> > {
private:
	typedef detail::RandomPerturbationIterator<typename boost::range_iterator<
			SinglePassRange>::type> iter;
	typedef boost::iterator_range<iter> base;

public:

	RandomPerturbationGen(SinglePassRange& gen, const size_t probes,
			const State::size_type h) :
		base(iter(boost::begin(gen), boost::end(gen), probes, h), iter(
				boost::end(gen), boost::end(gen), probes, h)) {
	}

	template<class OtherRange> RandomPerturbationGen(
			const RandomPerturbationGen<OtherRange>& other) :
		base(other) {
	}
};

namespace tag {

struct perturb_randomly {

	template<class SinglePassRange> struct apply {
		typedef RandomPerturbationGen<SinglePassRange> type;
	};
};

} // namespace tag

struct perturb_randomly {
	const size_t probes;
	const State::size_type h;

	perturb_randomly(const size_t probes, const State::size_type h) :
		probes(probes), h(h) {
	}
};

template<class SinglePassRange> RandomPerturbationGen<SinglePassRange> operator|(
		SinglePassRange& sr, const perturb_randomly& rp) {
	return RandomPerturbationGen<SinglePassRange> (sr, rp.probes, rp.h);
}

template<class SinglePassRange> RandomPerturbationGen<const SinglePassRange> operator|(
		const SinglePassRange& sr, const perturb_randomly& rp) {
	return RandomPerturbationGen<const SinglePassRange> (sr, rp.probes, rp.h);
}

} // namespace gen

} // namespace bn

#endif /* RANDOMPERTURBATIONGEN_HPP_ */
