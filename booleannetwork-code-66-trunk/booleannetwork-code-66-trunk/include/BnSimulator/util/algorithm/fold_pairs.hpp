/*
 * fold_pairs.hpp
 *
 *  Created on: Aug 28, 2009
 *      Author: stewie
 */

#ifndef FOLD_PAIRS_HPP_
#define FOLD_PAIRS_HPP_

#include <functional>

#include <boost/next_prior.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace bn {

namespace util {

/**
 * Applies binary functions @a s and @a p on all the <em>2-combinations without
 * repetition</em> of elements in the given ranges.
 *
 * See fold_cartesian_product() for a formal description of the algorithm
 * replacing the cartesian product with the sequence of 2-combinations.
 * @param first forward iterator to the initial position of the range
 * @param last forward iterator to the final position of the range
 * @param init initial value for the accumulator
 * @param s binary function that implements the sum
 * @param p binary function that implements the product
 * @return the accumulated value
 *
 * @see for_each_pair() for the iteration version.
 */
template<class ForwardIterator, class T, class Sum, class Product> T fold_pairs(
		ForwardIterator first, ForwardIterator last, T init, Sum s = std::plus<
				T>(), Product p = std::multiplies<T>()) {
	for (; first != last; ++first)
		for (ForwardIterator it = boost::next(first); it != last; ++it)
			init = s(init, p(*first, *it));
	return init;
}

template<class ForwardRange, class T, class Sum, class Product> T fold_pairs(
		ForwardRange& r, T init, Sum s = std::plus<T>(), Product p =
				std::multiplies<T>()) {
	return fold_pairs(boost::begin(r), boost::end(r), init, s, p);
}

template<class ForwardRange, class T, class Sum, class Product> T fold_pairs(
		const ForwardRange& r, T init, Sum s = std::plus<T>(), Product p =
				std::multiplies<T>()) {
	return fold_pairs(boost::begin(r), boost::end(r), init, s, p);
}

}

}

#endif /* FOLD_PAIRS_HPP_ */
