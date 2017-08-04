/*
 * RandomStateGen.hpp
 *
 *  Created on: Aug 26, 2009
 *      Author: stewie
 */

#ifndef RANDOMSTATEGEN_HPP_
#define RANDOMSTATEGEN_HPP_

#include <cstddef>
#include <limits>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/iterator_range.hpp>

#include "../core/network_state.hpp"
#include "../util/state_util.hpp"

namespace bn {

class BooleanDynamics;

/**
 * @ingroup generators
 *
 * This namespace contains all of the generator implementations.
 */
namespace gen {

namespace detail {

/**
 * Iterator class for RandomStateGen.
 *
 * It is a model of SinglePassIterator concept. It supports constant-time
 * differentiation.
 *
 * This iterator also keeps track of how many increment operations are
 * performed. It is useful for implementing a "stop iteration" guard.
 */
class RandomStateIterator : public boost::iterator_facade<RandomStateIterator,
		State, boost::single_pass_traversal_tag, State> {
private:
	friend class boost::iterator_core_access;

	/**
	 * Length of the state vector to produce.
	 */
	size_t n;
	/**
	 * Counter of increment operations.
	 */
	difference_type count;

	reference dereference() const {
		return util::random_state(n);
	}

	bool equal(const RandomStateIterator& other) const {
		return count == other.count;
	}

	void increment() {
		++count;
	}

	difference_type distance_to(const RandomStateIterator& other) const {
		return other.count - count;
	}

public:
	/**
	 * Default constructor.
	 *
	 * Iterators constructed this way produce 0-length state vector. Sets
	 * internal counter to 0.
	 */
	RandomStateIterator() {
	}

	/**
	 * @private
	 * Constructs an iterator that produces state vector of length @a n and
	 * sets the internal counter to @a count.
	 * @param n length of the state vector to produce
	 * @param count initial value of the internal counter
	 */
	RandomStateIterator(const size_t n, const difference_type count) :
		n(n), count(count) {
	}
};

}

/**
 * @ingroup generators
 *
 * This generator provides a finite number of random boolean state of a given
 * length.
 *
 * This class models ForwardRange concept.
 *
 * Due to the random nature of this generator, two iterations on the same range
 * never produce the same elements, up to the accuracy of the internal random
 * number generator of course.
 */
class RandomStateGen : public boost::iterator_range<detail::RandomStateIterator> {
private:
	typedef boost::iterator_range<detail::RandomStateIterator> base;

public:
	/**
	 * Initializes a generator that produces at most @a max state vector
	 * suitable for a network of size @a n.
	 *
	 * It holds the following:
	 * @code
	 * std::distance(begin(), end()) == max)
	 * @endcode
	 * @param n length of the boolean vectors to produce
	 * @param max maximum number of states to generate
	 */
	explicit RandomStateGen(const size_t n, const difference_type max =
			std::numeric_limits<difference_type>::max()) :
		base(iterator(n, 0), iterator(n, max)) {
	}
};

inline RandomStateGen random_states(const State::size_type n,
		const boost::range_difference<RandomStateGen>::type max) {
	return RandomStateGen(n, max);
}

RandomStateGen random_states(const BooleanDynamics& net,
		const boost::range_difference<RandomStateGen>::type max);

}

}

#endif /* RANDOMSTATEGEN_HPP_ */
