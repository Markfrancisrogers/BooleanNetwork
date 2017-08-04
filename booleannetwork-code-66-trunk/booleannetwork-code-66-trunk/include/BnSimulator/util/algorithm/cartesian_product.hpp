/*
 * cartesian_product.hpp
 *
 *  Created on: Aug 28, 2009
 *      Author: stewie
 */

#ifndef CARTESIAN_PRODUCT_HPP_
#define CARTESIAN_PRODUCT_HPP_

namespace bn {

namespace util {

/**
 * Applies binary function @a f on all the elements in the cartesian product
 * of the given ranges.
 *
 * The behaviour of this template function is equivalent to:
 * @code
 * for (; first1 != last1; ++first1)
 * 	for (InputIterator2 it = first2; it != last2; ++it)
 * 		f(*first1, *it);
 * 	return f;
 * @endcode
 * @param first1 input iterator to the initial position of the first range
 * @param last1 input iterator to the final position of the first range
 * @param first2 input iterator to the initial position of the second range
 * @param last2 input iterator to the final position of the second range
 * @param f binary function object to apply
 * @return the function object
 *
 * @see std::for_each() for a similar iteration algorithm.
 */
template<class InputIterator1, class ForwardIterator2, class Function> Function cartesian_product(
		InputIterator1 first1, const InputIterator1 last1,
		const ForwardIterator2 first2, const ForwardIterator2 last2, Function f) {
	for (; first1 != last1; ++first1)
		for (ForwardIterator2 it = first2; it != last2; ++it)
			f(*first1, *it);
	return f;
}

}

}

#endif /* CARTESIAN_PRODUCT_HPP_ */
