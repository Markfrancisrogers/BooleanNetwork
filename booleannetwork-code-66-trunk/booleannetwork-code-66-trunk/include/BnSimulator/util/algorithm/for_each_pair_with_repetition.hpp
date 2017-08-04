/*
 * for_each_pair_with_repetition.hpp
 *
 *  Created on: Aug 28, 2009
 *      Author: stewie
 */

#ifndef FOR_EACH_PAIR_WITH_REPETITION_HPP_
#define FOR_EACH_PAIR_WITH_REPETITION_HPP_

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace bn {

namespace util {

/**
 * Applies binary function @a f on all the <em>2-combinations with
 * repetition</em> of elements in the given ranges.
 *
 * This function calls @a f \f${n + 1 \choose 2} = \frac{n^2 + n}{2}\f$ times,
 * where @e n is the length of the range. The behaviour of this template
 * function is equivalent to:
 * @code
 * for (; first1 != last1; ++first1)
 * 	for (InputIterator it = first; it != last; ++it)
 * 		f(*first1, *it);
 * 	return f;
 * @endcode
 * @param first forward iterator to the initial position of the range
 * @param last forward iterator to the final position of the range
 * @param f binary function object to apply
 * @return the function object
 *
 * @see for_each_pair() and std::for_each() for a similar iteration algorithm.
 */
template<class ForwardIterator, class Function> void for_each_pair_with_repetition(
		ForwardIterator first, ForwardIterator last, Function f) {
	for (; first != last; ++first)
		for (ForwardIterator it = first; it != last; ++it)
			f(*first, *it);
}

template<class ForwardRange, class Function> void for_each_pair_with_repetition(
		ForwardRange& r, Function f) {
	for_each_pair_with_repetition(boost::begin(r), boost::end(r), f);
}

template<class ForwardRange, class Function> void for_each_pair_with_repetition(
		const ForwardRange& r, Function f) {
	for_each_pair_with_repetition(boost::begin(r), boost::end(r), f);
}

}

}

#endif /* FOR_EACH_PAIR_WITH_REPETITION_HPP_ */
