/*
 * AttractorExpression.hpp
 *
 *  Created on: Aug 15, 2009
 *      Author: stewie
 */

#ifndef ATTRACTOREXPRESSION_HPP_
#define ATTRACTOREXPRESSION_HPP_

#include <cstddef>
#include <ostream>
#include <vector>
#include <algorithm>
#include <functional>

#include "../core/Attractor.hpp"
#include "euclidean.hpp"

namespace bn {

/**
 * @ingroup lab
 *
 * This class represents the expression of an attractor.
 *
 * The expression of an attractor is a vector whose elements are the average on
 * all the states of an attractor of the number of 1's in a given position.
 * Formally, let \f$A = \{(a^1_1, a^1_2, \ldots, a^1_n), (a^2_1, a^2_2, \ldots, a^2_n),
 * \ldots, (a^m_1, a^m_2, \ldots, a^m_n)\} \ x^i_j \in \{0, 1\}\f$ be an
 * attractor of length @e m, then its expression is the vector:\f[
 * \frac{1}{m} \cdot (\sum_{i = 1}^m a^i_1, \sum_{i = 1}^m a^i_2, \ldots,
 * \sum_{i = 1}^m a^i_n) = \frac{1}{m} \cdot (\sum_{i = 1}^m a^i_j)_{j = 1}^n
 * \f]
 *
 * This class can be used in std algorithms since it is similar to a std::vector.
 *
 * This class is immutable.
 *
 * @see bn::AttractorPattern for a similar concept.
 */
class AttractorExpression {
private:
	/**
	 * Container that hold the expression levels for each position.
	 */
	std::vector<double> expression;

public:
	/**
	 * Constant iterator to the expression values for each position.
	 */
	typedef std::vector<double>::const_iterator const_iterator;

	/**
	 * Default constructor.
	 *
	 * Initializes an expression object of length 0.
	 */
	AttractorExpression() {
	}

	explicit AttractorExpression(const Attractor& attractor);

	/**
	 * Acces an expression level by index.
	 * @param i a position
	 * @return the <em>i</em>-th expression level
	 */
	double operator[](const size_t i) const {
		return expression[i];
	}

	/**
	 * Returns a const iterator pointing to the beginning of the expression level
	 * range.
	 * @return the beginning iterator
	 */
	const_iterator begin() const {
		return expression.begin();
	}

	/**
	 * Returns a const iterator pointing to the one-past-end element of the
	 * expression level range.
	 * @return the end iterator
	 */
	const_iterator end() const {
		return expression.end();
	}

	/**
	 * Returns the length of this container.
	 * @return he size of this container
	 */
	std::size_t size() const {
		return expression.size();
	}

	/**
	 * Counts the number of elements which compare less than or equal to the
	 * @a threshold value.
	 * @param threshold a threshold value
	 * @return the number of elements less than or equal to @a threshold
	 */
	std::size_t thresholdCount(const double threshold) const {
		return std::count_if(begin(), end(), std::bind2nd(std::less_equal<
				double>(), threshold));
	}

	/**
	 * Counts the number of either 0's or 1's in this expression.
	 *
	 * Semantically it is the same as counting the number of frozen 0 or 1
	 * nodes in the attractor that generated this expression.
	 * @param value set it to @e true to count 1's or to @e false to count 0's
	 * @return the number of 0's or 1's
	 *
	 * @see AttractorPattern::frozen(const bool)const
	 */
	std::size_t frozen(const bool value) const {
		return std::count(begin(), end(), value ? 1.0 : 0.0);
	}

	std::size_t frozen() const;

	/**
	 * Implementation of swap function.
	 *
	 * It simply swaps internal state containers of the two AttractorExpression
	 * objects.
	 * @param other the other attractor expression
	 */
	void swap(AttractorExpression& other) {
		std::swap(expression, other.expression);
	}

	/**
	 * Computes the euclidean distance between two expression objects.
	 * @param other another expression object
	 * @return the euclidean distance
	 */
	double distance(const AttractorExpression& other) const {
		return euclidean_distance(expression, other.expression);
	}

	friend std::ostream& operator<<(std::ostream& out,
			const AttractorExpression& e);
};

}

namespace std {

/**
 * Specialization that calls bn::AttractorExpression::swap(AttractorExpression&)
 * on the first argument.
 * @param a an expression object
 * @param b another expression object
 */
inline void swap(bn::AttractorExpression& a, bn::AttractorExpression& b) {
	a.swap(b);
}

} // namespace std

#endif /* ATTRACTOREXPRESSION_HPP_ */
