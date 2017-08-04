/*
 * Attractor.hpp
 *
 *  Created on: Jul 21, 2009
 *      Author: stewie
 */

#ifndef ATTRACTOR_HPP_
#define ATTRACTOR_HPP_

#include <cassert>
#include <cstddef>
#include <iosfwd>
#include <vector>

#include <boost/mpl/assert.hpp>
#include <boost/range/concepts.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/empty.hpp>
#include <boost/range/distance.hpp>
#include <boost/range/algorithm/min_element.hpp>

#include "network_state.hpp"

/**
 * @file Attractor.hpp
 *
 * This file contains the definitions of the attractor classes of the library.
 */

namespace bn {

/**
 * This class is a convenient memory-efficient and unique representation of an
 * attractor (cyclic or fixed-point) of a boolean network.
 *
 * In the Kauffman model, a Boolean network is deterministic, to completely
 * describe an attractor it is therefore sufficient one of its states. Moreover,
 * since it is possible to define a total order on the set of states (for example
 * lexicographical order), we can represent an attractor with the @e least
 * of its state. The @e least state of an attractor is called @e representant
 * (ImplicitAttractor::getRepresentant()). This makes the set of attractors a
 * totally ordered set too.
 *
 * This property makes these suitable objects to be inserted in an STL set.
 *
 * Instances of this class are immutable.
 */
class ImplicitAttractor {
public:
	ImplicitAttractor(const State& s, const std::size_t length) :
		representant(s), length(length) {
		assert(length > 0);
	}

	ImplicitAttractor() :
		length(0) {
	}

	ImplicitAttractor(const ImplicitAttractor& other) :
		representant(other.representant), length(other.length) {
	}

	/**
	 * Virtual destructor for inheritance.
	 *
	 * Does nothing.
	 */
	virtual ~ImplicitAttractor() {
	}

	/**
	 * Returns the length of the attractor cycle, that is the number of states.
	 * @return the attractor length
	 */
	std::size_t getLength() const {
		return length;
	}

	/**
	 * Returns the representant of this attractor.
	 * @return the representant state
	 */
	const State& getRepresentant() const {
		return representant;
	}

	/**
	 * Compares two attractors for equality.
	 *
	 * Has the same meaning as
	 * @code
	 * getRepresentant() == other.getRepresentant()
	 * @endcode
	 * @param other the other attractor
	 * @return @e true attractors are the same otherwise @e false
	 */
	bool operator==(const ImplicitAttractor& other) const {
		return representant == other.representant;
	}

	/**
	 * Compares two attractors for inequality.
	 *
	 * Has the same meaning as
	 * @code
	 * !(*this == other)
	 * @endcode
	 * @param other the other attractor
	 * @return @e true if attractors are different otherwise @e false
	 */
	bool operator!=(const ImplicitAttractor& other) const {
		return !operator==(other);
	}

	/**
	 * Compares two attractors with less-than relation.
	 *
	 * Has the same meaning as
	 * @code
	 * getRepresentant() < other.getRepresentant()
	 * @endcode
	 * @param other the other attractor
	 * @return @e true this attractor is less than the other, otherwise @e false
	 */
	bool operator<(const ImplicitAttractor& other) const {
		return representant < other.representant;
	}

	/**
	 * Prints this attractor to a stream.
	 *
	 * It prints only its representant.
	 * @param out a stream
	 * @param a an attractor
	 * @return the stream passed as argument
	 */
	friend std::ostream& operator<<(std::ostream&, const ImplicitAttractor&);

protected:
	/**
	 * Representant state of this attractor.
	 */
	State representant;
	/**
	 * Length of this attractor.
	 */
	std::size_t length;
};

/**
 * This class contains all the states of an attractor.
 *
 * It provides a STL forward const_iterator to sequentially iterate through all
 * its states.
 *
 * Instances of this class are immutable.
 */
class Attractor : public ImplicitAttractor {
private:
	/**
	 * The state sequence in this attractor.
	 */
	std::vector<State> states;

public:
	/**
	 * Constant iterator type for this class.
	 */
	typedef std::vector<State>::const_iterator const_iterator;
	/**
	 * Iterator type for this class.
	 *
	 * Since objects of this class are immutable, this type is equal to
	 * const_iterator.
	 */
	typedef const_iterator iterator;

	template<class SinglePassRange> explicit Attractor(const SinglePassRange& cycle) :
		states(boost::begin(cycle), boost::end(cycle)) {
		representant = *boost::min_element(states);
		length = states.size();
		assert(!boost::empty(cycle));
		assert(representant == *boost::min_element(states));
	}

	Attractor() {
	}

	Attractor(const Attractor& other) :
		ImplicitAttractor(other), states(other.states) {
	}

	/**
	 * Returns a constant iterator pointing to the beginning of the state
	 * sequence.
	 * @return the beginning iterator
	 */
	const_iterator begin() const {
		return states.begin();
	}

	/**
	 * Returns a constant iterator pointing to the end of the state sequence.
	 * @return the end iterator
	 */
	const_iterator end() const {
		return states.end();
	}

	/**
	 * Prints an attractor to a stream.
	 *
	 * It prints all of its states in lexicographical order, one for each line.
	 * @param out a stream
	 * @param a an attractor
	 * @return the stream passed as argument
	 */
	friend std::ostream& operator<<(std::ostream&, const Attractor&);
};

static const Attractor EMPTY_ATTRACTOR = Attractor();

}

#endif /* ATTRACTOR_HPP_ */
