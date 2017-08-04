/*
 * utility.hpp
 *
 *  Created on: Jul 15, 2009
 *      Author: stewie
 */

#ifndef UTILITY_HPP_
#define UTILITY_HPP_

#include <cassert>
#include <cstddef>
#include <vector>
#include <algorithm>

#include <boost/utility/result_of.hpp>
#include <boost/function_output_iterator.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include "Matrix.hpp"

/**
 * @file utility.hpp
 * Contains a small set of useful functions and strucures.
 */

namespace bn {

/**
 * This namespace contains classes and utility functions that ease development
 * with the library.
 */
namespace util {

template<class SinglePassRange> std::vector<typename boost::range_value<
		SinglePassRange>::type> make_vector(const SinglePassRange& r) {
	return std::vector<typename boost::range_value<SinglePassRange>::type>(
			boost::begin(r), boost::end(r));
}

template<class T> Matrix<T> ones(const size_t m, const size_t n) {
	return Matrix<T> (m, n, 1);
}

template<class F, class T> Matrix<typename boost::result_of<F(T)>::type> matrix_transform(
		const Matrix<T>& m, const F& f) {
	Matrix<typename boost::result_of<F(T)>::type> res(m.numRows(), m.numCols());
	std::transform(m.begin(), m.end(), res.begin(), f);
	return res;
}

/**
 * Utility function to delete all object pointed by pointers contained in an
 * stl sequence.
 *
 * The sequence is also cleared meaning that its size is 0 afterwards invocation
 * of this function
 * @param s a non-associative stl sequence
 */
template<class Sequence> void purge(Sequence& s) {
	for (typename Sequence::const_iterator it = s.begin(); it != s.end(); ++it)
		delete *it;
	s.clear();
}

}

}

#endif /* UTILITY_HPP_ */
