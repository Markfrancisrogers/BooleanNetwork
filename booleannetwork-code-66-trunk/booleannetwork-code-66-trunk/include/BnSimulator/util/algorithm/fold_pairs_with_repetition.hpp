/*
 * fold_pairs_with_repetition.hpp
 *
 *  Created on: Aug 28, 2009
 *      Author: stewie
 */

#ifndef FOLD_PAIRS_WITH_REPETITION_HPP_
#define FOLD_PAIRS_WITH_REPETITION_HPP_

#include <functional>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace bn {

namespace util {

/**
 * Applies binary functions @a s and @a p on all the <em>2-combinations with
 * repetition</em> of elements in the given ranges.
 *
 * See fold_pairs() for the detailed description.
 * @param first forward iterator to the initial position of the range
 * @param last forward iterator to the final position of the range
 * @param init initial value for the accumulator
 * @param s binary function that implements the sum
 * @param p binary function that implements the product
 * @return the accumulated value
 *
 * @see for_each_pair_with_repetition() for the iteration version.
 */
template<class ForwardIterator, class T, class Sum, class Product> T fold_pairs_with_repetition(
		ForwardIterator first, ForwardIterator last, T init, Sum s = std::plus<
				T>(), Product p = std::multiplies<T>()) {
	for (; first != last; ++first)
		for (ForwardIterator it = first; it != last; ++it)
			init = s(init, p(*first, *it));
	return init;
}

template<class ForwardRange, class T, class Sum, class Product> T fold_pairs_with_repetition(
		ForwardRange& r, T init, Sum s = std::plus<T>(), Product p =
				std::multiplies<T>()) {
	return fold_pairs_with_repetition(boost::begin(r), boost::end(r), s, p);
}

template<class ForwardRange, class T, class Sum, class Product> T fold_pairs_with_repetition(
		const ForwardRange& r, T init, Sum s = std::plus<T>(), Product p =
				std::multiplies<T>()) {
	return fold_pairs_with_repetition(boost::begin(r), boost::end(r), s, p);
}

}

}

#endif /* FOLD_PAIRS_WITH_REPETITION_HPP_ */
