/*
 * for_each_pair.hpp
 *
 *  Created on: Aug 28, 2009
 *      Author: stewie
 */

#ifndef FOR_EACH_PAIR_HPP_
#define FOR_EACH_PAIR_HPP_

#include <boost/next_prior.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace bn {

namespace util {

/**
 * Applies binary function @a f on all the <em>2-combinations without
 * repetition</em> of elements in the given ranges.
 *
 * This function calls @a f \f${n \choose 2} = \frac{n^2 - n}{2}\f$ times,
 * where @e n is the length of the range. The behaviour of this template
 * function is equivalent to:
 * @code
 * for (; first != last; ++first) {
 * 	ForwardIterator temp = first;
 * 	for (ForwardIterator it = ++temp; it != last; ++it)
 * 		f(*first, *it);
 * 	}
 * return f;
 * @endcode
 * @param first forward iterator to the initial position of the range
 * @param last forward iterator to the final position of the range
 * @param f binary function object to apply
 * @return the function object
 *
 * @see std::for_each() for a similar iteration algorithm.
 */
template<class ForwardIterator, class Function> Function for_each_pair(
		ForwardIterator first, ForwardIterator last, Function f) {
	for (; first != last; ++first)
		for (ForwardIterator it = boost::next(first); it != last; ++it)
			f(*first, *it);
	return f;
}

template<class ForwardRange, class Function> Function for_each_pair(
		ForwardRange& r, Function f) {
	return for_each_pair(boost::begin(r), boost::end(r), f);
}

template<class ForwardRange, class Function> Function for_each_pair(
		const ForwardRange& r, Function f) {
	return for_each_pair(boost::begin(r), boost::end(r), f);
}

}

}

#endif /* FOR_EACH_PAIR_HPP_ */
