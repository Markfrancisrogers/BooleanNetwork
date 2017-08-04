/*
 * functions.hpp
 *
 *  Created on: Aug 15, 2009
 *      Author: stewie
 */

#ifndef FACTORY_HPP_
#define FACTORY_HPP_

#include <cstddef>
#include <cmath> // for std::sqrt
#include <vector>
#include <iterator> // for std::back_insert_iterator

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include "../util/Matrix.hpp"
#include "../util/algorithm/cartesian_product.hpp"
#include "../core/Attractor.hpp"
#include "../experiment/StateSpaceRunner.hpp"
#include "AttractorPattern.hpp"
#include "AttractorExpression.hpp"

/**
 * @ingroup lab
 *
 * @file factory.hpp
 *
 * Contains factory functions to ease construction of objects, especially in
 * STL algorithms.
 */

namespace bn {

namespace detail {

/**
 * @private
 * Helper class to compute a distance between two elements and to store it in
 * a sequence.
 */
template<class Distance, class OutputIterator> struct ComputeDistance {
	/**
	 * Function object that implements a distance definition.
	 */
	Distance& d;
	/**
	 * Iterator to store the distance value.
	 */
	OutputIterator& out;

	/**
	 * Initializes this object with a distance implementation and an iterator
	 * to store values.
	 * @param d a function object that implements a distance definition
	 * @param out an iterator to store the distance value
	 */
	ComputeDistance(Distance& d, OutputIterator& out) :
		d(d), out(out) {
	}

	/**
	 * Computes the distance between the arguments by invocation of operator()
	 * on ComputeDistance::d, stores the result in the position pointed by
	 * the iterator and increments it.
	 * @param x an element
	 * @param y the other element
	 */
	template<class T> void operator()(const T& x, const T& y) const {
		*(out++) = d(x, y);
	}
};

}

AttractorPattern make_pattern(const Attractor& attractor);

AttractorExpression make_expression(const Attractor& attractor);

/**
 * @ingroup lab
 *
 * Constructs a distance matrix of a range of objects given a distance
 * definition.
 *
 * This function computes a distance value for al pairs of objects, that is for
 * all element in the cartesian product of the sequence by itself, even though
 * a distance is symmetric by definition. This means that you can actually use
 * whatever function you want.
 *
 * To use this function you have to explicitly instantiate template parameter
 * @a T.
 * @param first forward iterator to the initial position of the range
 * @param last forward iterator to the final position of the range
 * @param d a function object impelementation of a distance definition
 * @return a distance matrix
 */
template<class T, class ForwardIterator, class Distance> util::Matrix<T> make_distance_matrix(
		const ForwardIterator first, const ForwardIterator last, Distance d) {
	std::vector<T> buffer;
	std::back_insert_iterator<std::vector<T> > backIt(buffer);
	util::cartesian_product(first, last, first, last, detail::ComputeDistance<
			Distance, std::back_insert_iterator<std::vector<T> > >(d, backIt));
	const size_t n = std::sqrt(buffer.size());
	return util::Matrix<T>(n, n, buffer.begin(), buffer.end());
}

/**
 * @ingroup lab
 *
 * Constructs a distance matrix of a range of objects given a distance
 * definition.
 *
 * This overload works with Boost ranges instead of iterators.
 * @param r input range
 * @param d a function object impelementation of a distance definition
 * @return a distance matrix
 *
 * @see euclidean_distance(const SinglePassRange&,const SinglePassRange&) for a
 * similar concept.
 */
template<class T, class ForwardTraversalRange, class Distance> util::Matrix<T> make_distance_matrix(
		const ForwardTraversalRange& r, Distance d) {
	return make_distance_matrix<T> (boost::begin(r), boost::end(r), d);
}

util::Matrix<double> make_transition_matrix(const StateSpaceRunner::Graph& g,
		const size_t h);

}

#endif /* FACTORY_HPP_ */
