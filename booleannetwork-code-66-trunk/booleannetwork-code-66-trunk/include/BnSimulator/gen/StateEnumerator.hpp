/*
 * StateEnumerator.hpp
 *
 *  Created on: Aug 26, 2009
 *      Author: stewie
 */

#ifndef STATEENUMERATOR_HPP_
#define STATEENUMERATOR_HPP_

#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/iterator_range.hpp>

#include "../core/network_state.hpp"
#include "../util/state_util.hpp"

namespace bn {

namespace gen {

namespace detail {

class StateIterator : public boost::iterator_facade<StateIterator, const State,
		boost::forward_traversal_tag> {
private:
	friend class boost::iterator_core_access;

	/**
	 * Length of the state vector to produce.
	 */
	State::size_type n;
	/**
	 * End iterators have this flag set to @e true.
	 */
	bool overflow;
	/**
	 * Value to return upon dereferentiation.
	 */
	value_type res;

	reference dereference() const {
		return res;
	}

	bool equal(const StateIterator& other) const {
		return overflow == other.overflow && res == other.res;
	}

	void increment() {
		overflow = util::next(res);
	}

public:

	StateIterator() {
	}

	StateIterator(const State::size_type n, const bool overflow) :
		n(n), overflow(overflow), res(n) {
	}
};

}

/**
 * @ingroup generators
 *
 * This generator enumerates the whole set of boolean states of a given length
 * @e n in the natural order. This range therefore lazily represents all of the
 * possible \f$2^n\f$ states  starting from \f$\underbrace{00 \ldots 0}_n\f$.
 *
 * This class models the ForwardRange concept.
 *
 * @see RandomStateGen for another way to generate states.
 */
class StateEnumerator : public boost::iterator_range<detail::StateIterator> {
private:
	typedef boost::iterator_range<detail::StateIterator> base;

public:

	StateEnumerator() {
	}

	/**
	 * Initializes a generator that produces vectors of length @a n.
	 *
	 * @param n state vector length
	 */
	explicit StateEnumerator(const State::size_type n) :
		base(iterator(n, false), iterator(n, true)) {
	}
};

inline StateEnumerator enumerate_states(const State::size_type n) {
	return StateEnumerator(n);
}

}

}

#endif /* STATEENUMERATOR_HPP_ */
