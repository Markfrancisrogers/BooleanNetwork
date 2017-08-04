/*
 * euclidean.hpp
 *
 *  Created on: Aug 16, 2009
 *      Author: stewie
 */

#ifndef EUCLIDEAN_HPP_
#define EUCLIDEAN_HPP_

#include <cmath>
#include <functional> // for std::plus
#include <numeric> // for std::inner_product

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

/**
 * @ingroup lab
 *
 * @file euclidean.hpp
 *
 * This file contains routines for computing various kinds of distances between
 * attractors and other objects.
 */

namespace bn {

namespace detail {

/**
 * @private
 * Helper class to compute a squared difference.
 */
struct SquareDifference {
	/**
	 * Result type of this function object.
	 */
	typedef double result_type;

	/**
	 * Computes a squared difference.
	 * @param x an object
	 * @param y another object
	 * @return the squared difference
	 */
	template<class T> result_type operator()(const T& x, const T& y) const {
		return std::pow(x - y, 2);
	}
};

}

/**
 * @ingroup lab
 *
 * Computes the euclidean distance between two ranges.
 *
 * The second range shall have at least as many elements as the first.
 * @param first1 input iterator to the initial position of the first range
 * @param last1 input iterator to the final position of the first range
 * @param first2 input iterator to the initial position of the second range
 * @return the euclidean distance
 */
template<class InputIterator> double euclidean_distance(
		const InputIterator first1, const InputIterator last1,
		const InputIterator first2) {
	return std::sqrt(std::inner_product(first1, last1, first2, 0.0, std::plus<
			double>(), detail::SquareDifference()));
}

/**
 * @ingroup lab
 *
 * Computes the euclidean distance between two ranges.
 *
 * The second range shall have at least as many elements as the first. This
 * overload uses concepts from <a href="http://www.boost.org/libs/range">
 * The Boost Range Library</a>. Practically it is syntactic sugar which replaces:
 * @code
 * vector<double> v1 = ...
 * vector<double> v2 = ...
 * euclidean_distance(v1.begin(), v1.end(), v2.begin());
 * @endcode
 * with the tighter:
 * @code
 * vector<double> v1 = ...
 * vector<double> v2 = ...
 * euclidean_distance(v1, v2);
 * @endcode
 * @param x first input range
 * @param y second input range
 * @return the euclidean distance
 */
template<class SinglePassRange> double euclidean_distance(
		const SinglePassRange& x, const SinglePassRange& y) {
	return euclidean_distance(boost::begin(x), boost::end(x), boost::begin(y));
}

/**
 * @ingroup lab
 *
 * A function object to compute the euclidean distance between objects.
 *
 * This is particularly useful in STL algorithms since it is cumbersome (or
 * simply impossible) to pass template function pointers as arguments.
 *
 * For example:
 * @code
 * vector<AttractorExpression> expr = ...
 * util::Matrix<double> m = make_distance_matrix<double>(expr.begin(),
 * 	expr.end(), &euclidean_distance);
 * @endcode
 * is wrong because the compiler cannot determine which overload to choose; also
 * that function is a template so you have to explicitly instantiate its
 * parameters to take a pointer. On the contrary, this is simpler and correct:
 * @code
 * vector<AttractorExpression> expr = ...
 * util::Matrix<double> m = make_distance_matrix<double>(expr.begin(),
 * 	expr.end(), Euclidean());
 * @endcode
 */
struct Euclidean {
	/**
	 * Result type of this function object.
	 */
	typedef double result_type;

	/**
	 * Computes the euclidean distance between two vector of the same length.
	 * @param x the first vector
	 * @param y the second vector
	 * @return the euclidean distance
	 */
	template<class T> result_type operator()(const T& x, const T& y) const {
		return euclidean_distance(x, y);
	}
};

}

#endif /* EUCLIDEAN_HPP_ */
