/*
 * printing_cout.hpp
 *
 *  Created on: Apr 25, 2010
 *      Author: stewie
 */

#ifndef PRINTING_COUT_HPP_
#define PRINTING_COUT_HPP_

#include <iostream>

#include "printing.hpp"

namespace bn {

namespace util {

template<class SinglePassRange> std::ostream& print(const SinglePassRange& r,
		const char sep[] = " ") {
	return print(std::cout, r, sep);
}

}  // namespace util

}  // namespace bn

#endif /* PRINTING_COUT_HPP_ */
