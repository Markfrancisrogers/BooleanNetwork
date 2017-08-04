/*
 * AttractorPattern.hpp
 *
 *  Created on: Aug 15, 2009
 *      Author: stewie
 */

#ifndef ATTRACTORPATTERN_HPP_
#define ATTRACTORPATTERN_HPP_

#include <algorithm>

#include "../core/network_state.hpp"
#include "../core/Attractor.hpp"

namespace bn {

/**
 * @ingroup lab
 *
 * Possible value of an attractor pattern.
 */
enum PatternValue {
	/**
	 * The attractor states have only 0's in that position.
	 */
	ZERO,
	/**
	 * The attractor states have only 1's in that position.
	 */
	ONE,
	/**
	 * The attractor states have both 0's and 1's in that position.
	 */
	WILDCARD
};

/**
 * @ingroup lab
 *
 * This class represents the pattern of the states in an attractor.
 *
 * An attractor pattern is an incomplete representation of all the states in an
 * attractor. In a position @e i, a pattern can have one of three value:
 * - '0' if all the states have only 0's in the <em>i</em>-th position;
 * - '1' if all the states have only 1's in the <em>i</em>-th position;
 * - '*' if all the states have either 0's and 1's in the <em>i</em>-th position.
 *
 * Instances of this class are immutable.
 *
 * @see bn::AttractorExpression for a similar but more general concept and
 * bn::PatternValue for the concrete representation of a pattern value.
 */
class AttractorPattern {
private:
	/**
	 * Boolean vector that specifies the "constant part" of a pattern.
	 *
	 * If for some @e i <code>constant[i]</code> is @e true, then the pattern
	 * has value '1' in position @e i.
	 */
	State constant;
	/**
	 * Boolean vector that specifies the "variable part" of a pattern.
	 *
	 * If for some @e i <code>variable[i]</code> is @e true, then the pattern
	 * has value '*' in position @e i. If instead is @e false, then the pattern
	 * has value:
	 * - '0' if <code>constant[i]</code> is @e false;
	 * - '1' if <code>constant[i]</code> is @e true.
	 */
	State variable;
	/**
	 * Pattern length.
	 */
	size_t n;

public:
	/**
	 * Default constructor.
	 *
	 * Initializes a pattern of length 0.
	 */
	AttractorPattern() {
	}

	explicit AttractorPattern(const Attractor& attractor);

	/**
	 * Access the pattern value at position @a i.
	 * @return the pattern value
	 */
	PatternValue operator[](const size_t i) const {
		return variable[i] ? WILDCARD : (constant[i] ? ONE : ZERO);
	}

	/**
	 * Returns the length of this container.
	 * @return he size of this container
	 */
	size_t size() const {
		return n;
	}

	/**
	 * Counts the number of either 0's or 1's in this pattern.
	 *
	 * Semantically it is the same as counting the number of frozen 0 or 1
	 * nodes in the attractor that generated this pattern.
	 * @param value set it to @e true to count 1's or to @e false to count 0's
	 * @return the number of 0's or 1's
	 */
	size_t frozen(const bool value) const {
		const size_t ones = constant.count();
		return value ? ones : (n - variable.count() - ones);
	}

	/**
	 * Counts the number of 0's and 1's in this pattern.
	 *
	 * It is the same as counting the number of frozen nodes in the attractor
	 * that generated this pattern.
	 * @return the sum of the number occurrencies of 0's and 1's
	 */
	size_t frozen() const {
		return n - variable.count();
	}

	/**
	 * Returns @e true if this pattern is compatible to the @a other.
	 *
	 * A pattern @e P is compatible to pattern @e Q if, for each position @e i,
	 * all of the following hold:
	 * - \f$P[i] = 0 \Rightarrow Q[i] = 0 \vee Q[i] = *\f$
	 * - \f$P[i] = 1 \Rightarrow Q[i] = 1 \vee Q[i] = *\f$
	 * - \f$P[i] = * \Rightarrow Q[i] = 0 \vee Q[i] = 1 \vee Q[i] = *\f$
	 *
	 * @param other the other pattern to check compatibility with
	 * @return @e true if they are compatibles
	 *
	 * @todo
	 * To be tested.
	 */
	bool isCompatible(const AttractorPattern& other) const {
		return ((constant ^ other.constant) & ~(variable | other.variable)).none();
	}

	/**
	 * Returns @e true if this pattern is a superset (not necessarily strict) of
	 * the @a other pattern.
	 *
	 * A pattern @e P includes a pattern @e Q if, for each position @e i, all of
	 * the following hold:
	 * - \f$P[i] = 0 \Rightarrow Q[i] = 0\f$
	 * - \f$P[i] = 1 \Rightarrow Q[i] = 1\f$
	 * - \f$P[i] = * \Rightarrow Q[i] = 0 \vee Q[i] = 1 \vee Q[i] = *\f$
	 *
	 * @param other the other pattern to check for inclusion
	 * @return @e true if this pattern includes the @a other
	 *
	 * @todo
	 * To be tested.
	 */
	bool includes(const AttractorPattern& other) const {
		return constant == (other.constant & variable);
	}

	/**
	 * Returns @e true if this pattern is a subset (not necessarily strict) of
	 * the @a other pattern.
	 *
	 * It has the same meaning as:
	 * @code
	 * other.includes(*this)
	 * @endcode
	 * @param other the other pattern
	 * @return @e true if this pattern is a subset of the @a other
	 *
	 * @todo
	 * To be tested.
	 */
	bool isSubsetOf(const AttractorPattern& other) const {
		return other.includes(*this);
	}

	/**
	 * Checks for pattern equality, naturally defined.
	 *
	 * Notice that two pattern may be equal, but the attractors that generated
	 * them might be, and likely are, not.
	 * @param other the other pattern to check for equality
	 * @return @e true if the two patterns are the same
	 */
	bool operator==(const AttractorPattern& other) const {
		return constant == other.constant && variable == other.variable;
	}

	/**
	 * Implementation of swap function.
	 *
	 * It simply swaps internal state containers of the two AttractorPattern
	 * objects.
	 * @param other the other attractor expression
	 */
	void swap(AttractorPattern& other) {
		std::swap(constant, other.constant);
		std::swap(variable, other.variable);
		std::swap(n, other.n);
	}

	friend std::ostream& operator<<(std::ostream& out,
			const AttractorPattern& p);
};

}

namespace std {

/**
 * Specialization that calls bn::AttractorPattern::swap(AttractorPattern&)
 * on the first argument.
 * @param a a pattern object
 * @param b another pattern object
 */
inline void swap(bn::AttractorPattern& a, bn::AttractorPattern& b) {
	a.swap(b);
}

} // namespace std

#endif /* ATTRACTORPATTERN_HPP_ */
