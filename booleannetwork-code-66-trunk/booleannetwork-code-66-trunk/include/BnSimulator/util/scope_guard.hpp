/*
 * scope_guard.hpp
 *
 *  Created on: Sep 3, 2009
 *      Author: stewie
 */

#ifndef SCOPE_GUARD_HPP_
#define SCOPE_GUARD_HPP_

#include <boost/preprocessor/cat.hpp>

namespace bn {

namespace util {

namespace detail {

class scope_guard_base {
public:
	void release() const {
		released = true;
	}

	void __touch__() const {
	}

protected:
	scope_guard_base() :
		released(false) {
	}

	scope_guard_base(const scope_guard_base& other) :
		released(other.released) {
		other.release();
	}

	template<class T> static void execute(T& guard) {
		if (!guard.released)
			guard();
	}

	mutable bool released;

private:
	scope_guard_base& operator=(const scope_guard_base&);

	void* operator new(size_t);
};

}

template<class T> class scope_guard_facade : public detail::scope_guard_base {
protected:
	scope_guard_facade() {
	}

	~scope_guard_facade() {
		detail::scope_guard_base::execute(static_cast<T&> (*this));
	}
};

typedef const detail::scope_guard_base& scope_guard;

#define SCOPE_GUARD_HELPER(name, guard) \
	::bn::util::scope_guard name = guard; \
	name.__touch__()
#define SCOPE_GUARD(guard) \
	SCOPE_GUARD_HELPER(BOOST_PP_CAT(__scope_guard_at_line_, __LINE__), guard)

}

}

#endif /* SCOPE_GUARD_HPP_ */
