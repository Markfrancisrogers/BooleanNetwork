/*
 * printing.hpp
 *
 *  Created on: Apr 22, 2010
 *      Author: stewie
 */

#ifndef PRINTING_HPP_
#define PRINTING_HPP_

#include <ostream>
#include <iterator> // for std::ostream_iterator
#include <boost/range/value_type.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/algorithm/copy.hpp>

namespace bn {

namespace util {

namespace detail {

template<class SinglePassRange> class Printer {
private:
	const SinglePassRange& r;
	const char* const sep;

public:
	Printer(const SinglePassRange& r, const char sep[]) :
		r(r), sep(sep) {
	}

	friend std::ostream& operator<<(std::ostream& out, const Printer& p) {
		return print(out, p.r, p.sep);
	}
};

}

template<class SinglePassRange> std::ostream& print(std::ostream& out,
		const SinglePassRange& r, const char sep[] = " ") {
	boost::copy(
			r,
			std::ostream_iterator<
					typename boost::range_value<SinglePassRange>::type>(out,
					sep));
	return out;
}

template<class SinglePassRange> detail::Printer<SinglePassRange> dump(
		const SinglePassRange& r, const char sep[] = " ") {
	return detail::Printer<SinglePassRange>(r, sep);
}

} // namespace util

} // namespace bn

#endif /* PRINTING_HPP_ */
