/*
 * fold_cartesian_product.hpp
 *
 *  Created on: Aug 28, 2009
 *      Author: stewie
 */

#ifndef FOLD_CARTESIAN_PRODUCT_HPP_
#define FOLD_CARTESIAN_PRODUCT_HPP_

#include <functional>

namespace bn {

namespace util {

/**
 * Applies binary functions @a s and @a p to the sequence of elements in the
 * cartesian product of the given ranges.
 *
 * Let \f$X = \{x_1, x_2, \ldots, x_n\}\f$ and \f$Y = \{y_1, y_2, \ldots, y_m\}\f$
 * be the ranges, then \f$ X \times Y = \{(x_1, y_1), (x_1, y_2), \ldots,
 * (x_n, y_m)\}\f$; let us also use infix notation \f$x \oplus y\f$ and
 * \f$x \odot y\f$ to denote <code>s(x, y)</code> and <code>p(x, y)</code>
 * respectively. This function computes:\f[
 * init \oplus (x_1 \odot y_1) \oplus (x_1 \odot y_2) \ldots \oplus
 * (x_n \odot y_m) = init \oplus \bigoplus_{1 \leq i \leq n \atop 1 \leq j \leq m}
 * x_i \odot y_j
 * \f]
 * @param first1 input iterator to the initial position of the first range
 * @param last1 input iterator to the final position of the first range
 * @param first2 forward iterator to the initial position of the second range
 * @param last2 forward iterator to the final position of the second range
 * @param init initial value for the accumulator
 * @param s binary function that implements the sum
 * @param p binary function that implements the product
 * @return the accumulated value
 *
 * @see cartesian_product() for the iteration version.
 */
template<class InputIterator1, class ForwardIterator2, class T, class Sum,
		class Product> T fold_cartesian_product(InputIterator1 first1,
		const InputIterator1 last1, const ForwardIterator2 first2,
		const ForwardIterator2 last2, T init, Sum s = std::plus<T>(),
		Product p = std::multiplies<T>()) {
	for (; first1 != last1; ++first1)
		for (ForwardIterator2 it = first2; it != last2; ++it)
			init = s(init, p(*first1, *it));
	return init;
}

}

}

#endif /* FOLD_CARTESIAN_PRODUCT_HPP_ */
