/*
 * call_on_exit.hpp
 *
 *  Created on: Sep 3, 2009
 *      Author: stewie
 */

#ifndef CALL_ON_EXIT_HPP_
#define CALL_ON_EXIT_HPP_

#include "scope_guard.hpp"

namespace bn {

namespace util {

namespace detail {

template<class F> class function_guard : public scope_guard_facade<
		function_guard<F> > {
private:
	F f;

public:
	explicit function_guard(const F& f) :
		f(f) {
	}

	void operator()() {
		f();
	}
};

}

template<class F> detail::function_guard<F> call_on_exit(F f) {
	return detail::function_guard<F>(f);
}

}

}

#endif /* CALL_ON_EXIT_HPP_ */
